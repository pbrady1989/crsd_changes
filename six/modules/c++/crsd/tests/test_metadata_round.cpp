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
#include <cli/Value.h>
#include <cli/ArgumentParser.h>
#include <io/TempFile.h>
#include <io/FileInputStream.h>
#include <io/FileOutputStream.h>
#include <logging/NullLogger.h>
#include <xml/lite/MinidomParser.h>

#include <crsd/CRSDXMLControl.h>
#include <crsd/Global.h>
#include <crsd/Metadata.h>
#include <crsd/SceneCoordinates.h>

#include "TestCase.h"

/*!
 * Tests write and read of metadata
 * Fails if values don't match
 */
void parseXMLFile(xml::lite::MinidomParser& xmlParser, std::string pathname)
{
    // Parse XML file
    io::FileInputStream ifs(pathname);
    xmlParser.preserveCharacterData(true);
    xmlParser.parse(ifs, ifs.available());
}

bool testEqual(const std::string& inPathname, const std::string& outPathname,
          size_t numThreads, const std::vector<std::string>& schemas)
{
    // Read in first XML file
    xml::lite::MinidomParser xmlParser;
    std::cout << "Parsing XML file: " << inPathname << std::endl;
    parseXMLFile(xmlParser, inPathname);
    std::cout << "XML file parsed successfully." << std::endl;
    crsd::CRSDXMLControl xmlControl(new logging::NullLogger(), true);
    std::cout << "Populating metadata object from XML Document." << std::endl;
    const std::unique_ptr<crsd::Metadata> metadata =
            xmlControl.fromXML(xmlParser.getDocument(), schemas);
    std::cout << "Metadata object populated successfully." << std::endl;
    const auto xmlMetadata(xmlControl.toXMLString(*metadata));
    std::cout << "Metadata converted to XML string successfully." << std::endl;

    //Output XML file to temp file
    io::FileOutputStream ofs(outPathname);
    ofs.write(xmlMetadata.c_str(), xmlMetadata.size());
    std::cout << "XML string written to file: " << outPathname << std::endl;

    // Read in second XML file from temp file
    xml::lite::MinidomParser xmlParser2;
    parseXMLFile(xmlParser2, outPathname);
    std::cout << "Second XML file parsed successfully." << std::endl;

    crsd::CRSDXMLControl xmlControl2(new logging::NullLogger(), true);

    std::cout << "Populating second metadata object from XML Document." << std::endl;

    // Populate metadata object from XML Document
    const std::unique_ptr<crsd::Metadata> metadata2 =
            xmlControl2.fromXML(xmlParser2.getDocument(), schemas);

    std::cout << "Second metadata object populated successfully." << std::endl;

    // Check if both metadata are equal
    return (*metadata == *metadata2);
}

void runTests(const std::string& inPathname, size_t numThreads, const std::vector<std::string>& schemas)
{
    bool test1 = testEqual(inPathname, "outputCRSD.crsd", numThreads, schemas);

    if (!test1)
    {
        std::cerr << "Test Failed. Metadata are not equal. \n";
        return;
    }
    std::cout << "Metadata roundtrip passed! \n";
}

int main(int argc, char** argv)
{
    try
    {
        // Parse the command line
        cli::ArgumentParser parser;
        parser.setDescription("Round trip for an XML file.");
        parser.addArgument("-t --threads",
                           "Specify the number of threads to use",
                           cli::STORE,
                           "threads",
                           "NUM")->setDefault(std::thread::hardware_concurrency());
        parser.addArgument("input", "Input pathname", cli::STORE, "input",
                           "XML", 1, 1);
        parser.addArgument("schema", "Schema pathname", cli::STORE, "schema",
                           "XSD", 1, 10);
        const std::unique_ptr<cli::Results> options(parser.parse(argc, argv));
        const std::string inPathname(options->get<std::string>("input"));
        const size_t numThreads(options->get<size_t>("threads"));
        const cli::Value* value = options->getValue("schema");
        std::vector<std::string> schemaPathnames;
        for (size_t ii = 0; ii < value->size(); ++ii)
        {
            schemaPathnames.push_back(value->get<std::string>(ii));
        }

        runTests(inPathname, numThreads, schemaPathnames);
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
