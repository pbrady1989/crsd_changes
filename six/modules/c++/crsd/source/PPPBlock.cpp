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
#include <crsd/PPPBlock.h>
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

PPPBlock::PPPSet::PPPSet() :
    txTime(six::Init::undefined<int64_t>(),
             six::Init::undefined<double>()),
    txPos(six::Init::undefined<Vector3>()),
    txVel(six::Init::undefined<Vector3>()),
    fx1(six::Init::undefined<double>()),
    fx2(six::Init::undefined<double>()),
    txmt(six::Init::undefined<double>()),
    phiX0(six::Init::undefined<int64_t>(),
            six::Init::undefined<double>()),
    fxFreq0(six::Init::undefined<double>()),
    fxRate(six::Init::undefined<double>()),
    txRadInt(six::Init::undefined<double>()),
    txACX(six::Init::undefined<Vector3>()),
    txACY(six::Init::undefined<Vector3>()),
    txEB(six::Init::undefined<Vector2>()),
    fxResponseIndex(six::Init::undefined<int64_t>())
{
}

void PPPBlock::PPPSet::write(const PPPBlock& pppBlock, const Ppp& p, const sys::byte* input)
{
     double tmpVal;
    ::setData(input + p.txTime.getByteOffset(), tmpVal);
    txTime.first = static_cast<int64_t>(tmpVal);
    ::setData(input + p.txTime.getByteOffset() + sizeof(txTime.first), txTime.second);
    ::setData(input + p.txPos.getByteOffset(), txPos);
    ::setData(input + p.txVel.getByteOffset(), txVel);
    ::setData(input + p.fx1.getByteOffset(), fx1);
    ::setData(input + p.fx2.getByteOffset(), fx2);
    ::setData(input + p.txmt.getByteOffset(), txmt);
    ::setData(input + p.phiX0.getByteOffset(), tmpVal);
    phiX0.first = static_cast<int64_t>(tmpVal);
    ::setData(input + p.phiX0.getByteOffset() + sizeof(phiX0.first), phiX0.second);
    ::setData(input + p.fxFreq0.getByteOffset(), fxFreq0);
    ::setData(input + p.fxRate.getByteOffset(), fxRate);
    ::setData(input + p.txRadInt.getByteOffset(), txRadInt);
    ::setData(input + p.txACX.getByteOffset(), txACX);
    ::setData(input + p.txACY.getByteOffset(), txACY);
    ::setData(input + p.txEB.getByteOffset(), txEB);
    ::setData(input + p.fxResponseIndex.getByteOffset(), fxResponseIndex);
     if (pppBlock.hasXMIndex())
    {
        xmIndex.reset(new int64_t());
        ::setData(input + p.xmIndex.getByteOffset(), *xmIndex);
    }

    for (auto it = p.addedPPP.begin(); it != p.addedPPP.end(); ++it)
    {
        if (it->second.getFormat() == "F4")
        {
            float val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "F8")
        {
            double val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "U1")
        {
            std::uint8_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "U2")
        {
            std::uint16_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "U4")
        {
            std::uint32_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "U8")
        {
            std::int64_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "I1")
        {
            std::int8_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "I2")
        {
            std::int16_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "I4")
        {
            std::int32_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "I8")
        {
            std::int64_t val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "CI2")
        {
            std::complex<std::int8_t> val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "CI4")
        {
            std::complex<std::int16_t> val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "CI8")
        {
            std::complex<std::int32_t> val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "CI16")
        {
            std::complex<std::int64_t> val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "CF8")
        {
            std::complex<float> val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else if (it->second.getFormat() == "CF16")
        {
            std::complex<double> val;
            ::setData(input + it->second.getByteOffset(), val);
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
        else
        {
            const auto pVal = input + it->second.getByteOffset();
            std::string val;
            val.assign(reinterpret_cast<const char*>(pVal),
                       it->second.getByteSize());
            addedPPP[it->first] = six::Parameter();
            addedPPP.find(it->first)->second.setValue(val);
        }
    }
}

void PPPBlock::PPPSet::read(const Ppp& p, sys::ubyte* dest_) const
{
    auto dest = reinterpret_cast<std::byte*>(dest_);
    ::getData(dest + p.txTime.getByteOffset(), static_cast<double>(txTime.first));
    ::getData(dest + p.txTime.getByteOffset() + sizeof(txTime.first), txTime.second);
    ::getData(dest + p.txPos.getByteOffset(), txPos);
    ::getData(dest + p.txVel.getByteOffset(), txVel);
    ::getData(dest + p.fx1.getByteOffset(), fx1);
    ::getData(dest + p.fx2.getByteOffset(), fx2);
    ::getData(dest + p.txmt.getByteOffset(), txmt);
    ::getData(dest + p.phiX0.getByteOffset(),static_cast<double>(phiX0.first));
    ::getData(dest + p.phiX0.getByteOffset() + sizeof(phiX0.first), phiX0.second);
    ::getData(dest + p.fxFreq0.getByteOffset(), fxFreq0);
    ::getData(dest + p.fxRate.getByteOffset(), fxRate);
    ::getData(dest + p.txRadInt.getByteOffset(), txRadInt);
    ::getData(dest + p.txACX.getByteOffset(), txACX);
    ::getData(dest + p.txACY.getByteOffset(), txACY);
    ::getData(dest + p.txEB.getByteOffset(), txEB);
    ::getData(dest + p.fxResponseIndex.getByteOffset(), fxResponseIndex);
    if (xmIndex.get())
    {
        ::getData(dest + p.xmIndex.getByteOffset(), *xmIndex);
    }

    if (addedPPP.size() != p.addedPPP.size())
    {
        throw except::Exception(Ctxt(
            "Incorrect number of additional parameters instantiated"));
    }
    for (auto it = p.addedPPP.begin(); it != p.addedPPP.end(); ++it)
    {
        if (it->second.getFormat() == "F4")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<float>(addedPPP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "F8")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<double>(addedPPP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "U1")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::uint8_t>(addedPPP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "U2")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::uint16_t>(addedPPP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "U4")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::uint32_t>(addedPPP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "U8")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::int64_t>(addedPPP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "I1")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::int8_t>(addedPPP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "I2")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::int16_t>(addedPPP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "I4")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::int32_t>(addedPPP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "I8")
        {
            ::getData(dest + it->second.getByteOffset(), static_cast<std::int64_t>(addedPPP.find(it->first)->second));
        }
        else if (it->second.getFormat() == "CI2")
        {
            ::getData(dest + it->second.getByteOffset(), addedPPP.find(it->first)->second.getComplex<std::int8_t>());
        }
        else if (it->second.getFormat() == "CI4")
        {
            ::getData(dest + it->second.getByteOffset(), addedPPP.find(it->first)->second.getComplex<std::int16_t>());
        }
        else if (it->second.getFormat() == "CI8")
        {
            ::getData(dest + it->second.getByteOffset(), addedPPP.find(it->first)->second.getComplex<std::int32_t>());
        }
        else if (it->second.getFormat() == "CI16")
        {
            ::getData(dest + it->second.getByteOffset(), addedPPP.find(it->first)->second.getComplex<std::int64_t>());
        }
        else if (it->second.getFormat() == "CF8")
        {
            ::getData(dest + it->second.getByteOffset(), addedPPP.find(it->first)->second.getComplex<float>());
        }
        else if (it->second.getFormat() == "CF16")
        {
            ::getData(dest + it->second.getByteOffset(), addedPPP.find(it->first)->second.getComplex<double>());
        }
        else
        {
            ::getData(dest + it->second.getByteOffset(), addedPPP.find(it->first)->second.str().c_str(), it->second.getByteSize());
        }
    }
}

/*
 * Initialize PPP Array with a data object
 */
PPPBlock::PPPBlock(const Ppp& p, const Data& d) :
    mXMIndexEnabled(!six::Init::isUndefined<size_t>(p.xmIndex.getOffset()))
{
    mPpp = p;
    mNumBytesPerPulse = d.getNumBytesPPPSet();
    mData.resize(d.getNumTxSequences());
    for (size_t ii = 0; ii < d.getNumTxSequences(); ++ii)
    {
        mData[ii].resize(d.getNumPulses(ii));
    }
    size_t calculateBytesPerPulse = mPpp.getReqSetSize()*sizeof(double);

    if (six::Init::isUndefined<size_t>(mNumBytesPerPulse) ||
        calculateBytesPerPulse > mNumBytesPerPulse)
    {
        std::ostringstream oss;
        oss << "PPP size specified in metadata: " << mNumBytesPerPulse
            << " does not match PPP size calculated: " << calculateBytesPerPulse;
        throw except::Exception(oss.str());
    }
}

PPPBlock::PPPBlock(const Metadata& metadata)
    : PPPBlock(*metadata.ppp.get(), metadata.data)
{
}

/*
 * Initialize PPP Array with custom parameters
 */
PPPBlock::PPPBlock(size_t numTxSequences,
                   const std::vector<size_t>& numPulses,
                   const Ppp& p) :
    mXMIndexEnabled(!six::Init::isUndefined<size_t>(p.xmIndex.getOffset())),
    mNumBytesPerPulse(0),
    mPpp(p)
{
    mData.resize(numTxSequences);
    if(numTxSequences != numPulses.size())
    {
        throw except::Exception(Ctxt(
                "number of vector dims provided does not match number of pulses"));
    }
    for (size_t ii = 0; ii < numTxSequences; ++ii)
    {
        mData[ii].resize(numPulses[ii]);
    }
    size_t calculateBytesPerPulse = mPpp.getReqSetSize()*sizeof(double);
    if (six::Init::isUndefined<size_t>(mNumBytesPerPulse) ||
        calculateBytesPerPulse > mNumBytesPerPulse)
    {
        mNumBytesPerPulse = calculateBytesPerPulse;
    }
}

PPPBlock::PPPBlock(size_t numTxSequences,
                   const std::vector<size_t>& numPulses,
                   const Ppp& ppp,
                   const std::vector<const void*>& data) :
    PPPBlock(numTxSequences, numPulses, ppp)
{
    if (data.size() != numTxSequences)
    {
        std::ostringstream msg;
        msg << "<" << numTxSequences << "> pulses specified, "
            << "but `data` argument has <" << data.size() << "> pulses";
        throw except::Exception(Ctxt(msg.str()));
    }

    for (size_t pulse = 0; pulse < numTxSequences; ++pulse)
    {
        const std::byte* buf = static_cast<const std::byte*>(data[pulse]);

        for (size_t vector = 0; vector < numPulses[pulse]; ++vector)
        {
            mData[pulse][vector].write(*this, mPpp, buf);
            buf += mPpp.sizeInBytes();
        }
    }
}

size_t PPPBlock::getNumBytesPPPSet() const
{
    return mNumBytesPerPulse;
}

void PPPBlock::verifyTxSequencePulse(size_t pulse, size_t vector) const
{
    if (pulse >= mData.size())
    {
        throw except::Exception(Ctxt(
                "Invalid pulse number: " + std::to_string(pulse)));
    }
    if (vector >= mData[pulse].size())
    {
        throw except::Exception(Ctxt(
                "Invalid vector number: " + std::to_string(vector)));
    }
}

size_t PPPBlock::getPPPsize(size_t pulse) const
{
    verifyTxSequencePulse(pulse, 0);
    return getNumBytesPPPSet() * mData[pulse].size();
}

void PPPBlock::getPPPdata(size_t pulse,
                          std::vector<std::byte>& data) const
{
    verifyTxSequencePulse(pulse, 0);
    data.resize(getPPPsize(pulse));
    std::fill(data.begin(), data.end(), static_cast<std::byte>(0));

    getPPPdata(pulse, data.data());
}

void PPPBlock::getPPPdata(size_t pulse,
                          void* data) const
{
    verifyTxSequencePulse(pulse, 0);
    const size_t numBytes = getNumBytesPPPSet();
    auto ptr = static_cast<std::byte*>(data);

    for (size_t ii = 0;
         ii < mData[pulse].size();
         ++ii, ptr += numBytes)
    {
        mData[pulse][ii].read(mPpp, ptr);
    }
}

int64_t PPPBlock::load(io::SeekableInputStream& inStream,
                     int64_t startPPP,
                     int64_t sizePPP,
                     size_t numThreads)
{
    // Allocate the buffers
    size_t numBytesIn(0);

    // Compute the PPPBlock size per pulse
    // (pulses aren't necessarily the same size)
    for (size_t ii = 0; ii < mData.size(); ++ii)
    {
        numBytesIn += getPPPsize(ii);
    }

    if (numBytesIn != static_cast<size_t>(sizePPP))
    {
        std::ostringstream oss;
        oss << "PPPBlock::load: calculated PPP size(" << numBytesIn
            << ") != header PPP_DATA_SIZE(" << sizePPP << ")";
        throw except::Exception(Ctxt(oss.str()));
    }

    const bool swapToLittleEndian = std::endian::native == std::endian::little;

    // Seek to start of PPPBlock
    size_t totalBytesRead(0);
    inStream.seek(startPPP, io::Seekable::START);
    std::vector<std::byte> readBuf;
    const size_t numBytesPerPulse = getNumBytesPPPSet();

    // Read the data for each pulse
    for (size_t ii = 0; ii < mData.size(); ++ii)
    {
        readBuf.resize(getPPPsize(ii));
        if (!readBuf.empty())
        {
            auto const buf = readBuf.data();
            ptrdiff_t bytesThisRead = inStream.read(buf, readBuf.size());
            if (bytesThisRead == io::InputStream::IS_EOF)
            {
                std::ostringstream oss;
                oss << "EOF reached during PPP read for pulse " << (ii);
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
            for (size_t jj = 0; jj < mData[ii].size(); ++jj, ptr += numBytesPerPulse)
            {
                mData[ii][jj].write(*this, mPpp, ptr);
            }
        }
    }
    return totalBytesRead;
}
int64_t PPPBlock::load(io::SeekableInputStream& inStream, const FileHeader& fileHeader, size_t numThreads)
{
    return load(inStream, fileHeader.getPppBlockByteOffset(), fileHeader.getPppBlockSize(), numThreads);
}


std::pair<int64_t, double> PPPBlock::getTxStart(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].txTime;
}

Vector3 PPPBlock::getTxPos(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].txPos;
}

Vector3 PPPBlock::getTxVel(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].txVel;
}

