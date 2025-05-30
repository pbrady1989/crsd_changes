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

#include <crsd/PVP.h>
#include <crsd/Utilities.h>

namespace crsd
{
PVPType::PVPType() :
    mSize(0),
    mOffset(six::Init::undefined<size_t>())
{
}

APVPType::APVPType()
{
}

Pvp::Pvp()
{
    initialize();
}

void Pvp::initialize()
{
    // Default size and formats for each PVP
    // listed in Table 11-6 CRSD1.0 Spec
    setDefaultValues(2,"Int=I8;Frac=F8;", rcvStart);
    setDefaultValues(3,"X=F8;Y=F8;Z=F8;", rcvPos);
    setDefaultValues(3,"X=F8;Y=F8;Z=F8;", rcvVel);
    setDefaultValues(1,"F8", frcv1);
    setDefaultValues(1,"F8", frcv2);
    setDefaultValues(2,"Int=I8;Frac=F8;", refPhi0);
    setDefaultValues(1,"F8", refFreq);
    setDefaultValues(1,"F8", dfiC0);
    setDefaultValues(1,"F8", ficRate);
    setDefaultValues(3,"X=F8;Y=F8;Z=F8;", rcvACX);
    setDefaultValues(3,"X=F8;Y=F8;Z=F8;", rcvACY);
    setDefaultValues(2,"DCX=F8;DCY=F8;", rcvEB);
    setDefaultValues(1,"I8", signal);
    setDefaultValues(1,"F8", ampSF);
    setDefaultValues(1,"F8", dgrgc);
    setDefaultValues(1,"I8", txPulseIndex);
}

void Pvp::validate(size_t size, size_t offset)
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

void Pvp::setOffset(size_t offset, PVPType& param)
{
    validate(param.getSize(), offset);
    validateFormat(param.getFormat());
    param.setOffset(offset);
}

void Pvp::append(PVPType& param)
{
    size_t currentOffset = mParamLocations.size();
    setOffset(currentOffset, param);
}

// Assumes addedPVP is already correct size
void Pvp::setCustomParameter(size_t size, size_t offset, const std::string& format, const std::string& name)
{
    validate(size, offset);
    validateFormat(format);
    if (addedPVP.count(name) == 0)
    {
        addedPVP[name] = APVPType();
        addedPVP.find(name)->second.setData(size, offset, format, name);
        return;
    }
    throw except::Exception(Ctxt(
            "Additional parameter name is not unique"));
}

void Pvp::appendCustomParameter(size_t size, const std::string& format, const std::string& name)
{
    size_t currentOffset = mParamLocations.size();
    setCustomParameter(size, currentOffset, format, name);
}

void Pvp::setDefaultValues(size_t size, const std::string& format, PVPType& param)
{
    param.setSize(size);
    param.setFormat(format);
}

// Returns num blocks (not bytes)
size_t Pvp::getReqSetSize() const
{
    size_t res = rcvStart.getSize() + rcvPos.getSize() + rcvVel.getSize() +
            frcv1.getSize() + frcv2.getSize() + refPhi0.getSize() + refFreq.getSize() +
            dfiC0.getSize() + ficRate.getSize() + rcvACX.getSize() + rcvACY.getSize() +
            rcvEB.getSize() + signal.getSize();
    for (auto it = addedPVP.begin(); it != addedPVP.end(); ++it)
    {
        res += it->second.getSize();
    }
    return res;
}

size_t Pvp::sizeInBytes() const
{
    return getReqSetSize() * PVPType::WORD_BYTE_SIZE;
}

std::ostream& operator<< (std::ostream& os, const PVPType& p)
{
    os << "    Size           : " << p.getSize() << "\n"
        << "    Offset         : " << p.getOffset() << "\n"
        << "    Format         : " << p.getFormat() << "\n";
    return os;
}

std::ostream& operator<< (std::ostream& os, const APVPType& a)
{
    os << "    Name           : " << a.getName() << "\n"
        << (PVPType)a;

    return os;
}

std::ostream& operator<< (std::ostream& os, const Pvp& p)
{
    os  << "  RcvStart       :" << p.rcvStart << "\n"
        << "  RcvPos         :" << p.rcvPos << "\n"
        << "  RcvVel         :" << p.rcvVel << "\n"
        << "  Frcv1          :" << p.frcv1 << "\n"
        << "  Frcv2          :" << p.frcv2 << "\n"
        << "  RefPhi0        :" << p.refPhi0 << "\n"
        << "  RefFreq        :" << p.refFreq << "\n"
        << "  DfiC0          :" << p.dfiC0 << "\n"
        << "  FicRate        :" << p.ficRate << "\n"
        << "  RcvACX         :" << p.rcvACX << "\n"
        << "  RcvACY         :" << p.rcvACY << "\n"
        << "  RcvEB          :" << p.rcvEB << "\n"
        << "  Signal         :" << p.signal << "\n"
        << "  AmpSF          :" << p.ampSF << "\n"
        << "  Dgrgc          :" << p.dgrgc << "\n";
    if (!six::Init::isUndefined<size_t>(p.txPulseIndex.getOffset()))
    {
        os << "  TxPulseIndex        :" << p.txPulseIndex << "\n";
    }
    for (auto it = p.addedPVP.begin(); it != p.addedPVP.end(); ++it)
    {
        os << "  Additional Parameter : " << it->second << "\n";
    }
    return os;
}
}
