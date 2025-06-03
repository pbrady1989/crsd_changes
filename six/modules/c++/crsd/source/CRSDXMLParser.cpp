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
#include <set>
#include <io/StringStream.h>
#include <logging/NullLogger.h>
#include <mem/ScopedCopyablePtr.h>
#include <str/Convert.h>
#include <six/Utilities.h>
#include <six/SICommonXMLParser.h>
#include <crsd/CRSDXMLParser.h>
#include <crsd/Enums.h>
#include <crsd/Metadata.h>
#include <crsd/Types.h>


#define ENFORCESPEC 0

namespace
{
typedef xml::lite::Element* XMLElem;
}

namespace crsd
{

CRSDXMLParser::CRSDXMLParser(
        const std::string& uri,
        bool addClassAttributes,
        logging::Logger* log,
        bool ownLog) :
    six::XMLParser(uri, addClassAttributes, log, ownLog),
    mCommon(getDefaultURI(), addClassAttributes, getDefaultURI(), log)
{
}

/*
 * TO XML
 */
std::unique_ptr<xml::lite::Document> CRSDXMLParser::toXML(
        const Metadata& metadata)
{
    
    std::unique_ptr<xml::lite::Document> doc(new xml::lite::Document());

    XMLElem root = newElement("CRSD");
    doc->setRootElement(root);

    std::cout << "Converting global to XML..." << std::endl;
    toXML(metadata.global, root);    

    std::cout << "Converting scene to XML..." << std::endl;
    toXML(metadata.sceneCoordinates, root);    

    std::cout << "Converting data to XML..." << std::endl;
    toXML(metadata.data, root);    

    std::cout << "Converting productInfo to XML..." << std::endl;
    toXML(metadata.productInfo, root);    

    std::cout << "Converting support to XML..." << std::endl;
    toXML(metadata.supportArray, root);    

    std::cout << "Converting refGeo to XML..." << std::endl;
    toXML(metadata.referenceGeometry, root);    

    std::cout << "Converting antenna to XML..." << std::endl;
    toXML(metadata.antenna,root);    

    if (metadata.dwell.get())
    {
        std::cout << "Converting dwell to XML..." << std::endl;
        toXML(*(metadata.dwell), root);
    }
    if (metadata.pvp.get())
    {
        std::cout << "Converting pvp to XML..." << std::endl;
        toXML(*(metadata.pvp), root);
    }
    if (metadata.ppp.get())
    {
        std::cout << "Converting ppp to XML..." << std::endl;
        toXML(*(metadata.ppp), root);
    }
    if (metadata.sarInfo.get())
    {
        std::cout << "Converting sarInfo to XML..." << std::endl;
        toXML(*(metadata.sarInfo), root);
    }
    if (metadata.transmitInfo.get())
    {
        std::cout << "Converting transmitInfo to XML..." << std::endl;
        toXML(*(metadata.transmitInfo), root);
    }
    if (metadata.receiveInfo.get())
    {
        std::cout << "Converting receiveInfo to XML..." << std::endl;
        toXML(*(metadata.receiveInfo), root);
    }
    if (metadata.txSequence.get())
    {
        std::cout << "Converting txSequence to XML..." << std::endl;
        toXML(*(metadata.txSequence), root);
    }
    if (metadata.errorParameters.get())
    {
        std::cout << "Converting errorParams to XML..." << std::endl;
        toXML(*(metadata.errorParameters), root);
    }
    for (const auto& geoInfo : metadata.geoInfo)
    {
        std::cout << "Converting geoInfo to XML..." << std::endl;
        toXML(geoInfo, root);
    }
    //set the XMLNS
    root->setNamespacePrefix("", getDefaultURI());

    std::cout << "Done converting to XML..." << std::endl;
    
    return doc;
}

XMLElem CRSDXMLParser::toXML(const Global& global, XMLElem parent)
{
    XMLElem globalXML = newElement("Global", parent);

    createDateTime("CollectionRefTime", global.collectionRefTime, globalXML);
    if (global.tropoParameters.get())
    {
        XMLElem tropoXML = newElement("TropoParameters", globalXML);
        createDouble("N0", global.tropoParameters->n0, tropoXML);
        createString("RefHeight", global.tropoParameters->refHeight, tropoXML);
        createOptionalDouble("N0ErrorStdDev", global.tropoParameters->n0ErrorStdDev, tropoXML);
    }
    if (global.ionoParameters.get())
    {
        XMLElem ionoXML = newElement("IonoParameters", globalXML);
        createDouble("TECV", global.ionoParameters->tecv, ionoXML);
        createDouble("F2Height", global.ionoParameters->f2Height, ionoXML);
        createOptionalDouble("TECVErrorStdDev", global.ionoParameters->tecvErrorStdDev, ionoXML);
    }
    if (global.transmitParameters.get())
    {
        XMLElem txParamsXML = newElement("Transmit", globalXML);
        createDouble("TxTime1", global.transmitParameters.get()->txTime1, txParamsXML);
        createDouble("TxTime2", global.transmitParameters.get()->txTime2, txParamsXML);
        createDouble("FxMin", global.transmitParameters.get()->fxMin, txParamsXML);
        createDouble("FxMax", global.transmitParameters.get()->fxMax, txParamsXML);
    }
    if (global.receiveParameters.get())
    {
        XMLElem rcvParamsXML = newElement("Receive", globalXML);
        createDouble("RcvStartTime1", global.receiveParameters.get()->rcvStartTime1, rcvParamsXML);
        createDouble("RcvStartTime2", global.receiveParameters.get()->rcvStartTime2, rcvParamsXML);
        createDouble("FrcvMin", global.receiveParameters.get()->fRcvMin, rcvParamsXML);
        createDouble("FrcvMax", global.receiveParameters.get()->fRcvMax, rcvParamsXML);
    }
    return globalXML;
}

XMLElem CRSDXMLParser::toXML(const SceneCoordinates& sceneCoords, XMLElem parent)
{
    XMLElem sceneCoordsXML = newElement("SceneCoordinates", parent);
    createString("EarthModel", sceneCoords.earthModel, sceneCoordsXML);    

    XMLElem iarpXML = newElement("IARP", sceneCoordsXML);
    mCommon.createVector3D("ECF", sceneCoords.iarp.ecf, iarpXML);
    mCommon.createLatLonAlt("LLH", sceneCoords.iarp.llh, iarpXML);
    XMLElem refSurfXML = newElement("ReferenceSurface", sceneCoordsXML);
    if (sceneCoords.referenceSurface.planar.get())
    {
        XMLElem planarXML = newElement("Planar", refSurfXML);
        mCommon.createVector3D("uIAX", sceneCoords.referenceSurface.planar->uIax, planarXML);
        mCommon.createVector3D("uIAY", sceneCoords.referenceSurface.planar->uIay, planarXML);
    }
    else if (sceneCoords.referenceSurface.hae.get())
    {
        XMLElem haeXML = newElement("HAE", refSurfXML);
        mCommon.createLatLon("uIAXLL", sceneCoords.referenceSurface.hae->uIax, haeXML);
        mCommon.createLatLon("uIAYLL", sceneCoords.referenceSurface.hae->uIay, haeXML);
    }
    else
    {
        throw except::Exception(Ctxt(
                "Reference Surface must be one of two types"));
    }

    XMLElem imageAreaXML = newElement("ImageArea", sceneCoordsXML);
    mCommon.createVector2D("X1Y1", sceneCoords.imageArea.x1y1, imageAreaXML);
    mCommon.createVector2D("X2Y2", sceneCoords.imageArea.x2y2, imageAreaXML);

    if (!sceneCoords.imageArea.polygon.empty())
    {
        XMLElem polygonXML = newElement("Polygon", imageAreaXML);
        setAttribute(polygonXML, "size", sceneCoords.imageArea.polygon.size());
        for (size_t ii = 0; ii < sceneCoords.imageArea.polygon.size(); ++ii)
        {
            XMLElem vertexXML = mCommon.createVector2D("Vertex", sceneCoords.imageArea.polygon[ii], polygonXML);
            setAttribute(vertexXML, "index", ii+1);
        }
    }

    createLatLonFootprint("ImageAreaCornerPoints", "IACP", sceneCoords.imageAreaCorners, sceneCoordsXML);

    // Extended Area (Optional)
    if(sceneCoords.extendedArea.get())
    {
        XMLElem extendedAreaXML = newElement("ExtendedArea", sceneCoordsXML);
        mCommon.createVector2D("X1Y1", sceneCoords.extendedArea->x1y1, extendedAreaXML);
        mCommon.createVector2D("X2Y2", sceneCoords.extendedArea->x2y2, extendedAreaXML);

        if (!sceneCoords.extendedArea->polygon.empty())
        {
            XMLElem polygonXML = newElement("Polygon", sceneCoordsXML);
            setAttribute(polygonXML, "size", sceneCoords.extendedArea->polygon.size());
            for (size_t ii = 0; ii < sceneCoords.extendedArea->polygon.size(); ++ii)
            {
                XMLElem vertexXML = mCommon.createVector2D("Vertex", sceneCoords.extendedArea->polygon[ii], polygonXML);
                setAttribute(vertexXML, "index", ii+1);
            }
        }
    }

    // ImageGrid (Optional)
    if(sceneCoords.imageGrid.get())
    {
        XMLElem imageGridXML = newElement("ImageGrid", sceneCoordsXML);
        if(!six::Init::isUndefined(sceneCoords.imageGrid->identifier))
        {
            createString("Identifier", sceneCoords.imageGrid->identifier, imageGridXML);
        }
        XMLElem iarpLocationXML = newElement("IARPLocation", imageGridXML);
        createDouble("Line", sceneCoords.imageGrid->iarpLocation.line, iarpLocationXML);
        createDouble("Sample", sceneCoords.imageGrid->iarpLocation.sample, iarpLocationXML);

        XMLElem iaxExtentXML = newElement("IAXExtent", imageGridXML);
        createDouble("LineSpacing", sceneCoords.imageGrid->xExtent.lineSpacing, iaxExtentXML);
        createInt("FirstLine", sceneCoords.imageGrid->xExtent.firstLine, iaxExtentXML);
        createInt("NumLines", sceneCoords.imageGrid->xExtent.numLines, iaxExtentXML);

        XMLElem iayExtentXML = newElement("IAYExtent", imageGridXML);
        createDouble("SampleSpacing", sceneCoords.imageGrid->yExtent.sampleSpacing, iayExtentXML);
        createInt("FirstSample", sceneCoords.imageGrid->yExtent.firstSample, iayExtentXML);
        createInt("NumSamples", sceneCoords.imageGrid->yExtent.numSamples, iayExtentXML);

        if (!sceneCoords.imageGrid->segments.empty())
        {
            XMLElem segmentListXML = newElement("SegmentList", imageGridXML);
            createInt("NumSegments", sceneCoords.imageGrid->segments.size(), segmentListXML);

            for (size_t ii = 0; ii < sceneCoords.imageGrid->segments.size(); ++ii)
            {
                XMLElem segmentXML = newElement("Segment", segmentListXML);
                createString("Identifier", sceneCoords.imageGrid->segments[ii].identifier, segmentXML);
                createInt("StartLine", sceneCoords.imageGrid->segments[ii].startLine, segmentXML);
                createInt("StartSample", sceneCoords.imageGrid->segments[ii].startSample, segmentXML);
                createInt("EndLine", sceneCoords.imageGrid->segments[ii].endLine, segmentXML);
                createInt("EndSample", sceneCoords.imageGrid->segments[ii].endSample, segmentXML);
                if (!sceneCoords.imageGrid->segments[ii].polygon.empty())
                {
                    XMLElem polygonXML = newElement("SegmentPolygon", segmentXML);
                    setAttribute(polygonXML, "size", sceneCoords.imageGrid->segments[ii].polygon.size());
                    for (size_t jj = 0; jj < sceneCoords.imageGrid->segments[ii].polygon.size(); ++jj)
                    {
                        XMLElem svXML = newElement("SV", polygonXML);
                        setAttribute(svXML, "index", sceneCoords.imageGrid->segments[ii].polygon[jj].getIndex());
                        createDouble("Line", sceneCoords.imageGrid->segments[ii].polygon[jj].line, svXML);
                        createDouble("Sample", sceneCoords.imageGrid->segments[ii].polygon[jj].sample, svXML);
                    }
                }
            }
        }
    }

    return sceneCoordsXML;
}

XMLElem CRSDXMLParser::toXML(const Data& data, XMLElem parent)
{
    XMLElem dataXML = newElement("Data", parent);
    XMLElem supportXML = newElement("Support", dataXML);
    createInt("NumSupportArrays", data.supportArrayMap.size(), supportXML);
    for (const auto& entry : data.supportArrayMap)
    {
        XMLElem supportArrayXML = newElement("SupportArray", supportXML);
        createString("Identifier", entry.second.identifier, supportArrayXML);
        createInt("NumRows", entry.second.numRows, supportArrayXML);
        createInt("NumCols", entry.second.numCols, supportArrayXML);
        createInt("BytesPerElement", entry.second.bytesPerElement, supportArrayXML);
        createInt("ArrayByteOffset", entry.second.arrayByteOffset, supportArrayXML);
    }
    if (data.transmitParameters.get())
    {
        XMLElem transmitXML = newElement("Transmit", supportXML);
        createString("Identifier", data.transmitParameters->getIdentifier(), transmitXML);
        createInt("NumBytesPPP", data.transmitParameters->getNumBytesPPP(), transmitXML);
        createInt("NumTxSequences", data.transmitParameters->getNumTxSequences(), transmitXML);
        for (size_t ii = 0; ii < data.transmitParameters->txSequence.size(); ++ii)
        {
             XMLElem txSeqXML = newElement("TxSequence", transmitXML);
            createString("Identifier", data.transmitParameters->txSequence[ii].identifier, txSeqXML);
            createInt("NumPulses", data.transmitParameters->txSequence[ii].getNumPulses(), txSeqXML);
            createInt("PPPArrayByteOffset", data.transmitParameters->txSequence[ii].getPPPArrayByteOffset(), txSeqXML);
        }
    }

    if (data.receiveParameters.get())
    {
        XMLElem rcvXML = newElement("Receive", supportXML);
        createString("SignalArrayFormat", data.receiveParameters->getSignalArrayFormat(), rcvXML);
        createInt("NumBytesPVP", data.receiveParameters->getNumBytesPVP(), rcvXML);
        createInt("NumCRSDChannels", data.receiveParameters->getNumCRSDChannels(), rcvXML);
        for (size_t ii = 0; ii < data.receiveParameters->channels.size(); ++ii)
        {
            XMLElem channelXML = newElement("Channel", rcvXML);
            createString("Identifier", data.receiveParameters->channels[ii].identifier, channelXML);
            createInt("NumPulses", data.receiveParameters->channels[ii].getNumVectors(), channelXML);
            createInt("NumSamples", data.receiveParameters->channels[ii].getNumSamples(), channelXML);
            createInt("SignalArrayByteOffset", data.receiveParameters->channels[ii].getSignalArrayByteOffset(), channelXML);
            createInt("PVPArrayByteOffset", data.receiveParameters->channels[ii].pvpArrayByteOffset, channelXML);
        }
        if (data.receiveParameters->signalCompression.get())
        {
            XMLElem sigCompressXML = newElement("SignalCompression", rcvXML);
            createString("Identifier", data.receiveParameters->getSignalCompression()->getIdentifier(), sigCompressXML);
            createInt("CompressedSignalSize", data.receiveParameters->getSignalCompression()->getCompressedSignalSize(), sigCompressXML);
            if (data.receiveParameters->getSignalCompression()->processing.size() > 0)
            {
                for (size_t ii = 0; ii < data.receiveParameters->channels.size(); ++ii)
                {
                    XMLElem processingXML = newElement("Processing", sigCompressXML);
                    createString("Type", data.receiveParameters->getSignalCompression()->processing[ii].type, processingXML);
                    mCommon.addParameters("Parameter", getDefaultURI(), data.receiveParameters->getSignalCompression()->processing[ii].parameter, processingXML);
                }
            }
        }
    }
    return dataXML;
}

XMLElem CRSDXMLParser::toXML(const TxSequence& txSequence, XMLElem parent)
{
    XMLElem txSequenceXML = newElement("TxSequence", parent);
    createString("RefTxID", txSequence.refTxId, txSequenceXML);
    createString("TxWFType", txSequence.txWFType, txSequenceXML);
    for (size_t ii = 0; ii < txSequence.parameters.size(); ++ii)
    {
        XMLElem parametersXML = newElement("Parameters", txSequenceXML);
        createString("Identifier", txSequence.parameters[ii].identifier, parametersXML);
        createInt("RefPulseIndex", txSequence.parameters[ii].refPulseIndex, parametersXML);
        createString("XMId", txSequence.parameters[ii].xmid, parametersXML);
        createString("FxResponseId", txSequence.parameters[ii].fxResponseId, parametersXML);
        createBooleanType("FxBWFixed", txSequence.parameters[ii].fxBWFixed, parametersXML);
        createDouble("FxC", txSequence.parameters[ii].fxC, parametersXML);
        createDouble("FxBW", txSequence.parameters[ii].fxBW, parametersXML);
        createDouble("TXmtMin", txSequence.parameters[ii].txmtMin, parametersXML);
        createDouble("TXmtMax", txSequence.parameters[ii].txmtMax, parametersXML);
        createDouble("TxTime1", txSequence.parameters[ii].txTime1, parametersXML);
        createDouble("TxTime2", txSequence.parameters[ii].txTime2, parametersXML);
        createString("TxAPCId", txSequence.parameters[ii].txAPCId, parametersXML);
        createString("TxAPATId", txSequence.parameters[ii].txAPATId, parametersXML);
        XMLElem txRefPointXML = newElement("TxRefPoint", parametersXML);
        mCommon.createVector3D("ECF", txSequence.parameters[ii].txRefPoint.ecf, txRefPointXML);
        mCommon.createVector2D("IAC", txSequence.parameters[ii].txRefPoint.iac, txRefPointXML);
        XMLElem txPolXML = newElement("TxPolarization", parametersXML);
        createString("PolarizationID", txSequence.parameters[ii].txPolarization.polarizationID, txPolXML);
        createDouble("AmpH",txSequence.parameters[ii].txPolarization.ampH, txPolXML);
        createDouble("AmpV",txSequence.parameters[ii].txPolarization.ampV, txPolXML);
        createDouble("PhaseH",txSequence.parameters[ii].txPolarization.phaseH, txPolXML);
        createDouble("PhaseV",txSequence.parameters[ii].txPolarization.phaseV, txPolXML);
        createDouble("TxRefRadIntensity", txSequence.parameters[ii].txRefRadIntensity, parametersXML);
        createDouble("TxRadIntErrorStdDev", txSequence.parameters[ii].txRefRadIntensityStdDev, parametersXML);
        createDouble("TxRefLAtm", txSequence.parameters[ii].txRefLAtm, parametersXML);
        mCommon.addParameters("Parameter", getDefaultURI(), txSequence.parameters[ii].addedParameters, parametersXML);
    }
    return txSequenceXML;
}

XMLElem CRSDXMLParser::toXML(const Channel& channel, XMLElem parent)
{
    XMLElem channelXML = newElement("Channel", parent);
    createString("RefChId", channel.refChId, channelXML);
    for (size_t ii = 0; ii < channel.parameters.size(); ++ii)
    {
        XMLElem parametersXML = newElement("Parameters", channelXML);
        createString("Identifier", channel.parameters[ii].identifier, parametersXML);
        createInt("RefVectorIndex", channel.parameters[ii].refVectorIndex, parametersXML);
        createBooleanType("RefFreqFixed", channel.parameters[ii].refFreqFixed, parametersXML);
        createBooleanType("FrcvFixed", channel.parameters[ii].fRcvFixed, parametersXML);
        createBooleanType("SignalNormal", channel.parameters[ii].signalNormal, parametersXML);
        createDouble("F0Ref", channel.parameters[ii].f0Ref, parametersXML);
        createDouble("Fs", channel.parameters[ii].fs, parametersXML);
        createDouble("BWInst", channel.parameters[ii].bwInst, parametersXML);
        createDouble("RcvStartTime1", channel.parameters[ii].rcvStartTime1, parametersXML);
        createDouble("RcvStartTime2", channel.parameters[ii].rcvStartTime2, parametersXML);
        createDouble("FrcvMin", channel.parameters[ii].fRcvMin, parametersXML);
        createDouble("FrcvMax", channel.parameters[ii].fRcvMax, parametersXML);
        createString("RcvAPCId", channel.parameters[ii].rcvAPCId, parametersXML);
        createString("RcvAPATId", channel.parameters[ii].rcvAPATId, parametersXML);
        XMLElem refPointXML = newElement("RcvRefPoint", parametersXML);
        mCommon.createVector3D("ECF", channel.parameters[ii].rcvRefPoint.ecf, refPointXML);
        mCommon.createVector2D("IAC", channel.parameters[ii].rcvRefPoint.iac, refPointXML);
        XMLElem polXML = newElement("RcvPolarization", parametersXML);
        createString("PolarizationID", channel.parameters[ii].rcvPolarization.polarizationID, polXML);
        createDouble("AmpH", channel.parameters[ii].rcvPolarization.ampH, polXML);
        createDouble("AmpV", channel.parameters[ii].rcvPolarization.ampV, polXML);
        createDouble("PhaseH", channel.parameters[ii].rcvPolarization.phaseH, polXML);
        createDouble("PhaseV", channel.parameters[ii].rcvPolarization.phaseV, polXML);
        createDouble("RcvRefIrradiance", channel.parameters[ii].rcvRefIrradiance, parametersXML);
        createDouble("RcvIrradianceErrorStdDev", channel.parameters[ii].rcvRefIrradianceErrorStdDev, parametersXML);
        createDouble("RcvRefLAtm", channel.parameters[ii].rcvRefLAtm, parametersXML);
        createDouble("PNCRSD", channel.parameters[ii].pncrsd, parametersXML);
        createDouble("BNCRSD", channel.parameters[ii].bncrsd, parametersXML);

        if(channel.parameters[ii].sarImage.get())
        {
            XMLElem sarXML = newElement("SARImage", parametersXML);
            createString("TxId", channel.parameters[ii].sarImage->txID, sarXML);
            createInt("RefVectorPulseIndex", channel.parameters[ii].sarImage->refVectorPulseIndex,sarXML);
            XMLElem txPolXML = newElement("TxPolarization", sarXML);
            createString("PolarizationID", channel.parameters[ii].rcvPolarization.polarizationID, txPolXML);
            createDouble("AmpH", channel.parameters[ii].sarImage->txPolarization.ampH, txPolXML);
            createDouble("AmpV", channel.parameters[ii].sarImage->txPolarization.ampV, txPolXML);
            createDouble("PhaseH", channel.parameters[ii].sarImage->txPolarization.phaseH, txPolXML);
            createDouble("PhaseV", channel.parameters[ii].sarImage->txPolarization.phaseV, txPolXML);
            XMLElem dwellTimesXML = newElement("DwellTimes", sarXML);
            createString("CODId", channel.parameters[ii].sarImage->dwellTime.codId, dwellTimesXML);
            createString("DwellId", channel.parameters[ii].sarImage->dwellTime.dwellId, dwellTimesXML);
            createString("DTAId", channel.parameters[ii].sarImage->dwellTime.dtaId, dwellTimesXML);
            XMLElem imageAreaXML = newElement("ImageArea", sarXML);
            mCommon.createVector2D("X1Y1", channel.parameters[ii].sarImage->imageArea.x1y1, imageAreaXML);
            mCommon.createVector2D("X2Y2", channel.parameters[ii].sarImage->imageArea.x2y2, imageAreaXML);
            if(!channel.parameters[ii].sarImage->imageArea.polygon.empty())
            {
                XMLElem polygonXML = newElement("Polygon", imageAreaXML);
                setAttribute(polygonXML, "size", channel.parameters[ii].sarImage->imageArea.polygon.size());
                for (size_t jj = 0; jj < channel.parameters[ii].sarImage->imageArea.polygon.size(); ++jj)
                {
                    XMLElem vertexXML = mCommon.createVector2D("Vertex", channel.parameters[ii].sarImage->imageArea.polygon[jj], polygonXML);
                    setAttribute(vertexXML, "index", jj+1);
                }
            }
        }   
    }
    return channelXML;
}

XMLElem CRSDXMLParser::toXML(const Pvp& pvp, XMLElem parent)
{
    XMLElem pvpXML = newElement("PVP", parent);
    createPVPType("RcvStart", pvp.rcvStart, pvpXML);
    createPVPType("RcvPos", pvp.rcvPos, pvpXML);
    createPVPType("RcvVel", pvp.rcvVel, pvpXML);
    createPVPType("FRCV1", pvp.frcv1, pvpXML);
    createPVPType("FRCV2", pvp.frcv2, pvpXML);
    createPVPType("RefPhi0", pvp.refPhi0, pvpXML);
    createPVPType("RefFreq", pvp.refFreq, pvpXML);
    createPVPType("DFIC0", pvp.dfiC0, pvpXML);
    createPVPType("FICRate", pvp.ficRate, pvpXML);
    createPVPType("RcvACX", pvp.rcvACX, pvpXML);
    createPVPType("RcvACY", pvp.rcvACY, pvpXML);
    createPVPType("RcvEB", pvp.rcvEB, pvpXML);
    createPVPType("SIGNAL", pvp.signal, pvpXML);
    createPVPType("AmpSF", pvp.ampSF, pvpXML);
    createPVPType("DGRGC", pvp.dgrgc, pvpXML);
    if (!six::Init::isUndefined<size_t>(pvp.txPulseIndex.getOffset()))
    {
        createPVPType("TxPulseIndex", pvp.txPulseIndex, pvpXML);
    }
    for (auto it = pvp.addedPVP.begin(); it != pvp.addedPVP.end(); ++it)
    {
        createAPVPType("AddedPVP", it->second, pvpXML);
    }

    return pvpXML;
}

XMLElem CRSDXMLParser::toXML(const Ppp& ppp, XMLElem parent)
{
    XMLElem pppXML = newElement("PPP", parent);
    createPPPType("TxTime", ppp.txTime, pppXML);
    createPPPType("TxPos", ppp.txPos, pppXML);
    createPPPType("TxVel", ppp.txVel, pppXML);
    createPPPType("FX1", ppp.fx1, pppXML);
    createPPPType("FX2", ppp.fx2, pppXML);
    createPPPType("TXmt", ppp.txmt, pppXML);
    createPPPType("PhiX0", ppp.phiX0, pppXML);
    createPPPType("FxFreq0", ppp.fxFreq0, pppXML);
    createPPPType("FxRate", ppp.fxRate, pppXML);
    createPPPType("TxRadInt", ppp.txRadInt, pppXML);
    createPPPType("TxACX", ppp.txACX, pppXML);
    createPPPType("TxACY", ppp.txACY, pppXML);
    createPPPType("TxEB", ppp.txEB, pppXML);
    createPPPType("FxResponseIndex", ppp.fxResponseIndex, pppXML);
    if (!six::Init::isUndefined<size_t>(ppp.xmIndex.getOffset()))
    {
        createPPPType("XMIndex", ppp.xmIndex, pppXML);
    }
    for (auto it = ppp.addedPPP.begin(); it != ppp.addedPPP.end(); ++it)
    {
        createAPPPType("AddedPPP", it->second, pppXML);
    }

    return pppXML;
}

//Assumes optional handled by caller
XMLElem CRSDXMLParser::toXML(const SupportArray& supports, XMLElem parent)
{
    XMLElem supportsXML = newElement("SupportArray", parent);
    if (!supports.iazArray.empty())
    {
        for (size_t ii = 0; ii < supports.iazArray.size(); ++ii)
        {
            XMLElem iazArrayXML = newElement("IAZArray", supportsXML);
            createString("Identifier", supports.iazArray[ii].getIdentifier(), iazArrayXML);
            createString("ElementFormat", supports.iazArray[ii].elementFormat, iazArrayXML);
            createDouble("X0", supports.iazArray[ii].x0, iazArrayXML);
            createDouble("Y0", supports.iazArray[ii].y0, iazArrayXML);
            createDouble("XSS", supports.iazArray[ii].xSS, iazArrayXML);
            createDouble("YSS", supports.iazArray[ii].ySS, iazArrayXML);
        }
    }
    if (!supports.antGainPhase.empty())
    {
        for (size_t ii = 0; ii < supports.antGainPhase.size(); ++ii)
        {
            XMLElem antGainPhaseXML = newElement("AntGainPhase", supportsXML);
            createString("Identifier", supports.antGainPhase[ii].getIdentifier(), antGainPhaseXML);
            createString("ElementFormat", supports.antGainPhase[ii].elementFormat, antGainPhaseXML);
            createDouble("X0", supports.antGainPhase[ii].x0, antGainPhaseXML);
            createDouble("Y0", supports.antGainPhase[ii].y0, antGainPhaseXML);
            createDouble("XSS", supports.antGainPhase[ii].xSS, antGainPhaseXML);
            createDouble("YSS", supports.antGainPhase[ii].ySS, antGainPhaseXML);
        }
    }
    if (!supports.dwellTimeArray.empty())
    {
        for (size_t ii = 0; ii < supports.dwellTimeArray.size(); ++ii)
        {
            XMLElem dwellTimeXML = newElement("DwellTimeArray", supportsXML);
            createString("Identifier", supports.dwellTimeArray[ii].getIdentifier(), dwellTimeXML);
            createString("ElementFormat", supports.dwellTimeArray[ii].elementFormat, dwellTimeXML);
            createDouble("X0", supports.dwellTimeArray[ii].x0, dwellTimeXML);
            createDouble("Y0", supports.dwellTimeArray[ii].y0, dwellTimeXML);
            createDouble("XSS", supports.dwellTimeArray[ii].xSS, dwellTimeXML);
            createDouble("YSS", supports.dwellTimeArray[ii].ySS, dwellTimeXML);
        }
    }
    if (!supports.fxResponseArray.empty())
    {
        for (size_t ii = 0; ii < supports.fxResponseArray.size(); ++ii)
        {
            XMLElem fxResponseXML = newElement("FxResponseArray", supportsXML);
            createString("Identifier", supports.fxResponseArray[ii].getIdentifier(), fxResponseXML);
            createString("ElementFormat", supports.fxResponseArray[ii].elementFormat, fxResponseXML);
            createDouble("Fx0FXR", supports.fxResponseArray[ii].fx0FXR, fxResponseXML);
            createDouble("FxSSFXR", supports.fxResponseArray[ii].fxSSFXR, fxResponseXML);
        }
    }
    if (!supports.xmArray.empty())
    {
        for (size_t ii = 0; ii < supports.xmArray.size(); ++ii)
        {
            XMLElem xmXML = newElement("XMArray", supportsXML);
            createString("Identifier", supports.xmArray[ii].getIdentifier(), xmXML);
            createString("ElementFormat", supports.xmArray[ii].elementFormat, xmXML);
            createDouble("TsXMA", supports.xmArray[ii].tsXMA, xmXML);
            createDouble("MaxXMBW", supports.xmArray[ii].maxXMBW, xmXML);
        }
    }
    if (!supports.addedSupportArray.empty())
    {
        for (auto it = supports.addedSupportArray.begin(); it != supports.addedSupportArray.end(); ++it)
        {
            XMLElem addedSupportArrayXML = newElement("AddedSupportArray", supportsXML);
            createString("Identifier", it->first, addedSupportArrayXML);
            createString("ElementFormat", it->second.elementFormat, addedSupportArrayXML);
            createDouble("X0", it->second.x0, addedSupportArrayXML);
            createDouble("Y0", it->second.y0, addedSupportArrayXML);
            createDouble("XSS", it->second.xSS, addedSupportArrayXML);
            createDouble("YSS", it->second.ySS, addedSupportArrayXML);
            createString("XUnits", it->second.xUnits, addedSupportArrayXML);
            createString("YUnits", it->second.yUnits, addedSupportArrayXML);
            createString("ZUnits", it->second.zUnits, addedSupportArrayXML);
            mCommon.addParameters("Parameter", getDefaultURI(), it->second.parameter, addedSupportArrayXML);
        }
    }
    return supportsXML;
}

XMLElem CRSDXMLParser::toXML(const Dwell& dwell, XMLElem parent)
{
    XMLElem dwellXML = newElement("Dwell", parent);
    createInt("NumCODTimes", dwell.cod.size(), dwellXML);

    for (size_t ii = 0; ii < dwell.cod.size(); ++ii)
    {
        XMLElem codTimeXML = newElement("CODTime", dwellXML);
        createString("Identifier", dwell.cod[ii].identifier, codTimeXML);
        mCommon.createPoly2D("CODTimePoly", dwell.cod[ii].codTimePoly, codTimeXML);
    }
    createInt("NumDwellTimes", dwell.dtime.size(), dwellXML);
    for (size_t ii = 0; ii < dwell.dtime.size(); ++ii)
    {
        XMLElem dwellTimeXML = newElement("DwellTime", dwellXML);
        createString("Identifier", dwell.dtime[ii].identifier, dwellTimeXML);
        mCommon.createPoly2D("DwellTimePoly", dwell.dtime[ii].dwellTimePoly, dwellTimeXML);
    }
    return dwellXML;
}

XMLElem CRSDXMLParser::toXML(const ReferenceGeometry& refGeo, XMLElem parent)
{
    XMLElem refGeoXML = newElement("ReferenceGeometry", parent);
    XMLElem srpXML = newElement("RefPoint", refGeoXML);
    mCommon.createVector3D("ECF", refGeo.refPoint.ecf, srpXML);
    mCommon.createVector2D("IAC", refGeo.refPoint.iac, srpXML);
    if (refGeo.sarParameters.get())
    {
        XMLElem sarXML = newElement("SARImage", refGeoXML);
        createDouble("CODTime", refGeo.sarParameters->codTime, sarXML);
        createDouble("DwellTime", refGeo.sarParameters->dwellTime, sarXML);
        createDouble("ReferenceTime", refGeo.sarParameters->referenceTime, sarXML);
        mCommon.createVector3D("ARPPos", refGeo.sarParameters->arpPos, sarXML);
        mCommon.createVector3D("ARPVel", refGeo.sarParameters->arpPos, sarXML);
        createDouble("BistaticAngle", refGeo.sarParameters->bistaticAngle, sarXML);
        createDouble("BistaticAngleRate", refGeo.sarParameters->bistaticAngleRate, sarXML);
        const auto side = refGeo.sarParameters->sideOfTrack.toString();
        createString("SideOfTrack", (side == "LEFT" ? "L" : "R"), sarXML);
        createDouble("SlantRange", refGeo.sarParameters->slantRange, sarXML);
        createDouble("GroundRange", refGeo.sarParameters->groundRange, sarXML);
        createDouble("DopplerConeAngle", refGeo.sarParameters->dopplerConeAngle, sarXML);
        createDouble("SquintAngle", refGeo.sarParameters->squintAngle, sarXML);
        createDouble("AzimuthAngle", refGeo.sarParameters->azimuthAngle, sarXML);
        createDouble("GrazeAngle", refGeo.sarParameters->grazeAngle, sarXML);
        createDouble("IncidenceAngle", refGeo.sarParameters->incidenceAngle, sarXML);
        createDouble("TwistAngle", refGeo.sarParameters->twistAngle, sarXML);
        createDouble("SlopeAngle", refGeo.sarParameters->slopeAngle, sarXML);
        createDouble("LayoverAngle", refGeo.sarParameters->layoverAngle, sarXML);
    }
    if (refGeo.txParameters.get())
    {
        XMLElem txXML = newElement("TxParameters", refGeoXML);
        createDouble("CODTime", refGeo.txParameters->time, txXML);
        mCommon.createVector3D("ARPPos", refGeo.txParameters->apcPos, txXML);
        mCommon.createVector3D("ARPVel", refGeo.txParameters->apcVel, txXML);
        const auto side = refGeo.txParameters->sideOfTrack.toString();
        createString("SideOfTrack", (side == "LEFT" ? "L" : "R"), txXML);
        createDouble("SlantRange", refGeo.txParameters->slantRange, txXML);
        createDouble("GroundRange", refGeo.txParameters->groundRange, txXML);
        createDouble("DopplerConeAngle", refGeo.txParameters->dopplerConeAngle, txXML);
        createDouble("SquintAngle", refGeo.txParameters->squintAngle, txXML);
        createDouble("AzimuthAngle", refGeo.txParameters->azimuthAngle, txXML);
        createDouble("GrazeAngle", refGeo.txParameters->grazeAngle, txXML);
        createDouble("IncidenceAngle", refGeo.txParameters->incidenceAngle, txXML);
    }
    if (refGeo.rcvParameters.get())
    {
        XMLElem rcvXML = newElement("RcvParameters", refGeoXML);
        createDouble("CODTime", refGeo.rcvParameters->time, rcvXML);
        mCommon.createVector3D("ARPPos", refGeo.rcvParameters->apcPos, rcvXML);
        mCommon.createVector3D("ARPVel", refGeo.rcvParameters->apcVel, rcvXML);
        const auto side = refGeo.rcvParameters->sideOfTrack.toString();
        createString("SideOfTrack", (side == "LEFT" ? "L" : "R"), rcvXML);
        createDouble("SlantRange", refGeo.rcvParameters->slantRange, rcvXML);
        createDouble("GroundRange", refGeo.rcvParameters->groundRange, rcvXML);
        createDouble("DopplerConeAngle", refGeo.rcvParameters->dopplerConeAngle, rcvXML);
        createDouble("SquintAngle", refGeo.rcvParameters->squintAngle, rcvXML);
        createDouble("AzimuthAngle", refGeo.rcvParameters->azimuthAngle, rcvXML);
        createDouble("GrazeAngle", refGeo.rcvParameters->grazeAngle, rcvXML);
        createDouble("IncidenceAngle", refGeo.rcvParameters->incidenceAngle, rcvXML);
    }
    return refGeoXML;
}

XMLElem CRSDXMLParser::toXML(const Antenna& antenna, XMLElem parent)
{
    XMLElem antennaXML = newElement("Antenna", parent);
    createInt("NumACFs", antenna.antCoordFrame.size(), antennaXML);
    createInt("NumAPCs", antenna.antPhaseCenter.size(), antennaXML);
    createInt("NumAntPats", antenna.antPattern.size(), antennaXML);
    for (size_t ii = 0; ii < antenna.antCoordFrame.size(); ++ii)
    {
        XMLElem antCoordFrameXML = newElement("AntCoordFrame", antennaXML);
        createString("Identifier", antenna.antCoordFrame[ii].identifier, antCoordFrameXML);
    }
    for (size_t ii = 0; ii < antenna.antPhaseCenter.size(); ++ii)
    {
        XMLElem antPhaseCenterXML = newElement("AntPhaseCenter", antennaXML);
        createString("Identifier", antenna.antPhaseCenter[ii].identifier, antPhaseCenterXML);
        createString("ACFId", antenna.antPhaseCenter[ii].acfId, antPhaseCenterXML);
        mCommon.createVector3D("APCXYZ", antenna.antPhaseCenter[ii].apcXYZ, antPhaseCenterXML);
    }
    for (size_t ii = 0; ii < antenna.antPattern.size(); ++ii)
    {
        XMLElem antPatternXML = newElement("AntPattern", antennaXML);
        createString("Identifier", antenna.antPattern[ii].identifier, antPatternXML);
        createDouble("FreqZero", antenna.antPattern[ii].freqZero, antPatternXML);
        createString("ArrayGPId", antenna.antPattern[ii].arrayGPId, antPatternXML);
        createString("ElementGPId", antenna.antPattern[ii].elementGPId, antPatternXML);
        XMLElem ebShiftXML = newElement("EBFreqShift", antPatternXML);
        createDouble("DCXSF", antenna.antPattern[ii].ebFreqShift.first, ebShiftXML);
        createDouble("DCYSF", antenna.antPattern[ii].ebFreqShift.second, ebShiftXML);
        XMLElem mlFreqDilationXML = newElement("MLFreqDilation", antPatternXML);
        createDouble("DCXSF", antenna.antPattern[ii].mlFreqDilation.first, mlFreqDilationXML);
        createDouble("DCYSF", antenna.antPattern[ii].mlFreqDilation.second, mlFreqDilationXML);
        mCommon.createPoly1D("GainBSPoly", antenna.antPattern[ii].gainBSPoly, antPatternXML);
        XMLElem AntPolRefXML = newElement("AntPolRef", antPatternXML);
        createDouble("AmpX", antenna.antPattern[ii].antPolRef.ampX, AntPolRefXML);
        createDouble("AmpY", antenna.antPattern[ii].antPolRef.ampY, AntPolRefXML);
        createDouble("PhaseX", antenna.antPattern[ii].antPolRef.phaseX, AntPolRefXML);
        createDouble("PhaseY", antenna.antPattern[ii].antPolRef.phaseY, AntPolRefXML);
    }
    return antennaXML;
}

XMLElem CRSDXMLParser::toXML(const ErrorParameters& errParams, XMLElem parent)
{
    XMLElem errParamsXML = newElement("ErrorParameters", parent);
    if (errParams.monostatic.get())
    {
        XMLElem monoXML = newElement("Monostatic", errParamsXML);
        XMLElem posVelErrXML = newElement("PosVelErr", monoXML);
        createString("Frame", errParams.monostatic->posVelErr.frame, posVelErrXML);
        createDouble("P1", errParams.monostatic->posVelErr.p1, posVelErrXML);
        createDouble("P2", errParams.monostatic->posVelErr.p2, posVelErrXML);
        createDouble("P3", errParams.monostatic->posVelErr.p3, posVelErrXML);
        createDouble("V1", errParams.monostatic->posVelErr.v1, posVelErrXML);
        createDouble("V2", errParams.monostatic->posVelErr.v2, posVelErrXML);
        createDouble("V3", errParams.monostatic->posVelErr.v3, posVelErrXML);
        if(errParams.monostatic->posVelErr.corrCoefs.get())
        {
            XMLElem corrCoefsXML = newElement("CorrCoefs", posVelErrXML);
            createDouble("P1P2", errParams.monostatic->posVelErr.corrCoefs->p1p2, corrCoefsXML);
            createDouble("P1P3", errParams.monostatic->posVelErr.corrCoefs->p1p3, corrCoefsXML);
            createDouble("P1V1", errParams.monostatic->posVelErr.corrCoefs->p1v1, corrCoefsXML);
            createDouble("P1V2", errParams.monostatic->posVelErr.corrCoefs->p1v2, corrCoefsXML);
            createDouble("P1V3", errParams.monostatic->posVelErr.corrCoefs->p1v3, corrCoefsXML);
            createDouble("P2P3", errParams.monostatic->posVelErr.corrCoefs->p2p3, corrCoefsXML);
            createDouble("P2V1", errParams.monostatic->posVelErr.corrCoefs->p2v1, corrCoefsXML);
            createDouble("P2V2", errParams.monostatic->posVelErr.corrCoefs->p2v2, corrCoefsXML);
            createDouble("P2V3", errParams.monostatic->posVelErr.corrCoefs->p2v3, corrCoefsXML);
            createDouble("P3V1", errParams.monostatic->posVelErr.corrCoefs->p3v1, corrCoefsXML);
            createDouble("P3V2", errParams.monostatic->posVelErr.corrCoefs->p3v2, corrCoefsXML);
            createDouble("P3V3", errParams.monostatic->posVelErr.corrCoefs->p3v3, corrCoefsXML);
            createDouble("V1V2", errParams.monostatic->posVelErr.corrCoefs->v1v2, corrCoefsXML);
            createDouble("V1V3", errParams.monostatic->posVelErr.corrCoefs->v1v3, corrCoefsXML);
            createDouble("V2V3", errParams.monostatic->posVelErr.corrCoefs->v2v3, corrCoefsXML);
        }
        createDecorrType("PositionDecorr", errParams.monostatic->posVelErr.positionDecorr, posVelErrXML);

        // RadarSensor
        XMLElem radarXML = newElement("RadarSensor", monoXML);
        createDouble("RangeBias", errParams.monostatic->radarSensor.rangeBias, radarXML);
        createOptionalDouble("ClockFreqSF", errParams.monostatic->radarSensor.clockFreqSF, radarXML);
        createOptionalDouble("CollectionStartTime", errParams.monostatic->radarSensor.collectionStartTime, radarXML);
        if (errParams.monostatic->radarSensor.rangeBiasDecorr.get())
        {
            XMLElem rangeBiasDecorrXML = newElement("RangeBiasDecorr", radarXML);
            createDouble("CorrCoefZero", errParams.monostatic->radarSensor.rangeBiasDecorr->corrCoefZero, rangeBiasDecorrXML);
            createDouble("DecorrRate", errParams.monostatic->radarSensor.rangeBiasDecorr->decorrRate, rangeBiasDecorrXML);
        }

        if (errParams.monostatic->tropoError.get())
        {
            XMLElem tropoXML = newElement("TropoError", monoXML);
            createOptionalDouble("TropoRangeVertical", errParams.monostatic->tropoError->tropoRangeVertical, tropoXML);
            createOptionalDouble("TropoRangeSlant", errParams.monostatic->tropoError->tropoRangeSlant, tropoXML);
            createDecorrType("TropoRangeDecorr", errParams.monostatic->tropoError->tropoRangeDecorr, tropoXML);
        }
        if (errParams.monostatic->ionoError.get())
        {
            XMLElem ionoXML = newElement("IonoError", monoXML);
            createDouble("IonoRangeVertical", errParams.monostatic->ionoError->ionoRangeVertical, ionoXML);
            createOptionalDouble("IonoRangeRateVertical", errParams.monostatic->ionoError->ionoRangeRateVertical, ionoXML);
            createOptionalDouble("IonoRgRgRateCC", errParams.monostatic->ionoError->ionoRgRgRateCC, ionoXML);
            createDecorrType("IonoRangeVertDecorr", errParams.monostatic->tropoError->tropoRangeDecorr, ionoXML);
        }
        if (!errParams.monostatic->parameter.empty())
        {
            XMLElem addedParamsXML = newElement("AddedParameters", monoXML);
            mCommon.addParameters("Parameter", getDefaultURI(), errParams.monostatic->parameter, addedParamsXML);
        }
    }
    else if (errParams.bistatic.get())
    {
        XMLElem biXML = newElement("Bistatic", errParamsXML);
        XMLElem txPlatXML = newElement("TxPlatform", biXML);
        createErrorParamPlatform("TxPlatform", errParams.bistatic->txPlatform, txPlatXML);
        XMLElem radarTxXML = newElement("RadarSensor", txPlatXML);
        createOptionalDouble("ClockFreqSF", errParams.bistatic->txPlatform.radarSensor.clockFreqSF, radarTxXML);
        createDouble("CollectionStartTime", errParams.bistatic->txPlatform.radarSensor.collectionStartTime, radarTxXML);

        XMLElem rcvPlatXML = newElement("RcvPlatform", biXML);
        createErrorParamPlatform("RcvPlatform", errParams.bistatic->rcvPlatform, rcvPlatXML);
        XMLElem radarRcvXML = newElement("RadarSensor", rcvPlatXML);
        createOptionalDouble("ClockFreqSF", errParams.bistatic->rcvPlatform.radarSensor.clockFreqSF, radarRcvXML);
        createDouble("CollectionStartTime", errParams.bistatic->rcvPlatform.radarSensor.collectionStartTime, radarRcvXML);

        if (!errParams.bistatic->parameter.empty())
        {
            XMLElem addedParamsXML = newElement("AddedParameters", biXML);
            mCommon.addParameters("Parameter",  getDefaultURI(), errParams.bistatic->parameter, addedParamsXML);
        }
    }

    return errParamsXML;
}

XMLElem CRSDXMLParser::toXML(const ProductInfo& productInfo, XMLElem parent)
{
    XMLElem productInfoXML = newElement("ProductInfo", parent);
    createString("ProductName", productInfo.productName, productInfoXML);
    createString("Classification", productInfo.classification, productInfoXML);
    createString("ReleaseInfo", productInfo.releaseInfo, productInfoXML);
    if(!six::Init::isUndefined(productInfo.countryCode))
    {
        createString("CountryCode", productInfo.countryCode, productInfoXML);
    }
    if(!six::Init::isUndefined(productInfo.profile))
    {
        createString("Profile", productInfo.profile, productInfoXML);
    }
    for (size_t ii = 0; ii < productInfo.creationInfo.size(); ++ii)
    {
        XMLElem creationInfoXML = newElement("CreationInfo", productInfoXML);
        if(!six::Init::isUndefined(productInfo.creationInfo[ii].application))
        {
            createString("Application", productInfo.creationInfo[ii].application, creationInfoXML);
        }
        createDateTime("DateTime", productInfo.creationInfo[ii].dateTime, creationInfoXML);
        if (!six::Init::isUndefined(productInfo.creationInfo[ii].site))
        {
            createString("Site", productInfo.creationInfo[ii].site, creationInfoXML);
        }
        mCommon.addParameters("Parameter", getDefaultURI(), productInfo.creationInfo[ii].parameter, creationInfoXML);
    }
    mCommon.addParameters("Parameter", getDefaultURI(), productInfo.parameter, productInfoXML);
    return productInfoXML;
}

XMLElem CRSDXMLParser::toXML(const SARInfo& sarInfo, XMLElem parent)
{
    XMLElem sarInfoXML = newElement("SARInfo", parent);
    createString("CollectType", sarInfo.collectType, sarInfoXML);
    XMLElem radarModeXML = newElement("RadarMode", sarInfoXML);
    createString("ModeType", sarInfo.radarMode, radarModeXML);
    createString("ModeID", sarInfo.radarModeID, radarModeXML);
    mCommon.addParameters("Parameter", getDefaultURI(), sarInfo.parameter, sarInfoXML);
    return sarInfoXML;
}

XMLElem CRSDXMLParser::toXML(const TransmitInfo& transmitInfo, XMLElem parent)
{
    XMLElem transmitInfoXML = newElement("TransmitInfo", parent);
    createString("SensorName", transmitInfo.sensorName, transmitInfoXML);
    createString("EventName", transmitInfo.eventName, transmitInfoXML);
    mCommon.addParameters("Parameter", getDefaultURI(), transmitInfo.parameter, transmitInfoXML);
    return transmitInfoXML;
}

XMLElem CRSDXMLParser::toXML(const GeoInfo& geoInfo, XMLElem parent)
{
    XMLElem geoInfoXML = newElement("GeoInfo", parent);

    mCommon.addParameters("Desc", geoInfo.desc, geoInfoXML);

    const size_t numLatLons = geoInfo.geometryLatLon.size();
    if (numLatLons == 1)
    {
        mCommon.createLatLon("Point", geoInfo.geometryLatLon[0], geoInfoXML);
    }
    else if (numLatLons >= 2)
    {
        XMLElem linePolyXML = newElement(numLatLons == 2 ? "Line" : "Polygon",
                                         geoInfoXML);
        setAttribute(linePolyXML, "size", numLatLons);

        for (size_t ii = 0; ii < numLatLons; ++ii)
        {
            XMLElem v = mCommon.createLatLon(numLatLons == 2 ? "Endpoint" : "Vertex",
                         geoInfo.geometryLatLon[ii], linePolyXML);
            setAttribute(v, "index", ii + 1);
        }
    }

    if (!geoInfo.name.empty())
        setAttribute(geoInfoXML, "name", geoInfo.name);

    for (size_t ii = 0; ii < geoInfo.geoInfos.size(); ++ii)
    {
        toXML(*geoInfo.geoInfos[ii], geoInfoXML);
    }

    return geoInfoXML;
}

/*
 * FROM XML
 */

std::unique_ptr<Metadata> CRSDXMLParser::fromXML(
        const xml::lite::Document* doc)
{
    
    std::unique_ptr<Metadata> crsd(new Metadata());

    const auto root = doc->getRootElement();

    // All CRSD's have these
    XMLElem productInfoXML    = getFirstAndOnly(root, "ProductInfo");
    XMLElem globalXML         = getFirstAndOnly(root, "Global");
    XMLElem sceneCoordsXML    = getFirstAndOnly(root, "SceneCoordinates");
    XMLElem dataXML           = getFirstAndOnly(root, "Data");
    XMLElem refGeoXML         = getFirstAndOnly(root, "ReferenceGeometry");
    XMLElem supportArrayXML   = getFirstAndOnly(root, "SupportArray");
    XMLElem antennaXML        = getFirstAndOnly(root, "Antenna");


    // Optional, some are required, but it depends on the type of CRSD
    XMLElem pvpXML            = getOptional(root, "PVP");
    XMLElem pppXML            = getOptional(root, "PPP");
    XMLElem dwellXML          = getOptional(root, "Dwell");
    XMLElem channelXML        = getOptional(root, "Channel");
    XMLElem txSequenceXML     = getOptional(root, "TxSequence");
    XMLElem errParamXML       = getOptional(root, "ErrorParameters");
    XMLElem receiveInfoXML    = getOptional(root, "ReceiveInfo");
    XMLElem transmitInfoXML   = getOptional(root, "TransmitInfo");
    XMLElem sarInfoXML        = getOptional(root, "SARInfo");

    std::vector<XMLElem> geoInfoXMLVec;
    root->getElementsByTagName("GeoInfo", geoInfoXMLVec);
    crsd->geoInfo.resize(geoInfoXMLVec.size());

    // Parse XML for each section (required sections here)
    fromXML(productInfoXML, crsd->productInfo);
    
    fromXML(globalXML, crsd->global);
    
    fromXML(sceneCoordsXML, crsd->sceneCoordinates);
    
    fromXML(dataXML, crsd->data);
    
    fromXML(refGeoXML, crsd->referenceGeometry);
    
    fromXML(supportArrayXML, crsd->supportArray);
    
    fromXML(antennaXML, crsd->antenna);
    
    if(pvpXML)
    {
        
        crsd->pvp.reset(new Pvp());
        fromXML(pvpXML, *(crsd->pvp));
        
    }
    if(pppXML)
    {
        
        crsd->ppp.reset(new Ppp());
        fromXML(pppXML, *(crsd->ppp));
        
    }
    if(dwellXML)
    {
        
        crsd->dwell.reset(new Dwell());
        fromXML(dwellXML, *(crsd->dwell));
        
    }
    if(channelXML)
    {
        
        crsd->channel.reset(new Channel());
        fromXML(channelXML, *(crsd->channel));
        
    }
    if(txSequenceXML)
    {
        
        crsd->txSequence.reset(new TxSequence());
        fromXML(txSequenceXML, *(crsd->txSequence));
        
    }
    if(receiveInfoXML)
    {
        
        crsd->receiveInfo.reset(new TransmitInfo());
        fromXML(receiveInfoXML, *(crsd->receiveInfo));
        
    }
    if(transmitInfoXML)
    {
        
        crsd->transmitInfo.reset(new TransmitInfo());
        fromXML(transmitInfoXML, *(crsd->transmitInfo));
        
    }
    if(sarInfoXML)
    {
        
        crsd->sarInfo.reset(new SARInfo());
        fromXML(sarInfoXML, *(crsd->sarInfo));
        
    }
    // if(errParamXML)
    // {
    //     
    //     crsd->errorParameters.reset(new ErrorParameters());
    //     fromXML(errParamXML, *(crsd->errorParameters));
    //     
    // }
    // for (size_t ii = 0; ii < geoInfoXMLVec.size(); ++ii)
    // {
    //     
    //     fromXML(geoInfoXMLVec[ii], crsd->geoInfo[ii]);
    //     
    // }

    return crsd;
}

void CRSDXMLParser::fromXML(const xml::lite::Element* productInfoXML, ProductInfo& productInfo)
{
    parseString(getFirstAndOnly(productInfoXML,"ProductName"),productInfo.productName);
    parseString(getFirstAndOnly(productInfoXML,"Classification"),productInfo.classification);
    parseString(getFirstAndOnly(productInfoXML,"ReleaseInfo"),productInfo.releaseInfo);
    parseOptionalString(productInfoXML,"CountryCode",productInfo.countryCode);
    parseOptionalString(productInfoXML,"Profile",productInfo.profile);

    // CreationInfo
    std::vector<XMLElem> creationInfoXML;
    productInfoXML->getElementsByTagName("CreationInfo", creationInfoXML);
    productInfo.creationInfo.resize(creationInfoXML.size());

    for (size_t ii = 0; ii < creationInfoXML.size(); ++ii)
    {
        XMLElem applicationXML = getOptional(creationInfoXML[ii], "Application");
        if(applicationXML)
        {
            parseString(applicationXML, productInfo.creationInfo[ii].application);
        }

        parseDateTime(getFirstAndOnly(creationInfoXML[ii], "DateTime"), productInfo.creationInfo[ii].dateTime);

        XMLElem siteXML = getOptional(creationInfoXML[ii], "Site");
        if(siteXML)
        {
            parseString(siteXML, productInfo.creationInfo[ii].site);
        }
        mCommon.parseParameters(creationInfoXML[ii], "Parameter", productInfo.creationInfo[ii].parameter);
    }
    mCommon.parseParameters(productInfoXML, "Parameter", productInfo.parameter);
}

void CRSDXMLParser::fromXML(const xml::lite::Element* sarInfoXML, SARInfo& sarInfo)
{
    sarInfo.collectType = six::toType<six::CollectType>(
                              getFirstAndOnly(sarInfoXML, "CollectType")->getCharacterData());

    XMLElem radarModeXML = getFirstAndOnly(sarInfoXML, "RadarMode");
    sarInfo.radarMode
            = six::toType<RadarModeType>(getFirstAndOnly(radarModeXML,
                                         "ModeType")->getCharacterData());

    XMLElem element = getOptional(radarModeXML, "ModeID");
    if (element)
    {
        parseString(element, sarInfo.radarModeID);
    }

    mCommon.parseParameters(sarInfoXML, "Parameter", sarInfo.parameter);
}

void CRSDXMLParser::fromXML(const xml::lite::Element* transmitInfoXML, TransmitInfo& transmitInfo)
{
    parseString(getFirstAndOnly(transmitInfoXML,"SensorName"),transmitInfo.sensorName);
    parseString(getFirstAndOnly(transmitInfoXML,"EventName"),transmitInfo.eventName);
    mCommon.parseParameters(transmitInfoXML, "Parameter", transmitInfo.parameter);
}

void CRSDXMLParser::fromXML(const xml::lite::Element* globalXML, Global& global)
{
    parseDateTime(
            getFirstAndOnly(globalXML, "CollectionRefTime"),
            global.collectionRefTime);

    // TropoParameters
    const xml::lite::Element* tropoXML = getOptional(globalXML, "TropoParameters");
    if (tropoXML)
    {
        // Optional
        global.tropoParameters.reset(new TropoParameters());
        parseDouble(getFirstAndOnly(tropoXML, "N0"), global.tropoParameters->n0);
        global.tropoParameters->refHeight =
            RefHeight::toType(getFirstAndOnly(tropoXML, "RefHeight")->getCharacterData());
        parseOptionalDouble(tropoXML,"N0ErrorStdDev",global.tropoParameters->n0ErrorStdDev);
    }

    // IonoParameters
    const xml::lite::Element* ionoXML = getOptional(globalXML, "IonoParameters");
    if (ionoXML)
    {
        // Optional
        global.ionoParameters.reset(new IonoParameters());
        parseDouble(getFirstAndOnly(ionoXML, "TECV"), global.ionoParameters->tecv);
        parseDouble(getFirstAndOnly(ionoXML, "F2Height"), global.ionoParameters->f2Height);
        parseOptionalDouble(ionoXML,"TECVErrorStdDev",global.ionoParameters->tecvErrorStdDev);
    }

    // Transmit Parameters
    const xml::lite::Element* transmitXML = getOptional(globalXML, "Transmit");
    if (transmitXML)
    {
        // Required
        global.transmitParameters.reset(new TransmitParameters());
        parseDouble(getFirstAndOnly(transmitXML, "TxTime1"), global.transmitParameters->txTime1);
        parseDouble(getFirstAndOnly(transmitXML, "TxTime2"), global.transmitParameters->txTime2);
        parseDouble(getFirstAndOnly(transmitXML, "FxMin"), global.transmitParameters->fxMin);
        parseDouble(getFirstAndOnly(transmitXML, "FxMax"), global.transmitParameters->fxMax);
    }

    // Transmit Parameters
    const xml::lite::Element* receiveXML = getOptional(globalXML, "Receive");
    if (receiveXML)
    {
        // Required
        global.receiveParameters.reset(new ReceiveParameters());
        parseDouble(getFirstAndOnly(receiveXML, "RcvStartTime1"), global.receiveParameters->rcvStartTime1);
        parseDouble(getFirstAndOnly(receiveXML, "RcvStartTime2"), global.receiveParameters->rcvStartTime2);
        parseDouble(getFirstAndOnly(receiveXML, "FrcvMin"), global.receiveParameters->fRcvMin);
        parseDouble(getFirstAndOnly(receiveXML, "FrcvMax"), global.receiveParameters->fRcvMax);
    }
}

void CRSDXMLParser::parseTxSequenceParameters(
        const xml::lite::Element* paramXML, TxSequenceParameter& param) const
{
    parseString(getFirstAndOnly(paramXML,"Identifier"),param.identifier);
    parseUInt(getFirstAndOnly(paramXML, "RefPulseIndex"), param.refPulseIndex);
    parseOptionalString(paramXML,"XMId",param.xmid);
    parseString(getFirstAndOnly(paramXML,"FxResponseId"),param.fxResponseId);
    parseBooleanType(getFirstAndOnly(paramXML, "FxBWFixed"), param.fxBWFixed);
    parseDouble(getFirstAndOnly(paramXML, "FxC"), param.fxC);
    parseDouble(getFirstAndOnly(paramXML, "FxBW"), param.fxBW);
    parseDouble(getFirstAndOnly(paramXML, "TXmtMin"), param.txmtMin);
    parseDouble(getFirstAndOnly(paramXML, "TXmtMax"), param.txmtMax);
    parseDouble(getFirstAndOnly(paramXML, "TxTime1"), param.txTime1);
    parseDouble(getFirstAndOnly(paramXML, "TxTime2"), param.txTime2);
    parseString(getFirstAndOnly(paramXML, "TxAPCId"), param.txAPCId);
    parseString(getFirstAndOnly(paramXML, "TxAPATId"), param.txAPATId);
    XMLElem txRefPointXML = getFirstAndOnly(paramXML, "TxRefPoint");
    mCommon.parseVector3D(getFirstAndOnly(txRefPointXML, "ECF"), param.txRefPoint.ecf);
    mCommon.parseVector2D(getFirstAndOnly(txRefPointXML, "IAC"), param.txRefPoint.iac);
    XMLElem txPolXML = getFirstAndOnly(paramXML, "TxPolarization");
    param.txPolarization.polarizationID = PolarizationType::toType(getFirstAndOnly(txPolXML, "PolarizationID")->getCharacterData());
    parseDouble(getFirstAndOnly(txPolXML, "AmpH"), param.txPolarization.ampH);
    parseDouble(getFirstAndOnly(txPolXML, "AmpV"), param.txPolarization.ampV);
    parseDouble(getFirstAndOnly(txPolXML, "PhaseH"), param.txPolarization.phaseH);
    parseDouble(getFirstAndOnly(txPolXML, "PhaseV"), param.txPolarization.phaseV);
    parseDouble(getFirstAndOnly(paramXML, "TxRefRadIntensity"), param.txRefRadIntensity);
    parseDouble(getFirstAndOnly(paramXML, "TxRadIntErrorStdDev"), param.txRefRadIntensityStdDev);
    parseDouble(getFirstAndOnly(paramXML, "TxRefLAtm"), param.txRefLAtm);
    mCommon.parseParameters(paramXML, "Parameter", param.addedParameters);
}

void CRSDXMLParser::fromXML(const xml::lite::Element* txSequenceXML, TxSequence& txSequence)
{
    parseString(getFirstAndOnly(txSequenceXML,"RefTxID"),txSequence.refTxId);
    txSequence.txWFType = six::toType<six::TxWFType>(getFirstAndOnly(txSequenceXML,"TxWFType")->getCharacterData());

    std::vector<XMLElem> parametersXML;
    txSequenceXML->getElementsByTagName("Parameters", parametersXML);
    txSequence.parameters.resize(parametersXML.size());
    for (size_t ii = 0; ii < parametersXML.size(); ++ii)
    {
        parseTxSequenceParameters(parametersXML[ii], txSequence.parameters[ii]);
    }
}

void CRSDXMLParser::fromXML(const xml::lite::Element* sceneCoordsXML,
                             SceneCoordinates& scene)
{
    scene.earthModel = EarthModelType::toType(
            getFirstAndOnly(sceneCoordsXML, "EarthModel")->getCharacterData());

    // IARP
    const xml::lite::Element* iarpXML = getFirstAndOnly(sceneCoordsXML, "IARP");
    mCommon.parseVector3D(getFirstAndOnly(iarpXML, "ECF"), scene.iarp.ecf);
    mCommon.parseLatLonAlt(getFirstAndOnly(iarpXML, "LLH"), scene.iarp.llh);

    // ReferenceSurface
    const xml::lite::Element* surfaceXML = getFirstAndOnly(sceneCoordsXML, "ReferenceSurface");
    const xml::lite::Element* planarXML = getOptional(surfaceXML, "Planar");
    const xml::lite::Element* haeXML = getOptional(surfaceXML, "HAE");
    if (planarXML && !haeXML)
    {
        // Choice type
        scene.referenceSurface.planar.reset(new Planar());
        mCommon.parseVector3D(getFirstAndOnly(planarXML, "uIAX"),
                              scene.referenceSurface.planar->uIax);
        mCommon.parseVector3D(getFirstAndOnly(planarXML, "uIAY"),
                              scene.referenceSurface.planar->uIay);
    }
    else if (haeXML && !planarXML)
    {
        // Choice type
        scene.referenceSurface.hae.reset(new HAE());
        mCommon.parseLatLon(getFirstAndOnly(haeXML, "uIAXLL"),
                            scene.referenceSurface.hae->uIax);
        mCommon.parseLatLon(getFirstAndOnly(haeXML, "uIAYLL"),
                            scene.referenceSurface.hae->uIay);
    }
    else
    {
        throw except::Exception(Ctxt(
                "ReferenceSurface must exactly one of Planar or HAE element"));
    }

    // ImageArea
    const xml::lite::Element* imageAreaXML = getFirstAndOnly(sceneCoordsXML, "ImageArea");
    parseAreaType(imageAreaXML, scene.imageArea);

    // ImageAreaCorners
    const xml::lite::Element* cornersXML = getFirstAndOnly(sceneCoordsXML,
                                               "ImageAreaCornerPoints");
    mCommon.parseFootprint(cornersXML, "IACP", scene.imageAreaCorners);

    // Extended Area
    const xml::lite::Element* extendedAreaXML = getOptional(sceneCoordsXML, "ExtendedArea");
    if (extendedAreaXML)
    {
        scene.extendedArea.reset(new AreaType());
        parseAreaType(extendedAreaXML, *scene.extendedArea);
    }

    // Image Grid
    const xml::lite::Element* gridXML = getOptional(sceneCoordsXML, "ImageGrid");
    if (gridXML)
    {
        // Optional
        scene.imageGrid.reset(new ImageGrid());
        const xml::lite::Element* identifierXML = getOptional(gridXML, "Identifier");
        if (identifierXML)
        {
            parseString(identifierXML, scene.imageGrid->identifier);
        }
        parseLineSample(getFirstAndOnly(gridXML, "IARPLocation"),
                        scene.imageGrid->iarpLocation);
        parseIAExtent(getFirstAndOnly(gridXML, "IAXExtent"),
                      scene.imageGrid->xExtent);
        parseIAExtent(getFirstAndOnly(gridXML, "IAYExtent"),
                      scene.imageGrid->yExtent);

        // Segment List
        const xml::lite::Element* segListXML = getOptional(gridXML, "SegmentList");
        if (segListXML)
        {
            // Optional
            size_t numSegments;
            parseUInt(getFirstAndOnly(segListXML, "NumSegments"), numSegments);
            scene.imageGrid->segments.resize(numSegments);

            std::vector<XMLElem> segmentsXML;
            segListXML->getElementsByTagName("Segment", segmentsXML);

            for (size_t ii = 0; ii < segmentsXML.size(); ++ii)
            {
                const xml::lite::Element* segmentXML = segmentsXML[ii];
                parseString(getFirstAndOnly(segmentXML, "Identifier"),
                         scene.imageGrid->segments[ii].identifier);
                parseInt(getFirstAndOnly(segmentXML, "StartLine"),
                         scene.imageGrid->segments[ii].startLine);
                parseInt(getFirstAndOnly(segmentXML, "StartSample"),
                         scene.imageGrid->segments[ii].startSample);
                parseInt(getFirstAndOnly(segmentXML, "EndLine"),
                         scene.imageGrid->segments[ii].endLine);
                parseInt(getFirstAndOnly(segmentXML, "EndSample"),
                         scene.imageGrid->segments[ii].endSample);

                const xml::lite::Element* polygonXML = getOptional(segmentXML,
                                                       "SegmentPolygon");
                if (polygonXML)
                {
                    // Optional
                    size_t polygonSize = 0;
                    sscanf(const_cast<XMLElem>(polygonXML)->attribute("size").c_str(), "%zu", &polygonSize);
                    scene.imageGrid->segments[ii].polygon.resize(polygonSize);
                    std::vector<XMLElem> polyVerticesXMLVec;
                    polygonXML->getElementsByTagName("SV", polyVerticesXMLVec);
                    if (scene.imageGrid->segments[ii].polygon.size() != polyVerticesXMLVec.size())
                    {
                        throw except::Exception(Ctxt(
                                "Incorrect polygon vertices provided"));
                    }
                    if (polyVerticesXMLVec.size() < 3)
                    {
                        throw except::Exception(Ctxt(
                                "Polygon must have at least 3 vertices"));
                    }
                    std::vector<LineSample>& vertices =
                            scene.imageGrid->segments[ii].polygon;
                    vertices.resize(polyVerticesXMLVec.size());
                    for (size_t jj = 0; jj < polyVerticesXMLVec.size(); ++jj)
                    {
                        size_t tempIdx;
                        sscanf(polyVerticesXMLVec[jj]->attribute("index").c_str(), "%zu", &tempIdx);
                        vertices[jj].setIndex(tempIdx);
                        parseLineSample(polyVerticesXMLVec[jj], vertices[jj]);
                    }
                }
            }
        }
    }
}

void CRSDXMLParser::fromXML(const xml::lite::Element* dataXML, Data& data)
{
    const xml::lite::Element* supportXML = getFirstAndOnly(dataXML, "Support");

    // Support Arrays
    std::vector<XMLElem> supportsXML;
    supportXML->getElementsByTagName("SupportArray", supportsXML);
    for (size_t ii = 0; ii < supportsXML.size(); ++ii)
    {
        std::string id;
        size_t offset;
        size_t numRows;
        size_t numCols;
        size_t numBytes;
        parseString(getFirstAndOnly(supportsXML[ii], "Identifier"), id);
        parseUInt(getFirstAndOnly(supportsXML[ii], "ArrayByteOffset"), offset);
        parseUInt(getFirstAndOnly(supportsXML[ii], "NumRows"), numRows);
        parseUInt(getFirstAndOnly(supportsXML[ii], "NumCols"), numCols);
        parseUInt(getFirstAndOnly(supportsXML[ii], "BytesPerElement"), numBytes);
        data.setSupportArray(id, numRows, numCols, numBytes, offset);
    }
    // Transmit
    XMLElem transmitXML = getOptional(dataXML, "Transmit");
    if(transmitXML)
    {
        data.transmitParameters.reset(new crsd::Data::Transmit());
        parseUInt(getFirstAndOnly(transmitXML, "NumBytesPPP"), data.transmitParameters->numBytesPPP);
        std::vector<XMLElem> txSequenceXML;
        transmitXML->getElementsByTagName("TxSequence", txSequenceXML);
        data.transmitParameters->txSequence.resize(txSequenceXML.size());
        for (size_t ii = 0; ii < txSequenceXML.size(); ++ii)
        {
            parseString(getFirstAndOnly(txSequenceXML[ii], "Identifier"), data.transmitParameters->txSequence[ii].identifier);
            parseUInt(getFirstAndOnly(txSequenceXML[ii], "NumPulses"), data.transmitParameters->txSequence[ii].numPulses);
            parseUInt(getFirstAndOnly(txSequenceXML[ii], "PPPArrayByteOffset"), data.transmitParameters->txSequence[ii].pppArrayByteOffset);
        }
    }
    // Receive
    XMLElem receiveXML = getOptional(dataXML, "Receive");
    if(receiveXML)
    {
        data.receiveParameters.reset(new crsd::Data::Receive());
        const xml::lite::Element* signalXML = getFirstAndOnly(receiveXML, "SignalArrayFormat");
        data.receiveParameters->signalArrayFormat = SignalArrayFormat::toType(signalXML->getCharacterData());
        parseUInt(getFirstAndOnly(receiveXML, "NumBytesPVP"), data.receiveParameters->numBytesPVP);
        XMLElem sigCompressXML = getOptional(receiveXML, "SignalCompression");
        if (sigCompressXML)
        {
            parseString(getFirstAndOnly(sigCompressXML, "Identifier"), data.receiveParameters->signalCompression->identifier);
            parseUInt(getFirstAndOnly(sigCompressXML, "CompressedSignalSize"), data.receiveParameters->signalCompression->compressedSignalSize); 
            std::vector<XMLElem> processXML;
            sigCompressXML->getElementsByTagName("Processing", processXML);
            data.receiveParameters->signalCompression->processing.resize(processXML.size());
            for (size_t ii = 0; ii < processXML.size(); ++ii)
            {
                parseString(getFirstAndOnly(processXML[ii], "Type"), data.receiveParameters->signalCompression->processing[ii].type);
                mCommon.parseParameters(processXML[ii], "Parameter", data.receiveParameters->signalCompression->processing[ii].parameter);
            }
        }
        std::vector<XMLElem> channelXML;
        receiveXML->getElementsByTagName("Channel", channelXML);
        data.receiveParameters->channels.resize(channelXML.size());
        for (size_t ii = 0; ii < channelXML.size(); ++ii)
        {
            parseString(getFirstAndOnly(channelXML[ii], "Identifier"), data.receiveParameters->channels[ii].identifier);
            parseUInt(getFirstAndOnly(channelXML[ii], "NumVectors"), data.receiveParameters->channels[ii].numVectors);
            parseUInt(getFirstAndOnly(channelXML[ii], "NumSamples"), data.receiveParameters->channels[ii].numSamples);
            parseUInt(getFirstAndOnly(channelXML[ii], "SignalArrayByteOffset"), data.receiveParameters->channels[ii].signalArrayByteOffset);
            parseUInt(getFirstAndOnly(channelXML[ii], "PVPArrayByteOffset"), data.receiveParameters->channels[ii].pvpArrayByteOffset);
        }
    }
}

void CRSDXMLParser::fromXML(const xml::lite::Element* channelXML, Channel& channel)
{
    parseString(getFirstAndOnly(channelXML, "RefChId"), channel.refChId);

    std::vector<XMLElem> parametersXML;
    channelXML->getElementsByTagName("Parameters", parametersXML);
    channel.parameters.resize(parametersXML.size());
    for (size_t ii = 0; ii < parametersXML.size(); ++ii)
    {
        parseChannelParameters(parametersXML[ii], channel.parameters[ii]);
    }
}

void CRSDXMLParser::fromXML(const xml::lite::Element* pvpXML, Pvp& pvp)
{
    
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "RcvStart"), pvp.rcvStart); 
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "RcvPos"), pvp.rcvPos); 
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "RcvVel"), pvp.rcvVel); 
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "FRCV1"), pvp.frcv1); 
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "FRCV2"), pvp.frcv2); 
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "RefPhi0"), pvp.refPhi0); 
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "RefFreq"), pvp.refFreq); 
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "DFIC0"), pvp.dfiC0); 
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "FICRate"), pvp.ficRate); 
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "RcvACX"), pvp.rcvACX); 
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "RcvACY"), pvp.rcvACY); 
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "RcvEB"), pvp.rcvEB); 
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "SIGNAL"), pvp.signal); 
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "AmpSF"), pvp.ampSF);
    parsePVPType(pvp, getFirstAndOnly(pvpXML, "DGRGC"), pvp.dgrgc); 
    parseOptionalPVPType(pvpXML, "TxPulseIndex", pvp, pvp.txPulseIndex);

    std::vector<XMLElem> addedParamsXML;
    const std::string str = "AddedPVP";
    pvpXML->getElementsByTagName(str, addedParamsXML);
    if(addedParamsXML.empty())
    {
        return;
    }
    for (size_t ii = 0; ii < addedParamsXML.size(); ++ii)
    {
        parsePVPType(pvp, addedParamsXML[ii]);
    }
}