double PPPBlock::getFX1(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].fx1;
}

double PPPBlock::getTXMT(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].txmt;
}

double PPPBlock::getFX2(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].fx2;
}

std::pair<int64_t, double> PPPBlock::getPhiX0(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].phiX0;
}

double PPPBlock::getFxFreq0(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].fxFreq0;
}

double PPPBlock::getFxRate(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].fxRate;
}

double PPPBlock::getTxRadInt(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].txRadInt;
}

Vector3 PPPBlock::getTxACX(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].txACX;
}

Vector3 PPPBlock::getTxACY(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].txACY;
}

Vector2 PPPBlock::getTxEB(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].txEB;
}

std::int64_t PPPBlock::getFxResponseIndex(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    return mData[pulse][set].fxResponseIndex;
}

std::int64_t  PPPBlock::getXMIndex(size_t pulse, size_t set) const
{
    verifyTxSequencePulse(pulse, set);
    if (mData[pulse][set].xmIndex.get())
    {
        return *mData[pulse][set].xmIndex;
    }
    throw except::Exception(Ctxt(
                    "Parameter was not set"));
}

void PPPBlock::setTxStart(std::pair<int64_t, double> value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].txTime = value;
}

void PPPBlock::setTxPos(const crsd::Vector3& value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].txPos = value;
}

