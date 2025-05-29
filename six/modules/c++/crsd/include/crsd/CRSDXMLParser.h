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

#ifndef __CRSD_CRSD_XML_PARSER_H__
#define __CRSD_CRSD_XML_PARSER_H__

#include <memory>

#include <logging/Logger.h>
#include <xml/lite/Element.h>
#include <xml/lite/Document.h>
#include <six/XMLParser.h>
#include <six/SICommonXMLParser10x.h>
#include <crsd/SceneCoordinates.h>
#include <crsd/Data.h>
#include <crsd/Global.h>
#include <crsd/Metadata.h>
#include <crsd/TransmitInfo.h>
#include <crsd/SARInfo.h>
#include <crsd/TxSequence.h>
#include <crsd/PVP.h>
#include <crsd/PPP.h>
#include <crsd/ReferenceGeometry.h>
#include <crsd/Types.h>

namespace crsd
{
/*!
 *  \class CRSDXMLParser
 *
 *  \brief This class converts a Metadata object into a CRSD XML
 *  Document Object Model (DOM) and vise-versa.
 */
class CRSDXMLParser : public six::XMLParser
{
public:
    /*!
     *  \func CRSDXMLParser
     *  \brief Constructor
     *
     *  \param uri CRSD file uri string
     *  \param addClassAttributes flag indicates whether to add as attribute
     *  \param log provide logger object
     *  \param ownLog flag indicates if log should be deleted
     */
    CRSDXMLParser(const std::string& uri,
                  bool addClassAttributes,
                  logging::Logger* log = nullptr,
                  bool ownLog = false);

    /*!
     *  \func toXML
     *
     *  \brief Convert metadata to XML document object
     *
     *  \param metadata Valid CRSD metadata object
     *  \param schemaPaths Vector of XML Schema for validation
     *  \return pointer to xml Document object
     */
    std::unique_ptr<xml::lite::Document> toXML(
            const Metadata& metadata);

    /*!
     *  \func fromXML
     *
     *  \brief Parse XML document to Metadata object
     *
     *  \param doc XML document object of CRSD
     *  \param schemaPaths Vector of XML Schema for validation
     *
     *  \return pointer to metadata object
     */
    std::unique_ptr<Metadata> fromXML(
            const xml::lite::Document* doc);

private:
    typedef xml::lite::Element*  XMLElem;

private:

    //! Write to XML object
    XMLElem toXML(const CollectionInformation& obj, XMLElem parent);
    XMLElem toXML(const Global& obj, XMLElem parent);
    XMLElem toXML(const SARInfo& obj, XMLElem parent);
    XMLElem toXML(const TransmitInfo& obj, XMLElem parent);
    XMLElem toXML(const SceneCoordinates& obj, XMLElem parent);
    XMLElem toXML(const Data& obj, XMLElem parent);
    XMLElem toXML(const Channel& obj, XMLElem parent);
    XMLElem toXML(const Pvp& obj, XMLElem parent);
    XMLElem toXML(const Ppp& obj, XMLElem parent);
    XMLElem toXML(const SupportArray& obj, XMLElem parent);
    XMLElem toXML(const Dwell& obj, XMLElem parent);
    XMLElem toXML(const ReferenceGeometry& obj, XMLElem parent);
    XMLElem toXML(const Antenna& obj, XMLElem parent);
    XMLElem toXML(const TxSequence& obj, XMLElem parent);
    XMLElem toXML(const ErrorParameters& obj, XMLElem parent);
    XMLElem toXML(const ProductInfo& obj, XMLElem parent);
    XMLElem toXML(const GeoInfo& obj, XMLElem parent);
    XMLElem toXML(const MatchInformation& obj, XMLElem parent);

