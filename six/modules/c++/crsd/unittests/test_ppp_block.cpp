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

#include <complex>
#include <thread>
#include <tuple>

#include <crsd/PPP.h>
#include <crsd/PPPBlock.h>
#include <crsd/TestDataGenerator.h>

#include "TestCase.h"

static constexpr size_t NUM_SEQUENCES = 3;
static constexpr size_t NUM_PULSES = 2;

static void call_srand()
{
    const auto f = []() { ::srand(174); return nullptr; };
    static const auto result = f();
    std::ignore = result;
}

void setPulseParameters(size_t txSequence,
                         size_t pulse,
                         crsd::PPPBlock& pppBlock,
                         crsd::PPPBlock& pppBlock2)
{
    const double doubleVal = crsd::getRandom();
    const int64_t intVal = static_cast<int64_t>(crsd::getRandom());
    const crsd::Vector2 vec2 = crsd::getRandomVector2();
    const crsd::Vector3 vec3 = crsd::getRandomVector3();
    std::pair<int64_t,double> pairVal(intVal,doubleVal);

    pppBlock.setTxStart(pairVal, txSequence, pulse);
    pppBlock2.setTxStart(pairVal, txSequence, pulse);

    pppBlock.setTxPos(vec3, txSequence, pulse);
    pppBlock2.setTxPos(vec3, txSequence, pulse);

    pppBlock.setTxVel(vec3, txSequence, pulse);
    pppBlock2.setTxVel(vec3, txSequence, pulse);

    pppBlock.setFX1(doubleVal, txSequence, pulse);
    pppBlock2.setFX1(doubleVal, txSequence, pulse);

    pppBlock.setFX2(doubleVal, txSequence, pulse);
    pppBlock2.setFX2(doubleVal, txSequence, pulse);

    pppBlock.setFxFreq0(doubleVal, txSequence, pulse);
    pppBlock2.setFxFreq0(doubleVal, txSequence, pulse);

    pppBlock.setFxRate(doubleVal, txSequence, pulse);
    pppBlock2.setFxRate(doubleVal, txSequence, pulse);

    pppBlock.setTXMT(doubleVal, txSequence, pulse);
    pppBlock2.setTXMT(doubleVal, txSequence, pulse);

    pppBlock.setTxACX(vec3, txSequence, pulse);
    pppBlock2.setTxACX(vec3, txSequence, pulse);

    pppBlock.setTxACY(vec3, txSequence, pulse);
    pppBlock2.setTxACY(vec3, txSequence, pulse);

    pppBlock.setTxEB(vec2, txSequence, pulse);
    pppBlock2.setTxEB(vec2, txSequence, pulse);

    pppBlock.setTxRadInt(doubleVal, txSequence, pulse);
    pppBlock2.setTxRadInt(doubleVal, txSequence, pulse);

}

TEST_CASE(testPppRequired)
{
    call_srand();
    crsd::Ppp ppp;
    crsd::setPPPXML(ppp);
    crsd::PPPBlock pppBlock(NUM_SEQUENCES,
                            std::vector<size_t>(NUM_SEQUENCES, NUM_PULSES),
                            ppp);

    for (size_t txSequence = 0; txSequence < NUM_SEQUENCES; ++txSequence)
    {
        for (size_t pulse = 0; pulse < NUM_PULSES; ++pulse)
        {
            crsd::setPulseParameters(txSequence, pulse, pppBlock);
        }
    }
}

