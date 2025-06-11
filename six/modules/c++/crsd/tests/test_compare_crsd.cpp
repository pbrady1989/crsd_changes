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

#include <scene/sys_Conf.h>
#include <logging/NullLogger.h>
#include <cli/Value.h>
#include <cli/ArgumentParser.h>
#include <io/FileInputStream.h>
#include <io/FileOutputStream.h>
#include <crsd/Metadata.h>
#include <crsd/PVPBlock.h>
#include <crsd/CRSDReader.h>
#include <str/Convert.h>

/*!
 * Compares two CRSD files
 * Returns true if they do, false if not
 */

template <typename T>
bool compareCRSDData(const std::byte* data1,
                     const std::byte* data2,
                     size_t size,
                     size_t channel)
{
    const T* castData1 = reinterpret_cast<const T*>(data1);
    const T* castData2 = reinterpret_cast<const T*>(data2);

    for (size_t ii = 0; ii < size; ++ii)
    {
        if (castData1[ii] != castData2[ii])
        {
            std::cerr << "Wideband data at channel " << channel
                      << " has differing data starting at"
                      << " index " << ii << "\n";
            return false;
        }
    }
    return true;
}

bool compareSupportData(const std::unique_ptr<std::byte[]>& data1,
                     const std::unique_ptr<std::byte[]>& data2,
                     size_t size)
{
    for (size_t ii = 0; ii < size; ++ii)
    {
        if (data1[ii] != data2[ii])
        {
            std::cerr <<  "Support data has differing data starting at"
                      << " index " << ii << "\n";
            return false;
        }
    }
    return true;
}

bool compareWideband(crsd::CRSDReader& reader1,
                     crsd::CRSDReader& reader2,
                     size_t channelsToProcess,
                     size_t numThreads)
{
    bool dataMatches = true;

    const crsd::Wideband& wideband1 = reader1.getWideband();
    const crsd::Wideband& wideband2 = reader2.getWideband();

    std::unique_ptr<std::byte[]> crsdData1;
    std::unique_ptr<std::byte[]> crsdData2;

    for (size_t ii = 0; ii < channelsToProcess; ++ii)
    {
        const types::RowCol<size_t> dims1(
                reader1.getMetadata().data.getNumVectors(ii),
                reader1.getMetadata().data.getNumSamples(ii));
        const types::RowCol<size_t> dims2(
                reader2.getMetadata().data.getNumVectors(ii),
                reader2.getMetadata().data.getNumSamples(ii));

        if (dims1 == dims2)
        {
            crsdData1 = wideband1.read(ii,
                           0, crsd::Wideband::ALL, 0,
                           crsd::Wideband::ALL,
                           numThreads);

            crsdData2 = wideband2.read(ii,
                           0, crsd::Wideband::ALL, 0,
                           crsd::Wideband::ALL,
                           numThreads);

            switch (reader1.getMetadata().data.receiveParameters->signalArrayFormat)
            {
            case crsd::SampleType::RE08I_IM08I:
                if (!compareCRSDData<std::complex<int8_t> >(
                        crsdData1.get(),
                        crsdData2.get(),
                        dims1.area(),
                        ii))
                {
                    dataMatches = false;
                }
                break;
            case crsd::SampleType::RE16I_IM16I:
                if (!compareCRSDData<std::complex<int16_t> >(
                        crsdData1.get(),
                        crsdData2.get(),
                        dims1.area(),
                        ii))
                {
                    dataMatches = false;
                }
                break;
            case crsd::SampleType::RE32F_IM32F:
                if (!compareCRSDData<std::complex<float> >(
                        crsdData1.get(),
                        crsdData2.get(),
                        dims1.area(),
                        ii))
                {
                    dataMatches = false;
                }
                break;
            }
        }
        else
        {
            std::cerr << "Data at channel " << ii
                      << " has differing dimensions\n";
            dataMatches = false;
        }
    }

    return dataMatches;
}

