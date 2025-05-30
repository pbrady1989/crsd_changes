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

#ifndef __CRSD_DATA_H__
#define __CRSD_DATA_H__

#include <ostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <assert.h>
#include <stddef.h>

#include <scene/sys_Conf.h>
#include <six/Init.h>
#include <crsd/Enums.h>
#include <crsd/Utilities.h>

namespace crsd
{

/*
 *  \struct Data
 *
 *  \brief Parameter that describes binary data components
 *
 *  See section 2.3 through 2.6
 */
struct Data
{
    /*
     *  \struct SupportArray
     *
     *  \brief Support Array size parameters.
     *
     *  Branch repeated for each binary support array
     *  Support array referenced by its unique
     *  support array identifier SA_ID
     */
    // There's another SupportArray type in the Metadata,
    // so hiding this inside Data
    struct SupportArray
    {

        /*
         *  \func SupportArray
         *
         *  \brief Default constructor
         */
        SupportArray();

        /*
         *  \func SupportArray
         *
         *  \brief Custom constructor initializes all parameters
         */
        SupportArray(const std::string& id, size_t rows, size_t cols,
                size_t numBytes, size_t offset);

        //! Equality Operators
        bool operator==(const SupportArray& other) const
        {
            return identifier == other.identifier &&
                   numRows == other.numRows &&
                   numCols == other.numCols &&
                   bytesPerElement == other.bytesPerElement &&
                   arrayByteOffset == other.arrayByteOffset;
        }
        bool operator!=(const SupportArray& other) const
        {
            return !((*this) == other);
        }

        //! Get byte size of support array
        size_t getSize() const
        {
            return numRows * numCols * bytesPerElement;
        }

        //! Unique string that identifies this support array
        //! (SA_ID).
        std::string identifier;

        //! Number of rows in the array, NumRowsSA
        size_t numRows;

        //! Number of columns per row in the array,
        //! NumColsSA
        size_t numCols;

        //! Indicates the size in bytes of each data element in
        //! the support array.
        size_t bytesPerElement;

        //! Array offset from the start of the Support block
        //! (in bytes) to the start of the support array.
        size_t arrayByteOffset;
    };

      /*
     *  \struct TxSequence
     *
     *  \brief Param's that specify the PP array size and 
     *   location. One for each transmit sequence
     *
     *  TxSequence parameters that specify PPP arrays
     */
    struct TxSequence
    {
        /*
         *  \func Channel
         *
         *  \brief Default constructor
         */
        TxSequence();

        /*
         *  \func Channel
         *
         *  \brief Custom constructor
         *
         *  \param vectors Set number of vectors
         *  \param samples Set number of samples
         */
        TxSequence(size_t numPulses, size_t pppByteOffset);

        /*
         *  \func Channel
         *
         *  \brief Custom constructor
         *
         *  \param vectors Set number of vectors
         *  \param samples Set number of samples
         *  \param signalByteOffset set the byte offset of signal block
         *  \param pppByteOffset set the byte offset of ppp block
         *  \param compressedSize (Optional) Size of the compressed signal block
         */
        TxSequence(std::string identifier, size_t numPulses,
                   size_t pppByteOffset);

        //! Equality operators
        bool operator==(const TxSequence& other) const
        {
            return identifier == other.identifier &&
                   numPulses == other.numPulses &&
                   pppArrayByteOffset == other.pppArrayByteOffset;
        }
        bool operator!=(const TxSequence& other) const
        {
            return !((*this) == other);
        }

        //! Getter functions
        std::string getIdentifier() const
        {
            return identifier;
        }
        size_t getNumPulses() const
        {
            return numPulses;
        }
        size_t getPPPArrayByteOffset() const
        {
            return pppArrayByteOffset;
        }

        //! String that uniquely identifies the CRSD channel
        std::string identifier;

        //! Number of vectors in the signal array
        size_t numPulses;

        //! Number of samples per vector in the signal array
        size_t pppArrayByteOffset;

    };

    /*
     *  \struct Transmit
     *
     *  \brief Transmit parameters that specify PPP arrays
     *
     *  Transmit parameters that specify PPP arrays
     */
    struct Transmit
    {
        /*
         *  \func Channel
         *
         *  \brief Default constructor
         */
        Transmit();

        /*
         *  \func Channel
         *
         *  \brief Custom constructor
         *
         *  \param vectors Set number of vectors
         *  \param samples Set number of samples
         *  \param signalByteOffset set the byte offset of signal block
         *  \param pppByteOffset set the byte offset of ppp block
         *  \param compressedSize (Optional) Size of the compressed signal block
         */
        Transmit(std::string identifier, size_t numBytesPP);

        //! Equality operators
        bool operator==(const Transmit& other) const
        {
            return identifier == other.identifier &&
                   numBytesPPP == other.numBytesPPP &&
                   txSequence == other.txSequence;
        }
        bool operator!=(const Transmit& other) const
        {
            return !((*this) == other);
        }

        //! Getter functions
        std::string getIdentifier() const
        {
            return identifier;
        }
        size_t getNumBytesPPP() const
        {
            return numBytesPPP;
        }
        size_t getNumTxSequences() const
        {
            return txSequence.size();
        }

