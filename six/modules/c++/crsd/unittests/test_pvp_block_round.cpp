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

static constexpr size_t NUM_SUPPORT = 3;
static constexpr size_t NUM_ROWS = 3;
static constexpr size_t NUM_COLS = 4;

template<typename T>
std::vector<T> generateSupportData(size_t length)
{
    std::vector<T> data(length);
    srand(0);
    for (size_t ii = 0; ii < data.size(); ++ii)
    {
        data[ii] = rand() % 16;
    }
    return data;
}

void setSupport(crsd::Data& d)
{
    d.setSupportArray("1.0", NUM_ROWS, NUM_COLS, sizeof(double), 0);
    d.setSupportArray("2.0", NUM_ROWS, NUM_COLS, sizeof(double), NUM_ROWS*NUM_COLS*sizeof(double));
    d.setSupportArray("AddedSupport", NUM_ROWS, NUM_COLS, sizeof(double), 2*NUM_ROWS*NUM_COLS*sizeof(double));
}

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
               const std::vector<double>& writeSupportData,
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
    std::cout << "Writing support block..." << std::endl;
    writer.writeSupportData(writeSupportData.data());
    std::cout << "Writing PPP data..." << std::endl;
    writer.writePPPData(pppBlock);
    std::cout << "Writing PVP data..." << std::endl;
    writer.writePVPData(pvpBlock);
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
               crsd::PVPBlock& pvpBlock,
               crsd::PPPBlock& pppBlock)
{
    crsd::CRSDReader reader(pathname, numThreads);

    if (reader.getMetadata().data.getNumChannels() != metadata.data.getNumChannels())
    {
        std::cout << "Number of channels mismatch: "
                  << reader.getMetadata().data.getNumChannels() << " vs "
                  << metadata.data.getNumChannels() << std::endl;
        return false;
    }
    if (reader.getMetadata().data.getNumTxSequences() != metadata.data.getNumTxSequences())
    {
        std::cout << "Number of Tx Sequences mismatch: "
                  << reader.getMetadata().data.getNumTxSequences() << " vs "
                  << metadata.data.getNumTxSequences() << std::endl;
        return false;
    }
    if (reader.getMetadata().data.getNumSupportArrays() != metadata.data.getNumSupportArrays())
    {
        std::cout << "Number of support arrays mismatch: "
                  << reader.getMetadata().data.getNumSupportArrays() << " vs "
                  << metadata.data.getNumSupportArrays() << std::endl;
        return false;
    }
    if (reader.getMetadata().data.getNumBytesPVPSet() != metadata.data.getNumBytesPVPSet())
    {
        std::cout << "Number of PVP bytes mismatch: "
                  << reader.getMetadata().data.getNumBytesPVPSet() << " vs "
                  << metadata.data.getNumBytesPVPSet() << std::endl;
        return false;
    }
    if (reader.getMetadata().data.getNumBytesPPPSet() != metadata.data.getNumBytesPPPSet())
    {
        std::cout << "Number of PPP bytes mismatch: "
                  << reader.getMetadata().data.getNumBytesPPPSet() << " vs "
                  << metadata.data.getNumBytesPPPSet() << std::endl;
        return false;
    }
    if (metadata.global != reader.getMetadata().global)
    {
        std::cout << "Global metadata mismatch." << std::endl;
        return false;
    }
    if (metadata.data != reader.getMetadata().data)
    {
        std::cout << "Data metadata mismatch." << std::endl;
        return false;
    }
    if (metadata.sarInfo != reader.getMetadata().sarInfo)
    {
        std::cout << "SAR info metadata mismatch." << std::endl;
        return false;
    }
    if (metadata.productInfo != reader.getMetadata().productInfo)
    {
        std::cout << "Product info metadata mismatch." << std::endl;
        return false;
    }
    if (metadata.receiveInfo != reader.getMetadata().receiveInfo)
    {
        std::cout << "Receive info metadata mismatch." << std::endl;
        return false;
    }
    if (metadata.transmitInfo != reader.getMetadata().transmitInfo)
    {
        std::cout << "Transmit info metadata mismatch." << std::endl;
        return false;
    }
    if (metadata.sceneCoordinates != reader.getMetadata().sceneCoordinates)
    {
        std::cout << "Scene coordinates metadata mismatch." << std::endl;
        return false;
    }
    if (metadata.referenceGeometry != reader.getMetadata().referenceGeometry)
    {
        std::cout << "Reference geometry metadata mismatch." << std::endl;
        return false;
    }
    if (metadata.ppp != reader.getMetadata().ppp)
    {
        std::cout << "PPP metadata mismatch." << std::endl;
        return false;
    }
    if (pppBlock != reader.getPPPBlock())
    {
        std::cout << "PPPBlock mismatch." << std::endl;
        std::cout << "pppBlock.getFX1(): " << pppBlock.getFX1(0,0) << std::endl;
        std::cout << "reader.getPPPBlock().getFX1(): " << reader.getPPPBlock().getFX1(0,0) << std::endl;
        std::cout << "pppBlock.getFX2(): " << pppBlock.getFX2(0,0) << std::endl;
        std::cout << "reader.getPPPBlock().getFX2(): " << reader.getPPPBlock().getFX2(0,0) << std::endl;
        std::cout << "pppBlock.getFX1(): " << pppBlock.getFxFreq0(0,0) << std::endl;
        std::cout << "reader.getPPPBlock().getFX1(): " << reader.getPPPBlock().getFxFreq0(0,0) << std::endl;
        std::cout << "pppBlock.getFX1(): " << pppBlock.getFxRate(0,0) << std::endl;
        std::cout << "reader.getPPPBlock().getFX1(): " << reader.getPPPBlock().getFxRate(0,0) << std::endl;
        std::cout << "pppBlock.getFX1(): " << pppBlock.getFxResponseIndex(0,0) << std::endl;
        std::cout << "reader.getPPPBlock().getFX1(): " << reader.getPPPBlock().getFxResponseIndex(0,0) << std::endl;
        std::cout << "pppBlock.getFX1(): " << pppBlock.getNumBytesPPPSet() << std::endl;
        std::cout << "reader.getPPPBlock().getFX1(): " << reader.getPPPBlock().getNumBytesPPPSet() << std::endl;
        std::cout << "pppBlock.getFX1(): " << pppBlock.getTxACX(0,0) << std::endl;
        std::cout << "reader.getPPPBlock().getFX1(): " << reader.getPPPBlock().getTxACX(0,0) << std::endl;
        std::cout << "pppBlock.getTxRadInt(): " << pppBlock.getTxRadInt(0,0) << std::endl;
        std::cout << "reader.getPPPBlock().getTxRadInt(): " << reader.getPPPBlock().getTxRadInt(0,0) << std::endl;
        std::cout << "pppBlock.getFX1(): " << pppBlock.getTxACY(0,0) << std::endl;
        std::cout << "reader.getPPPBlock().getFX1(): " << reader.getPPPBlock().getTxACY(0,0) << std::endl;
        std::cout << "pppBlock.getFX1(): " << pppBlock.getTXMT(0,0) << std::endl;
        std::cout << "reader.getPPPBlock().getFX1(): " << reader.getPPPBlock().getTXMT(0,0) << std::endl;
        std::cout << "pppBlock.getPhiX0(): " << pppBlock.getPhiX0(0,0).first << std::endl;
        std::cout << "reader.getPPPBlock().getPhiX0(): " << reader.getPPPBlock().getPhiX0(0,0).first << std::endl;
        std::cout << "pppBlock.getPhiX0(): " << pppBlock.getPhiX0(0,0).second << std::endl;
        std::cout << "reader.getPPPBlock().getPhiX0(): " << reader.getPPPBlock().getPhiX0(0,0).second << std::endl;
        std::cout << "pppBlock.getTxPos(): " << pppBlock.getTxPos(0,0) << std::endl;
        std::cout << "reader.getPPPBlock().getTxPos(): " << reader.getPPPBlock().getTxPos(0,0) << std::endl;
        std::cout << "pppBlock.getTxVel(): " << pppBlock.getTxVel(0,0) << std::endl;
        std::cout << "reader.getPPPBlock().getTxVel(): " << reader.getPPPBlock().getTxVel(0,0) << std::endl;
        std::cout << "pppBlock.getTxTime(): " << pppBlock.getTxStart(0,0).first << std::endl;
        std::cout << "reader.getPPPBlock().getTxStart(): " << reader.getPPPBlock().getTxStart(0,0).first << std::endl;
        std::cout << "pppBlock.getTxTime(): " << pppBlock.getTxStart(0,0).second << std::endl;
        std::cout << "reader.getPPPBlock().getTxStart(): " << reader.getPPPBlock().getTxStart(0,0).second << std::endl;

        return false;
    }
    if (metadata.pvp != reader.getMetadata().pvp)
    {
        std::cout << "PVP metadata mismatch." << std::endl;
        return false;
    }
    if (pvpBlock != reader.getPVPBlock())
    {
        std::cout << "PVPBlock mismatch." << std::endl;
    
        std::cout << "pvpBlock.getRefPhi0(): " << pvpBlock.getRefPhi0(0,0).first << std::endl;
        std::cout << "reader.getPVPBlock().getRefPhi0(): " << reader.getPVPBlock().getRefPhi0(0,0).first << std::endl;
        std::cout << "pvpBlock.getRefPhi0(): " << pvpBlock.getRefPhi0(0,0).second << std::endl;
        std::cout << "reader.getPVPBlock().getRefPhi0(): " << reader.getPVPBlock().getRefPhi0(0,0).second << std::endl;
        std::cout << "pvpBlock.getRcvStart(): " << pvpBlock.getRcvStart(0,0).first << std::endl;
        std::cout << "reader.getPVPBlock().getRcvStart(): " << reader.getPVPBlock().getRcvStart(0,0).first << std::endl;
        std::cout << "pvpBlock.getRcvStart(): " << pvpBlock.getRcvStart(0,0).second << std::endl;
        std::cout << "reader.getPVPBlock().getRcvStart(): " << reader.getPVPBlock().getRcvStart(0,0).second << std::endl;

        std::cout << "pvpBlock.getRefPhi0(): " << pvpBlock.getRefPhi0(0,1).first << std::endl;
        std::cout << "reader.getPVPBlock().getRefPhi0(): " << reader.getPVPBlock().getRefPhi0(0,1).first << std::endl;
        std::cout << "pvpBlock.getRefPhi0(): " << pvpBlock.getRefPhi0(0,1).second << std::endl;
        std::cout << "reader.getPVPBlock().getRefPhi0(): " << reader.getPVPBlock().getRefPhi0(0,1).second << std::endl;
        std::cout << "pvpBlock.getRcvStart(): " << pvpBlock.getRcvStart(0,1).first << std::endl;
        std::cout << "reader.getPVPBlock().getRcvStart(): " << reader.getPVPBlock().getRcvStart(0,1).first << std::endl;
        std::cout << "pvpBlock.getRcvStart(): " << pvpBlock.getRcvStart(0,1).second << std::endl;
        std::cout << "reader.getPVPBlock().getRcvStart(): " << reader.getPVPBlock().getRcvStart(0,1).second << std::endl;

        return false;
    }
    
    return true;
}

