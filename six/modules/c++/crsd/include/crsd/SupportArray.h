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

#ifndef __CRSD_SUPPORT_ARRAY_H__
#define __CRSD_SUPPORT_ARRAY_H__

#include <ostream>
#include <vector>
#include <unordered_map>
#include <stddef.h>
#include <crsd/Enums.h>
#include <crsd/Types.h>


namespace crsd
{

/*!
 *  \struct SupportArrayParameter
 *
 *  \brief Parameters for defined support arrays
 *
 *  (Optional) Parameters that describe the binary support
 *  array(s) content and grid coordinates.
 */
struct SupportArrayParameter
{
    //! Constructor
    SupportArrayParameter();

    /*
     *  \func SupportArrayParameter
     *
     *  \brief Constructor initalizes values
     *
     *  \param format Speicifies the binary format of the data
     *   in the support array
     *  \param id Unique identifier of the support array
     *  \param x0_in Row 0 x coordinate
     *  \param y0_in Row 0 y coordinate
     *  \param xSS_in Row coordinate (X) sample spacing
     *  \param ySS_in Col coordinate (Y) sample spacing
     */
    SupportArrayParameter(
        const std::string& format, std::string id,
        double x0_in, double y0_in,
        double xSS_in, double ySS_in);

    //! Equality operator
    bool operator==(const SupportArrayParameter& other) const
    {
        return identifier == other.identifier &&
                elementFormat == other.elementFormat &&
                x0 == other.x0 && y0 == other.y0 &&
                xSS == other.xSS && ySS == other.ySS;
    }
    bool operator!=(const SupportArrayParameter& other) const
    {
        return !((*this) == other);
    }

    //! Set unique identifier
    void setIdentifier(std::string identifierIn)
    {
        identifier = identifierIn;
    }

    //! Get unique identifier
    inline std::string getIdentifier() const
    {
        return identifier;
    }

    //! Data element format
    std::string elementFormat;

    //! Row 0 X coordinate
    double x0;

    //! Column 0 Y coordinate.
    double y0;

    //! Row coordinate (X) sample spacing
    double xSS;

    //! Column coordinate (Y) sample spacing.
    double ySS;

protected:
    void initializeParams();
private:
    std::string identifier;
};

struct FxResponseArray
{
    //! Constructor
    FxResponseArray();

    /*
     *  \func FxResponseArray
     *
     *  \brief Constructor initalizes values
     *
     */
    FxResponseArray(
        const std::string& format, std::string id,
        double fx0_in, double fxSS_in);

    //! Equality operator
    bool operator==(const FxResponseArray& other) const
    {
        return identifier == other.identifier &&
                elementFormat == other.elementFormat &&
                fx0FXR == other.fx0FXR &&
                fxSSFXR == other.fxSSFXR;
    }
    bool operator!=(const FxResponseArray& other) const
    {
        return !((*this) == other);
    }

    //! Set unique identifier
    void setIdentifier(std::string identifierIn)
    {
        identifier = identifierIn;
    }

    //! Get unique identifier
    inline std::string getIdentifier() const
    {
        return identifier;
    }

    //! Data element format
    std::string elementFormat;

    //! Fx Frequency value for n = 0 for response fn's
    double fx0FXR;

    //! Samples spacing for all Fx response fn's
    double fxSSFXR;

protected:
    void initializeParams();
private:
    std::string identifier;
};

struct XMArray
{
    //! Constructor
    XMArray();

    /*
     *  \func XMArray
     *
     *  \brief Constructor initalizes values
     *
     */
    XMArray(
        const std::string& format, std::string id,
        double tsXMA_in, double maxXMBW_in);

    //! Equality operator
    bool operator==(const XMArray& other) const
    {
        return identifier == other.identifier &&
                elementFormat == other.elementFormat &&
                tsXMA == other.tsXMA &&
                maxXMBW == other.maxXMBW;
    }
    bool operator!=(const XMArray& other) const
    {
        return !((*this) == other);
    }

    //! Set unique identifier
    void setIdentifier(std::string identifierIn)
    {
        identifier = identifierIn;
    }

    //! Get unique identifier
    inline std::string getIdentifier() const
    {
        return identifier;
    }

    //! Data element format
    std::string elementFormat;

    //! Samples spacing for all fn's (sec)
    double tsXMA;

