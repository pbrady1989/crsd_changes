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

#ifndef __CRSD_PPP_H__
#define __CRSD_PPP_H__

#include <ostream>
#include <vector>
#include <map>
#include <stddef.h>
#include <six/Types.h>
#include <six/Init.h>
#include <six/Parameter.h>
#include <six/ParameterCollection.h>

namespace crsd
{

/*!
 *  \struct PPPType
 *
 *  \brief Specifies a defined Per Vector Parameter.
 *
 *  Size and Offset specify the size and placement of
 *  the binary parameter in the set of Per Vector
 *  parameters provided for each vector.
 */
struct PPPType
{
    static constexpr size_t WORD_BYTE_SIZE = 8;

    /*!
     *  \func PPPType
     *
     *  \brief Default constructor
     */
    PPPType();

    //! Equality operators
    bool operator==(const PPPType& other) const
    {
        return mSize == other.getSize() &&
        mOffset == other.getOffset() &&
        mFormat == other.getFormat();
    }
    bool operator!=(const PPPType& other) const
    {
        return !((*this) == other);
    }

    //! Setter Functions
    void setOffset(size_t offset)
    {
        mOffset = offset;
    }
    void setSize(size_t size)
    {
        mSize = size;
    }
    void setFormat(const std::string& format)
    {
        mFormat = format;
    }

    //! Get size
    size_t getSize() const
    {
        return mSize;
    }
    //! Get size in bytes
    size_t getByteSize() const
    {
        return mSize * WORD_BYTE_SIZE;
    }
    //! Get offset
    size_t getOffset() const
    {
        return mOffset;
    }

    //! Get offset in bytes
    size_t getByteOffset() const
    {
        return mOffset * WORD_BYTE_SIZE;
    }

    //! Get format
    std::string getFormat() const
    {
        return mFormat;
    }

protected:
    //! Size of parameter
    size_t mSize;
    //! Offset of parameter
    size_t mOffset;
    //! Format of parameter
    std::string mFormat;
};

/*!
 *  \struct APPPType
 *
 *  \brief Additional per vector parameter
 *
 *  Specifies additional (or custom) per vector parameters
 */
struct APPPType : PPPType
{
    //! Constructor
    APPPType();

    //! Equality operators
    bool operator==(const APPPType& other) const
    {
        if ( (PPPType)(*this) != (PPPType)other )
        {
            return false;
        }
        return mName == other.mName;
    }
    bool operator!=(const APPPType& other) const
    {
        return !((*this) == other);
    }

    /*!
     *  \func setData
     *
     *  \brief set PPPType data
     *
     *  \param size Size of the expected parameter
     *  \param offset Offset of the expected parameter
     *  \param format Format of the expected parameter
     *  \param name Unique name of the expected parameter
     *   See spec table 10.2 page 120 for allowed binary formats
     */
    void setData(size_t size, size_t offset, const std::string& format, const std::string& name)
    {
        mSize = size;
        mOffset = offset;
        mFormat = format;
        mName = name;
    }

    /*
     *  \func getName
     *
     *  \brief Get the name of the additional parameter
     *
     *  \return Returns name of the parameter
     */
    std::string getName() const
    {
        return mName;
    }

private:
    //! String contains the unique name of the parameter
    std::string mName;
};

/*!
 *  \struct PPP
 *
 *  \brief Structure used to specify the Per Vector
 *  parameters
 *
 *  Provided for each channel of a given product.
 */
struct Ppp
{

    /*!
     *  (required) Receive time for the center of the echo from the srp relative to the
     *  transmit platform Collection Start Time (sec). Time is the Time of
     *  Arrival (toa) of the received echo from the srp for the signal
     *  transmitted at txc(v). Format: Int=I8, Frac=F8
     */
    PPPType txTime;

    /*!
     *  (required) Receive APC position at time trc(v)srp in ECF coordinates
     *  (meters).
     *  3 8-byte floats required.
     */
    PPPType txPos;

    /*!
     *  (required) Receive APC velocity at time trc(v)srp in ECF coordinates
     *  (meters/sec).
     *  3 8-byte floats required
     */
    PPPType txVel;

    /*!
     *  (required) 1 8-byte floats required.
     */
    PPPType fx1;

    /*!
     *  (required) 1 8-byte floats required.
     */
    PPPType fx2;

     /*!
     *  (required) 1 8-byte floats required.
     */
    PPPType txmt;

    /*!
     *  (required) 2 8-byte floats required. Format: Int=I8, Frac=F8
     */
    PPPType phiX0;

