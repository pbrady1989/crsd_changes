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

#ifndef __CRSD_PPP_BLOCK_H__
#define __CRSD_PPP_BLOCK_H__

#include <ostream>
#include <vector>
#include <complex>
#include <cstdint>
#include <stddef.h>
#include <unordered_map>

#include <std/optional>

#include <scene/sys_Conf.h>
#include <crsd/Types.h>
#include <crsd/Data.h>
#include <crsd/PPP.h>
#include <crsd/Metadata.h>
#include <crsd/ByteSwap.h>
#include <six/Parameter.h>

namespace crsd
{
    class FileHeader;
/*
 *  \struct AddedPPP
 *  \brief Template Specialization to get additional ppp
 *
 *  Mimics function template specialization
 *
 * \tparam T Desired type to convert to
 */
template<typename T>
struct AddedPPP
{
    T getAddedPPP(const six::Parameter& val) const
    {
        return static_cast<T>(val);
    }
};
template<typename T>
struct AddedPPP<std::complex<T> >
{
    std::complex<T> getAddedPPP(const six::Parameter& val) const
    {
        return val.getComplex<T>();
    }
};
template<>
struct AddedPPP<std::string>
{
    std::string getAddedPPP(const six::Parameter& val) const
    {
        return val.str();
    }
};

/*!
 *  \struct PPPBlock
 *
 *  \brief The PPP Block contains the actual PPP data
 *
 *  PPPBlock handles reading PPPBlock from CRSD file, and loading the data structure
 */
struct PPPBlock
{
    PPPBlock() = default;

    /*
     *  \func PPPBlock
     *  \brief Consturctor sets up the internal structure of a PPPSet based on metadata.
     *
     *  \param ppp A filled out ppp sturcture. This will be used for
     *  information on where to allocate memory and set each
     *  parameter in a PPP set.
     *  \param[in, out] data A filled out data structure. This will be used to
     *  extract the number of channels and vectors of the VBM.
     *  The data metadata will also be updated with the number of ppp bytes required
     *  if provided number of bytes is not sufficient
    */
    PPPBlock(const Ppp& ppp, const Data& data);
    PPPBlock(const Metadata&);

    /*!
     *  \func PPPBlock
     *
     *  \brief Constructor sets up the internal sturcture of a PPPSet without metadata.
     *
     *  Does not update metadata data numBytesPPP with calculated ppp bytes per set
     *
     *  \param numChannels Set the number of PPP Arrays to allocate in the block
     *  \param numVectors Set the number of PPP Sets to allocate per array
     *  \param ppp A filled out ppp sturcture. This will be used for
     *  information on where to allocate memory and set each
     *  parameter in a PPP set.
     */
    PPPBlock(size_t numChannels,
             const std::vector<size_t>& numVectors,
             const Ppp& ppp);

    /*!
     * Constructor.
     * Same as above, and also sets the underlying PPPSets
     * from the provided data
     *
     * \param numChannels Set the number of PPP Arrays to allocate in the block
     * \param numVectors Set the number of PPP Sets to allocate per array
     * \param ppp A filled out ppp sturcture. This will be used for
     * information on where to allocate memory and set each
     * parameter in a PPP set.
     * \param data Actual PPP data, used to set the PPPSet members
     */
    PPPBlock(size_t numChannels,
            const std::vector<size_t>& numVectors,
            const Ppp& ppp,
            const std::vector<const void*>& data);

    /*!
     *  \func verifyChannelVector
     *
     *  \brief Verify channel and vector indexes provided are valid
     *
     *  \param channel A channel number
     *  \param vector A vector number
     *
     *  \throws except::Exception If channel or vector indexes are less than 0,
     *  or greater than available number of channels or vectors
     */
    void verifyChannelVector(size_t channel, size_t vector) const;

    //! Getter functions
    std::pair<int64_t, double> getTxStart(size_t channel, size_t set) const;
    Vector3 getTxPos(size_t channel, size_t set) const;
    Vector3 getTxVel(size_t channel, size_t set) const;
    double getFX1(size_t channel, size_t set) const;
    double getFX2(size_t channel, size_t set) const;
    double getTXMT(size_t channel, size_t set) const;
    std::pair<int64_t, double> getPhiX0(size_t channel, size_t set) const;
    double getFxFreq0(size_t channel, size_t set) const;
    double getFxRate(size_t channel, size_t set) const;
    double getTxRadInt(size_t channel, size_t set) const;
    Vector3 getTxACX(size_t channel, size_t set) const;
    Vector3 getTxACY(size_t channel, size_t set) const;
    Vector2 getTxEB(size_t channel, size_t set) const;
    int64_t getFxResponseIndex(size_t channel, size_t set) const;
    int64_t getXMIndex(size_t channel, size_t set) const;

