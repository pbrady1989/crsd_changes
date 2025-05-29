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

#include <complex>

#include <nitf/coda-oss.hpp>
#include <six/Init.h>

#include <crsd/PPP.h>
#include <crsd/Utilities.h>

namespace crsd
{
PPPType::PPPType() :
    mSize(0),
    mOffset(six::Init::undefined<size_t>())
{
}

APPPType::APPPType()
{
}

Ppp::Ppp()
{
    initialize();
}

void Ppp::initialize()
{
    // Default size and formats for each PPP
    // listed in Table 11-6 CRSD1.0 Spec
    setDefaultValues(2,"Int=I8;Frac=F8", txTime);
    setDefaultValues(3,"X=F8;Y=F8;Z=F8;", txPos);
    setDefaultValues(3,"X=F8;Y=F8;Z=F8;", txVel);
    setDefaultValues(1,"F8", fx1);
    setDefaultValues(1,"F8", fx2);
    setDefaultValues(1,"F8", txmt);
    setDefaultValues(2,"Int=I8;Frac=F8", phiX0);
    setDefaultValues(1,"F8", fxFreq0);
    setDefaultValues(1,"F8", fxRate);
    setDefaultValues(1,"F8", txRadInt);
    setDefaultValues(3,"X=F8;Y=F8;Z=F8;", txACX);
    setDefaultValues(3,"X=F8;Y=F8;Z=F8;", txACY);
    setDefaultValues(2,"DCX=F8;DCY=F8;", txEB);
    setDefaultValues(1,"I8", fxResponseIndex);
    setDefaultValues(1,"I8", xmIndex);
}

void Ppp::validate(size_t size, size_t offset)
{
    //Check if size of array is sufficient for write
    if (offset + size > mParamLocations.size())
    {
        mParamLocations.resize(offset+size);
    }

    //Check if any blocks will be overwritten
    for (size_t ii = 0; ii < size; ++ii)
    {
        if(mParamLocations.at(offset + ii) == true)
        {
            throw except::Exception(Ctxt(
                                    "This byte block is occupied"));
        }
    }

    // Mark each block as written
    for (size_t ii = 0; ii < size; ++ii)
    {
        mParamLocations.at(offset + ii) = true;
    }
}

void Ppp::setOffset(size_t offset, PPPType& param)
{
    validate(param.getSize(), offset);
    validateFormat(param.getFormat());
    param.setOffset(offset);
}

void Ppp::append(PPPType& param)
{
    size_t currentOffset = mParamLocations.size();
    setOffset(currentOffset, param);
}

// Assumes addedPPP is already correct size
void Ppp::setCustomParameter(size_t size, size_t offset, const std::string& format, const std::string& name)
{
    validate(size, offset);
    validateFormat(format);
    if (addedPPP.count(name) == 0)
    {
        addedPPP[name] = APPPType();
        addedPPP.find(name)->second.setData(size, offset, format, name);
        return;
    }
    throw except::Exception(Ctxt(
            "Additional parameter name is not unique"));
}

void Ppp::appendCustomParameter(size_t size, const std::string& format, const std::string& name)
{
    size_t currentOffset = mParamLocations.size();
    setCustomParameter(size, currentOffset, format, name);
}

void Ppp::setDefaultValues(size_t size, const std::string& format, PPPType& param)
{
    param.setSize(size);
    param.setFormat(format);
}

// Returns num blocks (not bytes)
size_t Ppp::getReqSetSize() const
{
    size_t res = txTime.getSize() + txPos.getSize() + txVel.getSize() +
            fx1.getSize() + fx2.getSize() + txmt.getSize() + phiX0.getSize() + fxFreq0.getSize() +
            fxRate.getSize() + txRadInt.getSize() + txACX.getSize() + txACY.getSize() +
            txEB.getSize() + fxResponseIndex.getSize();
    if(!six::Init::isUndefined<size_t>(xmIndex.getOffset()))
    {
        res += xmIndex.getSize();
    }
    for (auto it = addedPPP.begin(); it != addedPPP.end(); ++it)
    {
        res += it->second.getSize();
    }
    return res;
}

size_t Ppp::sizeInBytes() const
{
    return getReqSetSize() * PPPType::WORD_BYTE_SIZE;
}

std::ostream& operator<< (std::ostream& os, const PPPType& p)
{
    os << "    Size           : " << p.getSize() << "\n"
        << "    Offset         : " << p.getOffset() << "\n"
        << "    Format         : " << p.getFormat() << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const APPPType& a)
{
    os << "    Name           : " << a.getName() << "\n"
        << (PPPType)a;

    return os;
}

std::ostream& operator<< (std::ostream& os, const Ppp& p)
{
    os  << "  TxStart        : \n" << p.txTime << "\n"
        << "  TxPos          : \n" << p.txPos << "\n"
        << "  TxVel          : \n" << p.txVel << "\n"
        << "  Fx1            : \n" << p.fx1 << "\n"
        << "  Fx2            : \n" << p.fx2 << "\n"
        << "  TXmt           : \n" << p.txmt << "\n"
        << "  PhiX0          : \n" << p.phiX0 << "\n"
        << "  FxFreq0        : \n" << p.fxFreq0 << "\n"
        << "  FxRate         : \n" << p.fxRate << "\n"
        << "  TxRadInt       : \n" << p.txRadInt << "\n"
        << "  TxACX          : \n" << p.txACX << "\n"
        << "  TxACY          : \n" << p.txACY << "\n"
        << "  TxEB           : \n" << p.txEB << "\n"
        << "  FxResponseIndex: \n" << p.fxResponseIndex << "\n";
    if(!six::Init::isUndefined<size_t>(p.xmIndex.getOffset()))
    {
        os << "  XMIndex         : \n" << p.xmIndex << "\n";
    }
    for (auto it = p.addedPPP.begin(); it != p.addedPPP.end(); ++it)
    {
        os << "  Additional Parameter : " << it->second << "\n";
    }
    return os;
}
}
