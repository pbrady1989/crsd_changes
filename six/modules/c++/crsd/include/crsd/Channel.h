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
#ifndef __CRSD_CHANNEL_H__
#define __CRSD_CHANNEL_H__

#include <string>
#include <vector>
#include <crsd/Types.h>
#include <crsd/Enums.h>
#include <crsd/SceneCoordinates.h>

namespace crsd
{

/*
 *  \struct Polarization
 *
 *  \brief Polarization of the signals that formed the signal array.
 */
struct Polarization
{

    Polarization();

    //! Equality operator
    bool operator==(const Polarization& other) const
    {
        return polarizationID == other.polarizationID &&
               ampH == other.ampH &&
               ampV == other.ampV &&
               phaseH == other.phaseH &&
               phaseV == other.phaseV;
    }
    bool operator!=(const Polarization& other) const
    {
        return !((*this) == other);
    }

    //! Transmitted signal polarization for the channel
    PolarizationType polarizationID;

    //! E-field relative amplitude in H direction for ref vector
    double ampH;

    //! E-field relative amplitude in V direction for ref vector
    double ampV;

    //! E-field relative phase in H direction for ref vector
    double phaseH;

    //! E-field relative phase in V direction for ref vector
    double phaseV;
};

/*
 *  \struct DwellTimes
 *
 *  \brief COD Time & Dwell Time polynomials
 *
 *  See section 6.3 and section 7.2.6
 */
struct DwellTimes
{
    //! COD Time & Dwell Time polynomials over the
    //! image area
    DwellTimes();

    // Equality operator
    bool operator==(const DwellTimes& other) const
    {
        return codId == other.codId &&
               dwellId == other.dwellId &&
               dtaId == other.dtaId;
    }
    bool operator!=(const DwellTimes& other) const
    {
        return !((*this) == other);
    }

    //! Identifier of the center of Dwell Time polynomial
    //! that maps reference surface position to COD time
    std::string codId;

    //! Identifier of the dwell Time polynomial that
    //! maps reference surface position to dwell time
    std::string dwellId;

    // Dwell Time support array identifier
    std::string dtaId;
};

struct RcvRefPoint
{
    //! Constructor
    RcvRefPoint();

    //! Equality operator
    bool operator==(const RcvRefPoint& other) const
    {
        return ecf == other.ecf && iac == other.iac;
    }
    bool operator!=(const RcvRefPoint& other) const
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
 *  \struct ChannelSARImage
 *
 *  \brief CRSD SAR Information (two-way/bistatic)
 */
struct ChannelSARImage
{
    //! Constructor
    ChannelSARImage();

    //! Equality operators
    bool operator==(const ChannelSARImage& other) const
    {
        return  txID == other.txID &&
                refVectorPulseIndex == other.refVectorPulseIndex &&
                txPolarization == other.txPolarization &&
                dwellTime == other.dwellTime &&
                imageArea == other.imageArea;
    }
    bool operator!=(const ChannelSARImage& other) const
    {
        return !((*this) == other);
    }

    //! Center of dwell time for ref point (sec)
    std::string txID;

    //! Dwell time for the ref point (sec)
    size_t refVectorPulseIndex;

     //! Polarization(s) of the signals that formed the
    //! signal array
    Polarization txPolarization;

    //! ARP position in ECF coordinates (m)
    DwellTimes dwellTime;

    //! ARP velocity in ECF coordinates (m/s).
    AreaType imageArea;
};


/*
 *  \sturct ChannelParameter
 *
 *  \brief Channel parameters
 *
 *  Parameter Set that describes a CRSD data
 *  channel. Channels referenced by their unique
 *  Channel ID (Ch_ID). See Section 7.2.
*/
struct ChannelParameter
{
    //! Default constructor
    ChannelParameter();

