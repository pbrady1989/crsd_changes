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

#ifndef __CRSD_REFERENCE_GEOMETRY_H__
#define __CRSD_REFERENCE_GEOMETRY_H__

#include <ostream>

#include <crsd/Enums.h>
#include <crsd/Types.h>
#include <mem/ScopedCopyablePtr.h>

namespace crsd
{

/*!
 *  \struct RefPoint
 *
 *  \brief The SRP position for the reference vector of the
 *  reference channel
 */
struct RefPoint
{
    //! Constructor
    RefPoint();

    //! Equality operator
    bool operator==(const RefPoint& other) const
    {
        return ecf == other.ecf && iac == other.iac;
    }
    bool operator!=(const RefPoint& other) const
    {
        return !((*this) == other);
    }

    //! SRP position in ECF coordinates. Value
    //! included in the SRPPos PVP
    Vector3 ecf;

    //! SRP position in Image Area Coordinates (IAX,
    //! IAY, IAZ).
    Vector2 iac;
};

/*!
 *  \struct SARImage
 *
 *  \brief CRSD SAR Information (two-way/bistatic)
 */
struct SARImage
{
    //! Constructor
    SARImage();

    //! Equality operators
    bool operator==(const SARImage& other) const
    {
        return  codTime == other.codTime &&
                dwellTime == other.dwellTime &&
                referenceTime == other.referenceTime &&
                arpPos == other.arpPos &&
                arpVel == other.arpVel &&
                bistaticAngle == other.bistaticAngle &&
                bistaticAngleRate == other.bistaticAngleRate &&
                sideOfTrack == other.sideOfTrack &&
                slantRange == other.slantRange &&
                groundRange == other.groundRange &&
                dopplerConeAngle == other.dopplerConeAngle &&
                squintAngle == other.squintAngle &&
                azimuthAngle == other.azimuthAngle &&
                grazeAngle == other.grazeAngle &&
                incidenceAngle == other.incidenceAngle &&
                twistAngle == other.twistAngle &&
                slopeAngle == other.slopeAngle &&
                layoverAngle == other.layoverAngle;
    }
    bool operator!=(const SARImage& other) const
    {
        return !((*this) == other);
    }

    //! Center of dwell time for ref point (sec)
    double codTime;

    //! Dwell time for the ref point (sec)
    double dwellTime;

    //! Ref time for the ref vector of the ref channel (sec)
    double referenceTime;

    //! ARP position in ECF coordinates (m)
    Vector3 arpPos;

    //! ARP velocity in ECF coordinates (m/s).
    Vector3 arpVel;

    //! Bistatic angle for the ref geometry 
    //! (deg) [0 - 180]
    double bistaticAngle;

    //! Rate at which the bistatic angle changes (deg / s)
    double bistaticAngleRate;

    //! Side of Track parameter for the collection.
    //! L Left-looking or R Right-looking
    six::SideOfTrackType sideOfTrack;

    //! Slant range from the ARP to the SRP
    double slantRange;

    //! Ground range from the ARP nadir to the SRP.
    //! Distance measured along spherical earth model
    //! passing through the SRP (m).
    double groundRange;

    //! Doppler Cone Angle between ARP velocity and
    //! SRP Line of Sight (LOS) (deg).
    double dopplerConeAngle;

    //! Squint angle to the reference point off the 
    //! normal to the ARP ground track (deg).
    double squintAngle;

    //! Angle from north to the line from the SRP to the
    //! ARP ETP Nadir (i.e. North to +GPX). Measured
    //! clockwise from +North toward +East (deg).
    double azimuthAngle;

    //! Grazing angle for the ARP to SRP LOS and the
    //! Earth Tangent Plane (ETP) at the SRP (deg).
    double grazeAngle;

    //! Incidence angle for the ARP to SRP LOS and the
    //! Earth Tangent Plane (ETP) at the SRP (deg)
    double incidenceAngle;

    //! Twist angle between cross range in the ETP and
    //! cross range in the slant plane at the SRP (deg)
    double twistAngle;

