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

#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <thread>
#include <std/span>

#include <nitf/coda-oss.hpp>
#include <types/RowCol.h>
#include <io/TempFile.h>
#include <io/FileInputStream.h>
#include <io/FileOutputStream.h>
#include <crsd/CRSDWriter.h>
#include <crsd/CRSDReader.h>
#include <crsd/Wideband.h>
#include <crsd/Metadata.h>
#include <crsd/PVP.h>
#include <crsd/PVPBlock.h>
#include <crsd/ReferenceGeometry.h>
#include <crsd/TestDataGenerator.h>
#include <TestCase.h>

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

std::vector<std::byte> checkSupportData(
        const std::string& pathname,
        size_t /*size*/,
        size_t numThreads)
{

    crsd::CRSDReader reader(pathname, numThreads);
    const crsd::SupportBlock& supportBlock = reader.getSupportBlock();
    std::unique_ptr<std::byte[]> readPtr;
    supportBlock.readAll(numThreads, readPtr);
    std::vector<std::byte> readData(readPtr.get(), readPtr.get() + reader.getMetadata().data.getAllSupportSize());
    return readData;
}

/*!
 * Tests write and read of Signal Block with compressed data
 * Fails if values don't match
 */
std::vector<std::byte> generateCompressedData(size_t length)
{
    std::vector<std::byte> data(length);
    srand(0);
    for (size_t ii = 0; ii < data.size(); ++ii)
    {
        data[ii] = static_cast<std::byte>(rand() % 16);
    }
    return data;
}

void writeCompressedCRSD(const std::string& outPathname, size_t numThreads,
        const types::RowCol<size_t> dims,
        const std::vector<std::byte>& writeData,
        const std::vector<double>& writeSupportData,
        crsd::Metadata& metadata,
        crsd::PVPBlock& pvpBlock,
        crsd::PPPBlock& pppBlock)
{
    const size_t numChannels = 1;
    const std::vector<size_t> numVectors(numChannels, dims.row);

    for (size_t ii = 0; ii < numChannels; ++ii)
    {
        for (size_t jj = 0; jj < numVectors[ii]; ++jj)
        {
            crsd::setVectorParameters(ii, jj, pvpBlock);
        }
    }

    crsd::CRSDWriter writer(metadata, outPathname, std::vector<std::string>(), numThreads);

    std::cout << "Writing metadata portion..." << std::endl;
    writer.writeMetadata(pvpBlock, pppBlock);
    std::cout << "Writing support block..." << std::endl;
    writer.writeSupportData(writeSupportData.data());
    std::cout << "Writing PPP data..." << std::endl;
    writer.writePPPData(pppBlock);
    std::cout << "Writing PVP data..." << std::endl;
    writer.writePVPData(pvpBlock);

    for (size_t ii = 0; ii < numChannels; ++ii)
    {
        std::cout << "Writing CRSD data for channel " << ii << "..." << std::endl;
        writer.writeCRSDData(writeData.data(),1,ii);
    }
}

std::vector<std::byte> checkCompressedData(const std::string& pathname,
        size_t numThreads,
        const types::RowCol<size_t>& dims)
{
    std::cout << "Reading CRSD data from file and checking against stored data..." << std::endl;
    crsd::CRSDReader reader(pathname, numThreads);
    const crsd::Wideband& wideband = reader.getWideband();

    std::vector<std::byte> readData(dims.area());

    std::span<std::byte> data(readData.data(), readData.size());
    for (size_t ii = 0; ii < reader.getMetadata().data.getNumChannels(); ++ii)
    {
        wideband.read(ii, data);
    }
    return readData;
}

bool compareVectors(const std::vector<std::byte>& readData,
                    const std::vector<std::byte>& writeData)
{
    for (size_t ii = 0; ii < readData.size(); ++ii)
    {
        if (writeData[ii] != readData[ii])
        {
            std::cerr << "Value mismatch at index " << ii << std::endl;
            return false;
        }
    }
    return true;
}

bool runTest(const std::vector<std::byte>& writeData)
{
    io::TempFile tempfile;
    six::CRSDType type = six::CRSDType::SAR;
    const size_t numThreads = std::thread::hardware_concurrency();
    const types::RowCol<size_t> dims(128, 128);
    crsd::Metadata meta = crsd::Metadata(type);
    meta.data.receiveParameters.reset(new crsd::Data::Receive());
    meta.data.receiveParameters->signalCompression.reset(new crsd::Data::SignalCompression());
    meta.data.receiveParameters->signalCompression->identifier = "Huffman";
    setUpData(meta, dims, writeData);
    meta.pvp.reset(new crsd::Pvp());
    meta.ppp.reset(new crsd::Ppp());
    meta.setVersion("1.0.0");
    crsd::setPVPXML(*(meta.pvp));
    crsd::setPPPXML(*(meta.ppp));
    crsd::PVPBlock pvpBlock(*(meta.pvp), meta.data);
    crsd::PPPBlock pppBlock(*(meta.ppp), meta.data);
    const std::vector<double> writeSupportData =
            generateSupportData<double>(NUM_SUPPORT*NUM_ROWS*NUM_COLS);

    setSupport(meta.data);
    writeCompressedCRSD("output.crsd", numThreads, dims, writeData, writeSupportData, meta, pvpBlock, pppBlock);
    const std::vector<std::byte> readData =
            checkCompressedData("output.crsd", numThreads,
            dims);
    return compareVectors(readData, writeData);
}

TEST_CASE(testCompressed)
{
    const types::RowCol<size_t> dims(128, 256);
    const std::vector<std::byte> writeData =
            generateCompressedData(dims.area());
    TEST_ASSERT_TRUE(runTest(writeData));
}

TEST_MAIN(
        TEST_CHECK(testCompressed);
)

