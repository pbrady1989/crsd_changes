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
#ifndef __CRSD_TXSEQUENCE_H__
#define __CRSD_TXSEQUENCE_H__

#include <string>
#include <vector>
#include <crsd/Types.h>
//#include <six/Enums.h>
#include <crsd/SceneCoordinates.h>

namespace crsd
{

/*
 *  \struct Polarization
 *
 *  \brief Polarization of the signals that formed the signal array.
 */
struct TxPolarization
{

    TxPolarization();

    //! Equality operator
    bool operator==(const TxPolarization& other) const
    {
        return polarizationID == other.polarizationID &&
               ampH == other.ampH &&
               ampV == other.ampV &&
               phaseH == other.phaseH &&
               phaseV == other.phaseV;
    }
    bool operator!=(const TxPolarization& other) const
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

struct TxRefPoint
{
    //! Constructor
    TxRefPoint();

    //! Equality operator
    bool operator==(const TxRefPoint& other) const
    {
        return ecf == other.ecf && iac == other.iac;
    }
    bool operator!=(const TxRefPoint& other) const
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

/*
 *  \sturct ChannelParameter
 *
 *  \brief Channel parameters
 *
 *  Parameter Set that describes a CRSD data
 *  channel. Channels referenced by their unique
 *  Channel ID (Ch_ID). See Section 7.2.
*/
struct TxSequenceParameter
{
    //! Default constructor
    TxSequenceParameter();

    //! Equality operators
    bool operator==(const TxSequenceParameter& other) const
    {
        return  identifier == other.identifier &&
                refPulseIndex == other.refPulseIndex &&
                xmid == other.xmid && 
                fxResponseId == other.fxResponseId &&
                fxBWFixed == other.fxBWFixed && 
                fxC == other.fxC &&
                fxBW == other.fxBW && 
                txmtMin == other.txmtMin &&
                txmtMax == other.txmtMax && 
                txTime1 == other.txTime1 &&
                txTime2 == other.txTime2 && 
                txAPCId == other.txAPCId &&
                txAPATId == other.txAPATId && 
                txRefPoint == other.txRefPoint && 
                txPolarization == other.txPolarization &&
                txRefRadIntensity == other.txRefRadIntensity &&
                txRefRadIntensityStdDev == other.txRefRadIntensityStdDev &&
                txRefLAtm == other.txRefLAtm && 
                addedParameters == other.addedParameters;
    }
    bool operator!=(const TxSequenceParameter& other) const
    {
        return !((*this) == other);
    }

    //! String that uniquely identifies this CRSD data
    //! channel (Ch_ID).
    std::string identifier;

    //! Index of the reference vector (v_CH_REF) for
    //! the channel.
    size_t refPulseIndex;

    //! Flag to indicate when a constant FX band is
    //! saved for all signal vectors of the channel.
    std::string xmid;

    //! Flag to indicate when a constant TOA swath is
    //! saved for all signal vectors of the channel.
    std::string fxResponseId;

    //! (Optional) Flag to indicate when all signal array
    //! vectors are normal
    six::BooleanType fxBWFixed;

    //! Reference frequency for ref signal vector (Hz)
    double fxC;

    //! FX band spanned for the saved bandwidth for the
    //! channel. Computed from all vectors of the signal
    //! array
    double fxBW;

    //! Sample rate (samps / sec)
    double txmtMin;

    //! Nominal instantaneous bandwidth for channel
    double txmtMax; 

    //! Earliest receive start time for the channel
    double txTime1;

    //! Latest receive start time for the channel
    double txTime2;

    //! Identifier for the Receive APC used to collect rcv signal data
    std::string txAPCId;

    //! Identifier for the antenna pattern used to collect signal data
    std::string txAPATId;

    // Reference Point
    TxRefPoint txRefPoint;

    //! Polarization(s) of the signals that formed the
    //! signal array
    TxPolarization txPolarization;

    // reference irradiance incident at the receive APC (dbW/m^2)
    double txRefRadIntensity; 

    // std of error for reference irradiance incident at the receive APC (dbW/m^2)
    double txRefRadIntensityStdDev; 

    // one way atmospheric loss (dB)
    double txRefLAtm; 

    //! (Optional) Block for including additional parameters that
    //! describe the channels and/or signal arrays.
    six::ParameterCollection addedParameters;
};

/*
 *  \struct TxSequence
 *
 *  \brief TxSequence parameters
 *
 *  Parameters that describe the data TxSequence
 *  contained in the product.
 *  See section 7.
 */
struct TxSequence
{
    //! Constructor
    TxSequence();

    //! Equality operators
    bool operator==(const TxSequence& other) const
    {
        return refTxId == other.refTxId &&
               txWFType == other.txWFType &&
               parameters == other.parameters;
    }
    bool operator!=(const TxSequence& other) const
    {
        return !((*this) == other);
    }

    //! Tx ID (Tx_ID) for the Reference Tx
    //! in the product
    std::string refTxId;

    // Waveform Type
    six::TxWFType txWFType;

    //! Parameter Set that describes a CRSD data
    //! channel. Channels referenced by their unique
    //! Channel ID (Ch_ID).
    std::vector<TxSequenceParameter> parameters;
};

//! Ostream operators
std::ostream& operator<< (std::ostream& os, const TxSequenceParameter& c);
std::ostream& operator<< (std::ostream& os, const TxSequence& c);
}

#endif
