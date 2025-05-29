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

#ifndef __CRSD_RECEIVE_INFO_H__
#define __CRSD_RECEIVE_INFO_H__

#include <ostream>
#include <vector>
#include <stddef.h>

#include <crsd/Enums.h>
#include <crsd/Types.h>

namespace crsd
{
/*
 *  \struct SARInfo
 *
 *  \brief Contains product information
 *
 *  (Optional) Parameters that provide general information about the
 *  CRSD product and/or the derived products that may be created from it
 */
struct SARInfo
{

    //! Constructor
    SARInfo();

    //! Equality operators
    bool operator==(const SARInfo& other) const
    {
        return  collectType == other.collectType &&
                radarMode == other.radarMode &&
                radarModeID == other.radarModeID &&
                parameter == other.parameter;
    }
    bool operator!=(const SARInfo& other) const
    {
        return !((*this) == other);
    }

    /*!
     *  (Required) Collection type identifier.  Monotstatic collections include
     *  single platform collections with unique transmit and receive apertures
     *  Allowed values are "MONOSTATIC" and "BISTATIC"
     *
     */
    CollectType collectType;

   /*!
     *  Radar Mode, must be SPOTLIGHT, STRIPMAP, or
     *  DYNAMIC_STRIPMAP
     *
     */
    RadarModeType radarMode;

    /*!
     *  (Optional) RadarMode ModeID.  Value dependent on profile
     */
    std::string radarModeID;

    //! (Optional) Text field that can be used for program specific
    //! parameter name & value
    six::ParameterCollection parameter;
};

//! Ostream operator
std::ostream& operator<< (std::ostream& os, const SARInfo& p);
}

#endif