void PPPBlock::setTxVel(const crsd::Vector3& value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].txVel = value;
}

void PPPBlock::setFX1(double value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].fx1 = value;
}

void PPPBlock::setFX2(double value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].fx2 = value;
}

void PPPBlock::setTXMT(double value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].txmt = value;
}

void PPPBlock::setPhiX0(std::pair<int64_t, double> value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].phiX0 = value;
}

void PPPBlock::setFxFreq0(double value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].fxFreq0 = value;
}

void PPPBlock::setFxRate(double value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].fxRate = value;
}

void PPPBlock::setTxRadInt(double value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].txRadInt = value;
}

void PPPBlock::setTxACX(const crsd::Vector3& value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].txACX = value;
}

void PPPBlock::setTxACY(const crsd::Vector3& value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].txACY = value;
}

void PPPBlock::setTxEB(const Vector2& value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].txEB = value;
}

void PPPBlock::setFxResponseIndex(std::int64_t  value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    mData[pulse][vector].fxResponseIndex = value;
}

void PPPBlock::setXMIndex(std::int64_t value, size_t pulse, size_t vector)
{
    verifyTxSequencePulse(pulse, vector);
    if (hasXMIndex())
    {
        mData[pulse][vector].xmIndex.reset(new std::int64_t(value));
        return;
    }
    throw except::Exception(Ctxt(
                            "Parameter was not specified in XML"));
}

