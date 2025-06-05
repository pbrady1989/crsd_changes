/* =========================================================================
 * This file is part of crsd-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2019, MDA Information Systems LLC
 *
 * crsd-c++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; If not,
 * see <http://www.gnu.org/licenses/>.
 *
 */
#include <crsd/CRSDWriter.h>

#include <thread>
#include <std/bit>
#include <std/memory>

#include <except/Exception.h>

#include <crsd/ByteSwap.h>
#include <crsd/CRSDXMLControl.h>
#include <crsd/FileHeader.h>
#include <crsd/Utilities.h>
#include <crsd/Wideband.h>

#undef min
#undef max


namespace crsd
{
DataWriter::DataWriter(std::shared_ptr<io::SeekableOutputStream> stream,
                       size_t numThreads) :
    mStream(stream),
    mNumThreads(numThreads == 0 ? std::thread::hardware_concurrency() : numThreads)
{
}

DataWriter::~DataWriter()
{
}

DataWriterLittleEndian::DataWriterLittleEndian(
        std::shared_ptr<io::SeekableOutputStream> stream,
        size_t numThreads,
        size_t scratchSize) :
    DataWriter(stream, numThreads),
    mScratch(scratchSize)
{
}

void DataWriterLittleEndian::operator()(const sys::ubyte* data,
                                        size_t numElements,
                                        size_t elementSize)
{
    size_t dataProcessed = 0;
    const size_t dataSize = numElements * elementSize;
    while (dataProcessed < dataSize)
    {
        const size_t dataToProcess =
                std::min(mScratch.size(), dataSize - dataProcessed);

        memcpy(mScratch.data(), data + dataProcessed, dataToProcess);

        crsd::byteSwap(mScratch.data(),
                       elementSize,
                       dataToProcess / elementSize,
                       mNumThreads);

        mStream->write(mScratch.data(), dataToProcess);

        dataProcessed += dataToProcess;
    }
}

DataWriterBigEndian::DataWriterBigEndian(
        std::shared_ptr<io::SeekableOutputStream> stream, size_t numThreads) :
    DataWriter(stream, numThreads)
{
}

void DataWriterBigEndian::operator()(const sys::ubyte* data,
                                     size_t numElements,
                                     size_t elementSize)
{
    mStream->write(data,
                   numElements * elementSize);
}

void CRSDWriter::initializeDataWriter()
{
    // Get the correct dataWriter.
    // The CRSD file needs to be big endian.
    auto endianness = std::endian::native; // "conditional expression is constant"
    if (endianness == std::endian::big)
    {
        mDataWriter = std::make_unique<DataWriterBigEndian>(mStream, mNumThreads);
    }
    else
    {
        mDataWriter = std::make_unique<DataWriterLittleEndian>(mStream,
            mNumThreads,
            mScratchSpaceSize);
    }
}


CRSDWriter::CRSDWriter(const Metadata& metadata,
                       std::shared_ptr<io::SeekableOutputStream> outStream,
                       const std::vector<std::string>& schemaPaths,
                       size_t numThreads,
                       size_t scratchSpaceSize) :
    mMetadata(metadata),
    mElementSize(metadata.data.getNumBytesPerSample()),
    mScratchSpaceSize(scratchSpaceSize),
    mNumThreads(numThreads),
    mSchemaPaths(schemaPaths),
    mStream(outStream)
{
    initializeDataWriter();
}

CRSDWriter::CRSDWriter(const Metadata& metadata,
                       const std::string& pathname,
                       const std::vector<std::string>& schemaPaths,
                       size_t numThreads,
                       size_t scratchSpaceSize) :
    mMetadata(metadata),
    mElementSize(metadata.data.getNumBytesPerSample()),
    mScratchSpaceSize(scratchSpaceSize),
    mNumThreads(numThreads),
    mSchemaPaths(schemaPaths)
{
    // Initialize output stream
    mStream = std::make_shared<io::FileOutputStream>(pathname);

    initializeDataWriter();
}

void CRSDWriter::writeMetadata(size_t supportSize,
                               size_t pvpSize,
                               size_t pppSize,
                               size_t crsdSize)
{
    const auto xmlMetadata(CRSDXMLControl().toXMLString(mMetadata, nullptr, true));

    // update header version, or remains default if unset
    mHeader.setVersion(mMetadata.getVersion());
    mHeader.setType(mMetadata.getType());

    // update classification and release info
    if (!six::Init::isUndefined(
                mMetadata.productInfo.classification) &&
        !six::Init::isUndefined(mMetadata.productInfo.releaseInfo))
    {
        mHeader.setClassification(
                mMetadata.productInfo.classification);
        mHeader.setReleaseInfo(mMetadata.productInfo.releaseInfo);
    }
    else
    {
        throw except::Exception(Ctxt("Classification level and Release "
                                     "informaion must be specified"));
    }

    // set header size, final step before write
    mHeader.set(xmlMetadata.size(), supportSize, pvpSize, pppSize, crsdSize);

    mStream->write(mHeader.toString());

    mStream->write("\f\n");

    mStream->write(xmlMetadata);

    mStream->write("\f\n");
}

void CRSDWriter::writePVPData(const std::byte* pvpBlock, size_t channel)
{
    const size_t size = (mMetadata.data.getNumVectors(channel) *
                         mMetadata.data.getNumBytesPVPSet()) /
            8;

    //! The vector based parameters are always 64 bit
    (*mDataWriter)(pvpBlock, size, 8);
}

void CRSDWriter::writePPPData(const std::byte* pppBlock, size_t index)
{
    const size_t size = (mMetadata.data.transmitParameters->txSequence[index].getNumPulses() *
                         mMetadata.data.getNumBytesPPPSet()) /
            8;

    //! The vector based parameters are always 64 bit
    (*mDataWriter)(pppBlock, size, 8);
}

void CRSDWriter::writeCRSDDataImpl(const std::byte* data, size_t size)
{
    //! We have to pass in the data as though it was not complex
    //  thus we pass in twice the number of elements at half the size.
    (*mDataWriter)(data, size * 2, mElementSize / 2);
}

void CRSDWriter::writeCompressedCRSDDataImpl(const std::byte* data,
                                             size_t channel)
{
    //! We have to pass in the data as though it was 1 signal array sized
    // element of ubytes
    (*mDataWriter)(data, mMetadata.data.getCompressedSignalSize(channel), 1);
}

void CRSDWriter::writeSupportDataImpl(const std::byte* data,
                                      size_t numElements,
                                      size_t elementSize)
{
    (*mDataWriter)(data, numElements, elementSize);
}

template <typename T>
void CRSDWriter::write(const PVPBlock& pvpBlock,
                       const PPPBlock& pppBlock,
                       const T* widebandData,
                       const sys::ubyte* supportData)
{
    // Write File header and metadata to file
    // Padding is added in writeMetadata
    writeMetadata(pvpBlock, pppBlock);

    // Write optional support array block
    // Padding is added in writeSupportData
    if (mMetadata.data.getNumSupportArrays() != 0)
    {
        if (supportData == nullptr)
        {
            throw except::Exception(Ctxt("SupportData is not provided"));
        }
        writeSupportData(supportData);
    }

    // Write pvp data block
    // Padding is added in writePVPData
    writePVPData(pvpBlock);

    // Write pvp data block
    // Padding is added in writePVPData
    writePPPData(pppBlock);

    // Doesn't require padding because pvp block is always 8 bytes words
    // Write wideband (or signal) block
    size_t elementsWritten = 0;  // Used to increment widebandData pointer
    for (size_t ii = 0; ii < mMetadata.data.getNumChannels(); ++ii)
    {
        size_t numElements = mMetadata.data.getNumVectors(ii) *
                mMetadata.data.getNumSamples(ii);
        // writeCRSDData handles compressed data as well
        writeCRSDData<T>(widebandData + elementsWritten, numElements, ii);
        elementsWritten += numElements;
    }
}

// For compressed data
template void CRSDWriter::write<sys::ubyte>(const PVPBlock& pvpBlock,
                                            const PPPBlock& pppBlock,
                                            const sys::ubyte* widebandData,
                                            const sys::ubyte* supportData);
template void CRSDWriter::write<std::byte>(const PVPBlock& pvpBlock,
                                            const PPPBlock& pppBlock,
                                            const std::byte* widebandData,
                                            const std::byte* supportData);

template void CRSDWriter::write<std::complex<int8_t>>(
    const PVPBlock& pvpBlock,
    const PPPBlock& pppBlock,
    const std::complex<int8_t>* widebandData,
    const sys::ubyte* supportData);

template void CRSDWriter::write<std::complex<int16_t>>(
    const PVPBlock& pvpBlock,
    const PPPBlock& pppBlock,
    const std::complex<int16_t>* widebandData,
    const sys::ubyte* supportData);

template void CRSDWriter::write<std::complex<float>>(
    const PVPBlock& pvpBlock,
    const PPPBlock& pppBlock,
    const std::complex<float>* widebandData,
    const sys::ubyte* supportData);

template void CRSDWriter::write<std::complex<int8_t>>(
        const PVPBlock& pvpBlock,
        const PPPBlock& pppBlock,
        const std::complex<int8_t>* widebandData,
        const std::byte* supportData);

template void CRSDWriter::write<std::complex<int16_t>>(
        const PVPBlock& pvpBlock,
        const PPPBlock& pppBlock,
        const std::complex<int16_t>* widebandData,
        const std::byte* supportData);

template void CRSDWriter::write<std::complex<float>>(
        const PVPBlock& pvpBlock,
        const PPPBlock& pppBlock,
        const std::complex<float>* widebandData,
        const std::byte* supportData);

void CRSDWriter::writePVPData(const PVPBlock& pvpBlock)
{
    // Add padding
    char zero = 0;
    for (int64_t ii = 0; ii < mHeader.getPvpPadBytes(); ++ii)
    {
        mStream->write(&zero, 1);
    }

    // Write each PVP array
    const size_t numChannels = mMetadata.data.getNumChannels();
    std::vector<std::byte> pvpData;
    for (size_t ii = 0; ii < numChannels; ++ii)
    {
        std::cout << "Getting pvp data for channel " << ii << std::endl;
        pvpBlock.getPVPdata(ii, pvpData);
        if (pvpData.empty())
        {
            std::ostringstream ostr;
            ostr << "PVPBlock of channel " << ii << " is empty";
            throw except::Exception(Ctxt(ostr.str()));
        }
        std::cout << "Writing pvp data for channel " << ii << std::endl;
        writePVPData(pvpData.data(), ii);
        std::cout << "Succesfully wrote pvp data for channel " << ii << std::endl;
    }
}

void CRSDWriter::writePPPData(const PPPBlock& pppBlock)
{
    // Add padding
    char zero = 0;
    for (int64_t ii = 0; ii < mHeader.getPppPadBytes(); ++ii)
    {
        mStream->write(&zero, 1);
    }

    // Write each PVP array
    const size_t numTxSequences = mMetadata.data.getNumTxSequences();
    std::vector<std::byte> pppData;
    for (size_t ii = 0; ii < numTxSequences; ++ii)
    {
        pppBlock.getPPPdata(ii, pppData);
        if (pppData.empty())
        {
            std::ostringstream ostr;
            ostr << "PPPBlock of txSequence " << ii << " is empty";
            throw except::Exception(Ctxt(ostr.str()));
        }
        writePPPData(pppData.data(), ii);
    }
}

void CRSDWriter::writeMetadata(const PVPBlock& pvpBlock,
                               const PPPBlock& pppBlock)    
{
    // Update the number of bytes per PVP
    if (mMetadata.data.receiveParameters->getNumBytesPVP() != pvpBlock.getNumBytesPVPSet())
    {
        std::ostringstream ostr;
        ostr << "Number of pvp block bytes in metadata: "
             << mMetadata.data.receiveParameters->getNumBytesPVP() 
             << " does not match calculated size of pvp block: "
             << pvpBlock.getNumBytesPVPSet();
        throw except::Exception(ostr.str());
    }

    const size_t numChannels = mMetadata.data.getNumChannels();
    const size_t numTxSequences = mMetadata.data.getNumTxSequences();
    size_t totalSupportSize = 0;
    size_t totalPVPSize = 0;
    size_t totalPPPSize = 0;
    size_t totalCRSDSize = 0;

    for (auto it = mMetadata.data.supportArrayMap.begin();
         it != mMetadata.data.supportArrayMap.end();
         ++it)
    {
        totalSupportSize += it->second.getSize();
    }
    for (size_t ii = 0; ii < numChannels; ++ii)
    {
        totalPVPSize += pvpBlock.getPVPsize(ii);
        totalCRSDSize += mMetadata.data.getNumVectors(ii) *
                mMetadata.data.getNumSamples(ii) * mElementSize;
    }

    for (size_t ii = 0; ii < numTxSequences; ++ii)
    {
        totalPPPSize += pppBlock.getPPPsize(ii);
        totalCRSDSize += mMetadata.data.getNumPulses(ii) *
                mMetadata.data.getNumSamples(ii) * mElementSize;
    }

    writeMetadata(totalSupportSize, totalPVPSize, totalPPPSize, totalCRSDSize);
}

template <typename T>
void CRSDWriter::writeCRSDData(const T* data,
                               size_t numElements,
                               size_t channel)
{
    if (mMetadata.data.isCompressed())
    {
        writeCompressedCRSDDataImpl(reinterpret_cast<const std::byte*>(data),
                                    channel);
    }
    else
    {
        if (mElementSize != sizeof(T))
        {
            throw except::Exception(
                    Ctxt("Incorrect buffer data type used for metadata!"));
        }
        writeCRSDDataImpl(reinterpret_cast<const std::byte*>(data),
                          numElements);
    }
}

// For compressed data
template void CRSDWriter::writeCRSDData(const sys::ubyte* data,
                                        size_t numElements,
                                        size_t channel);
template void CRSDWriter::writeCRSDData(const std::byte* data,
                                        size_t numElements,
                                        size_t channel);

template void CRSDWriter::writeCRSDData<std::complex<int8_t>>(
        const std::complex<int8_t>* data,
        size_t numElements,
        size_t channel);

template void CRSDWriter::writeCRSDData<std::complex<int16_t>>(
        const std::complex<int16_t>* data,
        size_t numElements,
        size_t channel);

template void CRSDWriter::writeCRSDData<std::complex<float>>(
        const std::complex<float>* data, size_t numElements, size_t channel);
}