void CRSDXMLParser::fromXML(const xml::lite::Element* pppXML, Ppp& ppp)
{
    parsePPPType(ppp, getFirstAndOnly(pppXML, "TxTime"), ppp.txTime);
    parsePPPType(ppp, getFirstAndOnly(pppXML, "TxPos"), ppp.txPos);
    parsePPPType(ppp, getFirstAndOnly(pppXML, "TxVel"), ppp.txVel);
    parsePPPType(ppp, getFirstAndOnly(pppXML, "FX1"), ppp.fx1);
    parsePPPType(ppp, getFirstAndOnly(pppXML, "FX2"), ppp.fx2);
    parsePPPType(ppp, getFirstAndOnly(pppXML, "TXmt"), ppp.txmt);
    parsePPPType(ppp, getFirstAndOnly(pppXML, "PhiX0"), ppp.phiX0);
    parsePPPType(ppp, getFirstAndOnly(pppXML, "FxFreq0"), ppp.fxFreq0);
    parsePPPType(ppp, getFirstAndOnly(pppXML, "FxRate"), ppp.fxRate);
    parsePPPType(ppp, getFirstAndOnly(pppXML, "TxRadInt"), ppp.txRadInt);
    parsePPPType(ppp, getFirstAndOnly(pppXML, "TxACX"), ppp.txACX);
    parsePPPType(ppp, getFirstAndOnly(pppXML, "TxACY"), ppp.txACY);
    parsePPPType(ppp, getFirstAndOnly(pppXML, "TxEB"), ppp.txEB);
    parsePPPType(ppp, getFirstAndOnly(pppXML, "FxResponseIndex"), ppp.fxResponseIndex);
    parseOptionalPPPType(pppXML, "XMIndex", ppp, ppp.xmIndex);

    std::vector<XMLElem> addedParamsXML;
    const std::string str = "AddedPPP";
    pppXML->getElementsByTagName(str, addedParamsXML);
    if(addedParamsXML.empty())
    {
        return;
    }
    for (size_t ii = 0; ii < addedParamsXML.size(); ++ii)
    {
        parsePPPType(ppp, addedParamsXML[ii]);
    }
}

