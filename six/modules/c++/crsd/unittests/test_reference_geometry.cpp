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

#include <iostream>
#include <memory>

#include <xml/lite/MinidomParser.h>
#include <six/Init.h>
#include <crsd/Enums.h>
#include <crsd/Types.h>

#include "TestCase.h"

TEST_CASE(testEquality)
{
    crsd::ReferenceGeometry refGeo;
    crsd::ReferenceGeometry refGeoOther;
    refGeo.sarParameters.reset(new crsd::SARImage());
    refGeoOther.sarParameters.reset(new crsd::SARImage());

    // Fill Values
    refGeo.sarParameters->azimuthAngle = 1.0;
    refGeo.sarParameters->grazeAngle = 1.0;
    refGeo.sarParameters->twistAngle = 1.0;
    refGeo.sarParameters->slopeAngle = 1.0;
    refGeo.sarParameters->layoverAngle = 1.0;
    refGeo.sarParameters->sideOfTrack = six::SideOfTrackType::LEFT;
    refGeo.sarParameters->slantRange = 20.0;

    refGeoOther.sarParameters->azimuthAngle = 2.0;
    refGeoOther.sarParameters->grazeAngle = 2.0;
    refGeoOther.sarParameters->twistAngle = 2.0;
    refGeoOther.sarParameters->slopeAngle = 2.0;
    refGeoOther.sarParameters->layoverAngle = 2.0;
    refGeoOther.sarParameters->sideOfTrack = six::SideOfTrackType::LEFT;
    refGeoOther.sarParameters->slantRange = 20.0;

    TEST_ASSERT_NOT_EQ(refGeo, refGeoOther);
}

TEST_MAIN(
        TEST_CHECK(testEquality);
)
