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
#include <crsd/TxSequence.h>
#include <six/Init.h>

namespace crsd
{

TxPolarization::TxPolarization() :
    ampH(six::Init::undefined<double>()),
    ampV(six::Init::undefined<double>()),
    phaseH(six::Init::undefined<double>()),
    phaseV(six::Init::undefined<double>())
{
}

TxRefPoint::TxRefPoint() :
    ecf(six::Init::undefined<six::Vector3>()),
    iac(six::Init::undefined<six::Vector2>())
{
}

TxSequenceParameter::TxSequenceParameter() :
    refPulseIndex(six::Init::undefined<size_t>()),
    fxBWFixed(six::Init::undefined<six::BooleanType>()),
    fxC(six::Init::undefined<double>()),
    fxBW(six::Init::undefined<double>()),
    txmtMin(six::Init::undefined<double>()),
    txmtMax(six::Init::undefined<double>()),
    txTime1(six::Init::undefined<double>()),
    txTime2(six::Init::undefined<double>()),
    txRefRadIntensity(six::Init::undefined<double>()),
    txRefRadIntensityStdDev(six::Init::undefined<double>()),
    txRefLAtm(six::Init::undefined<double>())
{
}

TxSequence::TxSequence()
{
}

std::ostream& operator<< (std::ostream& os, const TxPolarization& p)
{
    os << "      polarizationID        : " << p.polarizationID << "\n"
    << "      ampH          : " << p.ampH << "\n"
    << "      ampV          : " << p.ampV << "\n"
    << "      phaseH        : " << p.phaseH << "\n"
    << "      phaseV        : " << p.phaseV << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const TxRefPoint& t)
{
    os << "TxRefPoint:: \n"
        << "  ECF              : " << t.ecf << "\n"
        << "  IAC              : " << t.iac << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const TxSequenceParameter& c)
{
    os << "    ChannelParameter:: \n"
        << "      Identifier              : " << c.identifier << "\n"
        << "      RefPulseIndex           : " << c.refPulseIndex << "\n"
        << "      XMId                    : " << c.xmid << "\n"
        << "      FxResponseId            : " << c.fxResponseId << "\n"
        << "      FxBWFixed               : " << c.fxBWFixed << "\n"
        << "      FxC                     : " << c.fxC << "\n"
        << "      FxBW                    : " << c.fxBW << "\n"
        << "      TxmtMin                 : " << c.txmtMin << "\n"
        << "      TxmtMax                 : " << c.txmtMax << "\n"
        << "      TxTime1                 : " << c.txTime1 << "\n"
        << "      TxTime2                 : " << c.txTime2 << "\n"
        << "      TxAPCId                 : " << c.txAPCId << "\n"
        << "      TxAPATId                : " << c.txAPATId << "\n"
        << "      TxRefPoint              : " << c.txRefPoint << "\n"
        << "      TxPolarization          : " << c.txPolarization << "\n"
        << "      TxRefRadIntensity       : " << c.txRefRadIntensity << "\n"
        << "      TxRefRadIntensityStdDev : " << c.txRefRadIntensityStdDev << "\n"
        << "      TxRefLAtm               : " << c.txRefLAtm << "\n";

    for (size_t ii = 0; ii < c.addedParameters.size(); ++ii)
    {
        os << "  Parameter name    : " << c.addedParameters[ii].getName() << "\n";
        os << "  Parameter value   : " << c.addedParameters[ii].str() << "\n";
    }
    
    return os;
}

std::ostream& operator<< (std::ostream& os, const TxSequence& c)
{
    os << "TxSequence:: \n"
        << "  RefTxId          : " << c.refTxId << "\n"
        << "  TxWFId           : " << c.txWFType << "\n"
        << "  Parameters:: \n";
    for (size_t ii = 0; ii < c.parameters.size(); ++ii)
    {
        os << c.parameters[ii] << "\n";
    }
    return os;
}
}
