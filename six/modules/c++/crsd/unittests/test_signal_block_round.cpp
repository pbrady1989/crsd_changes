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

#include <nitf/coda-oss.hpp>
#include <types/RowCol.h>
#include <io/TempFile.h>
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
 * Tests write and read of Signal Block
 * Fails if values don't match
 */

template<typename T>
std::vector<std::complex<T> > generateData(size_t length)
{
    std::vector<std::complex<T> > data(length);
    srand(0);
    for (size_t ii = 0; ii < data.size(); ++ii)
    {
        auto real = static_cast<T>(rand() / 100);
        auto imag = static_cast<T>(rand() / 100);
        data[ii] = std::complex<T>(real, imag);
    }
    return data;
}

inline std::vector<double> generateScaleFactors(size_t length, bool scale)
{
    std::vector<double> scaleFactors(length, 1);
    if (scale)
    {
        for (size_t ii = 0; ii < scaleFactors.size(); ++ii)
        {
            scaleFactors[ii] *= 2;
        }
    }
    return scaleFactors;
}

template<typename T>
void writeCRSD(const std::string& outPathname, size_t /*numThreads*/,
        const types::RowCol<size_t> dims,
        const std::vector<std::complex<T> >& writeData,
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

    crsd::CRSDWriter writer(metadata, outPathname);
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
        writer.writeCRSDData(writeData.data(),dims.area());
    }
}

std::vector<std::complex<float> > checkData(const std::string& pathname,
        size_t numThreads,
        const std::vector<double>& scaleFactors,
        const types::RowCol<size_t> dims)
{
    crsd::CRSDReader reader(pathname, numThreads);
    const crsd::Wideband& wideband = reader.getWideband();
    std::vector<std::complex<float> > readData(dims.area());

    size_t sizeInBytes = readData.size() * sizeof(readData[0]);
    std::vector<std::byte> scratchData(sizeInBytes);
    std::span<std::byte> scratch(scratchData.data(), scratchData.size());
    std::span<std::complex<float>> data(readData.data(), readData.size());

    wideband.read(0, 0, crsd::Wideband::ALL, 0, crsd::Wideband::ALL,
                  scaleFactors, numThreads, scratch, data);

    return readData;
}

template<typename T>
bool compareVectors(const std::vector<std::complex<float> >& readData,
                    const std::vector<std::complex<T> >& writeData,
                    const std::vector<double>& writeSupportData,
                    const std::vector<double>& scaleFactors,
                    bool scale)
{
    size_t pointsPerScale = readData.size() / scaleFactors.size();
    for (size_t ii = 0; ii < readData.size(); ++ii)
    {
        std::complex<float> val(writeData[ii].real(), writeData[ii].imag());
        if (scale)
        {
            val *= scaleFactors[ii / pointsPerScale];
        }

        if (val != readData[ii])
        {
            std::cerr << "Value mismatch at index " << ii << std::endl;
            return false;
        }
    }
    return true;
}

template<typename T>
bool runTest(bool scale, const std::vector<std::complex<T> >& writeData)
{
    io::TempFile tempfile;
    six::CRSDType type = six::CRSDType::SAR;
    const size_t numThreads = std::thread::hardware_concurrency();
    const types::RowCol<size_t> dims(128, 128);
    const std::vector<double> scaleFactors =
            generateScaleFactors(dims.row, scale);
    crsd::Metadata meta = crsd::Metadata(type);

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
    writeCRSD("output.crsd", numThreads, dims, writeData, writeSupportData, meta, pvpBlock, pppBlock);
    const std::vector<std::complex<float> > readData =
            checkData("output.crsd", numThreads,
                      scaleFactors, dims);
    return compareVectors(readData, writeData, writeSupportData, scaleFactors, scale);
    return true;
}

TEST_CASE(testUnscaledInt8)
{
    const types::RowCol<size_t> dims(128, 128);
    const std::vector<std::complex<int8_t> > writeData =
            generateData<int8_t>(dims.area());
    const bool scale = false;
    TEST_ASSERT_TRUE(runTest(scale, writeData));
}

TEST_CASE(testScaledInt8)
{
    const types::RowCol<size_t> dims(128, 128);
    const std::vector<std::complex<int8_t> > writeData =
            generateData<int8_t>(dims.area());
    const bool scale = true;
    TEST_ASSERT_TRUE(runTest(scale, writeData));
}

TEST_CASE(testUnscaledInt16)
{
    const types::RowCol<size_t> dims(128, 128);
    const std::vector<std::complex<int16_t> > writeData =
            generateData<int16_t>(dims.area());
    const bool scale = false;
    TEST_ASSERT_TRUE(runTest(scale, writeData));
}

TEST_CASE(testScaledInt16)
{
    const types::RowCol<size_t> dims(128, 128);
    const std::vector<std::complex<int16_t> > writeData =
            generateData<int16_t>(dims.area());
    const bool scale = true;
    TEST_ASSERT_TRUE(runTest(scale, writeData));
}

TEST_CASE(testUnscaledFloat)
{
    const types::RowCol<size_t> dims(128, 128);
    const std::vector<std::complex<float> > writeData =
            generateData<float>(dims.area());
    const bool scale = false;
    TEST_ASSERT_TRUE(runTest(scale, writeData));
}

TEST_CASE(testScaledFloat)
{
    const types::RowCol<size_t> dims(128, 128);
    const std::vector<std::complex<float> > writeData =
            generateData<float>(dims.area());
    const bool scale = true;
    TEST_ASSERT_TRUE(runTest(scale, writeData));
}

TEST_MAIN(
        TEST_CHECK(testUnscaledInt8);
        TEST_CHECK(testScaledInt8);
        TEST_CHECK(testUnscaledInt16);
        TEST_CHECK(testScaledInt16);
        TEST_CHECK(testUnscaledFloat);
        TEST_CHECK(testScaledFloat);
        )