std::ostream& operator<< (std::ostream& os, const PPPBlock::PPPSet& p)
{
    os  << "  TxTime         : " << p.txTime.first
        << " , "                 << p.txTime.second << "\n"
        << "  TxPos          : " << p.txPos << "\n"
        << "  TxVel          : " << p.txVel << "\n"
        << "  FRCV1          : " << p.fx1 << "\n"
        << "  FRCV2          : " << p.fx2 << "\n"
        << "  TXMT           : " << p.txmt << "\n"
        << "  PhiX0          : " << p.phiX0.first
        << " , "                 << p.phiX0.second << "\n"
        << "  FxFreq0        : " << p.fxFreq0 << "\n"
        << "  FxRate         : " << p.fxRate << "\n"
        << "  TxRadInt       : " << p.txRadInt << "\n"
        << "  RcvACX         : " << p.txACX << "\n"
        << "  RcvACY         : " << p.txACY << "\n"
        << "  RcvEB          : " << p.txEB << "\n"
        << "  SIGNAL         : " << p.fxResponseIndex << "\n";
    if (p.xmIndex.get())
    {
        os << "  XMIndex       : " << *p.xmIndex << "\n";
    }

    for (auto it = p.addedPPP.begin(); it != p.addedPPP.end(); ++it)
    {
        os << "  Additional Parameter : " << it->second.str() << "\n";
    }
    return os;
}


std::ostream& operator<< (std::ostream& os, const PPPBlock& p)
{
    os << "PPPBlock:: \n";

    if (p.mData.empty())
    {
        os << "  mData : (empty) : \n";
    }
    else
    {
        for (size_t ii = 0; ii < p.mData.size(); ++ii)
        {
            os << "[" << ii << "] mPPPsize: " << p.getPPPsize(ii) << "\n";
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
