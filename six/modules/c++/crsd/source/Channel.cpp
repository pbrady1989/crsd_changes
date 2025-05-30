/* =========================================================================
 * This file is part of cphd-c++
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
#include <crsd/Channel.h>
#include <six/Init.h>

namespace crsd
{

Polarization::Polarization() :
    ampH(six::Init::undefined<double>()),
    ampV(six::Init::undefined<double>()),
    phaseH(six::Init::undefined<double>()),
    phaseV(six::Init::undefined<double>())
{
}

DwellTimes::DwellTimes()
{
}

RcvRefPoint::RcvRefPoint() :
    ecf(six::Init::undefined<six::Vector3>()),
    iac(six::Init::undefined<six::Vector2>())
{
}

ChannelSARImage::ChannelSARImage() :
    refVectorPulseIndex(six::Init::undefined<size_t>())
{
}

ChannelParameter::ChannelParameter() :
    refVectorIndex(six::Init::undefined<size_t>()),
    refFreqFixed(six::Init::undefined<six::BooleanType>()),
    fRcvFixed(six::Init::undefined<six::BooleanType>()),
    signalNormal(six::Init::undefined<six::BooleanType>()),
    f0Ref(six::Init::undefined<double>()),
    fxBW(six::Init::undefined<double>()),
    fs(six::Init::undefined<double>()),
    bwInst(six::Init::undefined<double>()),
    rcvStartTime1(six::Init::undefined<double>()),
    rcvStartTime2(six::Init::undefined<double>()),
    fRcvMin(six::Init::undefined<double>()),
    fRcvMax(six::Init::undefined<double>()),
    rcvRefIrradiance(six::Init::undefined<double>()),
    rcvRefIrradianceErrorStdDev(six::Init::undefined<double>()),
    rcvRefLAtm(six::Init::undefined<double>()),
    pncrsd(six::Init::undefined<double>()),
    bncrsd(six::Init::undefined<double>())
{
}

Channel::Channel()
{
}

std::ostream& operator<< (std::ostream& os, const Polarization& p)
{
    os << "      polarizationID        : " << p.polarizationID << "\n"
    << "      ampH        : " << p.ampH << "\n"
    << "      ampV        : " << p.ampV << "\n"
    << "      phaseH        : " << p.phaseH << "\n"
    << "      phaseV       : " << p.phaseV << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const RcvRefPoint& t)
{
    os << "RcvRefPoint:: \n"
        << "  ECF              : " << t.ecf << "\n"
        << "  IAC              : " << t.iac << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const DwellTimes& d)
{
    os  << "      DwellTimes:: \n"
        << "      CODId          : " << d.codId << "\n"
        << "      DwellId        : " << d.dwellId << "\n"
        << "      DTAId          : " << d.dtaId << "\n";
        return os;
}

std::ostream& operator<< (std::ostream& os, const ChannelSARImage& t)
{
    os  << "      SARImage:: \n"
        << "      txID                 :" << t.txID
        << "      refVectorPulseIndex  :" << t.refVectorPulseIndex
        << "      txPolarization       :" << t.txPolarization
        << "      dwellTime            :" << t.dwellTime
        << "      imageArea            :" << t.imageArea << "\n";
        return os;
}

std::ostream& operator<< (std::ostream& os, const ChannelParameter& c)
{
    os << "    ChannelParameter:: \n"
        << "      Identifier   : " << c.identifier << "\n"
        << "      RefVectorIndex : " << c.refVectorIndex << "\n"
        << "      refFreqFixed      : " << c.refFreqFixed << "\n"
        << "      fRcvFixed     : " << c.fRcvFixed << "\n"
        << "      signalNormal     : " << c.signalNormal << "\n"
        << "      f0Ref : " << c.f0Ref << "\n"
        << "      fxBW : " << c.fxBW << "\n"
        << "      f0Ref : " << c.f0Ref << "\n"
        << "      fs : " << c.fs << "\n"
        << "      bwInst : " << c.bwInst << "\n"
        << "      rcvStartTime1 : " << c.rcvStartTime1 << "\n"
        << "      rcvStartTime2 : " << c.rcvStartTime2 << "\n"
        << "      fRcvMin : " << c.fRcvMin << "\n"
        << "      fRcvMax : " << c.fRcvMax << "\n"
        << "      rcvAPCId : " << c.rcvAPCId << "\n"
        << "      rcvAPATId : " << c.rcvAPATId << "\n"
        << "      rcvRefPoint : " << c.rcvRefPoint << "\n"
        << "      rcvPolarization : " << c.rcvPolarization << "\n"
        << "      rcvRefIrradiance : " << c.rcvRefIrradiance << "\n"
        << "      rcvRefIrradianceErrorStdDev : " << c.rcvRefIrradianceErrorStdDev << "\n"
        << "      rcvRefLAtm : " << c.rcvRefLAtm << "\n"
        << "      pncrsd : " << c.pncrsd << "\n"
        << "      bncrsd : " << c.bncrsd << "\n";
    if (c.sarImage.get())
    {
        os << *(c.sarImage) << "\n";
    }

    for (size_t ii = 0; ii < c.addedParameters.size(); ++ii)
    {
        os << "  Parameter name   : " << c.addedParameters[ii].getName() << "\n";
        os << "  Parameter value   : " << c.addedParameters[ii].str() << "\n";
    }
    
    return os;
}

std::ostream& operator<< (std::ostream& os, const Channel& c)
{
    os << "Channel:: \n"
        << "  RefChId          : " << c.refChId << "\n"
        << "  Parameters:: \n";
    for (size_t ii = 0; ii < c.parameters.size(); ++ii)
    {
        os << c.parameters[ii] << "\n";
    }
    return os;
}
}
