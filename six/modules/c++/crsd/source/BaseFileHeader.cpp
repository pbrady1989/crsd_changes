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
#include <crsd/BaseFileHeader.h>

#include <vector>

#include <mem/ScopedArray.h>
#include <io/SeekableStreams.h>
#include <str/Manip.h>
#include <nitf/coda-oss.hpp>
#include <six/Enums.h>
#include <six/Utilities.h>

namespace crsd
{
const char BaseFileHeader::FILE_TYPE_SAR[] = "CRSDsar";
const char BaseFileHeader::FILE_TYPE_TX[] = "CRSDtx";
const char BaseFileHeader::FILE_TYPE_RX[] = "CRSDrcv";
const char BaseFileHeader::KVP_DELIMITER[] = " := ";
const char BaseFileHeader::LINE_TERMINATOR = '\n';
const char BaseFileHeader::SECTION_TERMINATOR = '\f';
const size_t BaseFileHeader::MAX_HEADER_SIZE = 10485760;

bool BaseFileHeader::isCRSD(io::SeekableInputStream& inStream)
{
    inStream.seek(0, io::Seekable::START);

    char buf[7] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0'};
    inStream.read(buf, 7);
    return (  (::strncmp(buf, FILE_TYPE_SAR, 7) == 0)
           || (::strncmp(buf, FILE_TYPE_TX, 6) == 0)
           || (::strncmp(buf, FILE_TYPE_RX, 6) == 0));
}

std::string BaseFileHeader::readVersion(io::SeekableInputStream& inStream)
{
    char buf[128];
    inStream.seek(0, io::Seekable::START);
    inStream.readln(buf, sizeof(buf));
    const KeyValuePair kvPair(tokenize(buf, "/"));
    // std::cout << "buf: " << buf << std::endl;
    // std::cout << "kvPair.first: " << kvPair.first << std::endl;
    // std::cout << "kvPair.second: " << kvPair.second << std::endl;
    if ( (kvPair.first  != FILE_TYPE_SAR) 
       && (kvPair.first != FILE_TYPE_TX) 
       && (kvPair.first != FILE_TYPE_RX) )
    {
        throw except::Exception(Ctxt("Not a CRSD file"));
    }

    // Remove any trailing whitespace from the version
    std::string ret = kvPair.second;
    str::trim(ret);

    return (ret);
}

CRSDType BaseFileHeader::readType(io::SeekableInputStream& inStream)
{
    char buf[128];
    inStream.seek(0, io::Seekable::START);
    inStream.readln(buf, sizeof(buf));

    const KeyValuePair kvPair(tokenize(buf, "/"));
    if ( (kvPair.first  != FILE_TYPE_SAR) 
       && (kvPair.first != FILE_TYPE_TX) 
       && (kvPair.first != FILE_TYPE_RX) )
    {
        throw except::Exception(Ctxt("Not a CRSD file"));
    }

    // Remove any trailing whitespace from the type
    std::string ret = kvPair.first;
    str::trim(ret);

    return (six::toType<CRSDType>(ret));
}

void BaseFileHeader::tokenize(const std::string& in,
                              const std::string& delimiter,
                              KeyValuePair& kvPair)
{
    const std::string::size_type pos = in.find(delimiter);
    if (pos == std::string::npos)
    {
        throw except::Exception(Ctxt(
                "Delimiter '" + delimiter + "' not found in '" + in + "'"));
    }

    kvPair.first = in.substr(0, pos);
    kvPair.second = in.substr(pos + delimiter.length());
}

void BaseFileHeader::blockReadHeader(io::SeekableInputStream& inStream,
                                     size_t blockSize,
                                     std::string& headerBlock)
{
    static const char ERROR_MSG[] =
            "CRSD file malformed: Header must terminate with '\\f\\n'";

    std::vector<std::byte> buf(blockSize + 1);
    std::fill(buf.begin(), buf.end(), static_cast<std::byte>(0));
    headerBlock.clear();

    // read each block in succession
    size_t terminatorLoc = std::string::npos;
    size_t totalBytesRead = 0;
    while (inStream.read(buf.data(), blockSize) != io::InputStream::IS_EOF &&
           terminatorLoc == std::string::npos)
    {
        std::string thisBlock = reinterpret_cast<const char*>(buf.data());

        // find the terminator in the block
        terminatorLoc = thisBlock.find('\f');
        if (terminatorLoc != std::string::npos)
        {
            // read one more byte if our block missed the trailing '\n'
            if (terminatorLoc == thisBlock.length() - 1)
            {
                char c;
                inStream.read(&c, 1);

                thisBlock.insert(thisBlock.length(), &c, 1);
            }

            // verify proper formatting
            if (thisBlock[terminatorLoc + 1] != '\n')
            {
                throw except::Exception(Ctxt(ERROR_MSG));
            }

            // trim off anything after the terminator
            thisBlock = thisBlock.substr(0, terminatorLoc);
        }

        // build the header
        headerBlock += thisBlock;

        // verify we aren't past 10MB in the header --
        // this stops processing before we start reading into the
        // image data and potentially run out of memory
        totalBytesRead += thisBlock.length();
        if (totalBytesRead > MAX_HEADER_SIZE)
        {
            throw except::Exception(Ctxt(ERROR_MSG));
        }
    }
}
}

