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

#include <stddef.h>

#include <ostream>
#include <vector>
#include <typeinfo>
#include <string>

#include <std/bit>

#include <nitf/coda-oss.hpp>
#include <six/Init.h>
#include <sys/Conf.h>

#include <crsd/Types.h>
#include <crsd/PVPBlock.h>
#include <crsd/Metadata.h>
#include <crsd/Utilities.h>
#include <crsd/FileHeader.h>

namespace
{
// Set data from data block into data struct
template <typename T> inline void setData(const unsigned char* data,
                    T& dest)
{
    memcpy(&dest, data, sizeof(T));
}
template <typename T> inline void setData(const std::byte* data,
    T& dest)
{
    setData(reinterpret_cast<const unsigned char*>(data), dest);
}
template <typename T> inline void setData(const sys::byte* data,
    T& dest)
{
    setData(reinterpret_cast<const unsigned char*>(data), dest);
}
template <typename T> inline void setData(const double* data_,
    T& dest)
{
    const void* pData_ = data_;
    auto data = static_cast<const unsigned char*>(pData_);
    setData(data, dest);
}

inline void setData(const std::byte* data_,
                    crsd::Vector2& dest)
{
    const void* pData_ = data_;
    auto data = static_cast<const double*>(pData_);
    setData(&(data[0]), dest[0]);
    setData(&(data[1]), dest[1]);
}

inline void setData(const std::byte* data_,
                    int64_t& dest)
{
    const void* pData_ = data_;
    auto data = static_cast<const unsigned char*>(pData_);
    setData(data, dest);

}

inline void setData(const std::byte* data_,
                    crsd::Vector3& dest)
{
    const void* pData_ = data_;
    auto data = static_cast<const double*>(pData_);
    setData(&(data[0]), dest[0]);
    setData(&(data[1]), dest[1]);
    setData(&(data[2]), dest[2]);
}

// Get data from data struct and put into data block
template <typename T> inline void getData(std::byte* dest,
                    T value)
{
    memcpy(dest, &value, sizeof(value));
}
inline void getData(double* dest, double value)
{
    memcpy(dest, &value, sizeof(value));
}

template <typename T> inline void getData(std::byte* dest,
                    T* value, size_t size)
{
    memcpy(dest, value, size);
}

inline void getData(std::byte* dest_,
                    const crsd::Vector2& value)
{
    void* pDest_ = dest_;
    auto dest = static_cast<double*>(pDest_);

    getData(&(dest[0]), value[0]);
    getData(&(dest[1]), value[1]);
}

inline void getData(std::byte* dest_,
                    const std::pair<int64_t, double>& value)
{
    memcpy(dest_, &value.first, sizeof(int64_t));
    int offset = sizeof(int64_t)/sizeof(std::byte);
    getData(&(dest_[offset - 1]), value.second);
}

inline void getData(std::byte* dest_,
                    const crsd::Vector3& value)
{
    void* pDest_ = dest_;
    auto dest = static_cast<double*>(pDest_);

    getData(&(dest[0]), value[0]);
    getData(&(dest[1]), value[1]);
    getData(&(dest[2]), value[2]);
}
}

