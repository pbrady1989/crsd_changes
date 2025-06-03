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
#include <crsd/Metadata.h>
#include <crsd/Enums.h>

namespace crsd
{

Metadata::Metadata()
{
}

Metadata::Metadata(CRSDType type) :
    mType(type)
{
    if (mType == CRSDType::SAR)
    {
        // Set up the correct sections
        global.transmitParameters.reset(new crsd::TransmitParameters());
        global.receiveParameters.reset(new crsd::ReceiveParameters());
        receiveInfo.reset(new crsd::TransmitInfo());
        transmitInfo.reset(new crsd::TransmitInfo());
        sarInfo.reset(new crsd::SARInfo());
        channel.reset(new crsd::Channel());
        channel->parameters.resize(1);
        channel->parameters[0].sarImage.reset(new crsd::ChannelSARImage());
        ppp.reset(new crsd::Ppp());
        pvp.reset(new crsd::Pvp());
        data.setSupportArray("",0,0,0,0);
        data.transmitParameters.reset(new crsd::Data::Transmit());
        data.receiveParameters.reset(new crsd::Data::Receive());    
        txSequence.reset(new crsd::TxSequence());   
        txSequence->parameters.resize(1);
        referenceGeometry.sarParameters.reset(new crsd::SARImage()); 
        supportArray.antGainPhase.resize(1);
        supportArray.fxResponseArray.resize(1);     
    }
    else if (mType == CRSDType::TX)
    {
        // Set up the correct sections
        global.transmitParameters.reset(new crsd::TransmitParameters());
        transmitInfo.reset(new crsd::TransmitInfo());
        ppp.reset(new crsd::Ppp());
        data.setSupportArray("",0,0,0,0);
        data.transmitParameters.reset(new crsd::Data::Transmit());
        txSequence.reset(new crsd::TxSequence()); 
        txSequence->parameters.resize(1);
        referenceGeometry.txParameters.reset(new crsd::OneWayParams());    
        supportArray.antGainPhase.resize(1);     
        supportArray.fxResponseArray.resize(1);
    }
    else if (mType == CRSDType::RCV)
    {
        // Set up the correct sections
        global.receiveParameters.reset(new crsd::ReceiveParameters());
        receiveInfo.reset(new crsd::TransmitInfo());
        channel.reset(new crsd::Channel());
        channel->parameters.resize(1);
        pvp.reset(new crsd::Pvp());
        data.setSupportArray("",0,0,0,0);
        data.receiveParameters.reset(new crsd::Data::Receive()); 
        referenceGeometry.rcvParameters.reset(new crsd::OneWayParams());   
        supportArray.antGainPhase.resize(1);     
    }
}

size_t Metadata::getNumChannels() const
{
    return data.getNumChannels();
}

size_t Metadata::getNumVectors(size_t channel_) const
{
    return data.getNumVectors(channel_);
}

size_t Metadata::getNumSamples(size_t channel_) const
{
    return data.getNumSamples(channel_);
}

size_t Metadata::getNumBytesPerSample() const
{
    return data.getNumBytesPerSample();
}

size_t Metadata::getCompressedSignalSize(size_t channel_) const
{
    return data.getCompressedSignalSize(channel_);
}

bool Metadata::isCompressed() const
{
    return data.isCompressed();
}

std::string Metadata::getVersion() const
{
    return mVersion;
}
void Metadata::setVersion(const std::string& version)
{
    mVersion = version;
}

CRSDType Metadata::getType() const
{
    return mType;
}

void Metadata::setType(const CRSDType& type)
{
    mType = type;
}

bool Metadata::operator==(const Metadata& other) const
{
    return global == other.global &&
           sceneCoordinates == other.sceneCoordinates &&
           data == other.data &&
           channel == other.channel &&
           ppp == other.ppp &&
           pvp == other.pvp &&
           dwell == other.dwell &&
           referenceGeometry == other.referenceGeometry &&
           supportArray == other.supportArray &&
           antenna == other.antenna &&
           txSequence == other.txSequence &&
           errorParameters == other.errorParameters &&
           productInfo == other.productInfo &&
           sarInfo == other.sarInfo &&
           receiveInfo == other.receiveInfo &&
           transmitInfo == other.transmitInfo &&
           geoInfo == other.geoInfo;
}

std::ostream& operator<< (std::ostream& os, const Metadata& d)
{
    os << "Metadata:: \n"
        << d.productInfo << "\n"
        << d.global << "\n"
        << d.sceneCoordinates << "\n"
        << d.data << "\n"
        << d.supportArray << "\n"
        << d.antenna << "\n";

    if (d.dwell.get())
    {
        os << *(d.dwell) << "\n";
    }
    if (d.receiveInfo.get())
    {
        os << *(d.receiveInfo) << "\n";
    }
    if (d.channel.get())
    {
        os << *(d.channel) << "\n";
    }
    if (d.pvp.get())
    {
        os << *(d.pvp) << "\n";
    }
    if (d.ppp.get())
    {
        os << *(d.ppp) << "\n";
    }
    if (d.transmitInfo.get())
    {
        os << *(d.transmitInfo) << "\n";
    }
    if (d.txSequence.get())
    {
        os << *(d.txSequence) << "\n";
    }
    if (d.sarInfo.get())
    {
        os << *(d.sarInfo) << "\n";
    }
    if (d.errorParameters.get())
    {
        os << *(d.errorParameters) << "\n";
    }
    for (size_t ii = 0; ii < d.geoInfo.size(); ++ii)
    {
        os << d.geoInfo[ii] << "\n";
    }
    return os;
}
}