template <typename T>
bool runTest(bool /*scale*/,
             const std::vector<std::complex<T>>& writeData,
             const std::vector<double>& writeSupportData,
             crsd::Metadata& meta,
             crsd::PVPBlock& pvpBlock, 
             crsd::PPPBlock& pppBlock,
             const types::RowCol<size_t> dims)
{
    io::TempFile tempfile;
    const size_t numThreads = std::thread::hardware_concurrency();
    setSupport(meta.data);
    writeCRSD("./output.crsd", numThreads, dims, writeData, writeSupportData, meta, pvpBlock, pppBlock);
    return checkData("./output.crsd", numThreads, meta, pvpBlock, pppBlock);
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
    const std::vector<double> writeSupportData =
            generateSupportData<double>(NUM_SUPPORT*dims.area());
    TEST_ASSERT_TRUE(runTest(scale, writeData, writeSupportData, meta, pvpBlock, pppBlock, dims));
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

    //TEST_ASSERT_TRUE(runTest(scale, writeData, meta, pvpBlock, pppBlock, dims));
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


    //TEST_ASSERT_TRUE(runTest(scale, writeData, meta, pvpBlock, pppBlock, dims));
}

TEST_MAIN(
        TEST_CHECK(testPVPBlockSimple);
        //TEST_CHECK(testPVPBlockOptional);
        //TEST_CHECK(testPVPBlockAdditional);
        )
