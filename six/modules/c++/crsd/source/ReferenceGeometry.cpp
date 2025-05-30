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

#include <crsd/ReferenceGeometry.h>
#include <six/Init.h>

namespace crsd
{

RefPoint::RefPoint() :
    ecf(six::Init::undefined<Vector3>()),
    iac(six::Init::undefined<Vector2>())
{
}

SARImage::SARImage() :
    codTime(0.0),
    dwellTime(0.0),
    referenceTime(0.0),
    arpPos(six::Init::undefined<Vector3>()),
    arpVel(six::Init::undefined<Vector3>()),
    bistaticAngle(0.0),
    bistaticAngleRate(0.0),
    sideOfTrack(six::Init::undefined<six::SideOfTrackType>()),
    slantRange(0.0),
    groundRange(0.0),
    dopplerConeAngle(0.0),
    squintAngle(0.0),
    azimuthAngle(0.0),
    grazeAngle(0.0),
    incidenceAngle(0.0),
    twistAngle(0.0),
    slopeAngle(0.0),
    layoverAngle(0.0)
{
}

OneWayParams::OneWayParams() :
    time(0.0),
    apcPos(six::Init::undefined<Vector3>()),
    apcVel(six::Init::undefined<Vector3>()),
    sideOfTrack(six::Init::undefined<six::SideOfTrackType>()),
    slantRange(0.0),
    groundRange(0.0),
    dopplerConeAngle(0.0),
    squintAngle(0.0),
    azimuthAngle(0.0),
    grazeAngle(0.0),
    incidenceAngle(0.0)
{
}

ReferenceGeometry::ReferenceGeometry()
{
}

std::ostream& operator<< (std::ostream& os, const RefPoint& s)
{
    os << "RefPoint:: \n"
        << "  ECF              : " << s.ecf << "\n"
        << "  IAC              : " << s.iac << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const SARImage& i)
{
    os << "SARImage:: \n"
        << "  codTime          : " << i.codTime << "\n"
        << "  dwellTime        : " << i.dwellTime << "\n"
        << "  referenceTime    : " << i.referenceTime << "\n"
        << "  ArpPos:: \n"
        << "    X              : " << i.arpPos[0] << "\n"
        << "    Y              : " << i.arpPos[1] << "\n"
        << "    Z              : " << i.arpPos[2] << "\n"
        << "  ArpVel:: \n"
        << "    X              : " << i.arpVel[0] << "\n"
        << "    Y              : " << i.arpVel[1] << "\n"
        << "    Z              : " << i.arpVel[2] << "\n"
        << "  bistaticAngle    : " << i.bistaticAngle << "\n"
        << "  bistaticAngleRate: " << i.bistaticAngleRate << "\n"
        << "  sideOfTrack      : " << i.sideOfTrack << "\n"
        << "  slantRange       : " << i.slantRange << "\n"
        << "  groundRange      : " << i.groundRange << "\n"
        << "  dopplerConeAngle : " << i.dopplerConeAngle << "\n"
        << "  squintAngle      : " << i.squintAngle << "\n"
        << "  azimuthAngle     : " << i.azimuthAngle << "\n"
        << "  grazeAngle       : " << i.grazeAngle << "\n"
        << "  incidenceAngle   : " << i.incidenceAngle << "\n"
        << "  twistAngle       : " << i.twistAngle << "\n"
        << "  slopeAngle       : " << i.slopeAngle << "\n"
        << "  layoverAngle     : " << i.layoverAngle << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const OneWayParams& m)
{
    os << "OneWayParams:: \n"
        << "  time          : " << m.time << "\n"
        << "  ApcPos:: \n"
        << "    X              : " << m.apcPos[0] << "\n"
        << "    Y              : " << m.apcPos[1] << "\n"
        << "    Z              : " << m.apcPos[2] << "\n"
        << "  ApcVel:: \n"
        << "    X              : " << m.apcVel[0] << "\n"
        << "    Y              : " << m.apcVel[1] << "\n"
        << "    Z              : " << m.apcVel[2] << "\n"
        << "    X              : " << m.apcVel[0] << "\n"
        << "  sideOfTrack      : " << m.sideOfTrack << "\n"
        << "  slantRange       : " << m.slantRange << "\n"
        << "  groundRange      : " << m.groundRange << "\n"
        << "  dopplerConeAngle : " << m.dopplerConeAngle << "\n"
        << "  squintAngle      : " << m.squintAngle << "\n"
        << "  azimuthAngle     : " << m.azimuthAngle << "\n"
        << "  grazeAngle       : " << m.grazeAngle << "\n"
        << "  incidenceAngle   : " << m.incidenceAngle << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const ReferenceGeometry& r)
{
    os << "Reference Geometry:: \n"
       << r.refPoint;

    if (r.sarParameters.get())
    {
        os << *r.sarParameters << "\n";
    }
    if (r.txParameters.get())
    {
        os << *r.txParameters << "\n";
    }
    if (r.rcvParameters.get())
    {
        os << *r.rcvParameters << "\n";
    }
    return os;
}

}

