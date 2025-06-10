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

#include <crsd/PVP.h>
#include <crsd/PVPBlock.h>
#include <crsd/TestDataGenerator.h>

#include "TestCase.h"

static constexpr size_t NUM_CHANNELS = 3;
static constexpr size_t NUM_VECTORS = 2;

static void call_srand()
{
    const auto f = []() { ::srand(174); return nullptr; };
    static const auto result = f();
    std::ignore = result;
}

void setVectorParameters(size_t channel,
                         size_t vector,
                         crsd::PVPBlock& pvpBlock,
                         crsd::PVPBlock& pvpBlock2)
{
    const double doubleVal = crsd::getRandom();
    const int64_t intVal = static_cast<int64_t>(crsd::getRandom());
    const crsd::Vector2 vec2 = crsd::getRandomVector2();
    const crsd::Vector3 vec3 = crsd::getRandomVector3();
    std::pair<int64_t,double> pairVal(intVal,doubleVal);

    pvpBlock.setRcvStart(pairVal, channel, vector);
    pvpBlock2.setRcvStart(pairVal, channel, vector);

    pvpBlock.setRcvPos(vec3, channel, vector);
    pvpBlock2.setRcvPos(vec3, channel, vector);

    pvpBlock.setRcvVel(vec3, channel, vector);
    pvpBlock2.setRcvVel(vec3, channel, vector);

    pvpBlock.setAmpSF(doubleVal, channel, vector);
    pvpBlock2.setAmpSF(doubleVal, channel, vector);

    pvpBlock.setDFIC0(doubleVal, channel, vector);
    pvpBlock2.setDFIC0(doubleVal, channel, vector);

    pvpBlock.setDGRGC(doubleVal, channel, vector);
    pvpBlock2.setDGRGC(doubleVal, channel, vector);

    pvpBlock.setFICRate(doubleVal, channel, vector);
    pvpBlock2.setFICRate(doubleVal, channel, vector);

    pvpBlock.setFRCV1(doubleVal, channel, vector);
    pvpBlock2.setFRCV1(doubleVal, channel, vector);

    pvpBlock.setRefFreq(doubleVal, channel, vector);
    pvpBlock2.setRefFreq(doubleVal, channel, vector);

    pvpBlock.setFRCV2(doubleVal, channel, vector);
    pvpBlock2.setFRCV2(doubleVal, channel, vector);

    pvpBlock.setRcvEB(vec2, channel, vector);
    pvpBlock2.setRcvEB(vec2, channel, vector);

    pvpBlock.setRcvACX(vec3, channel, vector);
    pvpBlock2.setRcvACX(vec3, channel, vector);

    pvpBlock.setRcvACY(vec3, channel, vector);
    pvpBlock2.setRcvACY(vec3, channel, vector);

    pvpBlock.setRefPhi0(pairVal, channel, vector);
    pvpBlock2.setRefPhi0(pairVal, channel, vector);

    pvpBlock.setSignal(intVal, channel, vector);
    pvpBlock2.setSignal(intVal, channel, vector);

}

TEST_CASE(testPvpRequired)
{
    call_srand();
    crsd::Pvp pvp;
    crsd::setPVPXML(pvp);
    crsd::PVPBlock pvpBlock(NUM_CHANNELS,
                            std::vector<size_t>(NUM_CHANNELS, NUM_VECTORS),
                            pvp);

    for (size_t channel = 0; channel < NUM_CHANNELS; ++channel)
    {
        for (size_t vector = 0; vector < NUM_VECTORS; ++vector)
        {
            crsd::setVectorParameters(channel, vector, pvpBlock);
        }
    }
}

