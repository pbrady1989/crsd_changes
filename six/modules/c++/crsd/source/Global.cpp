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
#include <gsl/gsl.h>
#include <six/Init.h>
#include <six/Utilities.h>
#include <crsd/Global.h>

namespace crsd
{
    
Global::Global() : 
    collectionRefTime(six::Init::undefined<DateTime>())
{
}

TransmitParameters::TransmitParameters() :
    txTime1(0.0),
    txTime2(0.0),
    fxMin(0.0),
    fxMax(0.0)
{
}

ReceiveParameters::ReceiveParameters() :
    rcvStartTime1(0.0),
    rcvStartTime2(0.0),
    fRcvMin(0.0),
    fRcvMax(0.0)
{
}

TropoParameters::TropoParameters() :
    n0(0.0),
    n0ErrorStdDev(0.0)
{
}

IonoParameters::IonoParameters() :
    tecv(0.0),
    f2Height(gsl::narrow_cast<double>(six::Init::undefined<size_t>())),
    tecvErrorStdDev(0.0)
{
}

bool Global::operator==(const Global& other) const
{
    return collectionRefTime == other.collectionRefTime &&
           transmitParameters == other.transmitParameters &&
           receiveParameters == other.receiveParameters &&
           tropoParameters == other.tropoParameters &&
           ionoParameters == other.ionoParameters;
}


std::ostream& operator<< (std::ostream& os, const Global& d)
{
    os << "Global::\n"
       << "  collectionRefTime       : " << d.collectionRefTime << "\n";
    
    if (d.transmitParameters.get())
    {
        os << "  transmitParameters  : " << *d.transmitParameters << "\n";
    }
    if (d.receiveParameters.get())
    {
        os << "  receiveParameters  : " << *d.receiveParameters << "\n";
    }
    if (d.tropoParameters.get())
    {
        os << "  tropoParameters  : " << *d.tropoParameters << "\n";
    }
    if (d.ionoParameters.get())
    {
        os << " ionoParameters    : " << *d.ionoParameters << "\n";
    }
    return os;
}

std::ostream& operator<< (std::ostream& os, const TransmitParameters& d)
{
    os << "TransmitParameters::\n" 
       << "  txTime1               : " << d.txTime1 << "\n"
       << "  txTime2               : " << d.txTime2 << "\n"
       << "  fxMin                 : " << d.fxMin << "\n"
       << "  fxMax                 : " << d.fxMax << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const ReceiveParameters& d)
{
    os << "ReceiveParameters::\n" 
       << "  rcvStartTime1               : " << d.rcvStartTime1 << "\n"
       << "  rcvStartTime2               : " << d.rcvStartTime2 << "\n"
       << "  fRcvMin                 : " << d.fRcvMin << "\n"
       << "  fRcvMax                 : " << d.fRcvMax << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const TropoParameters& d)
{
    os << "TropoParameters::\n"
       << "  n0                    : " << d.n0<< "\n"
       << "  refHeight             : " << d.refHeight << "\n"
       << "  n0ErrorStdDev         : " << d.n0ErrorStdDev << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const IonoParameters& d)
{
    os << "IonoParameters::\n"
       << "  tecv                  : " << d.tecv << "\n"
       << "  f2Height              : " << d.f2Height << "\n"
       << "  tecvErrorStdDev       : " << d.tecvErrorStdDev << "\n";
    return os;
}
}