    template<typename T>
    T getAddedPPP(size_t channel, size_t set, const std::string& name) const
    {
        verifyChannelVector(channel, set);
        if(mData[channel][set].addedPPP.count(name) == 1)
        {
            AddedPPP<T> aP;
            return aP.getAddedPPP(mData[channel][set].addedPPP.find(name)->second);
            // return AddPPPNamespace::getAddedPPP<T>(mData[channel][set].addedPPP.find(name)->second);
        }
        throw except::Exception(Ctxt(
                "Parameter was not set"));
    }

    //! Setter functions
    void setTxStart(std::pair<int64_t, double> value, size_t channel, size_t set);
    void setTxPos(const Vector3& value, size_t channel, size_t set);
    void setTxVel(const Vector3& value, size_t channel, size_t set);
    void setFX1(double value, size_t channel, size_t set);
    void setFX2(double value, size_t channel, size_t set);
    void setTXMT(double value, size_t channel, size_t set);
    void setPhiX0(std::pair<int64_t, double> value, size_t channel, size_t set);
    void setFxFreq0(double value, size_t channel, size_t set);
    void setFxRate(double value, size_t channel, size_t set);
    void setTxRadInt(double value, size_t channel, size_t set);
    void setTxACX(const Vector3& value, size_t channel, size_t set);
    void setTxACY(const Vector3& value, size_t channel, size_t set);
    void setTxEB(const Vector2& value, size_t channel, size_t set);
    void setFxResponseIndex(int64_t value, size_t channel, size_t set);
    void setXMIndex(int64_t value, size_t channel, size_t set);

    template<typename T>
    void setAddedPPP(T value, size_t channel, size_t set, const std::string& name)
    {
        verifyChannelVector(channel, set);
        if(mPpp.addedPPP.count(name) != 0)
        {
            if(mData[channel][set].addedPPP.count(name) == 0)
            {
                mData[channel][set].addedPPP[name] = six::Parameter();
                mData[channel][set].addedPPP.find(name)->second.setValue(value);
                return;
            }
            throw except::Exception(Ctxt(
                                "Additional parameter requested already exists"));
        }
        throw except::Exception(Ctxt(
                                "Parameter was not specified in XML"));
    }

    /*
     *  \func getPPPdata
     *  \brief This will return a contiguous buffer all the PPP data.
     *
     *  \param channel 0 based index
     *  \param[out] data Will be filled with PPP data. This will
     *  be resized and zeroed internally.
     */
    void getPPPdata(size_t channel,
                    std::vector<sys::ubyte>& data) const;
    void getPPPdata(size_t channel,
                    std::vector<std::byte>& data) const;

    /*
     *  \func getPPPdata
     *  \brief Same as above but uses a void pointer.
     *
     *  \param channel 0 based index
     *  \param[out] data A preallocated buffer for the data.
     */
    void getPPPdata(size_t channel,
                    void*  data) const;

    /*
     *  \func getNumBytesVBP
     *  \brief Number of bytes per PPP seet
     *
     *  Returns the number of bytes of each vector in the PPPBlock.
     *  This does not necessarily match the number of bytes used in
     *  the case where a CRSD file had a larger number allocated than
     *  actually used.
     */
    size_t getNumBytesPPPSet() const;

    /*
     *  \func getPPPsize
     *  \brief Returns the number of bytes in a PPP channel.
     *
     *  \param channel 0 based index
     */
    size_t getPPPsize(size_t channel) const;

    //! Get optional parameter flags
    bool hasXMIndex() const
    {
        return mXMIndexEnabled;
    }

