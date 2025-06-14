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
#include <fstream>
#include <memory>
#include <thread>
#include <std/span>

#include <crsd/CRSDReader.h>
#include <crsd/CRSDWriter.h>

#include <mem/BufferView.h>
#include <mem/ScopedArray.h>
#include <str/Convert.h>
#include <logging/NullLogger.h>
#include <cli/Value.h>
#include <cli/ArgumentParser.h>
#include <io/FileInputStream.h>
#include <io/FileOutputStream.h>
#include <crsd/Metadata.h>
#include <crsd/PVPBlock.h>

/*!
 * Reads in CRSD file from InputFile
 * Writes out CRSD file to OutputFile
 * Files should match
 */
void testRoundTrip(const std::string& inPathname, const std::string& outPathname, size_t numThreads, const std::vector<std::string>& schemaPathnames)
{
    //! Open the CRSD file
    crsd::CRSDReader reader(inPathname, numThreads, schemaPathnames);

    // Read fileheader
    const crsd::FileHeader& header = reader.getFileHeader();

    // Read metadata
    const crsd::Metadata& metadata = reader.getMetadata();

    // Read SupportBlock
    const crsd::SupportBlock& supportBlock = reader.getSupportBlock();
    std::unique_ptr<std::byte[]> readPtr;
    supportBlock.readAll(numThreads, readPtr);

    // Read PVPBlock
    const crsd::PVPBlock& pvpBlock = reader.getPVPBlock();

    // Read PPPBlock
    const crsd::PPPBlock& pppBlock = reader.getPPPBlock();

    // Read Wideband
    const crsd::Wideband& wideband = reader.getWideband();

    crsd::SignalArrayFormat signalFormat;
    if (metadata.data.receiveParameters.get())
    {
        signalFormat = metadata.data.receiveParameters->getSignalArrayFormat();
    }

    // Create the writer
    crsd::CRSDWriter writer(reader.getMetadata(), outPathname, schemaPathnames, numThreads);

    // Declare and allocate the wideband data storage
    std::unique_ptr<std::byte[]> data;
    data.reset(new std::byte[header.getSignalBlockSize()]);

    // Check if signal data is compressed
    if (metadata.data.isCompressed())
    {
        std::cout << "Writing compressed data......" << std::endl;
        // If data is compressed
        for (size_t channel = 0, idx = 0; channel < metadata.data.getNumChannels(); ++channel)
        {
            const size_t bufSize = metadata.data.getCompressedSignalSize(channel);
            wideband.read(channel, std::span<std::byte>(&data[idx], bufSize));
            idx += bufSize;
        }
        writer.write(
                pvpBlock,
                pppBlock,
                data.get(),
                readPtr.get());
    }
    else
    {
        // If data is not compressed
        std::cout << "Writing uncompressed data......" << std::endl;
        for (size_t channel = 0, idx = 0; channel < metadata.data.getNumChannels(); ++channel)
        {
            const size_t bufSize = metadata.data.getSignalSize(channel);
            wideband.read(channel, 0, crsd::Wideband::ALL,
                 0, crsd::Wideband::ALL, numThreads,
                std::span<std::byte>(&data[idx], bufSize));
            idx += bufSize;
        }

        // Write full CRSD not compressed data
        if (metadata.data.receiveParameters.get())
        {
            switch (signalFormat)
            {
            case crsd::SignalArrayFormat::CI2:
                std::cout << "Writing CI2 data......" << std::endl;
                writer.write(
                        pvpBlock,
                        pppBlock,
                        reinterpret_cast<const std::complex<int8_t>* >(data.get()),
                        readPtr.get());
                break;
            case crsd::SignalArrayFormat::CI4:
                std::cout << "Writing CI4 data......" << std::endl;
                writer.write(
                        pvpBlock,
                        pppBlock,
                        reinterpret_cast<const std::complex<int16_t>* >(data.get()),
                        readPtr.get());
                break;
            case crsd::SignalArrayFormat::CF8:
                std::cout << "Writing CF8 data......" << std::endl;
                writer.write(
                        pvpBlock,
                        pppBlock,
                        reinterpret_cast<const std::complex<float>* >(data.get()),
                        readPtr.get());
                break;
            }
        }
        else
        {
            std::cout << "Writing raw data......" << std::endl;
            writer.write(
                    pvpBlock,
                    pppBlock,
                    reinterpret_cast<const sys::ubyte*>(data.get()),
                    readPtr.get());
        }
    }

    std::cout << "Succesfully finished writing to CRSD: " << outPathname << "\n";
}

int main(int argc, char** argv)
{
    try
    {
        // Parse the command line
        cli::ArgumentParser parser;
        parser.setDescription("Round trip for a CRSD file.");
        parser.addArgument("-t --threads",
                           "Specify the number of threads to use",
                           cli::STORE,
                           "threads",
                           "NUM")->setDefault(std::thread::hardware_concurrency());
        parser.addArgument("input", "Input pathname", cli::STORE, "input",
                           "CRSD", 1, 1);
        parser.addArgument("output", "Output pathname", cli::STORE, "output",
                           "CRSD", 1, 1);
        parser.addArgument("schema", "Schema pathname", cli::STORE, "schema",
                           "XSD", 1, 10);
        const std::unique_ptr<cli::Results> options(parser.parse(argc, argv));
        const std::string inPathname(options->get<std::string>("input"));
        const std::string outPathname(options->get<std::string>("output"));
        const size_t numThreads(options->get<size_t>("threads"));
        const cli::Value* value = options->getValue("schema");

        std::vector<std::string> schemaPathnames;
        for (size_t ii = 0; ii < value->size(); ++ii)
        {
            schemaPathnames.push_back(value->get<std::string>(ii));
        }

        testRoundTrip(inPathname, outPathname, numThreads, schemaPathnames);
        return 0;
    }
    catch (const except::Exception& ex)
    {
        std::cerr << ex.toString() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown exception\n";
    }
    return 1;

}
