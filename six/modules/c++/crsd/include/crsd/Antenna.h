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

#ifndef __CRSD_ANTENNA_H__
#define __CRSD_ANTENNA_H__

#include <ostream>
#include <vector>
#include <stddef.h>

#include <six/sicd/Antenna.h>

#include <crsd/Enums.h>
#include <crsd/Types.h>

namespace crsd
{
/*
 *  \struct AntCoordFrame
 *  \brief CRSD Antenna Coordinate Frame parameter
 *
 *  Unit vectors that describe the orientation of an
 *  Antenna Coordinate Frame as a function of time.
 *  AntCoordFrame used by CRSD, representing the tag
 *  <AntCoordFrame>.
 */
struct AntCoordFrame
{
    //! Constructor
    AntCoordFrame();

    //! Equality operators
    bool operator==(const AntCoordFrame& other) const
    {
        return (identifier == other.identifier);
    }
    bool operator!=(const AntCoordFrame& other) const
    {
        return !((*this) == other);
    }

    //! String that uniquely identifies this ACF
    //! (ACF_ID)
    std::string identifier;
};

/*
 *  \struct AntPhaseCenter
 *  \brief Parameters that describe each Antenna Phase Center.
 *
 *  AntPhaseCenter used by CRSD, representing the tag
 *  <AntPhaseCenter>.
 */
struct AntPhaseCenter
{
    //! Constructor
    AntPhaseCenter();

    //! Equality operators
    bool operator==(const AntPhaseCenter& other) const
    {
        return identifier == other.identifier &&
                acfId == other.acfId && apcXYZ == other.apcXYZ;
    }
    bool operator!=(const AntPhaseCenter& other) const
    {
        return !((*this) == other);
    }

    //! String that uniquely identifies this APC
    //! (APC_ID).
    std::string identifier;

    //! Identifier of Antenna Coordinate Frame used for
    //! computing the antenna gain and phase patterns
    std::string acfId;

    //! The APC location in the ACF XYZ coordinate
    //! frame
    Vector3 apcXYZ;
};

struct AntPolRef
{
    //! Default constructor
    AntPolRef();

    //! Equality operators
    bool operator==(const AntPolRef& other) const
    {
        return  ampX == other.ampX &&
                ampY == other.ampY && 
                phaseX == other.phaseX &&
                phaseY == other.phaseY;
    }
    bool operator!=(const AntPolRef& other) const
    {
        return !((*this) == other);
    }

    // E-field relative amplitude in ACX direction
    double ampX;

    // E-field relative amplitude in ACY direction
    double ampY;

    // E-field relative phase in ACX direction
    double phaseX;

    // E-field relative phase in ACY direction
    double phaseY;
};

/*
 *  \struct AntPattern
 *  \brief CRSD Antenna Pattern parameter
 *
 *  Parameter set that defines each Antenna Pattern
 *  as a function of time.
 *  AntPattern used by CRSD, representing the tag
 *  <AntPattern>.
 */
struct AntPattern
{
    //! Default constructor
    AntPattern();

    //! Equality operators
    bool operator==(const AntPattern& other) const
    {
        return  identifier == other.identifier &&
                freqZero == other.freqZero && 
                arrayGPId == other.arrayGPId &&
                elementGPId == other.elementGPId && 
                ebFreqShift == other.ebFreqShift &&
                mlFreqDilation == other.mlFreqDilation &&
                gainBSPoly == other.gainBSPoly &&
                antPolRef == other.antPolRef;
    }
    bool operator!=(const AntPattern& other) const
    {
        return !((*this) == other);
    }

    //! String that uniquely identifies this Antenna
    //! Pattern (APAT_ID).
    std::string identifier;

    //! The reference frequency (f_0) value for which
    //! the Electrical Boresight and array pattern
    //! polynomials are computed.
    double freqZero;

    // Identifier of the array antenna support array.
    std::string arrayGPId;

    // Identifier of the array antenna support array.
    std::string elementGPId;

    //! (required) Parameter to indicate the EB steering direction
    //! shifts with frequency.
    std::pair<double,double> ebFreqShift;

    //! (required) Parameter to indicate the mainlobe width varies
    //! with frequency.
    std::pair<double,double> mlFreqDilation;

    //! (Optional) Gain polynomial (dB) vs. frequency for the array
    //! pattern at boresight. Gain relative to gain at f_0.
    //! Poly1D gainBSPoly;
    Poly1D gainBSPoly;

    // Polarization parameters
    AntPolRef antPolRef;
};

/*
 *  \struct Antenna
 *  \brief (Required) Antenna parameters
 *
 *  Parameters that describe the transmit and
 *  receive antennas used to collect the signal array.
 *  See section 8.2
 */
struct Antenna
{
    //! Default constructor
    Antenna();

    //! Equality operators
    bool operator==(const Antenna& other) const
    {
        return antCoordFrame == other.antCoordFrame &&
                antPhaseCenter == other.antPhaseCenter &&
                antPattern == other.antPattern;
    }
    bool operator!=(const Antenna& other) const
    {
        return !((*this) == other);
    }

    //! Unit vectors that describe the orientation of an
    //! Antenna Coordinate Frame (ACF) as function of
    //! time.
    std::vector<AntCoordFrame> antCoordFrame;

    //! Parameters that describe each Antenna Phase
    //! Center (APC). Parameter set repeated for each
    //! APC
    std::vector<AntPhaseCenter> antPhaseCenter;

    //! Parameter set that defines each Antenna Pattern
    //! as function time. Parameters set repeated for
    //! each Antenna Pattern
    std::vector<AntPattern> antPattern;
};

//! Ostream operators
std::ostream& operator<< (std::ostream& os, const AntCoordFrame& a);
std::ostream& operator<< (std::ostream& os, const AntPhaseCenter& a);
std::ostream& operator<< (std::ostream& os, const AntPattern& a);
std::ostream& operator<< (std::ostream& os, const Antenna& a);
}

#endif
