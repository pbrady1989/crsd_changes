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

#ifndef __CRSD_METADATA_H__
#define __CRSD_METADATA_H__

#include <ostream>
#include <crsd/SARInfo.h>
#include <crsd/TransmitInfo.h>
#include <crsd/ProductInfo.h>
#include <crsd/Antenna.h>
#include <crsd/Channel.h>
#include <crsd/Data.h>
#include <crsd/Dwell.h>
#include <crsd/TxSequence.h>
#include <crsd/ErrorParameters.h>
#include <crsd/FileHeader.h>
#include <crsd/Global.h>
#include <crsd/MetadataBase.h>
#include <crsd/PVP.h>
#include <crsd/PPP.h>
#include <crsd/ReferenceGeometry.h>
#include <crsd/SceneCoordinates.h>
#include <crsd/SupportArray.h>
#include <crsd/Utilities.h>

namespace crsd
{
/*!
 *  \class Metadata
 *
 *  This class extends the Data object providing the CRSD
 *  layer and utilities for access.  In order to write a CRSD,
 *  you must have a CRSDData object that is populated with
 *  the mandatory parameters, and you must add it to the Container
 *  object first.
 *
 *  This object contains all of the sub-blocks for CRSD.
 *
 */
struct Metadata final : MetadataBase
{
    //! Default constructor
    //! Initializes CRSD version to default version specified in FileHeader
    Metadata();

    Metadata(std::string type);

    /*
     * Getter functions
     */
    size_t getNumChannels() const override;
    size_t getNumVectors(size_t channel) const override;    // 0-based channel number
    size_t getNumSamples(size_t channel) const override;   // 0-based channel number
    size_t getNumBytesPerSample() const override;   // 2, 4, or 8 bytes/complex sample
    size_t getCompressedSignalSize(size_t channel) const override;
    bool isCompressed() const override;

    //! Get CRSD version
    std::string getVersion() const;

    //! Set CRSD version
    void setVersion(const std::string& version);

    //! Get CRSD type
    std::string getType() const;

    //! Set CRSD version
    void setType(const std::string& type);

    //!  SARInfo Block 
    ProductInfo productInfo;

    //!  Global block. Contains the information
    Global global;

    //! Scene Coordinates block. Parameters that define geographic
    //! coordinates for in the imaged scene
    SceneCoordinates sceneCoordinates;

    //!  Data block. Very unfortunate name, but matches the CRSD spec.
    //!  Contains parameters that describe binary data components contained
    //!  in the product
    Data data;

    //! (required) SupportArray block. Describes the binary support array
    //! content and grid coordinates
    SupportArray supportArray;

    //! Reference Geometry block. Parameter describes the collection geometry
    //! for the reference vector (v_CH_REF) of the reference channel (REF_CH_ID)
    ReferenceGeometry referenceGeometry;

    //! (required) Antenna block. Describes the trasmit and receive antennas
    Antenna antenna;

    //! (optional) Dwell block. Paramaters that specify the dwell time supported by
    //! the signal arrays contained in the CRSD product
    mem::ScopedCopyablePtr<Dwell> dwell;

    //! Channel block. Parameters that describe the data channels contained
    //! in the product
    mem::ScopedCopyablePtr<Channel> channel;

    //! PVP block. Parameters that describe the size of position of each
    //! vector paramter
    mem::ScopedCopyablePtr<Pvp> pvp;

    //! PPP block. Parameters that describe the size of position of each
    //! per pulse  paramter
    mem::ScopedCopyablePtr<Ppp> ppp;

    //!  ReceiveInfo Block 
    mem::ScopedCopyablePtr<SARInfo> sarInfo;

    //!  TransmitInfo Block 
    mem::ScopedCopyablePtr<TransmitInfo> transmitInfo;

    //!  ReceiveInfo Block 
    mem::ScopedCopyablePtr<TransmitInfo> receiveInfo;

    //! (optional) Atenna block. Describes the trasmit and receive antennas
    mem::ScopedCopyablePtr<TxSequence> txSequence;

    //! (Optional) Error Parameters block. Describes the statistics of errors
    //! in measured or estimated parameters that describe the collection
    mem::ScopedCopyablePtr<ErrorParameters> errorParameters;

    //! (Optional) Geograpy Information block. Describes geographic features
    std::vector<six::GeoInfo> geoInfo;

    bool operator==(const Metadata& other) const;

    bool operator!=(const Metadata& other) const
    {
        return !((*this) == other);
    }

private:

    //! Stores file Version
    std::string mVersion;

    //! Stores CRSD type (SAR, TX, RX)
    std::string mType;
};

//! Ostream operator
std::ostream& operator<< (std::ostream& os, const Metadata& d);
}

#endif
