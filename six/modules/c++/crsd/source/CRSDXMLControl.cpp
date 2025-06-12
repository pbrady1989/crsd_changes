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
#include <crsd/CRSDXMLControl.h>

#include <set>
#include <unordered_map>
#include <algorithm>
#include <std/memory>
#include <iterator>

#include <io/StringStream.h>
#include <logging/NullLogger.h>
#include <xml/lite/MinidomParser.h>
#include <str/EncodedStringView.h>

#include <six/XMLControl.h>
#include <six/XmlLite.h>
#include <crsd/CRSDXMLParser.h>
#include <crsd/Enums.h>
#include <crsd/Metadata.h>
#include <crsd/Types.h>


#define ENFORCESPEC 0

namespace crsd
{

CRSDXMLControl::CRSDXMLControl(logging::Logger* log, bool ownLog) :
    mLogger(mLog, mOwnLog, nullptr)
{
    setLogger(log, ownLog);
}

/* TO XML */
std::u8string CRSDXMLControl::toXMLString(
    const Metadata& metadata,
    const std::vector<std::filesystem::path>* pSchemaPaths,
    bool prettyPrint)
{
    prettyPrint = true;
    std::vector<std::string> schemaPaths;
    if (pSchemaPaths != nullptr)
    {
        std::transform(pSchemaPaths->begin(), pSchemaPaths->end(), std::back_inserter(schemaPaths),
            [](const std::filesystem::path& p) { return p.string(); });
    }
    std::unique_ptr<xml::lite::Document> doc(toXML(metadata, schemaPaths));

    io::U8StringStream ss;
    (prettyPrint) ?
        doc->getRootElement()->prettyPrint(ss) :
        doc->getRootElement()->print(ss);
    return ss.stream().str();
}
std::string CRSDXMLControl::toXMLString(
        const Metadata& metadata,
        const std::vector<std::string>& schemaPaths_,
        bool prettyPrint)
{
    std::vector<std::filesystem::path> schemaPaths;
    std::transform(schemaPaths_.begin(), schemaPaths_.end(), std::back_inserter(schemaPaths),
        [](const std::string& s) { return s; });

    const auto result = toXMLString(metadata, &schemaPaths, prettyPrint);
    return str::EncodedStringView(result).native();
}

mem::auto_ptr<xml::lite::Document> CRSDXMLControl::toXML(
        const Metadata& metadata,
        const std::vector<std::string>& schemaPaths)
{
    mem::auto_ptr<xml::lite::Document> doc(toXMLImpl(metadata).release());
    if(!schemaPaths.empty())
    {
        six::XMLControl::validate(doc.get(), schemaPaths, mLog);
    }
    return doc;
}

std::unordered_map<std::string, xml::lite::Uri> CRSDXMLControl::getVersionUriMap()
{
    return {
        {"CRSDsar/1.0.0", xml::lite::Uri("urn:CRSDsar:1.0.0")},
        {"CRSDtx/1.0.0", xml::lite::Uri("urn:CRSDtx:1.0.0")},
        {"CRSDrcv/1.0.0", xml::lite::Uri("urn:CRSDrcv:1.0.0")}
    };
}

std::unordered_map<std::string, xml::lite::Uri> CRSDXMLControl::getTypeUriMap()
{
    return {
        {"CRSDsar", xml::lite::Uri("urn:CRSDsar:1.0.0")},
        {"CRSDtx", xml::lite::Uri("urn:CRSDtx:1.0.0")},
        {"CRSDrcv", xml::lite::Uri("urn:CRSDrcv:1.0.0")}
    };
}

std::unique_ptr<xml::lite::Document> CRSDXMLControl::toXMLImpl(const Metadata& metadata)
{
    const auto versionUriMap = getVersionUriMap();
    std::string metaVersion = six::toString<CRSDType>(metadata.getType()) + "/" + metadata.getVersion();
    if (versionUriMap.find(metaVersion) != versionUriMap.end())
    {
      return getParser(versionUriMap.find(metaVersion)->second)->toXML(metadata);
    }
    std::ostringstream ostr;
    ostr << "The version " << metadata.getVersion() << " is invalid. "
         << "Check if version is valid or "
         << "add a <version, URI> entry to versionUriMap";
    throw except::Exception(Ctxt(ostr.str()));
}

/* FROM XML */
std::unique_ptr<Metadata> CRSDXMLControl::fromXML(const std::string& xmlString,
                                     const std::vector<std::string>& schemaPaths_)
{
    std::vector<std::filesystem::path> schemaPaths;
    std::transform(schemaPaths_.begin(), schemaPaths_.end(), std::back_inserter(schemaPaths),
        [](const std::string& s) { return s; });

    return fromXML(str::EncodedStringView(xmlString).u8string(), schemaPaths);
}
std::unique_ptr<Metadata> CRSDXMLControl::fromXML(const std::u8string& xmlString,
    const std::vector<std::filesystem::path>& schemaPaths)
{
    io::U8StringStream stringStream;
    stringStream.write(xmlString);
    six::MinidomParser parser;
    parser.parse(stringStream);
    auto result = fromXML(parser.getDocument(), schemaPaths);
    return std::make_unique<Metadata>(std::move(result));
}

std::unique_ptr<Metadata> CRSDXMLControl::fromXML(const xml::lite::Document* doc,
                                     const std::vector<std::string>& schemaPaths)
{
    if(!schemaPaths.empty())
    {
        six::XMLControl::validate(doc, schemaPaths, mLog);
    }
    
    std::unique_ptr<Metadata> metadata = fromXMLImpl(doc);
    const xml::lite::Uri uri(doc->getRootElement()->getUri());

    metadata->setVersion(uriToVersion(uri));
    metadata->setType(six::toType<CRSDType>(uriToType(uri)));

    return metadata;
}
Metadata CRSDXMLControl::fromXML(const xml::lite::Document& doc, const std::vector<std::filesystem::path>& schemaPaths)
{
    std::vector<std::string> schemaPaths_;
    std::transform(schemaPaths.begin(), schemaPaths.end(), std::back_inserter(schemaPaths_),
        [](const std::filesystem::path& p) { return p.string(); });
    auto result = fromXML(&doc, schemaPaths_);

    return *(result.release());
}

std::unique_ptr<Metadata> CRSDXMLControl::fromXMLImpl(const xml::lite::Document* doc)
{
    const xml::lite::Uri uri(doc->getRootElement()->getUri());

    return getParser(uri)->fromXML(doc);
}

std::unique_ptr<CRSDXMLParser>
CRSDXMLControl::getParser(const xml::lite::Uri& uri) const
{
    return std::make_unique<CRSDXMLParser>(uri.value, false, mLog);
}

std::string CRSDXMLControl::uriToVersion(const xml::lite::Uri& uri) const
{
    const auto versionUriMap = getVersionUriMap();
    for (const auto& p : versionUriMap)
    {
        if (p.second == uri)
        {
            return "1.0.0";
        }
    }
    std::ostringstream ostr;
    ostr << "The URI " << uri << " is invalid. "
         << "Either input a valid URI or "
         << "add a <version, URI> entry to versionUriMap";
    throw except::Exception(Ctxt(ostr.str()));
}

std::string CRSDXMLControl::uriToType(const xml::lite::Uri& uri) const
{
    const auto typeUriMap = getTypeUriMap();
    for (const auto& p : typeUriMap)
    {
        if (p.second == uri)
        {
            return p.first;
        }
    }
    std::ostringstream ostr;
    ostr << "The URI " << uri << " is invalid. "
         << "Either input a valid URI or "
         << "add a <version, URI> entry to versionUriMap";
    throw except::Exception(Ctxt(ostr.str()));
}

}
