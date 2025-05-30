/* =========================================================================
 * This file is part of cphd-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2019, MDA Information Systems LLC
 *
 * cphd-c++ is free software; you can redistribute it and/or modify
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
#include <map>
#include <crsd/Data.h>
#include <six/Init.h>

namespace crsd
{

Data::Data()
{
}

bool Data::isCompressed() const
{
    if (!receiveParameters.get())
    {
        return false;
    }
    else
    {
        if (receiveParameters->signalCompression.get())
            return true;
        else 
            return false;
    }
}

Data::TxSequence::TxSequence() :
    numPulses(0),
    pppArrayByteOffset(0)
{
}

Data::TxSequence::TxSequence(size_t nPulses, size_t pppArrayOffset) :
    numPulses(nPulses),
    pppArrayByteOffset(pppArrayOffset)
{
}

Data::TxSequence::TxSequence(std::string identifier, size_t numPulses, 
                 size_t pppArrayByteOffset) :
    identifier(identifier),
    numPulses(numPulses),
    pppArrayByteOffset(pppArrayByteOffset)
{
}

Data::Transmit::Transmit() :
    numBytesPPP(0)
{
}

Data::Transmit::Transmit(std::string str, size_t bytesPPP) :
    identifier(str),
    numBytesPPP(bytesPPP)
{
}

Data::Receive::Receive() :
    numBytesPVP(0)
{
}

Data::Receive::Receive(size_t bytesPVP) :
    numBytesPVP(bytesPVP)
{
}

Data::Processing::Processing()
{
}

Data::Processing::Processing(std::string str) :
    type(str)
{
}

Data::SignalCompression::SignalCompression() :
    compressedSignalSize(0)
{
}

Data::SignalCompression::SignalCompression(std::string str, size_t compressedSize) :
    identifier(str),
    compressedSignalSize(compressedSize)
{
}

Data::Channel::Channel() :
    numVectors(0),
    numSamples(0),
    signalArrayByteOffset(0),
    pvpArrayByteOffset(0),
    compressedSignalSize(six::Init::undefined<size_t>())
{
}

Data::Channel::Channel(size_t vectors, size_t samples) :
    numVectors(vectors),
    numSamples(samples),
    signalArrayByteOffset(0),
    pvpArrayByteOffset(0),
    compressedSignalSize(six::Init::undefined<size_t>())
{
}

Data::Channel::Channel(size_t vectors, size_t samples,
        size_t signalByteOffset, size_t pvpByteOffset,size_t compressedSize) :
    numVectors(vectors),
    numSamples(samples),
    signalArrayByteOffset(signalByteOffset),
    pvpArrayByteOffset(pvpByteOffset),
    compressedSignalSize(compressedSize)
{
}

Data::SupportArray::SupportArray() :
    numRows(0),
    numCols(0),
    bytesPerElement(0),
    arrayByteOffset(0)
{
}

Data::SupportArray::SupportArray(const std::string& id, size_t rows, size_t cols,
                   size_t numBytes, size_t offset) :
    identifier(id),
    numRows(rows),
    numCols(cols),
    bytesPerElement(numBytes),
    arrayByteOffset(offset)
{
}

Data::SupportArray Data::getSupportArrayById(const std::string& id) const
{
    if(supportArrayMap.count(id) != 1)
    {
        std::ostringstream ostr;
        ostr << "ID: " << id << " is not a valid identifier";
        throw except::Exception(ostr.str());
    }
    return supportArrayMap.find(id)->second;
}

void Data::setSupportArray(const std::string& id, size_t numRows,
                           size_t numCols, size_t numBytes,
                           int64_t offset)
{
    if (supportArrayMap.count(id))
    {
        std::ostringstream ostr;
        ostr << "Identifier " << id << " is not unique";
        throw except::Exception(ostr.str());
    }

    if (mOffsetMap.count(offset))
    {
        std::ostringstream ostr;
        ostr << "Offset " << offset << " is not unique";
        throw except::Exception(ostr.str());
    }

    // Add to ordered map
    mOffsetMap.insert(std::pair<int64_t,size_t>(offset,numBytes));

    // Validate offset and size
    if (mOffsetMap.find(offset) != mOffsetMap.begin())
    {
        if (offset < (int64_t)((--mOffsetMap.find(offset))->first + (--mOffsetMap.find(offset))->second))
        {
            std::ostringstream ostr;
            ostr << "Invalid size or offset of support array given for id: " << id;
            throw except::Exception(ostr.str());
        }
    }
    if (mOffsetMap.upper_bound(offset) != mOffsetMap.end())
    {
        if ((int64_t)(offset + (numRows * numCols * numBytes)) > mOffsetMap.upper_bound(offset)->first)
        {
            std::ostringstream ostr;
            ostr << "Invalid size or offset of support array given for id: " << id;
            throw except::Exception(ostr.str());
        }
    }

    // Add to supportArrayMap after validation
    std::pair<std::string,Data::SupportArray> entry(id,Data::SupportArray(id, numRows, numCols, numBytes, offset));
    supportArrayMap.insert(entry);
}

//! Getter functions
size_t Data::getNumVectors(size_t channel) const
{
    verifyChannelInRange(channel);
    return receiveParameters->channels[channel].getNumVectors();
}
//! Getter functions
size_t Data::getNumPulses(size_t index) const
{
    verifyTxSequenceInRange(index);
    return transmitParameters->txSequence[index].getNumPulses();
}
size_t Data::getNumSamples(size_t channel) const
{
    verifyChannelInRange(channel);
    return receiveParameters->channels[channel].getNumSamples();
}
size_t Data::getCompressedSignalSize(size_t channel) const
{
    verifyChannelInRange(channel);
    return receiveParameters->channels[channel].getCompressedSignalSize();
}
size_t Data::getSignalSize(size_t channel) const
{
    verifyChannelInRange(channel);
    return getNumVectors(channel) *
           getNumSamples(channel) *
           getNumBytesPerSample();
}

size_t Data::getAllSupportSize() const
{
    size_t size = 0;
    for (auto it = supportArrayMap.begin(); it != supportArrayMap.end(); ++it)
    {
        size += getSupportArrayById(it->first).getSize();
    }
    return size;
}

/*
 * Check if channel is in range
 */