void CRSDXMLParser::fromXML(const xml::lite::Element* DwellXML,
                             Dwell& dwell)
{
    // CODTime
    size_t numCODTimes;
    parseUInt(getFirstAndOnly(DwellXML, "NumCODTimes"), numCODTimes);
    dwell.cod.resize(numCODTimes);

    std::vector<XMLElem> codXMLVec;
    DwellXML->getElementsByTagName("CODTime", codXMLVec);
    for(size_t ii = 0; ii < codXMLVec.size(); ++ii)
    {
        parseString(getFirstAndOnly(codXMLVec[ii], "Identifier"), dwell.cod[ii].identifier);
        mCommon.parsePoly2D(getFirstAndOnly(codXMLVec[ii], "CODTimePoly"), dwell.cod[ii].codTimePoly);
    }

    // DwellTime
    size_t numDwellTimes = 0;
    parseUInt(getFirstAndOnly(DwellXML, "NumDwellTimes"), numDwellTimes);
    dwell.dtime.resize(numDwellTimes);

    std::vector<XMLElem> dtimeXMLVec;
    DwellXML->getElementsByTagName("DwellTime", dtimeXMLVec);
    for(size_t ii = 0; ii < dtimeXMLVec.size(); ++ii)
    {
        parseString(getFirstAndOnly(dtimeXMLVec[ii], "Identifier"), dwell.dtime[ii].identifier);
        mCommon.parsePoly2D(getFirstAndOnly(dtimeXMLVec[ii], "DwellTimePoly"), dwell.dtime[ii].dwellTimePoly);
    }
}

