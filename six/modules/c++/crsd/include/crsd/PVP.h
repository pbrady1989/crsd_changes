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

#ifndef __CRSD_PVP_H__
#define __CRSD_PVP_H__

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
 *  \struct PVPType
 *
 *  \brief Specifies a defined Per Vector Parameter.
 *
 *  Size and Offset specify the size and placement of
 *  the binary parameter in the set of Per Vector
 *  parameters provided for each vector.
 */
struct PVPType
{
    static constexpr size_t WORD_BYTE_SIZE = 8;

    /*!
     *  \func PVPType
     *
     *  \brief Default constructor
     */
    PVPType();

    //! Equality operators
    bool operator==(const PVPType& other) const
    {
        return mSize == other.getSize() &&
        mOffset == other.getOffset() &&
        mFormat == other.getFormat();
    }
    bool operator!=(const PVPType& other) const
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
 *  \struct APVPType
 *
 *  \brief Additional per vector parameter
 *
 *  Specifies additional (or custom) per vector parameters
 */
struct APVPType : PVPType
{
    //! Constructor
    APVPType();

    //! Equality operators
    bool operator==(const APVPType& other) const
    {
        if ( (PVPType)(*this) != (PVPType)other )
        {
            return false;
        }
        return mName == other.mName;
    }
    bool operator!=(const APVPType& other) const
    {
        return !((*this) == other);
    }

    /*!
     *  \func setData
     *
     *  \brief set PVPType data
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
 *  \struct Pvp
 *
 *  \brief Structure used to specify the Per Vector
 *  parameters
 *
 *  Provided for each channel of a given product.
 */
struct Pvp
{

    /*!
     *  (required) Receive time for the center of the echo from the srp relative to the
     *  transmit platform Collection Start Time (sec). Time is the Time of
     *  Arrival (toa) of the received echo from the srp for the signal
     *  transmitted at txc(v). Format: Int=I8, Frac=F8
     */
    PVPType rcvStart;

    /*!
     *  (required) Receive APC position at time trc(v)srp in ECF coordinates
     *  (meters).
     *  3 8-byte floats required.
     */
    PVPType rcvPos;

    /*!
     *  (required) Receive APC velocity at time trc(v)srp in ECF coordinates
     *  (meters/sec).
     *  3 8-byte floats required
     */
    PVPType rcvVel;

    /*!
     *  (required) 1 8-byte floats required.
     */
    PVPType frcv1;

    /*!
     *  (required) 1 8-byte floats required.
     */
    PVPType frcv2;

    /*!
     *  (required) 2 8-byte floats required. Format: Int=I8, Frac=F8
     */
    PVPType refPhi0;

    /*!
     *  (required) 1 8-byte floats required. 
     */
    PVPType refFreq;

    /*!
     *  (required) 1 8-byte floats required. 
     */
    PVPType dfiC0;

    /*!
     *  (required) 1 8-byte floats required. 
     */
    PVPType ficRate;

    /*!
     *  (required) 3 8-byte floats required. 
     */
    PVPType rcvACX;

    /*!
     *  (required) 3 8-byte floats required. 
     */
    PVPType rcvACY;

    /*!
     *  (required) 2 8-byte floats required. DCX and DCY = F8
     */
    PVPType rcvEB;

    /*!
     *  (required) 1 8-byte integer required. 
     */
    PVPType signal;

    /*!
     *  (required) Amplitude scale Factor to be applied to all samples of the signal
     *  vector. For signal vector v, each sample value is multiplied by
     *  Amp_SF(v) to yield the proper sample values for the vector.
     */
    PVPType ampSF;

    /*!
     *  (required) 1 8-byte floats required. 
     */
    PVPType dgrgc;

    // Optional Tx Pulse Index
    PVPType txPulseIndex;

    /*
     *  (Optional) User defined PV parameters
     */
    std::map<std::string, APVPType> addedPVP;

    /*
     *  \func Constructor
     *  \brief Initialize default values for each parameter
     *
     */
    Pvp();

    //! Equality operators
    bool operator==(const Pvp& other) const
    {
        return  rcvStart == other.rcvStart && 
                rcvPos == other.rcvPos &&
                rcvVel == other.rcvVel && 
                frcv1 == other.frcv1 &&
                frcv2 == other.frcv2 && 
                rcvVel == other.rcvVel &&
                refPhi0 == other.refPhi0 && 
                refFreq == other.refFreq &&
                dfiC0 == other.dfiC0 && 
                ficRate == other.ficRate &&
                rcvACX == other.rcvACX && 
                rcvACY == other.rcvACY &&
                rcvEB == other.rcvEB && 
                signal == other.signal &&
                ampSF == other.ampSF && 
                dgrgc == other.dgrgc &&
                txPulseIndex == other.txPulseIndex &&
                addedPVP == other.addedPVP;
    }
    bool operator!=(const Pvp& other) const
    {
        return !((*this) == other);
    }

    //! Get size of pvp set in blocks
    size_t getReqSetSize() const;

    //! Get total byte size of pvp set
    size_t sizeInBytes() const;

    /*
     *  \func setOffset
     *
     *  \brief Validate and set the offset of parameters
     *
     *  \param offset The offset of the parameter to be expected for the param
     *  \param[out] param The PVPType parameter that should be set
     *
     *  \throws except::Exception If param offset or size overlaps another parameter, or
     *   if format is invalid
     */
    void setOffset(size_t offset, PVPType& param);

    /*
     *  \func append
     *
     *  \brief Validate and append parameter to the next available block
     *
     *  \param[out] param The PVPType parameter that should be set
     *
     *  \throws except::Exception If param offset or size overlaps another parameter, or
     *   if format is invalid
     */
    void append(PVPType& param);

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
    void setDefaultValues(size_t size, const std::string& format, PVPType& param);

    /*
     * Initializes default size and format for parameters
     */
    void initialize();
};

//! Ostream operators
std::ostream& operator<< (std::ostream& os, const PVPType& p);
std::ostream& operator<< (std::ostream& os, const APVPType& a);
std::ostream& operator<< (std::ostream& os, const Pvp& p);
}
#endif
