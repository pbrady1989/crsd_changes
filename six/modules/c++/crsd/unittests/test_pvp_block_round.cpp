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

#include <thread>

#include <TestCase.h>
#include <crsd/CRSDReader.h>
#include <crsd/CRSDWriter.h>
#include <crsd/Metadata.h>
#include <crsd/PVP.h>
#include <crsd/PVPBlock.h>
#include <crsd/ReferenceGeometry.h>
#include <crsd/TestDataGenerator.h>
#include <crsd/Wideband.h>
#include <io/FileInputStream.h>
#include <io/FileOutputStream.h>
#include <io/TempFile.h>
#include <crsd/Enums.h>
#include <stdlib.h>
#include <types/RowCol.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

template <typename T>
std::vector<std::complex<T>> generateComplexData(size_t length)
{
    std::vector<std::complex<T>> data(length);
    srand(0);
    for (size_t ii = 0; ii < data.size(); ++ii)
    {
        float real = static_cast<T>(rand() / 100);
        float imag = static_cast<T>(rand() / 100);
        data[ii] = std::complex<T>(real, imag);
    }
    return data;
}

void setPVPBlock(const types::RowCol<size_t> dims,
                 crsd::PVPBlock& pvpBlock,
                 const std::vector<std::string>& addedParams)
{
    const size_t numChannels = 1;
    const std::vector<size_t> numVectors(numChannels, dims.row);

    for (size_t ii = 0; ii < numChannels; ++ii)
    {
        for (size_t jj = 0; jj < numVectors[ii]; ++jj)
        {
            setVectorParameters(ii, jj, pvpBlock);
            for (size_t idx = 0; idx < addedParams.size(); ++idx)
            {
                const double val = crsd::getRandom();
                pvpBlock.setAddedPVP(val, ii, jj, addedParams[idx]);
            }
        }
    }
}

void setPPPBlock(const types::RowCol<size_t> dims,
                 crsd::PPPBlock& pppBlock,
                 const std::vector<std::string>& addedParams)
{
    const size_t numSequences = 1;
    const std::vector<size_t> numPulses(numSequences, dims.row);

    for (size_t ii = 0; ii < numSequences; ++ii)
    {
        for (size_t jj = 0; jj < numPulses[ii]; ++jj)
        {
            setPulseParameters(ii, jj, pppBlock);
            for (size_t idx = 0; idx < addedParams.size(); ++idx)
            {
                const double val = crsd::getRandom();
                pppBlock.setAddedPPP(val, ii, jj, addedParams[idx]);
            }
        }
    }
}

template <typename T>
void writeCRSD(const std::string& outPathname,
               size_t numThreads,
               const types::RowCol<size_t> dims,
               const std::vector<std::complex<T>>& writeData,
               crsd::Metadata& metadata,
               crsd::PVPBlock& pvpBlock,
               crsd::PPPBlock& pppBlock)
{
    const size_t numChannels = 1;
    const size_t numTxSequences = 1;
    std::cout << "Writing CRSD data to " << outPathname << std::endl;
    crsd::CRSDWriter writer(metadata,
                            outPathname,
                            std::vector<std::string>(),
                            numThreads);
    std::cout << "Writing metadata portion..." << std::endl;
    writer.writeMetadata(pvpBlock, pppBlock);
    std::cout << "Writing PVP data..." << std::endl;
    writer.writePVPData(pvpBlock);
    std::cout << "Writing PPP data..." << std::endl;
    writer.writePPPData(pppBlock);
    std::cout << "Successfully wrote PPP and PVP data..." << std::endl;
    for (size_t ii = 0; ii < numChannels; ++ii)
    {
        std::cout << "Writing CRSD data for channel " << ii << "..." << std::endl;
        writer.writeCRSDData(writeData.data(), dims.area());
    }
}

bool checkData(const std::string& pathname,
               size_t numThreads,
               crsd::Metadata& metadata,
               crsd::PVPBlock& pvpBlock)
{
    crsd::CRSDReader reader(pathname, numThreads);

    if (metadata.pvp != reader.getMetadata().pvp)
    {
        return false;
    }
    if (pvpBlock != reader.getPVPBlock())
    {
        return false;
    }
    return true;
}

template <typename T>
bool runTest(bool /*scale*/,
             const std::vector<std::complex<T>>& writeData,
             crsd::Metadata& meta,
             crsd::PVPBlock& pvpBlock, 
             crsd::PPPBlock& pppBlock,
             const types::RowCol<size_t> dims)
{
    io::TempFile tempfile;
    const size_t numThreads = 1;//std::thread::hardware_concurrency();
    writeCRSD("./output.crsd", numThreads, dims, writeData, meta, pvpBlock, pppBlock);
    return checkData("./output.crsd", numThreads, meta, pvpBlock);
}