void CRSDXMLParser::fromXML(const xml::lite::Element* refGeoXML, ReferenceGeometry& refGeo)
{
    XMLElem srpXML = getFirstAndOnly(refGeoXML, "RefPoint");
    mCommon.parseVector3D(getFirstAndOnly(srpXML, "ECF"), refGeo.refPoint.ecf);
    
    mCommon.parseVector2D(getFirstAndOnly(srpXML, "IAC"), refGeo.refPoint.iac);
    
    XMLElem sarXML = getOptional(refGeoXML, "SARImage");
    if (sarXML)
    {
        refGeo.sarParameters.reset(new crsd::SARImage());
        parseDouble(getFirstAndOnly(sarXML, "ReferenceTime"), refGeo.sarParameters->referenceTime);
        parseDouble(getFirstAndOnly(sarXML, "CODTime"), refGeo.sarParameters->codTime);
        parseDouble(getFirstAndOnly(sarXML, "DwellTime"), refGeo.sarParameters->dwellTime);
        mCommon.parseVector3D(getFirstAndOnly(sarXML, "ARPPos"), refGeo.sarParameters->arpPos);
        mCommon.parseVector3D(getFirstAndOnly(sarXML, "ARPVel"), refGeo.sarParameters->arpVel);
        parseDouble(getFirstAndOnly(sarXML, "BistaticAngle"), refGeo.sarParameters->bistaticAngle);
        parseDouble(getFirstAndOnly(sarXML, "BistaticAngleRate"), refGeo.sarParameters->bistaticAngleRate);
        std::string side = "";
        parseString(getFirstAndOnly(sarXML, "SideOfTrack"), side);
        refGeo.sarParameters->sideOfTrack = (side == "L" ? six::SideOfTrackType::LEFT : six::SideOfTrackType::RIGHT);
        parseDouble(getFirstAndOnly(sarXML, "SlantRange"), refGeo.sarParameters->slantRange);
        parseDouble(getFirstAndOnly(sarXML, "GroundRange"), refGeo.sarParameters->groundRange);
        parseDouble(getFirstAndOnly(sarXML, "DopplerConeAngle"), refGeo.sarParameters->dopplerConeAngle);
        parseDouble(getFirstAndOnly(sarXML, "SquintAngle"), refGeo.sarParameters->squintAngle);
        parseDouble(getFirstAndOnly(sarXML, "AzimuthAngle"), refGeo.sarParameters->azimuthAngle);
        parseDouble(getFirstAndOnly(sarXML, "GrazeAngle"), refGeo.sarParameters->grazeAngle);
        parseDouble(getFirstAndOnly(sarXML, "IncidenceAngle"), refGeo.sarParameters->incidenceAngle);
        parseDouble(getFirstAndOnly(sarXML, "TwistAngle"), refGeo.sarParameters->twistAngle);
        parseDouble(getFirstAndOnly(sarXML, "SlopeAngle"), refGeo.sarParameters->slopeAngle);
        parseDouble(getFirstAndOnly(sarXML, "LayoverAngle"), refGeo.sarParameters->layoverAngle);
    }
    XMLElem txXML = getOptional(refGeoXML, "TxParameters");
    if (txXML)
    {
        refGeo.txParameters.reset(new crsd::OneWayParams());
        parseDouble(getFirstAndOnly(txXML, "Time"), refGeo.txParameters->time);
        mCommon.parseVector3D(getFirstAndOnly(txXML, "APCPos"), refGeo.txParameters->apcPos);
        mCommon.parseVector3D(getFirstAndOnly(txXML, "APCVel"), refGeo.txParameters->apcVel);
        std::string side = "";
        parseString(getFirstAndOnly(txXML, "SideOfTrack"), side);
        refGeo.txParameters->sideOfTrack = (side == "L" ? six::SideOfTrackType::LEFT : six::SideOfTrackType::RIGHT);
        parseDouble(getFirstAndOnly(txXML, "SlantRange"), refGeo.txParameters->slantRange);
        parseDouble(getFirstAndOnly(txXML, "GroundRange"), refGeo.txParameters->groundRange);
        parseDouble(getFirstAndOnly(txXML, "DopplerConeAngle"), refGeo.txParameters->dopplerConeAngle);
        parseDouble(getFirstAndOnly(txXML, "SquintAngle"), refGeo.txParameters->squintAngle);
        parseDouble(getFirstAndOnly(txXML, "AzimuthAngle"), refGeo.txParameters->azimuthAngle);
        parseDouble(getFirstAndOnly(txXML, "GrazeAngle"), refGeo.txParameters->grazeAngle);
        parseDouble(getFirstAndOnly(txXML, "IncidenceAngle"), refGeo.txParameters->incidenceAngle);
    }
    XMLElem rcvXML = getOptional(refGeoXML, "RcvParameters");
    if (rcvXML)
    {
        refGeo.rcvParameters.reset(new crsd::OneWayParams());
        parseDouble(getFirstAndOnly(rcvXML, "Time"), refGeo.rcvParameters->time);
        mCommon.parseVector3D(getFirstAndOnly(rcvXML, "APCPos"), refGeo.rcvParameters->apcPos);
        mCommon.parseVector3D(getFirstAndOnly(rcvXML, "APCVel"), refGeo.rcvParameters->apcVel);
        std::string side = "";
        parseString(getFirstAndOnly(rcvXML, "SideOfTrack"), side);
        refGeo.rcvParameters->sideOfTrack = (side == "L" ? six::SideOfTrackType::LEFT : six::SideOfTrackType::RIGHT);
        parseDouble(getFirstAndOnly(rcvXML, "SlantRange"), refGeo.rcvParameters->slantRange);
        parseDouble(getFirstAndOnly(rcvXML, "GroundRange"), refGeo.rcvParameters->groundRange);
        parseDouble(getFirstAndOnly(rcvXML, "DopplerConeAngle"), refGeo.rcvParameters->dopplerConeAngle);
        parseDouble(getFirstAndOnly(rcvXML, "SquintAngle"), refGeo.rcvParameters->squintAngle);
        parseDouble(getFirstAndOnly(rcvXML, "AzimuthAngle"), refGeo.rcvParameters->azimuthAngle);
        parseDouble(getFirstAndOnly(rcvXML, "GrazeAngle"), refGeo.rcvParameters->grazeAngle);
        parseDouble(getFirstAndOnly(rcvXML, "IncidenceAngle"), refGeo.rcvParameters->incidenceAngle);
    }
}

