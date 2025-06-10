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
#include <iostream>
#include <memory>

#include <xml/lite/MinidomParser.h>
#include <six/Init.h>

#include <crsd/PVP.h>

#include "TestCase.h"


TEST_CASE(testSimpleEqualityOperatorTrue)
{
    crsd::Pvp pvp1, pvp2;

    pvp1.rcvStart.setOffset(0);
    pvp2.rcvStart.setOffset(0);

    TEST_ASSERT_TRUE((pvp1 == pvp2));
}

TEST_CASE(testAppend)
{
    crsd::Pvp pvp;
    pvp.append(pvp.rcvStart);
    pvp.append(pvp.rcvPos);
    pvp.append(pvp.rcvVel);
    pvp.append(pvp.ampSF);
    pvp.appendCustomParameter(8, "S8", "AddedParam1");
    pvp.append(pvp.signal);

    TEST_ASSERT_TRUE(pvp.rcvStart.getOffset() == 0);
    TEST_ASSERT_TRUE(pvp.rcvPos.getOffset() == 2);
    TEST_ASSERT_TRUE(pvp.rcvVel.getOffset() == 5);
    TEST_ASSERT_TRUE(pvp.ampSF.getOffset() == 8);
    TEST_ASSERT_TRUE(pvp.addedPVP["AddedParam1"].getOffset() == 9);
    TEST_ASSERT_TRUE(pvp.signal.getOffset() == 17);
}

TEST_CASE(testAddedParamsEqualityOperatorTrue)
{
    crsd::Pvp pvp1;
    pvp1.setCustomParameter(1, 0, "F8", "AddedParam1");
    pvp1.setCustomParameter(1, 1, "F8", "AddedParam2");

    crsd::Pvp pvp2;
    pvp2.setCustomParameter(1, 0, "F8", "AddedParam1");
    pvp2.setCustomParameter(1, 1, "F8", "AddedParam2");

    TEST_ASSERT_TRUE((pvp1 == pvp2));
}


TEST_CASE(testSimpleEqualityOperatorFalse)
{
    crsd::Pvp pvp1;
    pvp1.frcv1.setOffset(0);

    crsd::Pvp pvp2;
    pvp2.rcvStart.setOffset(1);

    TEST_ASSERT_TRUE((pvp1 != pvp2));

}

TEST_CASE(testAddedParamsEqualityOperatorFalse)
{
    crsd::Pvp pvp1;
    pvp1.setCustomParameter(1, 0, "F8", "AddedParam1");
    pvp1.setCustomParameter(1, 1, "F8", "AddedParam2");

    crsd::Pvp pvp2;
    pvp2.setCustomParameter(1, 0, "F8", "AddedParam1");

    crsd::Pvp pvp3;
    pvp3.setCustomParameter(1, 0, "F8", "AddedParam1");
    pvp3.setCustomParameter(1, 1, "F8", "AddedParam3");

    TEST_ASSERT_TRUE((pvp1 != pvp2));
    TEST_ASSERT_TRUE((pvp1 != pvp3));
}

TEST_MAIN(
        TEST_CHECK(testSimpleEqualityOperatorTrue);
        TEST_CHECK(testAppend);
        TEST_CHECK(testAddedParamsEqualityOperatorTrue);
        TEST_CHECK(testSimpleEqualityOperatorFalse);
        TEST_CHECK(testAddedParamsEqualityOperatorFalse);
        )