namespace crsd
{

PVPBlock::PVPSet::PVPSet() :
    rcvStart(six::Init::undefined<int64_t>(),
             six::Init::undefined<double>()),
    rcvPos(six::Init::undefined<Vector3>()),
    rcvVel(six::Init::undefined<Vector3>()),
    frcv1(six::Init::undefined<double>()),
    frcv2(six::Init::undefined<double>()),
    refPhi0(six::Init::undefined<int64_t>(),
            six::Init::undefined<double>()),
    refFreq(six::Init::undefined<double>()),
    dfiC0(six::Init::undefined<double>()),
    ficRate(six::Init::undefined<double>()),
    rcvACX(six::Init::undefined<Vector3>()),
    rcvACY(six::Init::undefined<Vector3>()),
    rcvEB(six::Init::undefined<Vector2>()),
    signal(six::Init::undefined<int64_t>()),
    ampSF(six::Init::undefined<double>()),
    dgrgc(six::Init::undefined<double>()),
    txPulseIndex(six::Init::undefined<int64_t>())
{
}

void PVPBlock::PVPSet::write(const PVPBlock& pvpBlock, const Pvp& p, const sys::byte* input)
{
    double tmpVal;
    ::setData(input + p.rcvStart.getByteOffset(), tmpVal);
    rcvStart.first = static_cast<int64_t>(tmpVal);
    ::setData(input + p.rcvStart.getByteOffset() + sizeof(rcvStart.first), rcvStart.second);
    ::setData(input + p.rcvPos.getByteOffset(), rcvPos);
    ::setData(input + p.rcvVel.getByteOffset(), rcvVel);
    ::setData(input + p.frcv1.getByteOffset(), frcv1);
    ::setData(input + p.frcv2.getByteOffset(), frcv2);
    ::setData(input + p.refPhi0.getByteOffset(), tmpVal);
    refPhi0.first = static_cast<int64_t>(tmpVal);
    ::setData(input + p.refPhi0.getByteOffset() + sizeof(refPhi0.first), refPhi0.second);
    ::setData(input + p.refFreq.getByteOffset(), refFreq);
    ::setData(input + p.dfiC0.getByteOffset(), dfiC0);
    ::setData(input + p.ficRate.getByteOffset(), ficRate);
    ::setData(input + p.rcvACX.getByteOffset(), rcvACX);
    ::setData(input + p.rcvACY.getByteOffset(), rcvACY);
    ::setData(input + p.rcvEB.getByteOffset(), rcvEB);
    ::setData(input + p.signal.getByteOffset(), signal);
    ::setData(input + p.ampSF.getByteOffset(), ampSF);
    ::setData(input + p.dgrgc.getByteOffset(), dgrgc);
    if (!six::Init::isUndefined<size_t>(p.txPulseIndex.getOffset()))
    {
        ::setData(input + p.txPulseIndex.getByteOffset(), txPulseIndex);
    }
    for (auto it = p.addedPVP.begin(); it != p.addedPVP.end(); ++it)
    {
        if (it->second.getFormat() == "F4")
        {
            float val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "F8")
        {
            double val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "U1")
        {
            std::uint8_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "U2")
        {
            std::uint16_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "U4")
        {
            std::uint32_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "U8")
        {
            std::int64_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "I1")
        {
            std::int8_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "I2")
        {
            std::int16_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "I4")
        {
            std::int32_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "I8")
        {
            std::int64_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "CI2")
        {
            std::complex<std::int8_t> val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "CI4")
        {
            std::complex<std::int16_t> val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "CI8")
        {
            std::complex<std::int32_t> val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "CI16")
        {
            std::complex<std::int64_t> val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "CF8")
        {
            std::complex<float> val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "CF16")
        {
            std::complex<double> val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
        else
        {
            const auto pVal = input + it->second.getByteOffset();
            std::string val;
            val.assign(reinterpret_cast<const char*>(pVal),
                       it->second.getByteSize());
            addedPVP[it->first] = six::Parameter();
            addedPVP.find(it->first)->second.setValue(val);
        }
    }
}

void PVPBlock::PVPSet::read(const Pvp& p, sys::ubyte* dest_) const
{
    auto dest = reinterpret_cast<std::byte*>(dest_);
    ::getData(dest + p.rcvStart.getByteOffset(), static_cast<double>(rcvStart.first));
    ::getData(dest + p.rcvStart.getByteOffset() + sizeof(rcvStart.first), rcvStart.second);
    ::getData(dest + p.rcvPos.getByteOffset(), rcvPos);
    ::getData(dest + p.rcvVel.getByteOffset(), rcvVel);
    ::getData(dest + p.frcv1.getByteOffset(), frcv1);
    ::getData(dest + p.frcv2.getByteOffset(), frcv2);
    ::getData(dest + p.refPhi0.getByteOffset(), static_cast<double>(refPhi0.first));
    ::getData(dest + p.refPhi0.getByteOffset() + sizeof(refPhi0.first), refPhi0.second);
    ::getData(dest + p.refFreq.getByteOffset(), refFreq);
    ::getData(dest + p.dfiC0.getByteOffset(), dfiC0);
    ::getData(dest + p.ficRate.getByteOffset(), ficRate);
    ::getData(dest + p.rcvACX.getByteOffset(), rcvACX);
    ::getData(dest + p.rcvACY.getByteOffset(), rcvACY);
    ::getData(dest + p.rcvEB.getByteOffset(), rcvEB);
    ::getData(dest + p.signal.getByteOffset(), signal);
    ::getData(dest + p.ampSF.getByteOffset(), ampSF);
    ::getData(dest + p.dgrgc.getByteOffset(), dgrgc);
    if (!six::Init::isUndefined<size_t>(p.txPulseIndex.getOffset()))
    {
        ::getData(dest + p.txPulseIndex.getByteOffset(), txPulseIndex);
    }
    if (addedPVP.size() != p.addedPVP.size())
    {
        throw except::Exception(Ctxt(
            "Incorrect number of additional parameters instantiated"));
    }
    for (auto it = p.addedPVP.begin(); it != p.addedPVP.end(); ++it)
    {
        if (it->second.getFormat() == "F4")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<float>(addedPVP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "F8")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<double>(addedPVP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "U1")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::uint8_t>(addedPVP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "U2")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::uint16_t>(addedPVP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "U4")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::uint32_t>(addedPVP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "U8")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::int64_t>(addedPVP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "I1")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::int8_t>(addedPVP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "I2")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::int16_t>(addedPVP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "I4")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::int32_t>(addedPVP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "I8")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::int64_t>(addedPVP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "CI2")
        {
            ::getData(dest + it->second.getByteOffset(), addedPVP.find(it->first)->second.getComplex<std::int8_t>());
        }
        else if (it->second.getFormat() == "CI4")
        {
            ::getData(dest + it->second.getByteOffset(), addedPVP.find(it->first)->second.getComplex<std::int16_t>());
        }
        else if (it->second.getFormat() == "CI8")
        {
            ::getData(dest + it->second.getByteOffset(), addedPVP.find(it->first)->second.getComplex<std::int32_t>());
        }
        else if (it->second.getFormat() == "CI16")
        {
            ::getData(dest + it->second.getByteOffset(), addedPVP.find(it->first)->second.getComplex<std::int64_t>());
        }
        else if (it->second.getFormat() == "CF8")
        {
            ::getData(dest + it->second.getByteOffset(), addedPVP.find(it->first)->second.getComplex<float>());
        }
        else if (it->second.getFormat() == "CF16")
        {
            ::getData(dest + it->second.getByteOffset(), addedPVP.find(it->first)->second.getComplex<double>());
        }
        else
        {
            ::getData(dest + it->second.getByteOffset(), addedPVP.find(it->first)->second.str().c_str(), it->second.getByteSize());
        }
    }
}

/*
 * Initialize PVP Array with a data object
 */
PVPBlock::PVPBlock(const Pvp& p, const Data& d)
{
    mPvp = p;
    mNumBytesPerVector = d.getNumBytesPVPSet();

    mData.resize(d.getNumChannels());
    for (size_t ii = 0; ii < d.getNumChannels(); ++ii)
    {
        mData[ii].resize(d.getNumVectors(ii));
    }
 
    size_t calculateBytesPerVector = mPvp.getReqSetSize()*sizeof(double);
    if (six::Init::isUndefined<size_t>(mNumBytesPerVector) ||
        calculateBytesPerVector > mNumBytesPerVector)
    {
        std::ostringstream oss;
        oss << "PVP size specified in metadata: " << mNumBytesPerVector
            << " does not match PVP size calculated: " << calculateBytesPerVector;
        throw except::Exception(oss.str());
    }
}
PVPBlock::PVPBlock(const Metadata& metadata)
    : PVPBlock(*metadata.pvp.get(), metadata.data)
{
}


/*
 * Initialize PVP Array with custom parameters
 */
PVPBlock::PVPBlock(size_t numChannels,
                   const std::vector<size_t>& numVectors,
                   const Pvp& p) :
    mNumBytesPerVector(0),
    mPvp(p)
{
    mData.resize(numChannels);
    if(numChannels != numVectors.size())
    {
        throw except::Exception(Ctxt(
                "number of vector dims provided does not match number of channels"));
    }
    for (size_t ii = 0; ii < numChannels; ++ii)
    {
        mData[ii].resize(numVectors[ii]);
    }
    size_t calculateBytesPerVector = mPvp.getReqSetSize()*sizeof(double);
    if (six::Init::isUndefined<size_t>(mNumBytesPerVector) ||
        calculateBytesPerVector > mNumBytesPerVector)
    {
        mNumBytesPerVector = calculateBytesPerVector;
    }
}

PVPBlock::PVPBlock(size_t numChannels,
                   const std::vector<size_t>& numVectors,
                   const Pvp& pvp,
                   const std::vector<const void*>& data) :
    PVPBlock(numChannels, numVectors, pvp)
{
    if (data.size() != numChannels)
    {
        std::ostringstream msg;
        msg << "<" << numChannels << "> channels specified, "
            << "but `data` argument has <" << data.size() << "> channels";
        throw except::Exception(Ctxt(msg.str()));
    }

    for (size_t channel = 0; channel < numChannels; ++channel)
    {
        const std::byte* buf = static_cast<const std::byte*>(data[channel]);

        for (size_t vector = 0; vector < numVectors[channel]; ++vector)
        {
            mData[channel][vector].write(*this, mPvp, buf);
            buf += mPvp.sizeInBytes();
        }
    }
}

size_t PVPBlock::getNumBytesPVPSet() const
{
    return mNumBytesPerVector;
}

void PVPBlock::verifyChannelVector(size_t channel, size_t vector) const
{
    if (channel >= mData.size())
    {
        throw except::Exception(Ctxt(
                "Invalid channel number: " + std::to_string(channel)));
    }
    if (vector >= mData[channel].size())
    {
        throw except::Exception(Ctxt(
                "Invalid vector number: " + std::to_string(vector)));
    }
}

size_t PVPBlock::getPVPsize(size_t channel) const
{
    verifyChannelVector(channel, 0);
    return getNumBytesPVPSet() * mData[channel].size();
}

void PVPBlock::getPVPdata(size_t channel,
                          std::vector<std::byte>& data) const
{
    verifyChannelVector(channel, 0);
    data.resize(getPVPsize(channel));
    std::fill(data.begin(), data.end(), static_cast<std::byte>(0));

    getPVPdata(channel, data.data());
}

void PVPBlock::getPVPdata(size_t channel,
                          void* data) const
{
    verifyChannelVector(channel, 0);
    const size_t numBytes = getNumBytesPVPSet();
    auto ptr = static_cast<std::byte*>(data);

    for (size_t ii = 0;
         ii < mData[channel].size();
         ++ii, ptr += numBytes)
    {
        mData[channel][ii].read(mPvp, ptr);
    }
}

int64_t PVPBlock::load(io::SeekableInputStream& inStream,
                     int64_t startPVP,
                     int64_t sizePVP,
                     size_t numThreads)
{
    // Allocate the buffers
    size_t numBytesIn(0);

    // Compute the PVPBlock size per channel
    // (channels aren't necessarily the same size)
    for (size_t ii = 0; ii < mData.size(); ++ii)
    {
        numBytesIn += getPVPsize(ii);
    }

    if (numBytesIn != static_cast<size_t>(sizePVP))
    {
        std::ostringstream oss;
        oss << "PVPBlock::load: calculated PVP size(" << numBytesIn
            << ") != header PVP_DATA_SIZE(" << sizePVP << ")";
        throw except::Exception(Ctxt(oss.str()));
    }

    const bool swapToLittleEndian = std::endian::native == std::endian::little;

    // Seek to start of PVPBlock
    size_t totalBytesRead(0);
    inStream.seek(startPVP, io::Seekable::START);
    std::vector<std::byte> readBuf;
    const size_t numBytesPerVector = getNumBytesPVPSet();

    // Read the data for each channel
    for (size_t ii = 0; ii < mData.size(); ++ii)
    {
        readBuf.resize(getPVPsize(ii));
        if (!readBuf.empty())
        {
            auto const buf = readBuf.data();
            ptrdiff_t bytesThisRead = inStream.read(buf, readBuf.size());
            if (bytesThisRead == io::InputStream::IS_EOF)
            {
                std::ostringstream oss;
                oss << "EOF reached during PVP read for channel " << (ii);
                throw except::Exception(Ctxt(oss.str()));
            }
            totalBytesRead += bytesThisRead;

            // Input CRSD is always Big Endian; swap to Little Endian if
            // necessary
            if (swapToLittleEndian)
            {
                byteSwap(buf,
                         sizeof(double),
                         readBuf.size() / sizeof(double),
                         numThreads);
            }

            std::byte* ptr = buf;
            for (size_t jj = 0; jj < mData[ii].size(); ++jj, ptr += numBytesPerVector)
            {
                mData[ii][jj].write(*this, mPvp, ptr);
            }
        }
    }
    return totalBytesRead;
}
int64_t PVPBlock::load(io::SeekableInputStream& inStream, const FileHeader& fileHeader, size_t numThreads)
{
    return load(inStream, fileHeader.getPvpBlockByteOffset(), fileHeader.getPvpBlockSize(), numThreads);
}


std::pair<int64_t, double> PVPBlock::getRcvStart(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].rcvStart;
}

Vector3 PVPBlock::getRcvPos(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].rcvPos;
}