TEST_CASE(testPvpThrow)
{
    call_srand();

    crsd::Pvp pvp;
    crsd::setPVPXML(pvp);
    pvp.setOffset(29, pvp.ampSF);
    pvp.setOffset(28, pvp.frcv1);
    TEST_EXCEPTION(pvp.setOffset(15, pvp.txPulseIndex)); // Overwrite block
    pvp.setOffset(27, pvp.txPulseIndex);

    pvp.setCustomParameter(1, 30, "F8", "Param1");
    pvp.setCustomParameter(1, 31, "F8", "Param2");
    TEST_EXCEPTION(pvp.setCustomParameter(1, 30, "F8", "Param1")); //Rewriting to existing memory block
    TEST_EXCEPTION(pvp.setCustomParameter(1, 30, "X=F8;YF8;", "Param1")); //
    TEST_EXCEPTION(pvp.setCustomParameter(1, 30, "X=F8;Y=F8;Z=", "Param1"));

    crsd::PVPBlock pvpBlock(NUM_CHANNELS,
                            std::vector<size_t>(NUM_CHANNELS, NUM_VECTORS),
                            pvp);

    for (size_t channel = 0; channel < NUM_CHANNELS; ++channel)
    {
        for (size_t vector = 0; vector < NUM_VECTORS; ++vector)
        {
            crsd::setVectorParameters(channel, vector, pvpBlock);

            const double ampSF = crsd::getRandom();
            pvpBlock.setAmpSF(ampSF, channel, vector);

            const double fxN1 = crsd::getRandom();
            pvpBlock.setFRCV1(fxN1, channel, vector);

            const double toaE1 = crsd::getRandom();
            pvpBlock.setFRCV2(toaE1, channel, vector);

            TEST_ASSERT_EQ(toaE1, pvpBlock.getFRCV2(channel, vector));

            const double addedParam1 = crsd::getRandom();
            pvpBlock.setAddedPVP(addedParam1, channel, vector, "Param1");

            const double addedParam2 = crsd::getRandom();
            TEST_EXCEPTION(pvpBlock.setAddedPVP(addedParam2, channel, vector, "Param3"));
            TEST_EXCEPTION(pvpBlock.getAddedPVP<double>(channel, vector, "Param3"));
        }
    }

    crsd::Pvp pvp2;
    crsd::setPVPXML(pvp2);
    pvp2.setCustomParameter(1, 27, "F8", "Param1");
    crsd::PVPBlock pvpBlock2(NUM_CHANNELS,
                             std::vector<size_t>(NUM_CHANNELS,NUM_VECTORS),
                             pvp2);

        for (size_t channel = 0; channel < NUM_CHANNELS; ++channel)
    {
        for (size_t vector = 0; vector < NUM_VECTORS; ++vector)
        {
            crsd::setVectorParameters(channel, vector, pvpBlock2);

            const double addedParam1 = crsd::getRandom();
            pvpBlock2.setAddedPVP(addedParam1, channel, vector, "Param1");
            pvpBlock2.getAddedPVP<double>(channel, vector, "Param1");
        }
    }
}

