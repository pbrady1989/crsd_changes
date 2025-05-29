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
#include <crsd/Antenna.h>
#include <six/Init.h>

namespace crsd
{

AntCoordFrame::AntCoordFrame()
{
}

AntPhaseCenter::AntPhaseCenter()
{
}

AntPolRef::AntPolRef() : 
    ampX(six::Init::undefined<double>()),
    ampY(six::Init::undefined<double>()),
    phaseX(six::Init::undefined<double>()),
    phaseY(six::Init::undefined<double>())
{
}

AntPattern::AntPattern() :
    freqZero(six::Init::undefined<double>()),
    ebFreqShift(six::Init::undefined<std::pair<double,double>>()),
    mlFreqDilation(six::Init::undefined<std::pair<double,double>>()),
    gainBSPoly(six::Init::undefined<Poly1D>())
{
}

Antenna::Antenna()
{
}

std::ostream& operator<< (std::ostream& os, const AntCoordFrame& a)
{
    os << "  AntCoordFrame:: \n"
        << "    Identifier     : " << a.identifier << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const AntPhaseCenter& a)
{
    os << "  AntPhaseCenter:: \n"
        << "    Identifier     : " << a.identifier << "\n"
        << "    ACFId          : " << a.acfId << "\n"
        << "    APCXYZ         : " << a.apcXYZ << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const AntPolRef& a)
{
    os << "  AntPhaseCenter:: \n"
        << "    ampX           : " << a.ampX << "\n"
        << "    ampY           : " << a.ampY << "\n"
        << "    phaseX         : " << a.phaseX << "\n"
        << "    phaseY         : " << a.phaseY << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const AntPattern& a)
{
    os << "  AntPattern \n"
        << "    Identifier        : " << a.identifier << "\n"
        << "    FreqZero          : " << a.freqZero << "\n"
        << "    ArrayGPId         : " << a.arrayGPId << "\n"
        << "    ElementGPId       : " << a.elementGPId << "\n"
        << "    EBFreqShift[0]    : " << a.ebFreqShift.first << "\n"
        << "    EBFreqShift[1]    : " << a.ebFreqShift.second << "\n"
        << "    MLFreqDilation[0] : " << a.mlFreqDilation.first << "\n"
        << "    MLFreqDilation[1] : " << a.mlFreqDilation.second << "\n"
        << "    GainBSPoly        : " << a.gainBSPoly << "\n"
        << "    AntPolRef         :" << a.antPolRef << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const Antenna& a)
{
    os << "Antenna:: \n"
        << "  NumACFs          : " << a.antCoordFrame.size() << "\n"
        << "  NumAPCs          : " << a.antPhaseCenter.size() << "\n"
        << "  NumAntPats          : " << a.antPattern.size() << "\n";
    for (size_t ii = 0; ii < a.antCoordFrame.size(); ++ii)
    {
        os << a.antCoordFrame[ii] << "\n";
    }
    for (size_t ii = 0; ii < a.antPhaseCenter.size(); ++ii)
    {
        os << a.antPhaseCenter[ii] << "\n";
    }
    for (size_t ii = 0; ii < a.antPattern.size(); ++ii)
    {
        os << a.antPattern[ii] << "\n";
    }
    return os;
}
}