Vector3 PVPBlock::getRcvVel(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].rcvVel;
}

double PVPBlock::getFRCV1(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].frcv1;
}

double PVPBlock::getFRCV2(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].frcv2;
}

std::pair<int64_t, double> PVPBlock::getRefPhi0(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].refPhi0;
}

double PVPBlock::getRefFreq(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].refFreq;
}

double PVPBlock::getDFIC0(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].dfiC0;
}

double PVPBlock::getFICRate(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].ficRate;
}

Vector3 PVPBlock::getRcvACX(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].rcvACX;
}

Vector3 PVPBlock::getRcvACY(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].rcvACY;
}

Vector2 PVPBlock::getRcvEB(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].rcvEB;
}

std::int64_t PVPBlock::getSignal(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].signal;
}

double PVPBlock::getAmpSF(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].ampSF;
}

double PVPBlock::getDGRGC(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].dgrgc;
}

double PVPBlock::getTxPulseIndex(size_t channel, size_t set) const
{
    verifyChannelVector(channel, set);
    return mData[channel][set].txPulseIndex;
}

void PVPBlock::setRcvStart(std::pair<int64_t, double> value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].rcvStart = value;
}

void PVPBlock::setRcvPos(const crsd::Vector3& value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].rcvPos = value;
}

void PVPBlock::setRcvVel(const crsd::Vector3& value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].rcvVel = value;
}