void Data::verifyChannelInRange(size_t channel) const
{
    if (receiveParameters.get())
    {
        if (channel >= receiveParameters->channels.size())
        {
        std::ostringstream ostr;
        ostr << "Channel provided is " << channel << "\n"
                << "while only " << receiveParameters->channels.size()
                << " channels exist \n";
        throw except::Exception(ostr.str());
        }
    }
    else
    {
        std::ostringstream ostr;
        ostr << "No receive parameters exist for this CRSD!! \n";
        throw except::Exception(ostr.str());
    }
}


/*
 * Check if channel is in range
 */
void Data::verifyTxSequenceInRange(size_t index) const
{

    if (transmitParameters.get())
    {
        if (index >= transmitParameters->txSequence.size())
        {
        std::ostringstream ostr;
        ostr << "TxSequence provided is " << index << "\n"
                << "while only " << transmitParameters->txSequence.size()
                << " tx sequences exist \n";
        throw except::Exception(ostr.str());
        }
    }
    else
    {
        std::ostringstream ostr;
        ostr << "No transmit parameters exist for this CRSD!! \n";
        throw except::Exception(ostr.str());
    }
}

std::ostream& operator<< (std::ostream& os, const Data::SupportArray& s)
{
    os << "    Identifier        : " << s.identifier << "\n"
        << "    NumRows        : " << s.numRows << "\n"
        << "    NumCols        : " << s.numCols << "\n"
        << "    BytesPerElement : " << s.bytesPerElement << "\n"
        << "    ArrayByteOffset : " << s.arrayByteOffset << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const Data::Channel& c)
{
    os << "  Channel:: \n"
        << "    Identifier     : " << c.identifier << "\n"
        << "    NumVectors     : " << c.numVectors << "\n"
        << "    NumSamples     : " << c.numSamples << "\n"
        << "    SignalArrayByteOffset : " << c.signalArrayByteOffset << "\n"
        << "    PVPArrayByteOffset : " << c.pvpArrayByteOffset << "\n"
        << "    CompressedSignalSize : " << c.compressedSignalSize << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const Data::TxSequence& c)
{
    os << "  TxSequence:: \n"
        << "    Identifier     : " << c.identifier << "\n"
        << "    NumPulses     : " << c.numPulses << "\n"
        << "    PPPArrayByteOffset : " << c.pppArrayByteOffset << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const Data& d)
{
    os << "Data:: \n";
    if (d.receiveParameters.get())
    {
        os << " ReceiveParameters:: \n"
           << "   numBytesPVP:" << d.receiveParameters->getNumBytesPVP() 
           << "   numCRSDChannels:" << d.receiveParameters->channels.size() << "\n";
        for (size_t ii = 0; ii < d.receiveParameters->channels.size(); ++ii)
        {
            os << d.receiveParameters->channels[ii] << "\n";
        }
        if (d.receiveParameters->signalCompression.get())
        {
            os << "   signalCompressionParameters:: \n"
               << "     identifier:" << d.receiveParameters->signalCompression->getIdentifier()
               << "     compressedSignalSize:" << d.receiveParameters->signalCompression->getCompressedSignalSize() << "\n";
        }
    }
    if (d.transmitParameters.get())
    {
        os << " TransmitParameters:: \n"
           << "   numBytesPPP:" << d.transmitParameters->getNumBytesPPP() 
           << "   numTxSequences:" << d.transmitParameters->txSequence.size() << "\n";
        for (size_t ii = 0; ii < d.transmitParameters->txSequence.size(); ++ii)
        {
            os << d.transmitParameters->txSequence[ii] << "\n";
        }
    }
    for (auto it = d.supportArrayMap.begin(); it != d.supportArrayMap.end(); ++it)
    {
        os << "  SupportArrays:: \n"
            << d.getSupportArrayById(it->first) << "\n";
    }
    return os;
}
}
