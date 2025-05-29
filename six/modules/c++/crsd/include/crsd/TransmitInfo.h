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

#ifndef __CRSD_TRANSMIT_INFO_H__
#define __CRSD_TRANSMIT_INFO_H__

#include <ostream>
#include <vector>
#include <stddef.h>

#include <crsd/Enums.h>
#include <crsd/Types.h>

namespace crsd
{
/*
 *  \struct TransmitInfo
 *
 *  \brief Contains product information
 *
 *  (Optional) Parameters that provide general information about the
 *  CRSD product and/or the derived products that may be created from it
 */
struct TransmitInfo
{

    //! Constructor
    TransmitInfo();

    //! Equality operators
    bool operator==(const TransmitInfo& other) const
    {
        return  sensorName == other.sensorName &&
                eventName == other.eventName &&
                parameter == other.parameter;
    }
    bool operator!=(const TransmitInfo& other) const
    {
        return !((*this) == other);
    }

    //! (Required) Identifies the Transmit sensor that collected the
    //! initial PHD or receive only data used to form the
    //! receive signal arrays
    std::string sensorName;

    //! (Required) Identifies the Transmit sensor imaging event or
    //! receive only event when the initial signal data
    //! was collected
    std::string eventName;

    //! (Optional) Text field that can be used for program specific
    //! parameter name & value
    six::ParameterCollection parameter;
};

//! Ostream operator
std::ostream& operator<< (std::ostream& os, const TransmitInfo& p);
}

#endif
