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

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>

#include <nitf/coda-oss.hpp>

#include <crsd/Enums.h>
#include <crsd/Types.h>
#include <crsd/PVP.h>
#include <crsd/PVPBlock.h>
#include <crsd/PPP.h>
#include <crsd/PPPBlock.h>
#include <crsd/Metadata.h>

namespace crsd
{

double getRandom()
{
    const double r = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    return -1000.0  + r * 2000.0;
}

Vector3 getRandomVector3()
{
    Vector3 ret;
    ret[0] = getRandom();
    ret[1] = getRandom();
    ret[2] = getRandom();
    return ret;
}

Vector2 getRandomVector2()
{
    Vector2 ret;
    ret[0] = getRandom();
    ret[1] = getRandom();
    return ret;
}

void setPVPXML(Pvp& pvp)
{
    // Size, Offset, Format
    pvp.append(pvp.rcvStart);
    pvp.append(pvp.rcvPos);
    pvp.append(pvp.rcvVel);
    pvp.append(pvp.frcv1);
    pvp.append(pvp.frcv2);
    pvp.append(pvp.refPhi0);
    pvp.append(pvp.refFreq);
    pvp.append(pvp.dfiC0);
    pvp.append(pvp.ficRate);
    pvp.append(pvp.rcvACX);
    pvp.append(pvp.rcvACY);
    pvp.append(pvp.rcvEB);
    pvp.append(pvp.signal);
    pvp.append(pvp.ampSF);
    pvp.append(pvp.txPulseIndex);
}

void setVectorParameters(size_t channel,
                          size_t vector,
                          PVPBlock& pvpBlock)
{
    double time1 = getRandom();
    double time2 = getRandom();
    pvpBlock.setRcvStart(std::pair<int64_t,double>(getRandom(),getRandom()), channel, vector);
    pvpBlock.setRcvPos(getRandomVector3(), channel, vector);
    pvpBlock.setRcvVel(getRandomVector3(), channel, vector);

    pvpBlock.setFRCV1(getRandom(), channel, vector);
    pvpBlock.setFRCV2(getRandom(), channel, vector);
    pvpBlock.setRefFreq(getRandom(), channel, vector);
    pvpBlock.setDFIC0(getRandom(), channel, vector);
    pvpBlock.setFICRate(getRandom(), channel, vector);
    pvpBlock.setRcvACX(getRandomVector3(), channel, vector);
    pvpBlock.setRcvACY(getRandomVector3(), channel, vector);
    pvpBlock.setRcvEB(getRandomVector2(), channel, vector);
    pvpBlock.setSignal(static_cast<int64_t>(getRandom()), channel, vector);
    pvpBlock.setAmpSF(getRandom(), channel, vector);
    pvpBlock.setDGRGC(getRandom(), channel, vector);
    pvpBlock.setTxPulseIndex(static_cast<int64_t>(getRandom()), channel, vector);
    pvpBlock.setRefPhi0(std::pair<int64_t,double>(getRandom(),getRandom()), channel, vector);
}

void setPPPXML(Ppp& ppp)
{
    // Size, Offset, Format
    ppp.append(ppp.txTime);
    ppp.append(ppp.txPos);
    ppp.append(ppp.txVel);
    ppp.append(ppp.fx1);
    ppp.append(ppp.fx2);
    ppp.append(ppp.txmt);
    ppp.append(ppp.phiX0);
    ppp.append(ppp.fxFreq0);
    ppp.append(ppp.fxRate);
    ppp.append(ppp.txRadInt);
    ppp.append(ppp.txACX);
    ppp.append(ppp.txACY);
    ppp.append(ppp.txEB);
    ppp.append(ppp.fxResponseIndex);
}

void setPulseParameters(size_t txSequence,
                        size_t pulse,
                        PPPBlock& pppBlock)
{
    pppBlock.setTxStart(std::pair<int64_t,double>(getRandom(),getRandom()), txSequence, pulse);
    pppBlock.setTxPos(getRandomVector3(), txSequence, pulse);
    pppBlock.setTxVel(getRandomVector3(), txSequence, pulse);
    pppBlock.setFX1(getRandom(), txSequence, pulse);
    pppBlock.setFX2(getRandom(), txSequence, pulse);
    pppBlock.setTXMT(getRandom(), txSequence, pulse);
    pppBlock.setPhiX0(std::pair<int64_t,double>(getRandom(),getRandom()), txSequence, pulse);
    pppBlock.setFxFreq0(getRandom(), txSequence, pulse);
    pppBlock.setFxRate(getRandom(), txSequence, pulse);
    pppBlock.setTxRadInt(getRandom(), txSequence, pulse);
    pppBlock.setTxACX(getRandomVector3(), txSequence, pulse);
    pppBlock.setTxACY(getRandomVector3(), txSequence, pulse);
    pppBlock.setTxEB(getRandomVector2(), txSequence, pulse);
    pppBlock.setFxResponseIndex(static_cast<int64_t>(getRandom()), txSequence, pulse);
}

void setUpMetadata(Metadata& metadata)
{
    // We must have a collectType set
    // metadata.collectionID.collectType = CollectType::MONOSTATIC;
    // metadata.collectionID.setClassificationLevel("Unclassified");
    // metadata.collectionID.releaseInfo = "Release";
    // // We must have a radar mode set
    // metadata.collectionID.radarMode = RadarModeType::SPOTLIGHT;

    // // We must set pvpNumBytes
    // // Set default pvp size based on setPVP function
    // metadata.data.numBytesPVP = (1*12 + 3*5) * 8;

    // metadata.sceneCoordinates.iarp.ecf = getRandomVector3();
    // metadata.sceneCoordinates.iarp.llh = LatLonAlt(0,0,0);
    // metadata.sceneCoordinates.referenceSurface.planar.reset(new Planar());
    // metadata.sceneCoordinates.referenceSurface.planar->uIax = getRandomVector3();
    // metadata.sceneCoordinates.referenceSurface.planar->uIay = getRandomVector3();
    // // We must have corners set
    // for (size_t ii = 0; ii < six::Corners<double>::NUM_CORNERS; ++ii)
    // {
    //     metadata.sceneCoordinates.imageAreaCorners.getCorner(ii).clearLatLon();
    // }
    // metadata.channel.fxFixedCphd = true;
    // metadata.channel.toaFixedCphd = false;
    // metadata.channel.srpFixedCphd = false;
    // metadata.referenceGeometry.srp.ecf = getRandomVector3();
    // metadata.referenceGeometry.srp.iac = getRandomVector3();
    // metadata.referenceGeometry.monostatic.reset(new Monostatic());
    // metadata.referenceGeometry.monostatic->arpPos = getRandomVector3();
    // metadata.referenceGeometry.monostatic->arpVel = getRandomVector3();
}
}