TEST_CASE(testPvpEquality)
{
    call_srand();

    crsd::Pvp pvp1;
    crsd::setPVPXML(pvp1);
    pvp1.setOffset(28, pvp1.ampSF);
    pvp1.setOffset(27, pvp1.frcv1);
    pvp1.setOffset(29, pvp1.frcv2);
    pvp1.setCustomParameter(1, 30, "F8", "Param1");
    pvp1.setCustomParameter(1, 31, "CI8", "Param2");
    crsd::PVPBlock pvpBlock1(NUM_CHANNELS,
                             std::vector<size_t>(NUM_CHANNELS, NUM_VECTORS),
                             pvp1);

    crsd::Pvp pvp2;
    crsd::setPVPXML(pvp2);
    pvp2.setOffset(28, pvp2.ampSF);
    pvp2.setOffset(27, pvp2.frcv1);
    pvp2.setOffset(29, pvp2.frcv2);
    pvp2.setCustomParameter(1, 30, "F8", "Param1");
    pvp2.setCustomParameter(1, 31, "CI8", "Param2");
    crsd::PVPBlock pvpBlock2(NUM_CHANNELS,
                             std::vector<size_t>(NUM_CHANNELS, NUM_VECTORS),
                             pvp2);

    TEST_ASSERT_EQ(pvp1, pvp2);
    TEST_ASSERT_TRUE(pvpBlock1 == pvpBlock2);

    for (size_t channel = 0; channel < NUM_CHANNELS; ++channel)
    {
        for (size_t vector = 0; vector < NUM_VECTORS; ++vector)
        {
            setVectorParameters(channel, vector, pvpBlock1, pvpBlock2);
            const double ampSF = crsd::getRandom();
            pvpBlock1.setAmpSF(ampSF, channel, vector);
            pvpBlock2.setAmpSF(ampSF, channel, vector);

            const double fxN1 = crsd::getRandom();
            pvpBlock1.setFRCV1(fxN1, channel, vector);
            pvpBlock2.setFRCV1(fxN1, channel, vector);

            const double fxN2 = crsd::getRandom();
            pvpBlock1.setFRCV2(fxN2, channel, vector);
            pvpBlock2.setFRCV2(fxN2, channel, vector);

            const double addedParam1 = crsd::getRandom();
            pvpBlock1.setAddedPVP(addedParam1, channel, vector, "Param1");
            pvpBlock2.setAddedPVP(addedParam1, channel, vector, "Param1");

            const std::complex<int> addedParam2(3,4);
            pvpBlock1.setAddedPVP(addedParam2, channel, vector, "Param2");
            pvpBlock2.setAddedPVP(addedParam2, channel, vector, "Param2");
        }
    }
    TEST_ASSERT_EQ(pvpBlock1, pvpBlock2);
}

TEST_CASE(testLoadPVPBlockFromMemory)
{
    call_srand();

    // For ease of testing, we make the somewhat specious assumption
    // that an item of PVP data is equivalent to a double.
    static_assert(sizeof(double) == crsd::PVPType::WORD_BYTE_SIZE,
                  "This test is not valid for compilers with "
                  "sizeof(double) != 8");

    crsd::Pvp pvp;
    crsd::setPVPXML(pvp);
    std::vector<std::vector<double>> dataSource(NUM_CHANNELS);
    std::vector<const void*> pvpData(NUM_CHANNELS);
    const size_t elementsPerChannel = pvp.getReqSetSize() * NUM_VECTORS;
    for (size_t channel = 0; channel < NUM_CHANNELS; channel++)
    {
        dataSource[channel].reserve(elementsPerChannel);
        for (size_t ii = 0; ii < elementsPerChannel; ++ii)
        {
            dataSource[channel].push_back(channel + ii * NUM_CHANNELS);
        }
        pvpData[channel] = dataSource[channel].data();
    }

    crsd::PVPBlock pvpBlock(NUM_CHANNELS,
                            std::vector<size_t>(NUM_CHANNELS, NUM_VECTORS),
                            pvp,
                            pvpData);

    TEST_ASSERT_EQ(pvpBlock.getRcvStart(0, 0).first, 0);
    TEST_ASSERT_EQ(pvpBlock.getRcvStart(1, 0).first, 1);
    TEST_ASSERT_EQ(pvpBlock.getRcvStart(2, 0).first, 2);

    TEST_ASSERT_EQ(pvpBlock.getRcvStart(0, 1).first, pvp.getReqSetSize() * NUM_CHANNELS);
    TEST_ASSERT_EQ(pvpBlock.getRcvStart(1, 1).first,
                   1 + pvp.getReqSetSize() * NUM_CHANNELS);
    TEST_ASSERT_EQ(pvpBlock.getRcvStart(2, 1).first,
                   2 + pvp.getReqSetSize() * NUM_CHANNELS);

    TEST_ASSERT_EQ(pvpBlock.getRcvPos(0, 0)[0], 6);
    TEST_ASSERT_EQ(pvpBlock.getRcvPos(0, 0)[1], 9);
    TEST_ASSERT_EQ(pvpBlock.getRcvPos(0, 0)[2], 12);
}

TEST_MAIN(
    TEST_CHECK(testPvpRequired);
    TEST_CHECK(testPvpThrow);
    TEST_CHECK(testPvpEquality);
    TEST_CHECK(testLoadPVPBlockFromMemory);
    )
