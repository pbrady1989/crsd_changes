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

#include <unordered_map>
#include <six/Init.h>
#include <crsd/SupportArray.h>
#include <crsd/Utilities.h>

namespace crsd
{

SupportArrayParameter::SupportArrayParameter() :
    x0(six::Init::undefined<double>()),
    y0(six::Init::undefined<double>()),
    xSS(six::Init::undefined<double>()),
    ySS(six::Init::undefined<double>()),
    identifier(six::Init::undefined<size_t>())
{
}

SupportArrayParameter::SupportArrayParameter(
        const std::string& format,
        size_t id,
        double x0_in,
        double y0_in,
        double xSS_in,
        double ySS_in) :
    elementFormat(format),
    x0(x0_in),
    y0(y0_in),
    xSS(xSS_in),
    ySS(ySS_in),
    identifier(id)
{
    initializeParams();
}

void SupportArrayParameter::initializeParams()
{
    validateFormat(elementFormat);
}

FxResponseArray::FxResponseArray() :
    fx0FXR(six::Init::undefined<double>()),
    fxSSFXR(six::Init::undefined<double>()),
    identifier(six::Init::undefined<size_t>())
{
}

FxResponseArray::FxResponseArray(
        const std::string& format,
        size_t id,
        double fx0_in,
        double fxSS_in) :
    elementFormat(format),
    fx0FXR(fx0_in),
    fxSSFXR(fxSS_in),
    identifier(id)
{
    initializeParams();
}

void FxResponseArray::initializeParams()
{
    validateFormat(elementFormat);
}

XMArray::XMArray() :
    tsXMA(six::Init::undefined<double>()),
    maxXMBW(six::Init::undefined<double>()),
    identifier(six::Init::undefined<size_t>())
{
}

XMArray::XMArray(
        const std::string& format,
        size_t id,
        double tsXMA_in,
        double maxXMBW_in) :
    elementFormat(format),
    tsXMA(tsXMA_in),
    maxXMBW(maxXMBW_in),
    identifier(id)
{
    initializeParams();
}

void XMArray::initializeParams()
{
    validateFormat(elementFormat);
}

AdditionalSupportArray::AdditionalSupportArray()
{
}

AdditionalSupportArray::AdditionalSupportArray(
        const std::string& format, const std::string& id,
        double x0_in, double y0_in, double xSS_in, double ySS_in,
        const std::string& xUnits_in, const std::string& yUnits_in,
        const std::string& zUnits_in) :
    identifier(id),
    xUnits(xUnits_in),
    yUnits(yUnits_in),
    zUnits(zUnits_in)
{
    elementFormat = format;
    x0 = x0_in;
    y0 = y0_in;
    xSS = xSS_in;
    ySS = ySS_in;
    initializeParams();
}

SupportArrayParameter SupportArray::getIAZSupportArray(const std::string& key) const
{
    size_t keyNum = str::toType<size_t>(key);
    if (iazArray.size() <= keyNum)
    {
        std::ostringstream oss;
        oss << "SA_ID was not found " << (key);
        throw except::Exception(Ctxt(oss.str()));
    }
    return iazArray[keyNum];
}

SupportArrayParameter SupportArray::getAGPSupportArray(const std::string& key) const
{
    size_t keyNum = str::toType<size_t>(key);
    if (antGainPhase.size() <= keyNum)
    {
        std::ostringstream oss;
        oss << "SA_ID was not found " << (key);
        throw except::Exception(Ctxt(oss.str()));
    }
    return antGainPhase[keyNum];
}

SupportArrayParameter SupportArray::getDwellTimeSupportArray(const std::string& key) const
{
    size_t keyNum = str::toType<size_t>(key);
    if (dwellTimeArray.size() <= keyNum)
    {
        std::ostringstream oss;
        oss << "DwellTime_ID was not found " << (key);
        throw except::Exception(Ctxt(oss.str()));
    }
    return dwellTimeArray[keyNum];
}

FxResponseArray SupportArray::getFxResponseArray(const std::string& key) const
{
    size_t keyNum = str::toType<size_t>(key);
    if (fxResponseArray.size() <= keyNum)
    {
        std::ostringstream oss;
        oss << "FxResponseArray_ID was not found " << (key);
        throw except::Exception(Ctxt(oss.str()));
    }
    return fxResponseArray[keyNum];
}

XMArray SupportArray::getXMArray(const std::string& key) const
{
    size_t keyNum = str::toType<size_t>(key);
    if (xmArray.size() <= keyNum)
    {
        std::ostringstream oss;
        oss << "XMArray_ID was not found " << (key);
        throw except::Exception(Ctxt(oss.str()));
    }
    return xmArray[keyNum];
}

AdditionalSupportArray SupportArray::getAddedSupportArray(const std::string& key) const
{
    if (addedSupportArray.count(key) == 0 || addedSupportArray.count(key) > 1)
    {
        std::ostringstream oss;
        oss << "SA_ID was not found " << (key);
        throw except::Exception(Ctxt(oss.str()));
    }
    return addedSupportArray.find(key)->second;
}

std::ostream& operator<< (std::ostream& os, const SupportArrayParameter& s)
{
    if (!six::Init::isUndefined(s.getIdentifier()))
    {
        os << "    Identifier     : " << s.getIdentifier() << "\n";
    }
    os << "    Element Format : " << s.elementFormat << "\n"
        << "    X0             : " << s.x0 << "\n"
        << "    Y0             : " << s.y0 << "\n"
        << "    xSS            : " << s.xSS << "\n"
        << "    ySS            : " << s.ySS << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const FxResponseArray& s)
{
    if (!six::Init::isUndefined(s.getIdentifier()))
    {
        os << "    Identifier     : " << s.getIdentifier() << "\n";
    }
    os << "    Element Format : " << s.elementFormat << "\n"
        << "    fx0FXR            : " << s.fx0FXR << "\n"
        << "    fxSSFXR           : " << s.fxSSFXR << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const XMArray& s)
{
    if (!six::Init::isUndefined(s.getIdentifier()))
    {
        os << "    Identifier     : " << s.getIdentifier() << "\n";
    }
    os << "    Element Format : " << s.elementFormat << "\n"
        << "    tsXMA             : " << s.tsXMA << "\n"
        << "    maxXMBW           : " << s.maxXMBW << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const AdditionalSupportArray& a)
{
    os << (SupportArrayParameter)a
        << "    XUnits         : " << a.xUnits << "\n"
        << "    YUnits         : " << a.yUnits << "\n"
        << "    ZUnits         : " << a.zUnits << "\n";
    for (size_t ii = 0; ii < a.parameter.size(); ++ii)
    {
        os << "    Parameter Name : " << a.parameter[ii].getName() << "\n"
            << "    Parameter Value : " << a.parameter[ii].str() << "\n";
    }
    return os;
}

std::ostream& operator<< (std::ostream& os, const SupportArray& s)
{
    os << "SupportArray:: \n";
    for (size_t ii = 0; ii < s.iazArray.size(); ++ii)
    {
        os << "  IAZ Array:: \n"
            << s.iazArray[ii];
    }
    for (size_t ii = 0; ii < s.antGainPhase.size(); ++ii)
    {
        os << "  Ant Gain Phase:: \n"
            << s.antGainPhase[ii];
    }
    for (size_t ii = 0; ii < s.dwellTimeArray.size(); ++ii)
    {
        os << "  Dwell Time Array:: \n"
            << s.dwellTimeArray[ii];
    }
    for (size_t ii = 0; ii < s.fxResponseArray.size(); ++ii)
    {
        os << "  Fx Response Array:: \n"
            << s.fxResponseArray[ii];
    }
    for (size_t ii = 0; ii < s.xmArray.size(); ++ii)
    {
        os << "  XM Array:: \n"
            << s.xmArray[ii];
    }
    for (auto it = s.addedSupportArray.begin(); it != s.addedSupportArray.end(); ++it)
    {
        os << "  Added Support Array:: \n"
            << "    " << it->first << ": \n"
            << it->second;
    }
    return os;
}

}