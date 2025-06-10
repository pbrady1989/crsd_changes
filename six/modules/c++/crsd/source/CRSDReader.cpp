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
#include <crsd/CRSDReader.h>

#include <std/memory>
#include <algorithm>

#include <except/Exception.h>
#include <io/StringStream.h>
#include <io/FileInputStream.h>
#include <logging/NullLogger.h>
#include <mem/ScopedArray.h>
#include <xml/lite/MinidomParser.h>
#include <gsl/gsl.h>

#include <six/XmlLite.h>
#include <crsd/CRSDXMLControl.h>

namespace crsd
{
CRSDReader::CRSDReader(std::shared_ptr<io::SeekableInputStream> inStream,
                       size_t numThreads,
                       const std::vector<std::string>& schemaPaths,
                       std::shared_ptr<logging::Logger> logger)
{
    initialize(inStream, numThreads, logger, schemaPaths);
}

CRSDReader::CRSDReader(const std::string& fromFile,
                       size_t numThreads,
                       const std::vector<std::string>& schemaPaths,
                       std::shared_ptr<logging::Logger> logger)
{
    initialize(std::make_shared<io::FileInputStream>(fromFile),
        numThreads, logger, schemaPaths);
}

void CRSDReader::initialize(std::shared_ptr<io::SeekableInputStream> inStream,
                            size_t numThreads,
                            std::shared_ptr<logging::Logger> logger,
                            const std::vector<std::string>& schemaPaths_)
{
    const bool DEBUG = true;
    if (DEBUG)
        std::cout << "Reading in file header..." << std::endl;
    
    mFileHeader.read(*inStream);


    // Read in the XML string
    if (DEBUG)
        std::cout << "Reading in XML block..." << std::endl;
        
    inStream->seek(mFileHeader.getXMLBlockByteOffset(), io::Seekable::START);

    six::MinidomParser xmlParser;
    xmlParser.preserveCharacterData(true);
    xmlParser.parse(*inStream, gsl::narrow<int>(mFileHeader.getXMLBlockSize()));

    if (logger.get() == nullptr)
    {
        logger = std::make_shared<logging::NullLogger>();
    }

    std::vector<std::filesystem::path> schemaPaths;
    std::transform(schemaPaths_.begin(), schemaPaths_.end(), std::back_inserter(schemaPaths),
        [](const std::string& s) { return s; });
    mMetadata = CRSDXMLControl(logger.get()).fromXML(xmlParser.getDocument(), schemaPaths);

    if (DEBUG)
        std::cout << "Reading in support block..." << std::endl;

    mSupportBlock = std::make_unique<SupportBlock>(inStream, mMetadata.data, mFileHeader);

    // Load the PPPBlock into memory
    if (DEBUG)
        std::cout << "reading in PPP block..." << std::endl;

    mPPPBlock = PPPBlock(mMetadata);
    mPPPBlock.load(*inStream, mFileHeader, numThreads);

    // Load the PVPBlock into memory
    if (DEBUG)
        std::cout << "reading in PVP block..." << std::endl;

    mPVPBlock = PVPBlock(mMetadata);
    mPVPBlock.load(*inStream, mFileHeader, numThreads);

    // Setup for wideband reading
    if (DEBUG)
        std::cout << "reading in wideband block..." << std::endl;

    mWideband = std::make_unique<Wideband>(inStream, mMetadata,
        mFileHeader.getSignalBlockByteOffset(), mFileHeader.getSignalBlockSize());
}
}