void CRSDXMLParser::fromXML(const xml::lite::Element* supportArrayXML, SupportArray& supportArray)
{
    std::vector<XMLElem> iazArrayXMLVec;
    supportArrayXML->getElementsByTagName("IAZArray", iazArrayXMLVec);
    supportArray.iazArray.resize(iazArrayXMLVec.size());
    for (size_t ii = 0; ii < iazArrayXMLVec.size(); ++ii)
    {
        parseSupportArrayParameter(iazArrayXMLVec[ii], supportArray.iazArray[ii], false);
    }

    std::vector<XMLElem> fxResponseArrayXMLVec;
    supportArrayXML->getElementsByTagName("FxResponseArray", fxResponseArrayXMLVec);
    supportArray.fxResponseArray.resize(fxResponseArrayXMLVec.size());
    for (size_t ii = 0; ii < fxResponseArrayXMLVec.size(); ++ii)
    {
        std::string identifierVal;
        parseString(getFirstAndOnly(fxResponseArrayXMLVec[ii], "Identifier"), identifierVal);
        supportArray.fxResponseArray[ii].setIdentifier(identifierVal);
        parseString(getFirstAndOnly(fxResponseArrayXMLVec[ii], "ElementFormat"), supportArray.fxResponseArray[ii].elementFormat);
        parseDouble(getFirstAndOnly(fxResponseArrayXMLVec[ii], "Fx0FXR"), supportArray.fxResponseArray[ii].fx0FXR);
        parseDouble(getFirstAndOnly(fxResponseArrayXMLVec[ii], "FxSSFXR"), supportArray.fxResponseArray[ii].fxSSFXR);
    }

    std::vector<XMLElem> antGainPhaseXMLVec;
    supportArrayXML->getElementsByTagName("AntGainPhase", antGainPhaseXMLVec);
    supportArray.antGainPhase.resize(antGainPhaseXMLVec.size());
    for (size_t ii = 0; ii < antGainPhaseXMLVec.size(); ++ii)
    {
        parseSupportArrayParameter(antGainPhaseXMLVec[ii], supportArray.antGainPhase[ii], false);
    }

    std::vector<XMLElem> xmXMLVec;
    supportArrayXML->getElementsByTagName("XMArray", xmXMLVec);
    supportArray.xmArray.resize(xmXMLVec.size());
    for (size_t ii = 0; ii < xmXMLVec.size(); ++ii)
    {
        std::string identifierVal;
        parseString(getFirstAndOnly(xmXMLVec[ii], "Identifier"), identifierVal);
        supportArray.xmArray[ii].setIdentifier(identifierVal);
        parseString(getFirstAndOnly(xmXMLVec[ii], "ElementFormat"), supportArray.xmArray[ii].elementFormat);
        parseDouble(getFirstAndOnly(xmXMLVec[ii], "TsXMA"), supportArray.xmArray[ii].tsXMA);
        parseDouble(getFirstAndOnly(xmXMLVec[ii], "MaxXMBW"), supportArray.xmArray[ii].maxXMBW);
    }

    std::vector<XMLElem> dwellXMLVec;
    supportArrayXML->getElementsByTagName("DwellTimeArray", dwellXMLVec);
    supportArray.dwellTimeArray.resize(dwellXMLVec.size());
    for (size_t ii = 0; ii < dwellXMLVec.size(); ++ii)
    {
        parseSupportArrayParameter(dwellXMLVec[ii], supportArray.dwellTimeArray[ii], false);
    }

    std::vector<XMLElem> addedSupportArrayXMLVec;
    supportArrayXML->getElementsByTagName("AddedSupportArray", addedSupportArrayXMLVec);
    for (size_t ii = 0; ii < addedSupportArrayXMLVec.size(); ++ii)
    {
        std::string id;
        parseString(getFirstAndOnly(addedSupportArrayXMLVec[ii], "Identifier"), id);
        if (supportArray.addedSupportArray.count(id) == 0)
        {
            parseSupportArrayParameter(addedSupportArrayXMLVec[ii], supportArray.addedSupportArray[id], true);
            parseString(getFirstAndOnly(addedSupportArrayXMLVec[ii], "XUnits"), supportArray.addedSupportArray[id].xUnits);
            parseString(getFirstAndOnly(addedSupportArrayXMLVec[ii], "YUnits"), supportArray.addedSupportArray[id].yUnits);
            parseString(getFirstAndOnly(addedSupportArrayXMLVec[ii], "ZUnits"), supportArray.addedSupportArray[id].zUnits);
            mCommon.parseParameters(addedSupportArrayXMLVec[ii], "Parameter", supportArray.addedSupportArray[id].parameter);
        }
        else
        {
            throw except::Exception(Ctxt(
                    "Support array identifier for support array is not unique: " + id));
        }
    }
}