    /*!
     *  (required) 1 8-byte floats required. 
     */
    PPPType fxFreq0;

    /*!
     *  (required) 1 8-byte floats required. 
     */
    PPPType fxRate;

    /*!
     *  (required) 1 8-byte floats required. 
     */
    PPPType txRadInt;

    /*!
     *  (required) 3 8-byte floats required. 
     */
    PPPType txACX;

    /*!
     *  (required) 3 8-byte floats required. 
     */
    PPPType txACY;

    /*!
     *  (required) 2 8-byte floats required. DCX and DCY = F8
     */
    PPPType txEB;

    /*!
     *  (required) 1 8-byte integer required. 
     */
    PPPType fxResponseIndex;

    /*!
     *  (required) Amplitude scale Factor to be applied to all samples of the signal
     *  vector. For signal vector v, each sample value is multiplied by
     *  Amp_SF(v) to yield the proper sample values for the vector.
     */
    PPPType xmIndex;

    /*
     *  (Optional) User defined PV parameters
     */
    std::map<std::string, APPPType> addedPPP;

    /*
     *  \func Constructor
     *  \brief Initialize default values for each parameter
     *
     */
    Ppp();

    //! Equality operators
    bool operator==(const Ppp& other) const
    {
        return  txTime == other.txTime && 
                txPos == other.txPos &&
                txVel == other.txVel && 
                fx1 == other.fx1 &&
                fx2 == other.fx2 && 
                txmt == other.txmt && 
                phiX0 == other.phiX0 &&
                fxFreq0 == other.fxFreq0 && 
                fxRate == other.fxRate &&
                txRadInt == other.txRadInt && 
                txACX == other.txACX &&
                txACY == other.txACY && 
                txEB == other.txEB &&
                fxResponseIndex == other.fxResponseIndex &&
                xmIndex == other.xmIndex && 
                addedPPP == other.addedPPP;
    }
    bool operator!=(const Ppp& other) const
    {
        return !((*this) == other);
    }

    //! Get size of PPP set in blocks
    size_t getReqSetSize() const;

    //! Get total byte size of PPP set
    size_t sizeInBytes() const;

    /*
     *  \func setOffset
     *
     *  \brief Validate and set the offset of parameters
     *
     *  \param offset The offset of the parameter to be expected for the param
     *  \param[out] param The PPPType parameter that should be set
     *
     *  \throws except::Exception If param offset or size overlaps another parameter, or
     *   if format is invalid
     */
    void setOffset(size_t offset, PPPType& param);

    /*
     *  \func append
     *
     *  \brief Validate and append parameter to the next available block
     *
     *  \param[out] param The PPPType parameter that should be set
     *
     *  \throws except::Exception If param offset or size overlaps another parameter, or
     *   if format is invalid
     */
    void append(PPPType& param);

    /*
     *  \func setParameter
     *
     *  \brief Validate and set the metadata of an additional parameter identified by name
     *
     *  \param size The size of the parameter to be expected for the param
     *  \param offset The offset of the parameter to be expected for the param
     *  \param format The string format of the parameter to be expected for the param
     *  \param name The unique identifier of the additional parameter that should be set
     *
     *  \throws except::Exception If param offset or size overlaps another parameter,
     *   if format is invalid or if name is not unique
     */
    void setCustomParameter(size_t size, size_t offset, const std::string& format, const std::string& name);

    /*
     *  \func setCustomParameter
     *
     *  \brief Validate and append additional parameter to the next available block
     *
     *  \param size The size of the parameter to be expected for the param
     *  \param format The string format of the parameter to be expected for the param
     *  \param name The unique identifier of the additional parameter that should be set
     *
     *  \throws except::Exception If param offset or size overlaps another parameter,
     *   if format is invalid or if name is not unique
     */
    void appendCustomParameter(size_t size, const std::string& format, const std::string& name);

private:
    /*
     *  Validate parameter size and offset when setting parameter
     */
    void validate(size_t size, size_t offset);

    /*
     *  Marks filled bytes
     */
    std::vector<bool> mParamLocations;

    /*
     * Set default size and format for each parameter
     */
    void setDefaultValues(size_t size, const std::string& format, PPPType& param);

    /*
     * Initializes default size and format for parameters
     */
    void initialize();
};

//! Ostream operators
std::ostream& operator<< (std::ostream& os, const PPPType& p);
std::ostream& operator<< (std::ostream& os, const APPPType& a);
std::ostream& operator<< (std::ostream& os, const Ppp& p);
}
#endif