    //! Equality operators
    bool operator==(const ChannelParameter& other) const
    {
        return  identifier == other.identifier &&
                refVectorIndex == other.refVectorIndex &&
                refFreqFixed == other.refFreqFixed && 
                fRcvFixed == other.fRcvFixed &&
                signalNormal == other.signalNormal && 
                f0Ref == other.f0Ref &&
                fxBW == other.fxBW && 
                fs == other.fs &&
                bwInst == other.bwInst && 
                rcvStartTime1 == other.rcvStartTime1 &&
                rcvStartTime2 == other.rcvStartTime2 && 
                fRcvMin == other.fRcvMin &&
                fRcvMax == other.fRcvMax && 
                rcvAPCId == other.rcvAPCId &&
                rcvAPATId == other.rcvAPATId && 
                rcvRefPoint == other.rcvRefPoint && 
                rcvPolarization == other.rcvPolarization &&
                rcvRefIrradiance == other.rcvRefIrradiance &&
                rcvRefIrradianceErrorStdDev == other.rcvRefIrradianceErrorStdDev &&
                rcvRefLAtm == other.rcvRefLAtm && 
                pncrsd == other.pncrsd &&
                bncrsd == other.bncrsd && 
                sarImage == other.sarImage &&
                addedParameters == other.addedParameters;
    }
    bool operator!=(const ChannelParameter& other) const
    {
        return !((*this) == other);
    }

    //! String that uniquely identifies this CRSD data
    //! channel (Ch_ID).
    std::string identifier;

    //! Index of the reference vector (v_CH_REF) for
    //! the channel.
    size_t refVectorIndex;

    //! Flag to indicate when a constant FX band is
    //! saved for all signal vectors of the channel.
    six::BooleanType refFreqFixed;

    //! Flag to indicate when a constant TOA swath is
    //! saved for all signal vectors of the channel.
    six::BooleanType fRcvFixed;

    //! (Optional) Flag to indicate when all signal array
    //! vectors are normal
    six::BooleanType signalNormal;

    //! Reference frequency for ref signal vector (Hz)
    double f0Ref;

    //! FX band spanned for the saved bandwidth for the
    //! channel. Computed from all vectors of the signal
    //! array
    double fxBW;

    //! Sample rate (samps / sec)
    double fs;

    //! Nominal instantaneous bandwidth for channel
    double bwInst; 

    //! Earliest receive start time for the channel
    double rcvStartTime1;

    //! Latest receive start time for the channel
    double rcvStartTime2;

    //! Min freqeuency received
    double fRcvMin;

    //! Max frequency received
    double fRcvMax;

    //! Identifier for the Receive APC used to collect rcv signal data
    std::string rcvAPCId;

    //! Identifier for the antenna pattern used to collect signal data
    std::string rcvAPATId;

    // Reference Point
    RcvRefPoint rcvRefPoint;

    //! Polarization(s) of the signals that formed the
    //! signal array
    Polarization rcvPolarization;

    // reference irradiance incident at the receive APC (dbW/m^2)
    double rcvRefIrradiance; 

    // std of error for reference irradiance incident at the receive APC (dbW/m^2)
    double rcvRefIrradianceErrorStdDev; 

    // one way atmospheric loss (dB)
    double rcvRefLAtm; 

    // thermal noise power level
    double pncrsd; 

    // noise equivalent bandwidth level
    double bncrsd; 

    //! (Optional) Parameters used to compute propagation effects
    //! due to the ionosphere
    mem::ScopedCopyablePtr<ChannelSARImage> sarImage;

    //! (Optional) Block for including additional parameters that
    //! describe the channels and/or signal arrays.
    six::ParameterCollection addedParameters;
};

/*
 *  \struct Channel
 *
 *  \brief Channel parameters
 *
 *  Parameters that describe the data channels
 *  contained in the product.
 *  See section 7.
 */
struct Channel
{
    //! Constructor
    Channel();

    //! Equality operators
    bool operator==(const Channel& other) const
    {
        return refChId == other.refChId &&
               parameters == other.parameters;
    }
    bool operator!=(const Channel& other) const
    {
        return !((*this) == other);
    }

    //! Channel ID (Ch_ID) for the Reference Channel
    //! in the product
    std::string refChId;

    //! Parameter Set that describes a CRSD data
    //! channel. Channels referenced by their unique
    //! Channel ID (Ch_ID).
    std::vector<ChannelParameter> parameters;
};

//! Ostream operators
std::ostream& operator<< (std::ostream& os, const ChannelParameter& c);
std::ostream& operator<< (std::ostream& os, const Channel& c);
}

#endif