void CRSDXMLParser::fromXML(const xml::lite::Element* antennaXML, Antenna& antenna)
{
    size_t numACFs = 0;
    size_t numAPCs = 0;
    size_t numAntPats = 0;
    parseUInt(getFirstAndOnly(antennaXML, "NumACFs"), numACFs);
    antenna.antCoordFrame.resize(numACFs);
    parseUInt(getFirstAndOnly(antennaXML, "NumAPCs"), numAPCs);
    antenna.antPhaseCenter.resize(numAPCs);
    parseUInt(getFirstAndOnly(antennaXML, "NumAntPats"), numAntPats);
    antenna.antPattern.resize(numAntPats);

    // Parse AntCoordFrame
    std::vector<XMLElem> antCoordFrameXMLVec;
    antennaXML->getElementsByTagName("AntCoordFrame", antCoordFrameXMLVec);
    if(antenna.antCoordFrame.size() != antCoordFrameXMLVec.size())
    {
        throw except::Exception(Ctxt(
                "Incorrect number of AntCoordFrames provided"));
    }
    antenna.antCoordFrame.resize(antCoordFrameXMLVec.size());
    for( size_t ii = 0; ii < antCoordFrameXMLVec.size(); ++ii)
    {
        parseString(getFirstAndOnly(antCoordFrameXMLVec[ii], "Identifier"), antenna.antCoordFrame[ii].identifier);
    }

    // Parse AntPhaseCenter
    std::vector<XMLElem> antPhaseCenterXMLVec;
    antennaXML->getElementsByTagName("AntPhaseCenter", antPhaseCenterXMLVec);
    if(antenna.antPhaseCenter.size() != antPhaseCenterXMLVec.size())
    {
        throw except::Exception(Ctxt(
                "Incorrect number of AntPhaseCenters provided"));
    }
    antenna.antPhaseCenter.resize(antPhaseCenterXMLVec.size());
    for( size_t ii = 0; ii < antPhaseCenterXMLVec.size(); ++ii)
    {
        parseString(getFirstAndOnly(antPhaseCenterXMLVec[ii], "Identifier"), antenna.antPhaseCenter[ii].identifier);
        parseString(getFirstAndOnly(antPhaseCenterXMLVec[ii], "ACFId"), antenna.antPhaseCenter[ii].acfId);
        mCommon.parseVector3D(getFirstAndOnly(antPhaseCenterXMLVec[ii], "APCXYZ"), antenna.antPhaseCenter[ii].apcXYZ);
    }

    std::vector<XMLElem> antPatternXMLVec;
    antennaXML->getElementsByTagName("AntPattern", antPatternXMLVec);
    if(antenna.antPattern.size() != antPatternXMLVec.size())
    {
        throw except::Exception(Ctxt(
                "Incorrect number of AntPatterns provided"));
    }
    antenna.antPattern.resize(antPatternXMLVec.size());
    for( size_t ii = 0; ii < antPatternXMLVec.size(); ++ii)
    {
        parseString(getFirstAndOnly(antPatternXMLVec[ii], "Identifier"), antenna.antPattern[ii].identifier);
        parseDouble(getFirstAndOnly(antPatternXMLVec[ii], "FreqZero"), antenna.antPattern[ii].freqZero);
        parseString(getFirstAndOnly(antPatternXMLVec[ii], "ArrayGPId"), antenna.antPattern[ii].arrayGPId);
        parseString(getFirstAndOnly(antPatternXMLVec[ii], "ElementGPId"), antenna.antPattern[ii].elementGPId);
        XMLElem ebFreqShiftXML = getFirstAndOnly(antPatternXMLVec[ii], "EBFreqShift");
        parseDouble(getFirstAndOnly(ebFreqShiftXML, "DCXSF"), antenna.antPattern[ii].ebFreqShift.first);
        parseDouble(getFirstAndOnly(ebFreqShiftXML, "DCYSF"), antenna.antPattern[ii].ebFreqShift.second);
        XMLElem mlFreqDilationXML = getOptional(antPatternXMLVec[ii], "MLFreqDilation");
        parseDouble(getFirstAndOnly(mlFreqDilationXML, "DCXSF"), antenna.antPattern[ii].mlFreqDilation.first);
        parseDouble(getFirstAndOnly(mlFreqDilationXML, "DCYSF"), antenna.antPattern[ii].mlFreqDilation.second);
        XMLElem gainBSPoly = getFirstAndOnly(antPatternXMLVec[ii], "GainBSPoly");
        mCommon.parsePoly1D(gainBSPoly, antenna.antPattern[ii].gainBSPoly);

        // Parse EB
        XMLElem antPolRefXML = getFirstAndOnly(antPatternXMLVec[ii], "AntPolRef");
        parseDouble(getFirstAndOnly(antPolRefXML, "AmpX"), antenna.antPattern[ii].antPolRef.ampX);
        parseDouble(getFirstAndOnly(antPolRefXML, "AmpY"), antenna.antPattern[ii].antPolRef.ampY);
        parseDouble(getFirstAndOnly(antPolRefXML, "PhaseX"), antenna.antPattern[ii].antPolRef.phaseX);
        parseDouble(getFirstAndOnly(antPolRefXML, "PhaseY"), antenna.antPattern[ii].antPolRef.phaseY);
    }
}

