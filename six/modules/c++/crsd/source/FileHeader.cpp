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
#include <string.h>
#include <sstream>

#include <nitf/coda-oss.hpp>
#include <mem/ScopedArray.h>
#include <except/Exception.h>
#include <str/Manip.h>
#include <crsd/FileHeader.h>
#include <six/Utilities.h>

namespace crsd
{
const char FileHeader::DEFAULT_VERSION[] = "1.0.0";

FileHeader::FileHeader() :
    mVersion(DEFAULT_VERSION),
    mXmlBlockSize(0),
    mXmlBlockByteOffset(0),
    mPvpBlockSize(0),
    mPvpBlockByteOffset(0),
    mPppBlockSize(0),
    mPppBlockByteOffset(0),
    mSignalBlockSize(0),
    mSignalBlockByteOffset(0),
    mSupportBlockSize(0),
    mSupportBlockByteOffset(0)
{
}

void FileHeader::read(io::SeekableInputStream& inStream)
{
    // Read mVersion first
    mVersion = readVersion(inStream);

     // Read mType (CRSD type) first
    std::string crsdType = readType(inStream);

    mType = six::toType<CRSDType>(crsdType);

    // Block read the header for more efficient IO
    KeyValuePair headerEntry;
    std::string headerBlock;
    blockReadHeader(inStream, 1024, headerBlock);

    // Read each line with its tokens
    if (!headerBlock.empty())
    {
        std::vector<std::string> headerLines = str::split(headerBlock, "\n");
        for (size_t ii = 0; ii < headerLines.size(); ++ii)
        {
            // Determine our header entry
            tokenize(headerLines[ii], KVP_DELIMITER, headerEntry);

            if (headerEntry.first == "XML_BLOCK_SIZE")
            {
                mXmlBlockSize = str::toType<int64_t>(headerEntry.second);
            }
            else if (headerEntry.first == "XML_BLOCK_BYTE_OFFSET")
            {
                mXmlBlockByteOffset =
                        str::toType<int64_t>(headerEntry.second);
            }
            else if (headerEntry.first == "SUPPORT_BLOCK_SIZE")
            {
               mSupportBlockSize = str::toType<int64_t>(headerEntry.second);
            }
            else if (headerEntry.first == "SUPPORT_BLOCK_BYTE_OFFSET")
            {
               mSupportBlockByteOffset =
                    str::toType<int64_t>(headerEntry.second);
            }
            else if (headerEntry.first == "PVP_BLOCK_SIZE")
            {
               mPvpBlockSize = str::toType<int64_t>(headerEntry.second);
            }
            else if (headerEntry.first == "PVP_BLOCK_BYTE_OFFSET")
            {
               mPvpBlockByteOffset =
                    str::toType<int64_t>(headerEntry.second);
            }
            else if (headerEntry.first == "PPP_BLOCK_SIZE")
            {
               mPppBlockSize = str::toType<int64_t>(headerEntry.second);
            }
            else if (headerEntry.first == "PPP_BLOCK_BYTE_OFFSET")
            {
               mPppBlockByteOffset =
                    str::toType<int64_t>(headerEntry.second);
            }
            else if (headerEntry.first == "SIGNAL_BLOCK_SIZE")
            {
                mSignalBlockSize = str::toType<int64_t>(headerEntry.second);
            }
            else if (headerEntry.first == "SIGNAL_BLOCK_BYTE_OFFSET")
            {
                mSignalBlockByteOffset =
                        str::toType<int64_t>(headerEntry.second);
            }
            else if (headerEntry.first == "CLASSIFICATION")
            {
                mClassification = headerEntry.second;
            }
            else if (headerEntry.first == "RELEASE_INFO")
            {
                mReleaseInfo = headerEntry.second;
            }
            else
            {
                throw except::Exception(Ctxt(
                        "Invalid CRSD header entry '" + headerEntry.first));
            }
        }
    }

    // check for any required values that are uninitialized
    if (mXmlBlockSize == 0  ||
        mXmlBlockByteOffset == 0  ||
        mPvpBlockSize == 0  ||
        mPvpBlockByteOffset == 0  ||
        mPppBlockSize == 0  ||
        mPppBlockByteOffset == 0  ||
        mSignalBlockSize == 0  ||
        mSignalBlockByteOffset == 0 ||
        mClassification.empty() ||
        mReleaseInfo.empty())
    {
        std::ostringstream err;
        err << "CRSD header information is incomplete" << " ,  mXmlBlockSize=" << std::to_string(mXmlBlockSize) 
                  << " , mXmlBlockByteOffset=" << std::to_string(mXmlBlockByteOffset) 
                  << " ,  mPvpBlockSize=" << std::to_string(mPvpBlockSize) 
                  << " , mPvpBlockByteOffset=" << std::to_string(mPvpBlockByteOffset) 
                  << " ,  mPppBlockSize=" << std::to_string(mPppBlockSize) 
                  << " , mPppBlockByteOffset=" << std::to_string(mPppBlockByteOffset) 
                  << " ,  mSignalBlockSize=" << std::to_string(mSignalBlockSize) 
                  << " , mSignalBlockByteOffset=" << std::to_string(mSignalBlockByteOffset) 
                  << " ,  mClassification=" << mClassification
                  << " , mReleaseInfo=" << mReleaseInfo;
        throw except::Exception(Ctxt(err.str()));
    }
}

// Does not include the Section Terminator
std::string FileHeader::toString() const
{
    std::ostringstream os;

    // Send the values as they are, no calculating

    // File type
    os << mType << "/" << mVersion << LINE_TERMINATOR;

    // SUPPORT block is required fields, if present
    os << "SUPPORT_BLOCK_SIZE" << KVP_DELIMITER << mSupportBlockSize
        << LINE_TERMINATOR
        << "SUPPORT_BLOCK_BYTE_OFFSET" << KVP_DELIMITER
        << mSupportBlockByteOffset << LINE_TERMINATOR;

    // XML is Required.
    os << "XML_BLOCK_SIZE" << KVP_DELIMITER << mXmlBlockSize << LINE_TERMINATOR
       << "XML_BLOCK_BYTE_OFFSET" << KVP_DELIMITER << mXmlBlockByteOffset << LINE_TERMINATOR;
    
    if (mPppBlockSize > 0)
    {
        os << "PPP_BLOCK_SIZE" << KVP_DELIMITER << mPppBlockSize << LINE_TERMINATOR
           << "PPP_BLOCK_BYTE_OFFSET" << KVP_DELIMITER << mPppBlockByteOffset
           << LINE_TERMINATOR;
    }
    if (mPvpBlockSize)
    {
        os << "PVP_BLOCK_SIZE" << KVP_DELIMITER << mPvpBlockSize << LINE_TERMINATOR
       << "PVP_BLOCK_BYTE_OFFSET" << KVP_DELIMITER << mPvpBlockByteOffset << LINE_TERMINATOR;
    }
    if (mSignalBlockSize > 0)
    {
        os << "SIGNAL_BLOCK_SIZE" << KVP_DELIMITER << mSignalBlockSize
           << LINE_TERMINATOR
           << "SIGNAL_BLOCK_BYTE_OFFSET" << KVP_DELIMITER << mSignalBlockByteOffset
           << LINE_TERMINATOR;
    }
    
    // Required classification and release info
    os << "CLASSIFICATION" << KVP_DELIMITER << mClassification
       << LINE_TERMINATOR
       << "RELEASE_INFO" << KVP_DELIMITER << mReleaseInfo << LINE_TERMINATOR
       << SECTION_TERMINATOR << LINE_TERMINATOR;
    return os.str();
}

std::string FileHeader::getVersion() const
{
    return mVersion;
}

void FileHeader::setVersion(const std::string& version)
{
    mVersion = version;
}

size_t FileHeader::set(int64_t xmlBlockSize,
                       int64_t supportBlockSize,
                       int64_t pvpBlockSize,
                       int64_t pppBlockSize,
                       int64_t signalBlockSize)
{
    // Resolve all of the offsets based on known sizes.
    setXMLBlockSize(xmlBlockSize);
    setSupportBlockSize(supportBlockSize);
    setPvpBlockSize(pvpBlockSize);
    setPppBlockSize(pppBlockSize);
    setSignalBlockSize(signalBlockSize);
    return set();
}

size_t FileHeader::set()
{
    // Compute the three offsets that depend on a stable header size
    // loop until header size doesn't change
    size_t initialHeaderSize;
    do
    {
        initialHeaderSize = size();

        // Add the header section terminator, not part of the header size
        int64_t xmlOffset = initialHeaderSize + 2;
        setXMLBlockByteOffset(xmlOffset);


        if (mSupportBlockSize > 0)
        {
            // Add two for the XML section terminator
            int64_t supportOff = getXMLBlockByteOffset() + getXMLBlockSize() + 2;
            setSupportBlockByteOffset(supportOff);

            // Calculate pvp offset based on support position and size
            int64_t pvpOff = getSupportBlockByteOffset() +
                    getSupportBlockSize();
            // Add padding (pvp are doubles)
            const int64_t pvpRemainder = pvpOff % sizeof(double);
            if (pvpRemainder != 0)
            {
                pvpOff += sizeof(double) - pvpRemainder;
            }
            setPvpBlockByteOffset(pvpOff);
        }
        else
        {
            // Add two for the XML section terminator
            int64_t pvpOff = getXMLBlockByteOffset() +
                    getXMLBlockSize() + 2;
            // Add padding (pvp are doubles)
            const int64_t pvpRemainder = pvpOff % sizeof(double);
            if (pvpRemainder != 0)
            {
                pvpOff += sizeof(double) - pvpRemainder;
            }
            setPvpBlockByteOffset(pvpOff);
        }

        // Position to the CRSD, no padding needed, as PVP entries are all
        // doubles
        setSignalBlockByteOffset(getPvpBlockByteOffset() + getPvpBlockSize());

    } while (size() != initialHeaderSize);

    return size();
}

//! Pad bytes don't include the Section terminator
int64_t FileHeader::getPvpPadBytes() const
{
    if (mSupportBlockSize != 0)
    {
        return (getPvpBlockByteOffset() - (getSupportBlockByteOffset() + getSupportBlockSize()));
    }
    return (getPvpBlockByteOffset() - (getXMLBlockByteOffset() + getXMLBlockSize() + 2));
}


//! Pad bytes don't include the Section terminator
int64_t FileHeader::getPppPadBytes() const
{
    return (getPppBlockByteOffset() - (getPvpBlockByteOffset() + getPvpBlockSize()));
}


std::ostream& operator<< (std::ostream& os, const FileHeader& fh)
{
    os << "FileHeader::\n"
       << "  mVersion               : " << fh.mVersion << "\n"
       << "  mXmlBlockSize          : " << fh.mXmlBlockSize << "\n"
       << "  mXmlBlockByteOffset    : " << fh.mXmlBlockByteOffset << "\n"
       << "  mSupportBlockSize      : " << fh.mSupportBlockSize << "\n"
       << "  mSupportBlockSize      : " << fh.mSupportBlockByteOffset << "\n"
       << "  mPvpBlockByteOffset    : " << fh.mPvpBlockSize << "\n"
       << "  mPvpBlockByteOffset    : " << fh.mPvpBlockByteOffset << "\n"
       << "  mPppBlockByteOffset    : " << fh.mPppBlockSize << "\n"
       << "  mPppBlockByteOffset    : " << fh.mPppBlockByteOffset << "\n"
       << "  mSignalBlockSize       : " << fh.mSignalBlockSize << "\n"
       << "  mSignalBlockByteOffset : " << fh.mSignalBlockByteOffset << "\n"
       << "  mClassification: " << fh.mClassification << "\n"
       << "  mReleaseInfo   : " << fh.mReleaseInfo << "\n";
    return os;
}
}
