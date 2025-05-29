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

#ifndef __CRSD_GLOBAL_H__
#define __CRSD_GLOBAL_H__

#include <ostream>

#include <mem/ScopedCopyablePtr.h>
#include <six/sicd/RadarCollection.h>

#include <crsd/Enums.h>
#include <crsd/Types.h>

namespace crsd
{

/*
 *  \struct TransmitParameters
 *
 *  \brief (SAR Tx only) Parameters dervied from the
 *    transmit pulse PPP included in the product
 */
struct TransmitParameters
{
    //! Constructor
    TransmitParameters();

    //! Equality operators
    bool operator==(const TransmitParameters& other) const
    {
        return fxMin == other.fxMin &&
               fxMax == other.fxMax &&
               txTime1 == other.txTime1 &&
               txTime2 == other.txTime2;
    }
    bool operator!=(const TransmitParameters& other) const
    {
        return !((*this) == other);
    }

    //! Earliest TxTime value for any transmit pulse  in the
    //! product (sec)
    double txTime1;

    //! Latest TxTime value for any transmit pulse in the
    //! product (sec)
    double txTime2;

    //! Minimum frequency value for any transmit pulse in the
    //! product (Hz)
    double fxMin;

    //! Maximum frequency value for any transmit pulse in the
    //! product (Hz)
    double fxMax;
};

/*
 *  \struct Receive Parameters
 *
 *  \brief (SAR Rcv only) Parameters dervied from the 
 *   receive signal PVP included in the product
 */
struct ReceiveParameters
{
    //! Constructor
    ReceiveParameters();

    //! Equality operators
    bool operator==(const ReceiveParameters& other) const
    {
        return fRcvMin == other.fRcvMin &&
               fRcvMax == other.fRcvMax &&
               rcvStartTime1 == other.rcvStartTime1 &&
               rcvStartTime2 == other.rcvStartTime2;
    }
    bool operator!=(const ReceiveParameters& other) const
    {
        return !((*this) == other);
    }

    //! Earliest receive Time value for any signal vector in the
    //! product (sec)
    double rcvStartTime1;

    //! Latest receive time value for any signal vector in the
    //! product (sec)
    double rcvStartTime2;

    //! Minimum frequency value for any signal vector in the
    //! product (Hz)
    double fRcvMin;

    //! Maximum frequency value for any signal vector in the
    //! product (Hz)
    double fRcvMax;
};

/*
 *  \struct TropoParameters
 *
 *  \brief (Optional) Parameters that compute the propogation
 *   delay due to the troposphere
 */
struct TropoParameters
{
    //! Constructor
    TropoParameters();

    //! Equality operators
    bool operator==(const TropoParameters& other) const
    {
        return n0 == other.n0 && 
               refHeight == other.refHeight &&
               n0ErrorStdDev == other.n0ErrorStdDev;
    }
    bool operator!=(const TropoParameters& other) const
    {
        return !((*this) == other);
    }

    //! Refractivity value of the troposphere for the
    //! imaged scene used to form the product
    //! (dimensionless).
    double n0;

    //! Reference Height for the N0 value.
    RefHeight refHeight;

    //! Std deviation for the error in N0.
    double n0ErrorStdDev;
};

/*
 *  \struct IonoParameters
 *
 *  \brief (Optional) Parameters that compute the propogation
 *   delay due to the ionosphere
 */
struct IonoParameters
{
    //! Constructor
    IonoParameters();

    //! Equality operators
    bool operator==(const IonoParameters& other) const
    {
        return tecv == other.tecv && 
               f2Height == other.f2Height &&
               tecvErrorStdDev == other.tecvErrorStdDev;
    }
    bool operator!=(const IonoParameters& other) const
    {
        return !((*this) == other);
    }

    //! Total Electron Content (TEC)
    double tecv;

    //! (Optional) The F2 height of the ionosphere.
    double f2Height; // Optional

    //! Std deviation for the error in tecv.
    double tecvErrorStdDev;
};

/*
 *  \sturct Global
 *
 *  \brief Global parameters that apply to metadata components
 *   and CRSD signal arrays
 */
struct Global
{
    //! Constructor
    Global();

    //! Equality operators
    //! Equality operators
    bool operator==(const Global& other) const;
    bool operator!=(const Global& other) const
    {
        return !((*this) == other);
    }

    //! Time reference used for all other times in the product.
    //! All Tx and Rx times are specified in seconds relative to this.
    DateTime collectionRefTime;

    //! (Optional) Parameters used to compute the propagation
    //! delay due to the troposphere.
    mem::ScopedCopyablePtr<TropoParameters> tropoParameters;

    //! (Optional) Parameters used to compute propagation effects
    //! due to the ionosphere
    mem::ScopedCopyablePtr<IonoParameters> ionoParameters;

    //! (Optional) Parameters used to compute propagation effects
    //! due to the ionosphere
    mem::ScopedCopyablePtr<TransmitParameters> transmitParameters;

    //! (Optional) Parameters used to compute propagation effects
    //! due to the ionosphere
    mem::ScopedCopyablePtr<ReceiveParameters> receiveParameters;
};

//! Ostream operators
std::ostream& operator<< (std::ostream& os, const Global& d);
std::ostream& operator<< (std::ostream& os, const TropoParameters& d);
std::ostream& operator<< (std::ostream& os, const IonoParameters& d);
std::ostream& operator<< (std::ostream& os, const TropoParameters& d);
std::ostream& operator<< (std::ostream& os, const TransmitParameters& d);
std::ostream& operator<< (std::ostream& os, const ReceiveParameters& d);
}

#endif