void CRSDXMLParser::fromXML(const xml::lite::Element* errParamXML, ErrorParameters& errParam)
{
    XMLElem monostaticXML = getOptional(errParamXML, "Monostatic");
    XMLElem bistaticXML = getOptional(errParamXML, "Bistatic");

    if(monostaticXML && !bistaticXML)
    {
        errParam.monostatic.reset(new ErrorParameters::Monostatic());
        parsePosVelErr(getFirstAndOnly(monostaticXML, "PosVelErr"), errParam.monostatic->posVelErr);

        XMLElem radarSensorXML = getFirstAndOnly(monostaticXML, "RadarSensor");
        parseDouble(getFirstAndOnly(radarSensorXML, "RangeBias"), errParam.monostatic->radarSensor.rangeBias);

        parseOptionalDouble(radarSensorXML, "ClockFreqSF", errParam.monostatic->radarSensor.clockFreqSF);
        parseOptionalDouble(radarSensorXML, "CollectionStartTime", errParam.monostatic->radarSensor.collectionStartTime);

        XMLElem rangeBiasDecorrXML = getOptional(radarSensorXML, "RangeBiasDecorr");
        if(rangeBiasDecorrXML)
        {
            errParam.monostatic->radarSensor.rangeBiasDecorr.reset(new six::DecorrType());
            mCommon.parseDecorrType(rangeBiasDecorrXML, *(errParam.monostatic->radarSensor.rangeBiasDecorr));
        }

        XMLElem tropoErrorXML = getFirstAndOnly(monostaticXML, "TropoError");
        if(tropoErrorXML)
        {
            errParam.monostatic->tropoError.reset(new six::TropoError());
            parseOptionalDouble(tropoErrorXML, "TropoRangeVertical", errParam.monostatic->tropoError->tropoRangeVertical);
            parseOptionalDouble(tropoErrorXML, "TropoRangeSlant", errParam.monostatic->tropoError->tropoRangeSlant);
            mCommon.parseOptionalDecorrType(tropoErrorXML, "TropoRangeDecorr", errParam.monostatic->tropoError->tropoRangeDecorr);
        }

        XMLElem ionoErrorXML = getFirstAndOnly(monostaticXML, "IonoError");
        if(ionoErrorXML)
        {
            errParam.monostatic->ionoError.reset(new six::IonoError());
            parseDouble(getFirstAndOnly(ionoErrorXML, "IonoRangeVertical"), errParam.monostatic->ionoError->ionoRangeVertical);

            parseOptionalDouble(ionoErrorXML, "IonoRangeRateVertical", errParam.monostatic->ionoError->ionoRangeRateVertical);
            parseOptionalDouble(ionoErrorXML, "IonoRgRgRateCC", errParam.monostatic->ionoError->ionoRgRgRateCC);
            mCommon.parseOptionalDecorrType(ionoErrorXML, "IonoRangeVertDecorr", errParam.monostatic->ionoError->ionoRangeVertDecorr);
        }
        mCommon.parseParameters(monostaticXML, "Parameter", errParam.monostatic->parameter);
    }
    else if(!monostaticXML && bistaticXML)
    {
        errParam.bistatic.reset(new ErrorParameters::Bistatic());
        parsePlatform(getFirstAndOnly(bistaticXML, "TxPlatform"), errParam.bistatic->txPlatform);
        parsePlatform(getFirstAndOnly(bistaticXML, "RcvPlatform"), errParam.bistatic->rcvPlatform);
        mCommon.parseParameters(bistaticXML, "Parameter", errParam.bistatic->parameter);
    }
    else
    {
        throw except::Exception(Ctxt(
                "Must be one of monostatic or bistatic"));
    }
}

void CRSDXMLParser::fromXML(const xml::lite::Element* geoInfoXML, GeoInfo& geoInfo)
{
    std::vector < XMLElem > geoInfosXML;
    geoInfoXML->getElementsByTagName("GeoInfo", geoInfosXML);
    geoInfo.name = geoInfoXML->getAttributes().getValue("name");

    //optional
    mCommon.parseParameters(geoInfoXML, "Desc", geoInfo.desc);

    XMLElem tmpElem = getOptional(geoInfoXML, "Point");
    if (tmpElem)
    {
        LatLon ll;
        mCommon.parseLatLon(tmpElem, ll);
        geoInfo.geometryLatLon.push_back(ll);
    }
    else
    {
        std::string pointName = "Endpoint";
        tmpElem = getOptional(geoInfoXML, "Line");
        if (!tmpElem)
        {
            pointName = "Vertex";
            tmpElem = getOptional(geoInfoXML, "Polygon");
        }
        if (tmpElem)
        {
            mCommon.parseLatLons(tmpElem, pointName, geoInfo.geometryLatLon);
        }
    }

    //optional
    size_t idx(geoInfo.geoInfos.size());
    geoInfo.geoInfos.resize(idx + geoInfosXML.size());

    for (auto it = geoInfosXML.begin(); it
            != geoInfosXML.end(); ++it, ++idx)
    {
        geoInfo.geoInfos[idx].reset(new GeoInfo());
        fromXML(*it, *geoInfo.geoInfos[idx]);
    }
}

XMLElem CRSDXMLParser::createLatLonFootprint(const std::string& name,
                                              const std::string& cornerName,
                                              const crsd::LatLonCorners& corners,
                                              XMLElem parent) const
{
    XMLElem footprint = newElement(name, parent);

    // Write the corners in CW order
    XMLElem vertex =
        mCommon.createLatLon(cornerName, corners.upperLeft, footprint);
    setAttribute(vertex, "index", "1");

    vertex = mCommon.createLatLon(cornerName, corners.upperRight, footprint);
    setAttribute(vertex, "index", "2");

    vertex = mCommon.createLatLon(cornerName, corners.lowerRight, footprint);
    setAttribute(vertex, "index", "3");

    vertex = mCommon.createLatLon(cornerName, corners.lowerLeft, footprint);
    setAttribute(vertex, "index", "4");

    return footprint;
}

XMLElem CRSDXMLParser::createPVPType(const std::string& name,
                                      const PVPType& p,
                                      XMLElem parent) const
{
    XMLElem pvpXML = newElement(name, parent);
    createInt("Offset", p.getOffset(), pvpXML);
    createInt("Size", p.getSize(), pvpXML);
    createString("Format", p.getFormat(), pvpXML);
    return pvpXML;
}

XMLElem CRSDXMLParser::createPPPType(const std::string& name,
                                      const PPPType& p,
                                      XMLElem parent) const
{
    XMLElem pppXML = newElement(name, parent);
    createInt("Offset", p.getOffset(), pppXML);
    createInt("Size", p.getSize(), pppXML);
    createString("Format", p.getFormat(), pppXML);
    return pppXML;
}

XMLElem CRSDXMLParser::createAPVPType(const std::string& name,
                                       const APVPType& p,
                                       XMLElem parent) const
{
    XMLElem apvpXML = newElement(name, parent);
    createString("Name", p.getName(), apvpXML);
    createInt("Offset", p.getOffset(), apvpXML);
    createInt("Size", p.getSize(), apvpXML);
    createString("Format", p.getFormat(), apvpXML);
    return apvpXML;
}

XMLElem CRSDXMLParser::createAPPPType(const std::string& name,
                                       const APPPType& p,
                                       XMLElem parent) const
{
    XMLElem apppXML = newElement(name, parent);
    createString("Name", p.getName(), apppXML);
    createInt("Offset", p.getOffset(), apppXML);
    createInt("Size", p.getSize(), apppXML);
    createString("Format", p.getFormat(), apppXML);
    return apppXML;
}

XMLElem CRSDXMLParser::createErrorParamPlatform(
        const std::string& /*name*/,
        const ErrorParameters::Bistatic::Platform p,
        XMLElem parent) const
{
    XMLElem posVelErrXML = newElement("PosVelErr", parent);
    createString("Frame", p.posVelErr.frame, posVelErrXML);
    createDouble("P1", p.posVelErr.p1, posVelErrXML);
    createDouble("P2", p.posVelErr.p2, posVelErrXML);
    createDouble("P3", p.posVelErr.p3, posVelErrXML);
    createDouble("V1", p.posVelErr.v1, posVelErrXML);
    createDouble("V2", p.posVelErr.v2, posVelErrXML);
    createDouble("V3", p.posVelErr.v3, posVelErrXML);
    if(p.posVelErr.corrCoefs.get())
    {
        XMLElem corrCoefsXML = newElement("CorrCoefs", posVelErrXML);
        createDouble("P1P2", p.posVelErr.corrCoefs->p1p2, corrCoefsXML);
        createDouble("P1P3", p.posVelErr.corrCoefs->p1p3, corrCoefsXML);
        createDouble("P1V1", p.posVelErr.corrCoefs->p1v1, corrCoefsXML);
        createDouble("P1V2", p.posVelErr.corrCoefs->p1v2, corrCoefsXML);
        createDouble("P1V3", p.posVelErr.corrCoefs->p1v3, corrCoefsXML);
        createDouble("P2P3", p.posVelErr.corrCoefs->p2p3, corrCoefsXML);
        createDouble("P2V1", p.posVelErr.corrCoefs->p2v1, corrCoefsXML);
        createDouble("P2V2", p.posVelErr.corrCoefs->p2v2, corrCoefsXML);
        createDouble("P2V3", p.posVelErr.corrCoefs->p2v3, corrCoefsXML);
        createDouble("P3V1", p.posVelErr.corrCoefs->p3v1, corrCoefsXML);
        createDouble("P3V2", p.posVelErr.corrCoefs->p3v2, corrCoefsXML);
        createDouble("P3V3", p.posVelErr.corrCoefs->p3v3, corrCoefsXML);
        createDouble("V1V2", p.posVelErr.corrCoefs->v1v2, corrCoefsXML);
        createDouble("V1V3", p.posVelErr.corrCoefs->v1v3, corrCoefsXML);
        createDouble("V2V3", p.posVelErr.corrCoefs->v2v3, corrCoefsXML);
    }
    createDecorrType("PositionDecorr", p.posVelErr.positionDecorr, posVelErrXML);
    return posVelErrXML;
}

XMLElem CRSDXMLParser::createDecorrType(const std::string& name, const six::DecorrType& dt,
    XMLElem parent) const
{
    if (six::Init::isDefined(dt))
    {
        XMLElem element = newElement(name, parent);
        createDouble("CorrCoefZero", dt.corrCoefZero, element);
        createDouble("DecorrRate", dt.decorrRate, element);
        return element;
    }
    return nullptr;
}
XMLElem CRSDXMLParser::createDecorrType(const std::string& name, const std::optional<six::DecorrType>& dt,
    XMLElem parent) const
{
    if (dt.has_value())
    {
        return createDecorrType(name, *dt, parent);
    }
    return nullptr;
}

/*
 * Parser helper functions
 */