bool checkCRSD(const std::string& pathname1, const std::string& pathname2, size_t numThreads, const std::vector<std::string>& schemaPathname)
{
    crsd::CRSDReader reader1(pathname1, numThreads, schemaPathname);
    crsd::CRSDReader reader2(pathname2, numThreads, schemaPathname);

    // Check metadata
    if (reader1.getMetadata() != reader2.getMetadata())
    {
        std::cerr << "Metadata does not match \n";
        return false;
    }
    else
    {
        std::cout << "XML Metadata matches \n";
    }

    // Check pvp block
    // if (reader1.getPVPBlock() != reader2.getPVPBlock())
    // {
    //     std::cerr << "PVPBlock does not match \n";
    //     return false;
    // }
    // else
    // {
    //     std::cout << "PVP Block matches \n";
    // }

    // Check ppp block
    // if (reader1.getPPPBlock() != reader2.getPPPBlock())
    // {
    //     std::cerr << "PPPBlock does not match \n";
    //     return false;
    // }
    // else
    // {
    //     std::cout << "PPP Block matches \n";
    // }

    // Check support block
    std::unique_ptr<std::byte[]> readPtr1;
    reader1.getSupportBlock().readAll(numThreads, readPtr1);
    std::unique_ptr<std::byte[]> readPtr2;
    reader2.getSupportBlock().readAll(numThreads, readPtr2);
    if (!compareSupportData(readPtr1, readPtr2, reader1.getMetadata().data.getAllSupportSize()))
    {
        std::cerr << "SupportBlock does not match \n";
        return false;
    }
    else
    {
        std::cout << "Support Data matches \n";
    }

    // // Check wideband
    const size_t channelsToProcess = std::min(
            reader1.getMetadata().data.getNumChannels(),
            reader2.getMetadata().data.getNumChannels());
    if (reader1.getMetadata().data.getNumChannels() !=
        reader2.getMetadata().data.getNumChannels())
    {
        std::cerr << "Files contain a differing number of channels "
                  << "comparison will continue but will only look at "
                  << "the first " << channelsToProcess << " channels\n";
        return false;
    }
    //! Only process wideband data if the data types are the same
    if (reader1.getMetadata().data.receiveParameters->signalArrayFormat ==
        reader2.getMetadata().data.receiveParameters->signalArrayFormat)
    {
        const bool crsdDataMatches = compareWideband(reader1,
                                                     reader2,
                                                     channelsToProcess,
                                                     numThreads);
        if (!crsdDataMatches)
        {
            std::cerr << "Wideband data does not match \n";
            return false;
        }
        else
        {
            std::cout << "Wideband data matches \n";
        }
    }
    else
    {
        std::cerr << "Data has differing sample type\n";
        return false;
    }
    return true;
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
        parser.addArgument("file1", "First pathname", cli::STORE, "file1",
                           "CRSD", 1, 1);
        parser.addArgument("file2", "Second pathname", cli::STORE, "file2",
                           "CRSD", 1, 1);
        parser.addArgument("schema", "Schema pathname", cli::STORE, "schema",
                           "XSD", 1, 10);
        const std::unique_ptr<cli::Results> options(parser.parse(argc, argv));
        const std::string pathname1(options->get<std::string>("file1"));
        const std::string pathname2(options->get<std::string>("file2"));
        const size_t numThreads(options->get<size_t>("threads"));
        const cli::Value* value = options->getValue("schema");

        std::vector<std::string> schemaPathnames;
        for (size_t ii = 0; ii < value->size(); ++ii)
        {
            schemaPathnames.push_back(value->get<std::string>(ii));
        }

        const bool isMatch = checkCRSD(pathname1, pathname2, numThreads, schemaPathnames);
        if (!isMatch)
        {
            std::cerr << "CRSD Files do not match \n";
            return 1;
        }
        std::cout << "CRSD Files match \n";
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
