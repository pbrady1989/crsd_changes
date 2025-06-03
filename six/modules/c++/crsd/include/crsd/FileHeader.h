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
#ifndef __CRSD_FILE_HEADER_H__
#define __CRSD_FILE_HEADER_H__

#include <string>
#include <ostream>
#include <utility>

#include <io/SeekableStreams.h>
#include <crsd/BaseFileHeader.h>
#include <six/Types.h>

namespace crsd
{
/*
 *  \class FileHeader
 *
 *  \brief Stores CRSD file header information
 */
class FileHeader : public BaseFileHeader
{
public:
    static const char DEFAULT_VERSION[];

    /*
     *  \func FileHeader
     *
     *  \brief Constructor
     *   initialize default variables
     */
    FileHeader();

    virtual ~FileHeader()
    {
    }

    /*
     *  \func read
     *
     *  \brief Initialize member variables from input stream
     *
     *  \param inStream Valid input stream of CRSD file
     */
    void read(io::SeekableInputStream& inStream) override;

    /*
     *  \func toString
     *
     *  \brief Convert header info to string
     *
     *  For writing to file
     *  Does not include section terminator string
     *
     *  \return Returns header as string
     */
    std::string toString() const override;

    /*
     *  \func getVersion
     *
     *  \brief Get the file version
     *
     *  Get the version of the CRSD file
     *  ex: 1.0.0, 1.0.1 etc
     *
     * returns file version string
     */
    std::string getVersion() const;

    /*
     *  \func setVersion
     *
     *  \brief Set the file version
     *
     *  Set the version of the CRSD file
     *  ex: 1.0.0, 1.0.1 etc
     *
     */
    void setVersion(const std::string& version);

    /*
     *  \func set
     *
     *  \brief Set the file header
     *
     *  Set the file header to its final state.
     *  Compute the offsets of the sections that
     *  depend on the file header size
     *
     *  \return Return size of header
     */
    size_t set();

    /*
     *  \func set
     *
     *  \brief Set the file header to its final state
     *
     *  Set the file header to its final state.
     *  Compute the offsets of the sections that depend
     *  on the file header size.
     *  Adds padding to modulo 8 for pvp block
     *
     *  \param xmlBlockSize Size of XML block
     *  \param supportBlockSize Size of support block
     *  \param pvpBlockSize Size of pvp block
     *  \param pppBlockSize Size of ppp block
     *  \param signalBlockSize Size of signal block
     *
     *  \return Return size of header
     */
    size_t set(int64_t xmlBlockSize, int64_t supportBlockSize,
            int64_t pvpBlockSize, int64_t pppBlockSize, int64_t signalBlockSize);

    //! Set required elements
    void setType(CRSDType type)
    {
        mType = type;
    }
    void setXMLBlockSize(int64_t size)
    {
        mXmlBlockSize = size;
    }
    void setPvpBlockSize(int64_t size)
    {
        mPvpBlockSize = size;
    }
    void setPppBlockSize(int64_t size)
    {
        mPppBlockSize = size;
    }
    void setSignalBlockSize(int64_t size)
    {
        mSignalBlockSize = size;
    }
    void setXMLBlockByteOffset(int64_t offset)
    {
        mXmlBlockByteOffset = offset;
    }
    void setPvpBlockByteOffset(int64_t offset)
    {
        mPvpBlockByteOffset = offset;
    }
    void setPppBlockByteOffset(int64_t offset)
    {
        mPppBlockByteOffset = offset;
    }
    void setSignalBlockByteOffset(int64_t offset)
    {
        mSignalBlockByteOffset = offset;
    }
    void setClassification(const std::string& str)
    {
        mClassification = str;
    }
    void setReleaseInfo(const std::string& str)
    {
        mReleaseInfo = str;
    }

    //! Get required elements
    CRSDType getType() const
    {
        return mType;
    }
    int64_t getXMLBlockSize() const
    {
        return mXmlBlockSize;
    }
    int64_t getPvpBlockSize() const
    {
        return mPvpBlockSize;
    }
    int64_t getPppBlockSize() const
    {
        return mPppBlockSize;
    }
    int64_t getSignalBlockSize() const
    {
        return mSignalBlockSize;
    }
    int64_t getXMLBlockByteOffset() const
    {
        return mXmlBlockByteOffset;
    }
    int64_t getPvpBlockByteOffset() const
    {
        return mPvpBlockByteOffset;
    }
    int64_t getPppBlockByteOffset() const
    {
        return mPppBlockByteOffset;
    }
    int64_t getSignalBlockByteOffset() const
    {
        return mSignalBlockByteOffset;
    }
    std::string getClassification() const
    {
        return mClassification;
    }
    std::string getReleaseInfo() const
    {
        return mReleaseInfo;
    }

    /*
     *  \func getPvpPadBytes
     *  \brief Calculate padding in CRSD file for PVP block
     *
     *  Pad bytes don't include section terminator
     *
     *  \return number of pad bytes
     */
    int64_t getPvpPadBytes() const;
    int64_t getPppPadBytes() const;

    //! Optional elements
    void setSupportBlockSize(int64_t size)
    {
        mSupportBlockSize = size;
    }
    void setSupportBlockByteOffset(int64_t offset)
    {
        mSupportBlockByteOffset = offset;
    }

    int64_t getSupportBlockSize() const
    {
        return mSupportBlockSize;
    }
    int64_t getSupportBlockByteOffset() const
    {
        return mSupportBlockByteOffset;
    }

private:
    friend std::ostream& operator<< (std::ostream& os, const FileHeader& fh);

private:
    // File type header
    std::string mVersion;
    CRSDType mType;

    // Required key-value pairs
    int64_t mXmlBlockSize;
    int64_t mXmlBlockByteOffset;
    int64_t mSupportBlockSize;
    int64_t mSupportBlockByteOffset;


    // Optional key-value pairs
    int64_t mPvpBlockSize;
    int64_t mPvpBlockByteOffset;
    int64_t mPppBlockSize;
    int64_t mPppBlockByteOffset;
    int64_t mSignalBlockSize;
    int64_t mSignalBlockByteOffset;
    std::string mClassification;
    std::string mReleaseInfo;

};
}

#endif