TEST_CASE(testPppThrow)
{
    call_srand();

    crsd::Ppp ppp;
    crsd::setPPPXML(ppp);
    ppp.setOffset(29, ppp.fx1);
    ppp.setOffset(28, ppp.fx2);
    TEST_EXCEPTION(ppp.setOffset(15, ppp.xmIndex)); // Overwrite block
    ppp.setOffset(27, ppp.xmIndex);

    ppp.setCustomParameter(1, 30, "F8", "Param1");
    ppp.setCustomParameter(1, 31, "F8", "Param2");
    TEST_EXCEPTION(ppp.setCustomParameter(1, 30, "F8", "Param1")); //Rewriting to existing memory block
    TEST_EXCEPTION(ppp.setCustomParameter(1, 30, "X=F8;YF8;", "Param1")); //
    TEST_EXCEPTION(ppp.setCustomParameter(1, 30, "X=F8;Y=F8;Z=", "Param1"));

    crsd::PPPBlock pppBlock(NUM_SEQUENCES,
                            std::vector<size_t>(NUM_SEQUENCES, NUM_PULSES),
                            ppp);

    for (size_t txSequence = 0; txSequence < NUM_SEQUENCES; ++txSequence)
    {
        for (size_t pulse = 0; pulse < NUM_PULSES; ++pulse)
        {
            crsd::setPulseParameters(txSequence, pulse, pppBlock);

            const double ampSF = crsd::getRandom();
            pppBlock.setFxFreq0(ampSF, txSequence, pulse);

            const double fx1 = crsd::getRandom();
            pppBlock.setFX1(fx1, txSequence, pulse);

            const double fx2 = crsd::getRandom();
            pppBlock.setFX2(fx2, txSequence, pulse);

            TEST_ASSERT_EQ(fx2, pppBlock.getFX2(txSequence, pulse));

            const double addedParam1 = crsd::getRandom();
            pppBlock.setAddedPPP(addedParam1, txSequence, pulse, "Param1");

            const double addedParam2 = crsd::getRandom();
            TEST_EXCEPTION(pppBlock.setAddedPPP(addedParam2, txSequence, pulse, "Param3"));
            TEST_EXCEPTION(pppBlock.getAddedPPP<double>(txSequence, pulse, "Param3"));
        }
    }

    crsd::Ppp ppp2;
    crsd::setPPPXML(ppp2);
    ppp2.setCustomParameter(1, 27, "F8", "Param1");
    crsd::PPPBlock pppBlock2(NUM_SEQUENCES,
                             std::vector<size_t>(NUM_SEQUENCES,NUM_PULSES),
                             ppp2);

        for (size_t txSequence = 0; txSequence < NUM_SEQUENCES; ++txSequence)
    {
        for (size_t pulse = 0; pulse < NUM_PULSES; ++pulse)
        {
            crsd::setPulseParameters(txSequence, pulse, pppBlock2);

            const double addedParam1 = crsd::getRandom();
            pppBlock2.setAddedPPP(addedParam1, txSequence, pulse, "Param1");
            pppBlock2.getAddedPPP<double>(txSequence, pulse, "Param1");
        }
    }
}

TEST_CASE(testPppEquality)
{
    call_srand();

    crsd::Ppp ppp1;
    crsd::setPPPXML(ppp1);
    ppp1.setOffset(28, ppp1.fxFreq0);
    ppp1.setOffset(27, ppp1.fx1);
    ppp1.setOffset(29, ppp1.fx2);
    ppp1.setCustomParameter(1, 30, "F8", "Param1");
    ppp1.setCustomParameter(1, 31, "CI8", "Param2");
    crsd::PPPBlock pppBlock1(NUM_SEQUENCES,
                             std::vector<size_t>(NUM_SEQUENCES, NUM_PULSES),
                             ppp1);

    crsd::Ppp ppp2;
    crsd::setPPPXML(ppp2);
    ppp2.setOffset(28, ppp2.fxFreq0);
    ppp2.setOffset(27, ppp2.fx1);
    ppp2.setOffset(29, ppp2.fx2);
    ppp2.setCustomParameter(1, 30, "F8", "Param1");
    ppp2.setCustomParameter(1, 31, "CI8", "Param2");
    crsd::PPPBlock pppBlock2(NUM_SEQUENCES,
                             std::vector<size_t>(NUM_SEQUENCES, NUM_PULSES),
                             ppp2);

    TEST_ASSERT_EQ(ppp1, ppp2);
    TEST_ASSERT_TRUE(pppBlock1 == pppBlock2);

    for (size_t txSequence = 0; txSequence < NUM_SEQUENCES; ++txSequence)
    {
        for (size_t pulse = 0; pulse < NUM_PULSES; ++pulse)
        {
            setPulseParameters(txSequence, pulse, pppBlock1, pppBlock2);
            const double freq0 = crsd::getRandom();
            pppBlock1.setFxFreq0(freq0, txSequence, pulse);
            pppBlock2.setFxFreq0(freq0, txSequence, pulse);

            const double fx1 = crsd::getRandom();
            pppBlock1.setFX1(fx1, txSequence, pulse);
            pppBlock2.setFX1(fx1, txSequence, pulse);

            const double fx2 = crsd::getRandom();
            pppBlock1.setFX2(fx2, txSequence, pulse);
            pppBlock2.setFX2(fx2, txSequence, pulse);

            const double addedParam1 = crsd::getRandom();
            pppBlock1.setAddedPPP(addedParam1, txSequence, pulse, "Param1");
            pppBlock2.setAddedPPP(addedParam1, txSequence, pulse, "Param1");

            const std::complex<int> addedParam2(3,4);
            pppBlock1.setAddedPPP(addedParam2, txSequence, pulse, "Param2");
            pppBlock2.setAddedPPP(addedParam2, txSequence, pulse, "Param2");
        }
    }
    TEST_ASSERT_EQ(pppBlock1, pppBlock2);
}