        //! String that uniquely identifies the CRSD channel
        std::string identifier;

        //! Number of vectors in the signal array
        size_t numBytesPPP;

        std::vector<TxSequence> txSequence;
    };

    struct Processing
    {
        Processing();

        Processing(std::string type);

        //! Equality operators
        bool operator==(const Processing& other) const
        {
            return  type == other.type &&
                    parameter == other.parameter;
        }
        bool operator!=(const Processing& other) const
        {
            return !((*this) == other);
        }

        //! Type of processing
        std::string type;

        //! (Optional) Text field that can be used for program specific
        //! parameter name & value
        six::ParameterCollection parameter;
    };

    struct SignalCompression
    {
        SignalCompression();

        /*
         *  \func SignalCompression
         *
         *  \brief SignalCompression constructor
         *
         *  \param vectors Set number of vectors
         *  \param samples Set number of samples
         *  \param signalByteOffset set the byte offset of signal block
         *  \param pvpByteOffset set the byte offset of pvp block
         *  \param compressedSize (Optional) Size of the compressed signal block
         */
        SignalCompression(std::string identifier, size_t compressedSignalSize);

        //! Equality operators
        bool operator==(const SignalCompression& other) const
        {
            return identifier == other.identifier &&
                   compressedSignalSize == other.compressedSignalSize;
        }
        bool operator!=(const SignalCompression& other) const
        {
            return !((*this) == other);
        }

        //! Getter functions
        std::string getIdentifier() const
        {
            return identifier;
        }
        size_t getCompressedSignalSize() const
        {
            return compressedSignalSize;
        }
       
        //! Identifier string
        std::string identifier;

        //! Compressed signal size
        size_t compressedSignalSize;

        //! Vector of processing params
        std::vector<Processing> processing;
    };

    /*
     *  \struct Channel
     *
     *  \brief Channel parameters sizes and locations
     *
     *  Paramters that define the Channel signal array and
     *  PVP array size and location.
     *  See section 2.4 and 2.5
     */
    // There's another Channel type in the Metadata,
    // so hiding this inside Data
    struct Channel
    {
        /*
         *  \func Channel
         *
         *  \brief Default constructor
         */
        Channel();

        /*
         *  \func Channel
         *
         *  \brief Custom constructor
         *
         *  \param vectors Set number of vectors
         *  \param samples Set number of samples
         */
        Channel(size_t vectors, size_t samples);

        /*
         *  \func Channel
         *
         *  \brief Custom constructor
         *
         *  \param vectors Set number of vectors
         *  \param samples Set number of samples
         *  \param signalByteOffset set the byte offset of signal block
         *  \param pvpByteOffset set the byte offset of pvp block
         *  \param compressedSize (Optional) Size of the compressed signal block
         */
        Channel(size_t vectors, size_t samples,
                size_t signalByteOffset, size_t pvpByteOffset,
                size_t compressedSize = six::Init::undefined<size_t>());

        //! Equality operators
        bool operator==(const Channel& other) const
        {
            return identifier == other.identifier &&
                   numVectors == other.numVectors &&
                   numSamples == other.numSamples &&
                   signalArrayByteOffset == other.signalArrayByteOffset &&
                   pvpArrayByteOffset == other.pvpArrayByteOffset &&
                   compressedSignalSize == other.compressedSignalSize;
        }
        bool operator!=(const Channel& other) const
        {
            return !((*this) == other);
        }

        //! Getter functions
        size_t getNumVectors() const
        {
            return numVectors;
        }
        size_t getNumSamples() const
        {
            return numSamples;
        }
        size_t getSignalArrayByteOffset() const
        {
            return signalArrayByteOffset;
        }
        size_t getCompressedSignalSize() const
        {
            return compressedSignalSize;
        }

        //! String that uniquely identifies the CPHD channel
        std::string identifier;

        //! Number of vectors in the signal array
        size_t numVectors;

        //! Number of samples per vector in the signal array
        size_t numSamples;

        //! Signal Array offset from the start of the Signal
        //! block (in bytes) to the start of the Signal Array
        //! for the channel.
        size_t signalArrayByteOffset;

        //! PVP Array offset from the start of the PVP block
        //! (in bytes) to the start of the PVP Array for the
        //! channel.
        size_t pvpArrayByteOffset;

        //! (Optional) Size (in bytes) of the compressed signal array
        //! byte sequence for the data channel.
        size_t compressedSignalSize; // Optional
    };

    /*
     *  \Receive Receive
     *
     *  \brief Receive parameters sizes and locations
     *
     *  Paramters that define the Receive signal array and
     *  PVP array size and location.
     *  See section 2.4 and 2.5
     */
    // There's another Receive type in the Metadata,
    // so hiding this inside Data
    struct Receive
    {
        /*
         *  \func Channel
         *
         *  \brief Default constructor
         */
        Receive();

        /*
         *  \func Channel
         *
         *  \brief Custom constructor
         *
         *  \param vectors Set number of vectors
         *  \param samples Set number of samples
         */
        Receive(size_t numBytesPVP);