    //! Read from XML object
    void fromXML(const xml::lite::Element* collectionIDXML, CollectionInformation& collectionID);
    void fromXML(const xml::lite::Element* globalXML, Global& global);
    void fromXML(const xml::lite::Element* sceneCoordsXML, SceneCoordinates& scene);
    void fromXML(const xml::lite::Element* dataXML, Data& data);
    void fromXML(const xml::lite::Element* channelXML, Channel& channel);
    void fromXML(const xml::lite::Element* pvpXML, Pvp& pvp);
    void fromXML(const xml::lite::Element* pppXML, Ppp& ppp);
    void fromXML(const xml::lite::Element* DwellXML, Dwell& dwell);
    void fromXML(const xml::lite::Element* TxSequenceXML, TxSequence& txSequence);
    void fromXML(const xml::lite::Element* refGeoXML, ReferenceGeometry& refGeo);
    void fromXML(const xml::lite::Element* supportArrayXML, SupportArray& supportArray);
    void fromXML(const xml::lite::Element* antennaXML, Antenna& antenna);
    void fromXML(const xml::lite::Element* errParamXML, ErrorParameters& errParam);
    void fromXML(const xml::lite::Element* productInfoXML, ProductInfo& productInfo);
    void fromXML(const xml::lite::Element* sarInfoXML, SARInfo& sarInfo);
    void fromXML(const xml::lite::Element* transmitInfoXML, TransmitInfo& transmitInfo);
    void fromXML(const xml::lite::Element* geoInfoXML, GeoInfo& geoInfo);

    //! Create helper functions
    XMLElem createLatLonFootprint(const std::string& name,
                                  const std::string& cornerName,
                                  const crsd::LatLonCorners& corners,
                                  XMLElem parent) const;
    XMLElem createPVPType(const std::string& name,
                          const PVPType& p,
                          XMLElem parent) const;
    XMLElem createAPVPType(const std::string& name,
                           const APVPType& p,
                           XMLElem parent) const;
    XMLElem createPPPType(const std::string& name,
                          const PPPType& p,
                          XMLElem parent) const;
    XMLElem createAPPPType(const std::string& name,
                           const APPPType& p,
                           XMLElem parent) const;

    XMLElem createErrorParamPlatform(const std::string& name,
                                     const ErrorParameters::Bistatic::Platform p,
                                     XMLElem parent) const;
    XMLElem createDecorrType(const std::string& name, const six::DecorrType& dt,
        XMLElem parent) const;
    XMLElem createDecorrType(const std::string& name, const std::optional<six::DecorrType>& dt,
        XMLElem parent) const;

    //! Parse helper functions
    void parseAreaType(const xml::lite::Element* areaXML, AreaType& area) const;
    void parseLineSample(const xml::lite::Element* lsXML, LineSample& ls) const;
    void parseIAExtent(const xml::lite::Element* extentXML, ImageAreaXExtent& extent) const;
    void parseIAExtent(const xml::lite::Element* extentXML, ImageAreaYExtent& extent) const;
    void parseChannelParameters(const xml::lite::Element* paramXML,
                                ChannelParameter& param) const;
    void parseTxSequenceParameters(const xml::lite::Element* paramXML,
                                   TxSequenceParameter& param) const;
    void parsePVPType(Pvp& p, const xml::lite::Element* paramXML, PVPType& param) const;
    void parsePVPType(Pvp& p, const xml::lite::Element* paramXML) const;
    bool parseOptionalPVPType(const xml::lite::Element* parent, const std::string& tag, Pvp& p, PVPType& param) const;
    void parsePPPType(Ppp& p, const xml::lite::Element* paramXML, PPPType& param) const;
    void parsePPPType(Ppp& p, const xml::lite::Element* paramXML) const;
    bool parseOptionalPPPType(const xml::lite::Element* parent, const std::string& tag, Ppp& p, PPPType& param) const;
    void parsePosVelErr(const xml::lite::Element* posVelErrXML, six::PosVelError& posVelErr) const;
    void parsePlatform(const xml::lite::Element* platXML,  ErrorParameters::Bistatic::Platform& plat) const;
    void parseSupportArrayParameter(const xml::lite::Element* paramXML, SupportArrayParameter& param,
                                    bool additionalFlag) const;
private:
    six::SICommonXMLParser10x mCommon;
};
}

#endif