void PVPBlock::setFRCV1(double value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].frcv1 = value;
}

void PVPBlock::setFRCV2(double value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].frcv2 = value;
}

void PVPBlock::setRefPhi0(std::pair<int64_t, double> value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].refPhi0 = value;
}

void PVPBlock::setRefFreq(double value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].refFreq = value;
}

void PVPBlock::setDFIC0(double value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].dfiC0 = value;
}

void PVPBlock::setFICRate(double value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].ficRate = value;
}

void PVPBlock::setRcvACX(const crsd::Vector3& value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].rcvACX = value;
}

void PVPBlock::setRcvACY(const crsd::Vector3& value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].rcvACY = value;
}

void PVPBlock::setRcvEB(const Vector2& value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].rcvEB = value;
}

void PVPBlock::setSignal(int64_t value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].signal = value;
}

void PVPBlock::setAmpSF(double value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].ampSF = value;
}

void PVPBlock::setDGRGC(double value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].dgrgc = value;
}

void PVPBlock::setTxPulseIndex(double value, size_t channel, size_t vector)
{
    verifyChannelVector(channel, vector);
    mData[channel][vector].txPulseIndex = value;
}

std::ostream& operator<< (std::ostream& os, const PVPBlock::PVPSet& p)
{
    os  << "  RcvStart       : " << p.rcvStart.first
        << " , "                 << p.rcvStart.second << "\n"
        << "  RcvPos         : " << p.rcvPos << "\n"
        << "  RcvVel         : " << p.rcvVel << "\n"
        << "  FRCV1          : " << p.frcv1 << "\n"
        << "  FRCV2          : " << p.frcv2 << "\n"
        << "  RefPhi0        : " << p.refPhi0.first
        << " , "                 << p.refPhi0.second << "\n"
        << "  RefFreq        : " << p.refFreq << "\n"
        << "  DFIC0          : " << p.dfiC0 << "\n"
        << "  FICRate        : " << p.ficRate << "\n"
        << "  RcvACX         : " << p.rcvACX << "\n"
        << "  RcvACY         : " << p.rcvACY << "\n"
        << "  RcvEB          : " << p.rcvEB << "\n"
        << "  SIGNAL         : " << p.signal << "\n"
        << "  AmpSF          : " << p.ampSF << "\n"
        << "  DGRGC          : " << p.dgrgc << "\n";

    if (!six::Init::isUndefined<int64_t>(p.txPulseIndex))
    {
        os << "  TxPulseIndex        :" << p.txPulseIndex << "\n";
    }

    for (auto it = p.addedPVP.begin(); it != p.addedPVP.end(); ++it)
    {
        os << "  Additional Parameter : " << it->second.str() << "\n";
    }
    return os;
}


std::ostream& operator<< (std::ostream& os, const PVPBlock& p)
{
    os << "PVPBlock:: \n";

    if (p.mData.empty())
    {
        os << "  mData : (empty) : \n";
    }
    else
    {
        for (size_t ii = 0; ii < p.mData.size(); ++ii)
        {
            os << "[" << ii << "] mPVPsize: " << p.getPVPsize(ii) << "\n";
        }

        for (size_t ii = 0; ii < p.mData.size(); ++ii)
        {
            if (p.mData[ii].empty())
            {
                os << "[" << ii << "] mData: (empty)\n";
            }
            else
            {
                for (size_t jj = 0; jj < p.mData[ii].size(); ++jj)
                {
                    if (p.mData[ii].empty())
                    {
                        os << "[" << ii << "] [" << jj << "] mData: (empty)\n";
                    }
                    else {
                        os << "[" << ii << "] [" << jj << "] mData: " << p.mData[ii][jj] << "\n";
                    }
                }
            }
        }
    }
    return os;
}
}