        /*
         *  \func Channel
         *
         *  \brief Custom constructor
         *
         *  \param vectors Set number of vectors
         *  \param samples Set number of samples
         *  \param signalByteOffset set the byte offset of signal block
         *  \param pvpByteOffset set the byte offset of pvp block
         *  \param compressedSize (Optional) Size of the compressed signal block
         */
        Receive(size_t vectors, size_t samples,
                size_t signalByteOffset, size_t pvpByteOffset,
                size_t compressedSize = six::Init::undefined<size_t>());

        //! Equality operators
        bool operator==(const Receive& other) const
        {
            return signalArrayFormat == other.signalArrayFormat &&
                   numBytesPVP == other.numBytesPVP &&
                   channels == other.channels;
        }
        bool operator!=(const Receive& other) const
        {
            return !((*this) == other);
        }

        //! Getter functions
        SignalArrayFormat getSignalArrayFormat() const
        {
            return signalArrayFormat;
        }
        size_t getNumBytesPVP() const
        {
            return numBytesPVP;
        }
        size_t getNumCRSDChannels() const
        {
            return channels.size();
        }
        Channel getChannel(int channel) const
        {
            return channels[channel];
        }
        SignalCompression* getSignalCompression() const
        {
            if (signalCompression.get())
                return signalCompression.get();
            else
                return nullptr;
        }

        //! Signal array format
        SignalArrayFormat signalArrayFormat;

        //! Number bytes of PVP
        size_t numBytesPVP;

        //! Channels object (vector)
        std::vector<Channel> channels;

        //! Signal Compression info
         mem::ScopedCopyablePtr<SignalCompression> signalCompression;
    };

    //! Constructor
    Data();

    //! Destructor
    virtual ~Data()
    {
    }

    //! Equality operator
    bool operator==(const Data& other) const
    {
        return supportArrayMap == other.supportArrayMap ;
    }
    bool operator!=(const Data& other) const
    {
        return !((*this) == other);
    }

    //! Getter functions
    virtual size_t getNumVectors(size_t channel) const;
    virtual size_t getNumPulses(size_t index) const;
    virtual size_t getNumSamples(size_t channel) const;
    size_t getCompressedSignalSize(size_t channel) const;
    size_t getSignalSize(size_t channel) const;

    size_t getNumSupportArrays() const
    {
        return supportArrayMap.size();
    }

    size_t getNumChannels() const
    {
        if (receiveParameters.get())
            return receiveParameters->channels.size();
        else
            return 0;
    }

    size_t getNumTxSequences() const
    {
        if (transmitParameters.get())
            return transmitParameters->getNumTxSequences();
        else
            return 0;
    }

    size_t getNumBytesPVPSet() const
    {
        if (receiveParameters.get())
            return receiveParameters->numBytesPVP;
        else
            return 0;
    }

    size_t getNumBytesPPPSet() const
    {
        if (transmitParameters.get())
            return transmitParameters->numBytesPPP;
        else
            return 0;
    }

    virtual size_t getNumBytesPerSample() const
    {
        if (receiveParameters.get())
        {
            return crsd::getNumBytesPerSample(receiveParameters->getSignalArrayFormat());
        }
        else
        {
            return 0;
        }
    }

    // SupportArray get functions
    //! Get specific support array by identifier
    SupportArray getSupportArrayById(const std::string& id) const;

    //! Get size of support array element
    size_t getElementSize(const std::string& id) const
    {
        return getSupportArrayById(id).bytesPerElement;
    }
    size_t getAllSupportSize() const;

    //! Create and add new support array
    //! Validates, and add new support array to supportArrayMap
    void setSupportArray(const std::string& id, size_t numRows,
                         size_t numCols, size_t numBytes,
                         int64_t offset);

    //! Checks if wideband data is compressed
    bool isCompressed() const;

public:

    //! (Optional) Transmit Parameters
    mem::ScopedCopyablePtr<Transmit> transmitParameters;

    //! (Optional) Receive parameters
    mem::ScopedCopyablePtr<Receive> receiveParameters;

    //! (Optional) Unordered map key: unique identifier string,
    //! value: supportArray object
    std::unordered_map<std::string, SupportArray> supportArrayMap;

private:
    // Custom comparator
    struct CmpByOffset
    {
        // Overload operator ()
        // Returns true if offset lhs is less than other offset
        bool operator()(const int64_t& lhs, const int64_t& rhs) const
        {
            return lhs < rhs;
        }
    };

    /*
     * Check if channel is in range
     */
    void verifyChannelInRange(size_t channel) const;

    /*
     * Check if tx sequence is in range
     */
    void verifyTxSequenceInRange(size_t index) const;

    // Book keeping map for efficient validation
    // Support Array Map with:
    // key: offset, value: array size
    std::map<int64_t, size_t, CmpByOffset> mOffsetMap;
};

//! Ostream operators
std::ostream& operator<< (std::ostream& os, const Data::SupportArray& d);
std::ostream& operator<< (std::ostream& os, const Data::Transmit& d);
std::ostream& operator<< (std::ostream& os, const Data::Receive& d);
std::ostream& operator<< (std::ostream& os, const Data& d);
}

#endif