    /*
     *  \func load
     *
     *  \brief Reads in the entire PPP array from an input stream
     *
     *  \param inStream Input stream that contains a valid CRSD file
     *  \param startPPP Offset of start of ppp block
     *  \param sizePPP Size of ppp block
     *  \param numThreads Number of threads desired for parallelism
     *
     *  \throw except::Exception If reach EOF before reading sizePPP bytes
     *
     *  \return Returns the size of the ppp block read in
     */
    // startPPP = crsd header keyword "PPP_BYTE_OFFSET" contains PPP block starting offset
    // sizePPP = crsd header keyword "PPP_DATA_SIZE" contains PPP block size
    int64_t load(io::SeekableInputStream& inStream,
                    int64_t startPPP,
                    int64_t sizePPP,
                    size_t numThreads);
    int64_t load(io::SeekableInputStream& inStream, const FileHeader&, size_t numThreads);

    //! Equality operators
    bool operator==(const PPPBlock& other) const
    {
        return mData == other.mData &&
               mNumBytesPerVector == other.mNumBytesPerVector;
    }

    bool operator!=(const PPPBlock& other) const
    {
        return !((*this) == other);
    }

protected:
    /*!
     *  \struct PPPSet
     *
     *  \brief Parameters for each vector
     *
     *  Stores a single set of parameters
     *  Each channel consists of a PPP Array,
     *  which consists of multiple sets
     */
    struct PPPSet
    {
        /*!
         *  \func PPPSet
         *
         *  \brief Default constructor
         */
        PPPSet();

        /*
         *  \func write
         *
         *  \brief Writes binary data input into PPP Set
         *
         *  \param pppBlock A pppBlock struct to access optional parameter flags
         *  \param ppp A filled out ppp sturcture. This will be used for
         *  information on where to allocate memory and set each
         *  parameter in a PPP set.
         *  \param input A pointer to an array of bytes that contains the
         *  parameter data to write into the ppp set
         */
        void write(const PPPBlock& pppBlock, const Ppp& ppp, const sys::byte* input);
        void write(const PPPBlock& pppBlock, const Ppp& ppp, const std::byte* input)
        {
            write(pppBlock, ppp, reinterpret_cast<const sys::byte*>(input));
        }

        /*
         *  \func read
         *
         *  \brief Read PPP set into binary data output
         *
         *  \param ppp A filled out ppp sturcture. This will be used for
         *  information on where to allocate memory and set each
         *  parameter in a PPP set.
         *  \param[out] output A pointer to an array of allocated bytes that
         *  will be written to
         */
        void read(const Ppp& ppp, sys::ubyte* output) const;
        void read(const Ppp& ppp, std::byte* output) const
        {
            read(ppp, reinterpret_cast<sys::ubyte*>(output));
        }

        //! Equality operators
        bool operator==(const PPPSet& other) const
        {
            return txTime == other.txTime && txPos == other.txPos &&
                    txVel == other.txVel && fx1 == other.fx1 &&
                    fx2 == other.fx2 && phiX0 == other.phiX0 &&
                    fxFreq0 == other.fxFreq0 && fxRate == other.fxRate &&
                    txRadInt == other.txRadInt && txACX == other.txACX &&
                    txACY == other.txACY && txEB == other.txEB &&
                    fxResponseIndex == other.fxResponseIndex && xmIndex == other.xmIndex &&
                    txmt == other.txmt && addedPPP == other.addedPPP;
        }
        bool operator!=(const PPPSet& other) const
        {
            return !((*this) == other);
        }

        //! Required Parameters
        std::pair<int64_t, double> txTime;
        Vector3 txPos;
        Vector3 txVel;
        double fx1;
        double fx2;
        double txmt;
        std::pair<int64_t, double> phiX0;
        double fxFreq0;
        double fxRate;
        double txRadInt;
        Vector3 txACX;
        Vector3 txACY;
        Vector2 txEB;
        int64_t fxResponseIndex;
        mem::ScopedCopyablePtr<std::int64_t> xmIndex;

        //! (Optional) Additional parameters
        std::unordered_map<std::string,six::Parameter> addedPPP;
    };
    friend std::ostream& operator<< (std::ostream& os, const PPPSet& p);

private:
    //! The PPP Block [Num Channles][Num Parameters]
    std::vector<std::vector<PPPSet> > mData;
    //! Number of bytes per PPP vector
    size_t mNumBytesPerVector = 0;
    //! PPP block metadata
    Ppp mPpp;

    // Optional parameter flags
    bool mXMIndexEnabled;

    //! Ostream operator
    friend std::ostream& operator<< (std::ostream& os, const PPPBlock& p);
};
}
#endif