    //! Maximum modulation BW for any XM function contained
    //! in the array (Hz)
    double maxXMBW;

protected:
    void initializeParams();
private:
    std::string identifier;
};

/*!
 *  \struct AdditionalSupportArray
 *
 *  \brief Addiitonal support array custom parameters
 *
 *  (Optional) Added Array: Z(m,n) is a two dimensional array
 *  of data elements. Content and format of each
 *  element is user defined. Array coordinates X(m)
 *  and Y(m) are also user defined.
 */
struct AdditionalSupportArray : SupportArrayParameter
{
    //! Constructor
    AdditionalSupportArray();

    /*
     *  \func AdditionalSupportArray
     *
     *  \brief Constructor initalizes values
     *
     *  \param format Speicifies the binary format of the data
     *   in the support array
     *  \param id Unique identifier of the support array
     *  \param x0_in Row 0 x coordinate
     *  \param y0_in Row 0 y coordinate
     *  \param xSS_in Row coordinate (X) sample spacing
     *  \param ySS_in Col coordinate (Y) sample spacing
     *  \param xUnits Defines the X units of the sampled grid
     *  \param yUnits Defines the Y units of the sampled grid
     *  \param zUnits Defines the Z units of the sampled grid
     */
    AdditionalSupportArray(
        const std::string& format, const std::string& id,
        double x0_in, double y0_in, double xSS_in, double ySS_in,
        const std::string& xUnits_in, const std::string& yUnits_in,
        const std::string& zUnits_in);

    //! Equality operator
    bool operator==(const AdditionalSupportArray& other) const
    {
        return elementFormat == other.elementFormat &&
                x0 == other.x0 && y0 == other.y0 &&
                xSS == other.xSS && ySS == other.ySS &&
                identifier == other.identifier &&
                xUnits == other.xUnits && yUnits == other.yUnits &&
                zUnits == other.zUnits && parameter == other.parameter;
    }
    bool operator!=(const AdditionalSupportArray& other) const
    {
        return !((*this) == other);
    }

    //! Unique identifier of support array
    std::string identifier;

    //! Defines the X units of the sampled grid
    std::string xUnits;

    //! Defines the Y units of the sampled grid
    std::string yUnits;

    //! Defines the units of each component of the Data
    //! Element
    std::string zUnits;

    //! (Optional) Text field that can be used to further
    //! describe the added Support Array
    six::ParameterCollection parameter;
};

/*
 *  \struct SupportArray
 *
 *  \brief Contains parameters of all support arrays
 *
 *  (Optional) Parameters that describe the binary support
 *  array(s) content and grid coordinates.
 *  See section 2.3
 */
struct SupportArray
{
    //! Equality operators
    bool operator==(const SupportArray& other) const
    {
        return  iazArray == other.iazArray &&
                antGainPhase == other.antGainPhase &&
                dwellTimeArray == other.dwellTimeArray &&
                addedSupportArray == other.addedSupportArray;
    }
    bool operator!=(const SupportArray& other) const
    {
        return !((*this) == other);
    }

    //! Get IAZ support array by unique id
    SupportArrayParameter getIAZSupportArray(const std::string& key) const;

    //! Get AGP support array by unique id
    SupportArrayParameter getAGPSupportArray(const std::string& key) const;

    //! Get Dwell Time support array by unique id
    SupportArrayParameter getDwellTimeSupportArray(const std::string& key) const;

    //! Get AGP support array by unique id
    AdditionalSupportArray getAddedSupportArray(const std::string& key) const;

    // Get the FxResponseArray
    FxResponseArray getFxResponseArray(const std::string& key) const;

    // Get the XMArray
    XMArray getXMArray(const std::string& key) const;

    //! Vector of IAZ type arrays
    std::vector<SupportArrayParameter> iazArray;

    //! Vector of AGP type arrays
    std::vector<SupportArrayParameter> antGainPhase;

    //! Vector of Dwell Time type arrays
    std::vector<SupportArrayParameter> dwellTimeArray;

    //! Vector of Dwell Time type arrays
    std::vector<FxResponseArray> fxResponseArray;

    //! Vector of Dwell Time type arrays
    std::vector<XMArray> xmArray;

    //! Map of additonally defined support arrays
    std::unordered_map<std::string, AdditionalSupportArray> addedSupportArray;
};

//! Ostream operators
std::ostream& operator<< (std::ostream& os, const SupportArrayParameter& s);
std::ostream& operator<< (std::ostream& os, const AdditionalSupportArray& a);
std::ostream& operator<< (std::ostream& os, const FxResponseArray& a);
std::ostream& operator<< (std::ostream& os, const XMArray& a);
std::ostream& operator<< (std::ostream& os, const SupportArray& s);
}

#endif
