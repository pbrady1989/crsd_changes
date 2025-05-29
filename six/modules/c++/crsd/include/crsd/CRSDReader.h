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
#ifndef __CRSD_CRSD_READER_H__
#define __CRSD_CRSD_READER_H__

#include <memory>

#include <scene/sys_Conf.h>

#include <crsd/Metadata.h>
#include <crsd/FileHeader.h>
#include <crsd/PVPBlock.h>
#include <crsd/PPPBlock.h>
#include <crsd/Wideband.h>
#include <crsd/SupportBlock.h>

namespace crsd
{
/*
 *  \class CRSDReader
 *
 *  \brief Used to read a CRSD file.
 *  Requires a valid CRSD file,and optional schemas
 *  for XML format verification
 */
struct CRSDReader final
{
    /*
     *  \func CRSDReader constructor
     *  \brief Construct CRSDReader from an input stream
     *
     *  \param inStream Input stream containing CRSD file
     *  \param numThreads Number of threads for parallelization
     *  \param schemaPaths (Optional) XML schemas for validation
     *  \param logger (Optional) Provide custom log
     */
    // Provides access to wideband but doesn't read it
    CRSDReader(std::shared_ptr<io::SeekableInputStream> inStream,
               size_t numThreads,
               const std::vector<std::string>& schemaPaths =
                       std::vector<std::string>(),
               std::shared_ptr<logging::Logger> logger =
                       std::shared_ptr<logging::Logger>());

    /*
     *  \func CRSDReader constructor
     *  \brief Construct CRSDReader from a file pathname
     *
     *  \param fromFile File path of CRSD file
     *  \param numThreads Number of threads for parallelization
     *  \param schemaPaths (Optional) XML schemas for validation
     *  \param logger (Optional) Provide custom log
     */
    // Provides access to wideband but doesn't read it
    CRSDReader(const std::string& fromFile,
               size_t numThreads,
               const std::vector<std::string>& schemaPaths =
                       std::vector<std::string>(),
               std::shared_ptr<logging::Logger> logger =
                       std::shared_ptr<logging::Logger>());

    //! Get parameter functions
    size_t getNumChannels() const
    {
        return mMetadata.data.getNumChannels();
    }
    //! Get parameter functions
    size_t getNumTxSequences() const
    {
        return mMetadata.data.getNumTxSequences();
    }
    //! 0-based channel number
    size_t getNumVectors(size_t channel) const
    {
        return mMetadata.data.getNumVectors(channel);
    }
    //! 0-based tx sequence number
    size_t getNumPulses(size_t index) const
    {
        return mMetadata.data.getNumPulses(index);
    }
    //! 0-based channel number
    size_t getNumSamples(size_t channel) const
    {
        return mMetadata.data.getNumSamples(channel);
    }
    //! returns total per complex sample (2, 4, or 8)
    size_t getNumBytesPerSample() const
    {
        return mMetadata.data.getNumBytesPerSample();
    }

    /*
     *  \func getFileOffset
     *  \brief Calculate signal array offset in file
     *
     *  \param channel The channel number
     *  \param vector The vector number
     *  \param sample The sample number
     *
     *  \return offset of signal sample in file
     */
    int64_t getFileOffset(size_t channel, size_t vector, size_t sample) const
    {
        return mWideband->getFileOffset(channel, vector, sample);
    }

    //! Get file header object
    const FileHeader& getFileHeader() const
    {
        return mFileHeader;
    }
    //! Get metadata object
    const Metadata& getMetadata() const
    {
        return mMetadata;
    }
    //! Get per vector parameters
    const PVPBlock& getPVPBlock() const
    {
        return mPVPBlock;
    }
    //! Get per vector parameters
    const PPPBlock& getPPPBlock() const
    {
        return mPPPBlock;
    }
    //! Get signal data
    const Wideband& getWideband() const
    {
        return *mWideband;
    }
    //! Get support data
    const SupportBlock& getSupportBlock() const
    {
        return *mSupportBlock;
    }

private:
    // Keep info about the CRSD collection
    //! New crsd file header
    FileHeader mFileHeader;
    //! Metadata read in from CRSD file
    Metadata mMetadata;
    //! Support Block book-keeping info read in from CRSD file
    std::unique_ptr<SupportBlock> mSupportBlock;
    //! Per Vector Parameter info read in from CRSD file
    PVPBlock mPVPBlock;
    //! Per Pulse Parameter info read in from CRSD file
    PPPBlock mPPPBlock;
    //! Signal block book-keeping info read in from CRSD file
    std::unique_ptr<Wideband> mWideband;

    /*
     *  Read in header, metadata, supportblock, pvpblock and wideband
     */
    void initialize(std::shared_ptr<io::SeekableInputStream> inStream,
                    size_t numThreads,
                    std::shared_ptr<logging::Logger> logger,
                    const std::vector<std::string>& schemaPaths);
};
}

#endif