TEST_CASE(testLoadPPPBlockFromMemory)
{
    call_srand();

    // For ease of testing, we make the somewhat specious assumption
    // that an item of PPP data is equivalent to a double.
    static_assert(sizeof(double) == crsd::PPPType::WORD_BYTE_SIZE,
                  "This test is not valid for compilers with "
                  "sizeof(double) != 8");

    crsd::Ppp ppp;
    crsd::setPPPXML(ppp);
    std::vector<std::vector<double>> dataSource(NUM_SEQUENCES);
    std::vector<const void*> pppData(NUM_SEQUENCES);
    const size_t elementsPerChannel = ppp.getReqSetSize() * NUM_PULSES;
    for (size_t txSequence = 0; txSequence < NUM_SEQUENCES; txSequence++)
    {
        dataSource[txSequence].reserve(elementsPerChannel);
        for (size_t ii = 0; ii < elementsPerChannel; ++ii)
        {
            dataSource[txSequence].push_back(txSequence + ii * NUM_SEQUENCES);
        }
        pppData[txSequence] = dataSource[txSequence].data();
    }

    crsd::PPPBlock pppBlock(NUM_SEQUENCES,
                            std::vector<size_t>(NUM_SEQUENCES, NUM_PULSES),
                            ppp,
                            pppData);

    TEST_ASSERT_EQ(pppBlock.getTxStart(0, 0).first, 0);
    TEST_ASSERT_EQ(pppBlock.getTxStart(1, 0).first, 1);
    TEST_ASSERT_EQ(pppBlock.getTxStart(2, 0).first, 2);

    TEST_ASSERT_EQ(pppBlock.getTxStart(0, 1).first, ppp.getReqSetSize() * NUM_SEQUENCES);
    TEST_ASSERT_EQ(pppBlock.getTxStart(1, 1).first,
                   1 + ppp.getReqSetSize() * NUM_SEQUENCES);
    TEST_ASSERT_EQ(pppBlock.getTxStart(2, 1).first,
                   2 + ppp.getReqSetSize() * NUM_SEQUENCES);

    TEST_ASSERT_EQ(pppBlock.getTxPos(0, 0)[0], 6);
    TEST_ASSERT_EQ(pppBlock.getTxPos(0, 0)[1], 9);
    TEST_ASSERT_EQ(pppBlock.getTxPos(0, 0)[2], 12);
}

TEST_MAIN(
    TEST_CHECK(testPppRequired);
    TEST_CHECK(testPppThrow);
    TEST_CHECK(testPppEquality);
    TEST_CHECK(testLoadPPPBlockFromMemory);
    )