TEST_CASE(testPVPBlockSimple)
{
    six::CRSDType type = six::CRSDType::SAR;
    const types::RowCol<size_t> dims(128, 256);
    const std::vector<std::complex<int16_t>> writeData =
            generateComplexData<int16_t>(dims.area());
    const bool scale = false;
    crsd::Metadata meta = crsd::Metadata(type);
    crsd::setUpData(meta, dims, writeData);
    meta.pvp.reset(new crsd::Pvp());
    meta.ppp.reset(new crsd::Ppp());

    meta.setVersion("1.0.0");
    crsd::setPVPXML(*(meta.pvp));
    crsd::setPPPXML(*(meta.ppp));
    crsd::PVPBlock pvpBlock(*(meta.pvp), meta.data);
    std::vector<std::string> addedParams;
    setPVPBlock(dims,
                pvpBlock,
                addedParams);
    crsd::PPPBlock pppBlock(*(meta.ppp), meta.data);
    std::vector<std::string> addedParams2;
    setPPPBlock(dims,
                pppBlock,
                addedParams2);
    TEST_ASSERT_TRUE(runTest(scale, writeData, meta, pvpBlock, pppBlock, dims));
}

TEST_CASE(testPVPBlockOptional)
{
    const types::RowCol<size_t> dims(128, 256);
    const std::vector<std::complex<int16_t>> writeData =
            generateComplexData<int16_t>(dims.area());
    const bool scale = false;
    crsd::Metadata meta = crsd::Metadata();
    crsd::setUpData(meta, dims, writeData);
    crsd::setPVPXML(*(meta.pvp));
    meta.pvp->setOffset(27, meta.pvp->frcv1);
    meta.pvp->setOffset(28, meta.pvp->frcv2);
    meta.data.receiveParameters->numBytesPVP += 2 * 8;
    crsd::PVPBlock pvpBlock(*(meta.pvp), meta.data);
    std::vector<std::string> addedParams;
    setPVPBlock(dims,
                pvpBlock,
                addedParams);
    crsd::setPVPXML(*(meta.pvp));
    meta.ppp->setOffset(27, meta.ppp->fx1);
    meta.ppp->setOffset(28, meta.ppp->fx2);
    meta.data.transmitParameters->numBytesPPP += 2 * 8;
    crsd::PPPBlock pppBlock(*(meta.ppp), meta.data);
    std::vector<std::string> addedParams2;
    setPPPBlock(dims,
                pppBlock,
                addedParams2);

    TEST_ASSERT_TRUE(runTest(scale, writeData, meta, pvpBlock, pppBlock, dims));
}

TEST_CASE(testPVPBlockAdditional)
{
    const types::RowCol<size_t> dims(128, 256);
    const std::vector<std::complex<int16_t>> writeData =
            generateComplexData<int16_t>(dims.area());
    const bool scale = false;
    crsd::Metadata meta = crsd::Metadata();
    crsd::setUpData(meta, dims, writeData);
    crsd::setPVPXML(*(meta.pvp));
    meta.pvp->setCustomParameter(1, 27, "F8", "param1");
    meta.pvp->setCustomParameter(1, 28, "F8", "param2");
    meta.data.receiveParameters->numBytesPVP += 2 * 8;
    crsd::PVPBlock pvpBlock(*(meta.pvp), meta.data);
    std::vector<std::string> addedParams;
    addedParams.push_back("param1");
    addedParams.push_back("param2");
    setPVPBlock(dims,
                pvpBlock,
                addedParams);
    crsd::setPPPXML(*(meta.ppp));
    meta.ppp->setCustomParameter(1, 27, "F8", "param1");
    meta.ppp->setCustomParameter(1, 28, "F8", "param2");
    meta.data.transmitParameters->numBytesPPP += 2 * 8;
    meta.setVersion("1.0.0");
    crsd::PPPBlock pppBlock(*(meta.ppp), meta.data);
    std::vector<std::string> addedParams2;
    addedParams2.push_back("param1");
    addedParams2.push_back("param2");
    setPPPBlock(dims,
                pppBlock,
                addedParams);


    TEST_ASSERT_TRUE(runTest(scale, writeData, meta, pvpBlock, pppBlock, dims));
}

TEST_MAIN(
        TEST_CHECK(testPVPBlockSimple);
        //TEST_CHECK(testPVPBlockOptional);
        //TEST_CHECK(testPVPBlockAdditional);
        )