    //! Angle between the ETP normal (uUP) and the
    //! slant plane normal (uSPN) at the SRP (deg)
    double slopeAngle;

    //! Angle from north to the layover direction in the
    //! ETP. Measured clockwise from +North toward
    //! +East (deg)
    double layoverAngle;
};


/*!
    *  \struct OneWayParams
    *
    *  \brief Describe transmit or recieve platform parameters
    */
struct OneWayParams
{
    //! Constructor
    OneWayParams();

    //! Equality operators
    bool operator==(const OneWayParams& other) const
    {
        return  time == other.time &&
                apcPos == other.apcPos &&
                apcVel == other.apcVel &&
                sideOfTrack == other.sideOfTrack &&
                slantRange == other.slantRange &&
                groundRange == other.groundRange &&
                dopplerConeAngle == other.dopplerConeAngle &&
                squintAngle == other.squintAngle &&
                azimuthAngle == other.azimuthAngle &&
                grazeAngle == other.grazeAngle &&
                incidenceAngle == other.incidenceAngle;
    }
    bool operator!=(const OneWayParams& other) const
    {
        return !((*this) == other);
    }

    //! Tx or rx start time for ref point (sec)
    double time;

    //! ARP position in ECF coordinates (m)
    Vector3 apcPos;

    //! ARP velocity in ECF coordinates (m/s).
    Vector3 apcVel;

    //! Side of Track parameter for the collection.
    //! L Left-looking or R Right-looking
    six::SideOfTrackType sideOfTrack;

    //! Slant range from the ARP to the SRP
    double slantRange;

    //! Ground range from the ARP nadir to the SRP.
    //! Distance measured along spherical earth model
    //! passing through the SRP (m).
    double groundRange;

    //! Doppler Cone Angle between ARP velocity and
    //! SRP Line of Sight (LOS) (deg).
    double dopplerConeAngle;

    //! Squint angle to the reference point off the 
    //! normal to the ARP ground track (deg).
    double squintAngle;

    //! Angle from north to the line from the SRP to the
    //! ARP ETP Nadir (i.e. North to +GPX). Measured
    //! clockwise from +North toward +East (deg).
    double azimuthAngle;

    //! Grazing angle for the ARP to SRP LOS and the
    //! Earth Tangent Plane (ETP) at the SRP (deg).
    double grazeAngle;

    //! Incidence angle for the ARP to SRP LOS and the
    //! Earth Tangent Plane (ETP) at the SRP (deg)
    double incidenceAngle;
};

/*!
 *  \struct ReferenceGeometry
 *
 *  \brief Parameters that describe the collection geometry
 */
struct ReferenceGeometry
{
    //! Constructor
    ReferenceGeometry();

    //! Equality operator
    bool operator==(const ReferenceGeometry& other) const
    {
        return  refPoint == other.refPoint &&
                sarParameters == other.sarParameters &&
                txParameters == other.txParameters &&
                rcvParameters == other.rcvParameters;
    }
    bool operator!=(const ReferenceGeometry& other) const
    {
        return !((*this) == other);
    }

    //! The SRP position for the reference vector of the
    //! reference channel.
    RefPoint refPoint;

    //! Parameters for CSD Type = "SAR"
    mem::ScopedCopyablePtr<SARImage> sarParameters;

    //! Parameters for CSD Type = "SAR" or "Tx"
    mem::ScopedCopyablePtr<OneWayParams> txParameters;

    //! Parameters for CSD Type = "SAR" or "Rcv"
    mem::ScopedCopyablePtr<OneWayParams> rcvParameters;
};

//! Ostream operators
std::ostream& operator<< (std::ostream& os, const RefPoint& s);
std::ostream& operator<< (std::ostream& os, const OneWayParams& i);
std::ostream& operator<< (std::ostream& os, const SARImage& m);
std::ostream& operator<< (std::ostream& os, const ReferenceGeometry& r);
}

#endif
