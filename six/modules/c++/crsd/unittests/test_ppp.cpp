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

#include <crsd/PPP.h>

#include "TestCase.h"


TEST_CASE(testSimpleEqualityOperatorTrue)
{
    crsd::Ppp ppp1, ppp2;

    ppp1.txTime.setOffset(0);
    ppp2.txTime.setOffset(0);

    TEST_ASSERT_TRUE((ppp1 == ppp2));
}

TEST_CASE(testAppend)
{
    crsd::Ppp ppp;
    ppp.append(ppp.txTime);
    ppp.append(ppp.txPos);
    ppp.append(ppp.txVel);
    ppp.append(ppp.fx1);
    ppp.appendCustomParameter(8, "S8", "AddedParam1");
    ppp.append(ppp.fx2);

    TEST_ASSERT_TRUE(ppp.txTime.getOffset() == 0);
    TEST_ASSERT_TRUE(ppp.txPos.getOffset() == 2);
    TEST_ASSERT_TRUE(ppp.txVel.getOffset() == 5);
    TEST_ASSERT_TRUE(ppp.fx1.getOffset() == 8);
    TEST_ASSERT_TRUE(ppp.addedPPP["AddedParam1"].getOffset() == 9);
    TEST_ASSERT_TRUE(ppp.fx2.getOffset() == 17);
}

TEST_CASE(testAddedParamsEqualityOperatorTrue)
{
    crsd::Ppp ppp1;
    ppp1.setCustomParameter(1, 0, "F8", "AddedParam1");
    ppp1.setCustomParameter(1, 1, "F8", "AddedParam2");

    crsd::Ppp ppp2;
    ppp2.setCustomParameter(1, 0, "F8", "AddedParam1");
    ppp2.setCustomParameter(1, 1, "F8", "AddedParam2");

    TEST_ASSERT_TRUE((ppp1 == ppp2));
}


TEST_CASE(testSimpleEqualityOperatorFalse)
{
    crsd::Ppp ppp1;
    ppp1.fx1.setOffset(0);

    crsd::Ppp ppp2;
    ppp2.txTime.setOffset(1);

    TEST_ASSERT_TRUE((ppp1 != ppp2));

}

TEST_CASE(testAddedParamsEqualityOperatorFalse)
{
    crsd::Ppp ppp1;
    ppp1.setCustomParameter(1, 0, "F8", "AddedParam1");
    ppp1.setCustomParameter(1, 1, "F8", "AddedParam2");

    crsd::Ppp ppp2;
    ppp2.setCustomParameter(1, 0, "F8", "AddedParam1");

    crsd::Ppp ppp3;
    ppp3.setCustomParameter(1, 0, "F8", "AddedParam1");
    ppp3.setCustomParameter(1, 1, "F8", "AddedParam3");

    TEST_ASSERT_TRUE((ppp1 != ppp2));
    TEST_ASSERT_TRUE((ppp1 != ppp3));
}

TEST_MAIN(
        TEST_CHECK(testSimpleEqualityOperatorTrue);
        TEST_CHECK(testAppend);
        TEST_CHECK(testAddedParamsEqualityOperatorTrue);
        TEST_CHECK(testSimpleEqualityOperatorFalse);
        TEST_CHECK(testAddedParamsEqualityOperatorFalse);
        )