void CRSDXMLParser::parseAreaType(const xml::lite::Element* areaXML, AreaType& area) const
{
    mCommon.parseVector2D(getFirstAndOnly(areaXML, "X1Y1"), area.x1y1);
    mCommon.parseVector2D(getFirstAndOnly(areaXML, "X2Y2"), area.x2y2);
    const xml::lite::Element* polygonXML = getOptional(areaXML, "Polygon");
    if (polygonXML)
    {
        std::vector<XMLElem> verticesXML;
        polygonXML->getElementsByTagName("Vertex", verticesXML);
        if (verticesXML.size() < 3)
        {
            throw except::Exception(Ctxt(
                    "Polygons must have at least 3 sides"));
        }
        area.polygon.resize(verticesXML.size());
        for (size_t ii = 0; ii < area.polygon.size(); ++ii)
        {
            Vector2& vertex = area.polygon[ii];
            const xml::lite::Element* vertexXML = verticesXML[ii];
            mCommon.parseVector2D(vertexXML, vertex);
        }
    }
}

void CRSDXMLParser::parseLineSample(const xml::lite::Element* lsXML, LineSample& ls) const
{
    parseDouble(getFirstAndOnly(lsXML, "Line"), ls.line);
    parseDouble(getFirstAndOnly(lsXML, "Sample"), ls.sample);
}

void CRSDXMLParser::parseIAExtent(const xml::lite::Element* extentXML,
                                   ImageAreaXExtent& extent) const
{
    parseDouble(getFirstAndOnly(extentXML, "LineSpacing"),
                extent.lineSpacing);
    parseInt(getFirstAndOnly(extentXML, "FirstLine"),
             extent.firstLine);
    parseUInt(getFirstAndOnly(extentXML, "NumLines"),
              extent.numLines);
}

void CRSDXMLParser::parseIAExtent(const xml::lite::Element* extentXML,
                                   ImageAreaYExtent& extent) const
{
    parseDouble(getFirstAndOnly(extentXML, "SampleSpacing"),
                extent.sampleSpacing);
    parseInt(getFirstAndOnly(extentXML, "FirstSample"),
             extent.firstSample);
    parseUInt(getFirstAndOnly(extentXML, "NumSamples"),
              extent.numSamples);
}

void CRSDXMLParser::parseChannelParameters(
        const xml::lite::Element* paramXML, ChannelParameter& param) const
{
    parseString(getFirstAndOnly(paramXML, "Identifier"), param.identifier);
    parseUInt(getFirstAndOnly(paramXML, "RefVectorIndex"), param.refVectorIndex);
    parseBooleanType(getFirstAndOnly(paramXML, "RefFreqFixed"), param.refFreqFixed);
    parseBooleanType(getFirstAndOnly(paramXML, "FrcvFixed"), param.fRcvFixed);
    parseBooleanType(getFirstAndOnly(paramXML, "SignalNormal"), param.signalNormal);
    parseDouble(getFirstAndOnly(paramXML, "F0Ref"), param.f0Ref);
    parseDouble(getFirstAndOnly(paramXML, "Fs"), param.fs);
    parseDouble(getFirstAndOnly(paramXML, "BWInst"), param.bwInst);
    parseDouble(getFirstAndOnly(paramXML, "RcvStartTime1"), param.rcvStartTime1);
    parseDouble(getFirstAndOnly(paramXML, "RcvStartTime2"), param.rcvStartTime2);
    parseDouble(getFirstAndOnly(paramXML, "FrcvMin"), param.fRcvMin);
    parseDouble(getFirstAndOnly(paramXML, "FrcvMax"), param.fRcvMax);
    parseString(getFirstAndOnly(paramXML, "RcvAPCId"), param.rcvAPCId);
    parseString(getFirstAndOnly(paramXML, "RcvAPATId"), param.rcvAPATId);
    XMLElem rcvRefPointXML = getFirstAndOnly(paramXML, "RcvRefPoint");
    mCommon.parseVector3D(getFirstAndOnly(rcvRefPointXML, "ECF"), param.rcvRefPoint.ecf);
    mCommon.parseVector2D(getFirstAndOnly(rcvRefPointXML, "IAC"), param.rcvRefPoint.iac);
    XMLElem rcvPolXML = getFirstAndOnly(paramXML, "RcvPolarization");
    param.rcvPolarization.polarizationID = PolarizationType::toType(getFirstAndOnly(rcvPolXML, "PolarizationID")->getCharacterData());
    parseDouble(getFirstAndOnly(rcvPolXML, "AmpH"), param.rcvPolarization.ampH);
    parseDouble(getFirstAndOnly(rcvPolXML, "AmpV"), param.rcvPolarization.ampV);
    parseDouble(getFirstAndOnly(rcvPolXML, "PhaseH"), param.rcvPolarization.phaseH);
    parseDouble(getFirstAndOnly(rcvPolXML, "PhaseV"), param.rcvPolarization.phaseV);
    parseDouble(getFirstAndOnly(paramXML, "RcvRefIrradiance"), param.rcvRefIrradiance);
    parseDouble(getFirstAndOnly(paramXML, "RcvIrradianceErrorStdDev"), param.rcvRefIrradianceErrorStdDev);
    parseDouble(getFirstAndOnly(paramXML, "RcvRefLAtm"), param.rcvRefLAtm);
    parseDouble(getFirstAndOnly(paramXML, "PNCRSD"), param.pncrsd);
    parseDouble(getFirstAndOnly(paramXML, "BNCRSD"), param.bncrsd);
    mCommon.parseParameters(paramXML, "Parameter", param.addedParameters);
    XMLElem sarImageXML = getOptional(paramXML, "SARImage");
    if(sarImageXML)
    {
        param.sarImage.reset(new crsd::ChannelSARImage());
        parseString(getFirstAndOnly(sarImageXML, "TxId"), param.sarImage->txID);
        parseUInt(getFirstAndOnly(sarImageXML, "RefVectorPulseIndex"), param.sarImage->refVectorPulseIndex);  
        XMLElem txPolXML = getFirstAndOnly(sarImageXML, "TxPolarization");
        param.sarImage->txPolarization.polarizationID = PolarizationType::toType(getFirstAndOnly(txPolXML, "PolarizationID")->getCharacterData());
        parseDouble(getFirstAndOnly(txPolXML, "AmpH"), param.sarImage->txPolarization.ampH);
        parseDouble(getFirstAndOnly(txPolXML, "AmpV"), param.sarImage->txPolarization.ampV);
        parseDouble(getFirstAndOnly(txPolXML, "PhaseH"), param.sarImage->txPolarization.phaseH);
        parseDouble(getFirstAndOnly(txPolXML, "PhaseV"), param.sarImage->txPolarization.phaseV);
        XMLElem dwellXML = getFirstAndOnly(sarImageXML, "DwellTimes");
        XMLElem polyXML = getOptional(dwellXML, "Polynomials");
        if (polyXML)
        {
            parseString(getFirstAndOnly(polyXML, "CODId"), param.sarImage->dwellTime.codId);
            parseString(getFirstAndOnly(polyXML, "DwellId"), param.sarImage->dwellTime.dwellId);
        }
        XMLElem arrayXML = getOptional(dwellXML, "Array");
        if (arrayXML)
        {
            parseString(getFirstAndOnly(arrayXML, "DTAId"), param.sarImage->dwellTime.dtaId);
        }
        const xml::lite::Element* imageAreaXML = getFirstAndOnly(sarImageXML, "ImageArea");
        parseAreaType(imageAreaXML, param.sarImage->imageArea);
    }
}

void CRSDXMLParser::parsePVPType(Pvp& pvp, const xml::lite::Element* paramXML, PVPType& param) const
{
    size_t size;
    size_t offset;
    std::string format;
    parseUInt(getFirstAndOnly(paramXML, "Size"), size);
    parseUInt(getFirstAndOnly(paramXML, "Offset"), offset);
    parseString(getFirstAndOnly(paramXML, "Format"), format);
    if (param.getSize() != size)
    {
        std::ostringstream ostr;
        ostr << "Specified size: " << size
             << " does not match default size: "
             << param.getSize();
        throw except::Exception(Ctxt(ostr.str()));
    }
    if (param.getFormat() != format)
    {
        std::ostringstream ostr;
        ostr << "Specified format: " << format
             << " does not match default format: "
             << param.getFormat();
        throw except::Exception(Ctxt(ostr.str()));
    }
    pvp.setOffset(offset, param);
}

void CRSDXMLParser::parsePVPType(Pvp& pvp, const xml::lite::Element* paramXML) const
{
    std::string name;
    size_t size;
    size_t offset;
    std::string format;
    parseString(getFirstAndOnly(paramXML, "Name"), name);
    parseUInt(getFirstAndOnly(paramXML, "Size"), size);
    parseUInt(getFirstAndOnly(paramXML, "Offset"), offset);
    parseString(getFirstAndOnly(paramXML, "Format"), format);
    pvp.setCustomParameter(size, offset, format, name);
}

bool CRSDXMLParser::parseOptionalPVPType(const xml::lite::Element* parent, const std::string& tag, Pvp& pvp, PVPType& param) const
{    
    if (const xml::lite::Element* const element = getOptional(parent, tag))
    {
        parsePVPType(pvp, element, param);
        return true;
    }
    return false;
}

void CRSDXMLParser::parsePPPType(Ppp& ppp, const xml::lite::Element* paramXML, PPPType& param) const
{
    size_t size;
    size_t offset;
    std::string format;
    parseUInt(getFirstAndOnly(paramXML, "Size"), size);
    parseUInt(getFirstAndOnly(paramXML, "Offset"), offset);
    parseString(getFirstAndOnly(paramXML, "Format"), format);
    if (param.getSize() != size)
    {
        std::ostringstream ostr;
        ostr << "Specified size: " << size
             << " does not match default size: "
             << param.getSize();
        throw except::Exception(Ctxt(ostr.str()));
    }
    if (param.getFormat() != format)
    {
        std::ostringstream ostr;
        ostr << "Specified format: " << format
             << " does not match default format: "
             << param.getFormat();
        throw except::Exception(Ctxt(ostr.str()));
    }
    ppp.setOffset(offset, param);
}

void CRSDXMLParser::parsePPPType(Ppp& ppp, const xml::lite::Element* paramXML) const
{
    std::string name;
    size_t size;
    size_t offset;
    std::string format;
    parseString(getFirstAndOnly(paramXML, "Name"), name);
    parseUInt(getFirstAndOnly(paramXML, "Size"), size);
    parseUInt(getFirstAndOnly(paramXML, "Offset"), offset);
    parseString(getFirstAndOnly(paramXML, "Format"), format);
    ppp.setCustomParameter(size, offset, format, name);
}

bool CRSDXMLParser::parseOptionalPPPType(const xml::lite::Element* parent, const std::string& tag, Ppp& ppp, PPPType& param) const
{    
    if (const xml::lite::Element* const element = getOptional(parent, tag))
    {
        parsePPPType(ppp, element, param);
        return true;
    }
    return false;
}

void CRSDXMLParser::parsePosVelErr(const xml::lite::Element* posVelErrXML, six::PosVelError& posVelErr) const
{
    std::string frameStr;
    parseString(getFirstAndOnly(posVelErrXML, "Frame"), frameStr);
    posVelErr.frame.mValue = scene::FrameType::fromString(frameStr);
    parseDouble(getFirstAndOnly(posVelErrXML, "P1"), posVelErr.p1);
    parseDouble(getFirstAndOnly(posVelErrXML, "P2"), posVelErr.p2);
    parseDouble(getFirstAndOnly(posVelErrXML, "P3"), posVelErr.p3);
    parseDouble(getFirstAndOnly(posVelErrXML, "V1"), posVelErr.v1);
    parseDouble(getFirstAndOnly(posVelErrXML, "V2"), posVelErr.v2);
    parseDouble(getFirstAndOnly(posVelErrXML, "V3"), posVelErr.v3);

    XMLElem corrCoefsXML = getOptional(posVelErrXML, "CorrCoefs");

    if(corrCoefsXML)
    {
        posVelErr.corrCoefs.reset(new six::CorrCoefs());
        parseDouble(getFirstAndOnly(corrCoefsXML, "P1P2"), posVelErr.corrCoefs->p1p2);
        parseDouble(getFirstAndOnly(corrCoefsXML, "P1P3"), posVelErr.corrCoefs->p1p3);
        parseDouble(getFirstAndOnly(corrCoefsXML, "P1V1"), posVelErr.corrCoefs->p1v1);
        parseDouble(getFirstAndOnly(corrCoefsXML, "P1V2"), posVelErr.corrCoefs->p1v2);
        parseDouble(getFirstAndOnly(corrCoefsXML, "P1V3"), posVelErr.corrCoefs->p1v3);
        parseDouble(getFirstAndOnly(corrCoefsXML, "P2P3"), posVelErr.corrCoefs->p2p3);
        parseDouble(getFirstAndOnly(corrCoefsXML, "P2V1"), posVelErr.corrCoefs->p2v1);
        parseDouble(getFirstAndOnly(corrCoefsXML, "P2V2"), posVelErr.corrCoefs->p2v2);
        parseDouble(getFirstAndOnly(corrCoefsXML, "P2V3"), posVelErr.corrCoefs->p2v3);
        parseDouble(getFirstAndOnly(corrCoefsXML, "P3V1"), posVelErr.corrCoefs->p3v1);
        parseDouble(getFirstAndOnly(corrCoefsXML, "P3V2"), posVelErr.corrCoefs->p3v2);
        parseDouble(getFirstAndOnly(corrCoefsXML, "P3V3"), posVelErr.corrCoefs->p3v3);
        parseDouble(getFirstAndOnly(corrCoefsXML, "V1V2"), posVelErr.corrCoefs->v1v2);
        parseDouble(getFirstAndOnly(corrCoefsXML, "V1V3"), posVelErr.corrCoefs->v1v3);
        parseDouble(getFirstAndOnly(corrCoefsXML, "V2V3"), posVelErr.corrCoefs->v2v3);
    }

    // posVelErr.positionDecorr.reset(new six::DecorrType());
    mCommon.parseOptionalDecorrType(posVelErrXML, "PositionDecorr", posVelErr.positionDecorr);
}

void CRSDXMLParser::parsePlatform(const xml::lite::Element* platXML, ErrorParameters::Bistatic::Platform& plat) const
{
    parsePosVelErr(getFirstAndOnly(platXML, "PosVelErr"), plat.posVelErr);
    XMLElem radarSensorXML = getFirstAndOnly(platXML, "RadarSensor");
    parseOptionalDouble(radarSensorXML, "ClockFreqSF", plat.radarSensor.clockFreqSF);
    parseDouble(getFirstAndOnly(radarSensorXML, "CollectionStartTime"), plat.radarSensor.collectionStartTime);
}

void CRSDXMLParser::parseSupportArrayParameter(const xml::lite::Element* paramXML, SupportArrayParameter& param, bool additionalFlag) const
{
    if(!additionalFlag)
    {
        std::string identifierVal;
        parseString(getFirstAndOnly(paramXML, "Identifier"), identifierVal);
        param.setIdentifier(identifierVal);
    }
    parseString(getFirstAndOnly(paramXML, "ElementFormat"), param.elementFormat);
    parseDouble(getFirstAndOnly(paramXML, "X0"), param.x0);
    parseDouble(getFirstAndOnly(paramXML, "Y0"), param.y0);
    parseDouble(getFirstAndOnly(paramXML, "XSS"), param.xSS);
    parseDouble(getFirstAndOnly(paramXML, "YSS"), param.ySS);
}
}
