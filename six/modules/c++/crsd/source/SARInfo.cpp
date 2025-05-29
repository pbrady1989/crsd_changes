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

#include <crsd/SARInfo.h>
#include <six/Init.h>

namespace crsd
{
SARInfo::SARInfo()
{
}

std::ostream& operator<< (std::ostream& os, const SARInfo& p)
{
    os << "SAR Information      : \n"
       << "  radarMode            : " << p.radarMode << "\n"
       << "  radarModeID          : " << p.radarModeID << "\n";
       
    for (size_t ii = 0; ii < p.parameter.size(); ++ii)
    {
        os << "  Parameter name   : " << p.parameter[ii].getName() << "\n"
            << "  Parameter value  : " << p.parameter[ii].str() << "\n";
    }
    return os;
}
}
