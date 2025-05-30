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
#include <fstream>
#include <memory>

#include <crsd/Global.h>
#include <crsd/Data.h>
#include <crsd/Channel.h>
#include <crsd/ProductInfo.h>
#include <crsd/Dwell.h>
#include <crsd/PPP.h>
#include <crsd/PVP.h>
#include <crsd/ErrorParameters.h>
#include <crsd/FileHeader.h>
#include <crsd/TxSequence.h>
#include <crsd/TransmitInfo.h>
#include <crsd/SARInfo.h>
#include <crsd/SupportArray.h>
#include <crsd/Antenna.h>
#include <crsd/Metadata.h>
#include <crsd/SceneCoordinates.h>
#include <io/StringStream.h>
#include <xml/lite/MinidomParser.h>
#include "TestCase.h"
#include <crsd/CRSDXMLControl.h>

namespace
{
std::string testCRSDXMLBody()
{
    const char* xmlBody =R"(<ProductInfo>
  <ProductName>EXAMPLE_PRODUCT</ProductName>
  <Classification>UNCLASSIFIED</Classification>
  <ReleaseInfo>Approved for public release</ReleaseInfo>
  <CountryCode>USA</CountryCode>
  <Profile>DEFAULT</Profile>
  <CreationInfo>
    <Application>ExampleApp</Application>
    <DateTime>2024-12-30T12:00:00Z</DateTime>
    <Site>ExampleSite</Site>
  </CreationInfo>
</ProductInfo>

<SARInfo>
  <CollectType>MONOSTATIC</CollectType>
  <RadarMode>
    <ModeType>SPOTLIGHT</ModeType>
    <ModeID>MODE1</ModeID>
  </RadarMode>
</SARInfo>

<TransmitInfo>
  <SensorName>TX_SENSOR</SensorName>
  <EventName>TX_EVENT</EventName>
</TransmitInfo>

<ReceiveInfo>
  <SensorName>RX_SENSOR</SensorName>
  <EventName>RX_EVENT</EventName>
</ReceiveInfo>

<Global>
  <CollectionRefTime>2024-12-30T12:00:00Z</CollectionRefTime>
  <Transmit>
    <TxTime1>0.0</TxTime1>
    <TxTime2>1.0</TxTime2>
    <FxMin>950000000.0</FxMin>
    <FxMax>1050000000.0</FxMax>
  </Transmit>
  <Receive>
    <RcvStartTime1>0.0</RcvStartTime1>
    <RcvStartTime2>1.0</RcvStartTime2>
    <FrcvMin>950000000.0</FrcvMin>
    <FrcvMax>1050000000.0</FrcvMax>
  </Receive>
</Global>

<SceneCoordinates>
  <EarthModel>WGS_84</EarthModel>
  <IARP>
    <ECF>
      <X>1234567.0</X>
      <Y>2345678.0</Y>
      <Z>3456789.0</Z>
    </ECF>
    <LLH>
      <Lat>38.0</Lat>
      <Lon>-77.0</Lon>
      <HAE>100.0</HAE>
    </LLH>
  </IARP>
  <ReferenceSurface>
    <Planar>
      <uIAX>
        <X>1.0</X>
        <Y>0.0</Y>
        <Z>0.0</Z>
      </uIAX>
      <uIAY>
        <X>0.0</X>
        <Y>1.0</Y>
        <Z>0.0</Z>
      </uIAY>
    </Planar>
  </ReferenceSurface>
  <ImageArea>
    <X1Y1>
      <X>0.0</X>
      <Y>0.0</Y>
    </X1Y1>
    <X2Y2>
      <X>1000.0</X>
      <Y>1000.0</Y>
    </X2Y2>
    <Polygon size="4">
      <Vertex index="1">
        <X>0.0</X>
        <Y>0.0</Y>
      </Vertex>
      <Vertex index="2">
        <X>1000.0</X>
        <Y>0.0</Y>
      </Vertex>
      <Vertex index="3">
        <X>1000.0</X>
        <Y>1000.0</Y>
      </Vertex>
      <Vertex index="4">
        <X>0.0</X>
        <Y>1000.0</Y>
      </Vertex>
    </Polygon>
  </ImageArea>
  <ImageAreaCornerPoints>
    <IACP index="1">
      <Lat>38.0</Lat>
      <Lon>-77.0</Lon>
    </IACP>
    <IACP index="2">
      <Lat>38.0</Lat>
      <Lon>-76.9</Lon>
    </IACP>
    <IACP index="3">
      <Lat>38.1</Lat>
      <Lon>-76.9</Lon>
    </IACP>
    <IACP index="4">
      <Lat>38.1</Lat>
      <Lon>-77.0</Lon>
    </IACP>
  </ImageAreaCornerPoints>
</SceneCoordinates>

<Data>
  <Support>
    <NumSupportArrays>1</NumSupportArrays>
    <SupportArray>
      <Identifier>SUPPORT1</Identifier>
      <NumRows>10</NumRows>
      <NumCols>10</NumCols>
      <BytesPerElement>4</BytesPerElement>
      <ArrayByteOffset>0</ArrayByteOffset>
    </SupportArray>
  </Support>
  <Transmit>
    <NumBytesPPP>128</NumBytesPPP>
    <NumTxSequences>1</NumTxSequences>
    <TxSequence>
      <Identifier>TXSEQ1</Identifier>
      <NumPulses>64</NumPulses>
      <PPPArrayByteOffset>0</PPPArrayByteOffset>
    </TxSequence>
  </Transmit>
  <Receive>
    <SignalArrayFormat>CI2</SignalArrayFormat>
    <NumBytesPVP>256</NumBytesPVP>
    <NumCRSDChannels>1</NumCRSDChannels>
    <Channel>
      <Identifier>CHAN1</Identifier>
      <NumVectors>128</NumVectors>
      <NumSamples>1024</NumSamples>
      <SignalArrayByteOffset>0</SignalArrayByteOffset>
      <PVPArrayByteOffset>0</PVPArrayByteOffset>
    </Channel>
  </Receive>
</Data>

<TxSequence>
  <RefTxID>TXSEQ1</RefTxID>
  <TxWFType>LFM</TxWFType>
  <Parameters>
    <Identifier>TXSEQ1_PARAM</Identifier>
    <RefPulseIndex>0</RefPulseIndex>
    <FxResponseId>FXRESP1</FxResponseId>
    <FxBWFixed>true</FxBWFixed>
    <FxC>1000000000.0</FxC>
    <FxBW>10000000.0</FxBW>
    <TXmtMin>0.0001</TXmtMin>
    <TXmtMax>0.001</TXmtMax>
    <TxTime1>0.0</TxTime1>
    <TxTime2>1.0</TxTime2>
    <TxAPCId>APC1</TxAPCId>
    <TxAPATId>APAT1</TxAPATId>
    <TxRefPoint>
      <ECF>
        <X>1234567.0</X>
        <Y>2345678.0</Y>
        <Z>3456789.0</Z>
      </ECF>
      <IAC>
        <X>0.0</X>
        <Y>0.0</Y>
      </IAC>
    </TxRefPoint>
    <TxPolarization>
      <PolarizationID>H</PolarizationID>
      <AmpH>1.0</AmpH>
      <AmpV>0.0</AmpV>
      <PhaseH>0.0</PhaseH>
      <PhaseV>0.0</PhaseV>
    </TxPolarization>
    <TxRefRadIntensity>1.0</TxRefRadIntensity>
    <TxRadIntErrorStdDev>0.01</TxRadIntErrorStdDev>
    <TxRefLAtm>0.0</TxRefLAtm>
  </Parameters>
</TxSequence>

<Channel>
  <RefChId>CHAN1</RefChId>
  <Parameters>
    <Identifier>CHAN1</Identifier>
    <RefVectorIndex>0</RefVectorIndex>
    <RefFreqFixed>true</RefFreqFixed>
    <FrcvFixed>true</FrcvFixed>
    <SignalNormal>true</SignalNormal>
    <F0Ref>1000000000.0</F0Ref>
    <Fs>20000000.0</Fs>
    <BWInst>10000000.0</BWInst>
    <RcvStartTime1>0.0</RcvStartTime1>
    <RcvStartTime2>1.0</RcvStartTime2>
    <FrcvMin>950000000.0</FrcvMin>
    <FrcvMax>1050000000.0</FrcvMax>
    <RcvAPCId>APC2</RcvAPCId>
    <RcvAPATId>APAT2</RcvAPATId>
    <RcvRefPoint>
      <ECF>
        <X>1234567.0</X>
        <Y>2345678.0</Y>
        <Z>3456789.0</Z>
      </ECF>
      <IAC>
        <X>0.0</X>
        <Y>0.0</Y>
      </IAC>
    </RcvRefPoint>
    <RcvPolarization>
      <PolarizationID>V</PolarizationID>
      <AmpH>0.0</AmpH>
      <AmpV>1.0</AmpV>
      <PhaseH>0.0</PhaseH>
      <PhaseV>0.0</PhaseV>
    </RcvPolarization>
    <RcvRefIrradiance>1.0</RcvRefIrradiance>
    <RcvIrradianceErrorStdDev>0.01</RcvIrradianceErrorStdDev>
    <RcvRefLAtm>0.0</RcvRefLAtm>
    <PNCRSD>0.0</PNCRSD>
    <BNCRSD>1.0</BNCRSD>
    <SARImage>
      <TxId>TXSEQ1</TxId>
      <RefVectorPulseIndex>0</RefVectorPulseIndex>
      <TxPolarization>
        <PolarizationID>H</PolarizationID>
        <AmpH>1.0</AmpH>
        <AmpV>0.0</AmpV>
        <PhaseH>0.0</PhaseH>
        <PhaseV>0.0</PhaseV>
      </TxPolarization>
      <DwellTimes>
        <Polynomials>
          <CODId>COD1</CODId>
          <DwellId>DWELL1</DwellId>
        </Polynomials>
      </DwellTimes>
      <ImageArea>
        <X1Y1>
          <X>0.0</X>
          <Y>0.0</Y>
        </X1Y1>
        <X2Y2>
          <X>1000.0</X>
          <Y>1000.0</Y>
        </X2Y2>
        <Polygon size="4">
          <Vertex index="1">
            <X>0.0</X>
            <Y>0.0</Y>
          </Vertex>
          <Vertex index="2">
            <X>1000.0</X>
            <Y>0.0</Y>
          </Vertex>
          <Vertex index="3">
            <X>1000.0</X>
            <Y>1000.0</Y>
          </Vertex>
          <Vertex index="4">
            <X>0.0</X>
            <Y>1000.0</Y>
          </Vertex>
        </Polygon>
      </ImageArea>
    </SARImage>
  </Parameters>
</Channel>

<ReferenceGeometry>
  <RefPoint>
    <ECF>
      <X>1234567.0</X>
      <Y>2345678.0</Y>
      <Z>3456789.0</Z>
    </ECF>
    <IAC>
      <X>0.0</X>
      <Y>0.0</Y>
    </IAC>
  </RefPoint>
  <SARImage>
    <CODTime>0.5</CODTime>
    <DwellTime>0.001</DwellTime>
    <ReferenceTime>0.5</ReferenceTime>
    <ARPPos>
      <X>1234567.0</X>
      <Y>2345678.0</Y>
      <Z>3456789.0</Z>
    </ARPPos>
    <ARPVel>
      <X>100.0</X>
      <Y>0.0</Y>
      <Z>0.0</Z>
    </ARPVel>
    <BistaticAngle>30.0</BistaticAngle>
    <BistaticAngleRate>0.0</BistaticAngleRate>
    <SideOfTrack>R</SideOfTrack>
    <SlantRange>10000.0</SlantRange>
    <GroundRange>9000.0</GroundRange>
    <DopplerConeAngle>45.0</DopplerConeAngle>
    <SquintAngle>0.0</SquintAngle>
    <AzimuthAngle>90.0</AzimuthAngle>
    <GrazeAngle>10.0</GrazeAngle>
    <IncidenceAngle>30.0</IncidenceAngle>
    <TwistAngle>0.0</TwistAngle>
    <SlopeAngle>5.0</SlopeAngle>
    <LayoverAngle>0.0</LayoverAngle>
  </SARImage>
  <TxParameters>
    <Time>0.5</Time>
    <APCPos>
      <X>1234567.0</X>
      <Y>2345678.0</Y>
      <Z>3456789.0</Z>
    </APCPos>
    <APCVel>
      <X>100.0</X>
      <Y>0.0</Y>
      <Z>0.0</Z>
    </APCVel>
    <SideOfTrack>R</SideOfTrack>
    <SlantRange>10000.0</SlantRange>
    <GroundRange>9000.0</GroundRange>
    <DopplerConeAngle>45.0</DopplerConeAngle>
    <SquintAngle>0.0</SquintAngle>
    <AzimuthAngle>90.0</AzimuthAngle>
    <GrazeAngle>10.0</GrazeAngle>
    <IncidenceAngle>30.0</IncidenceAngle>
  </TxParameters>
  <RcvParameters>
    <Time>0.5</Time>
    <APCPos>
      <X>1234567.0</X>
      <Y>2345678.0</Y>
      <Z>3456789.0</Z>
    </APCPos>
    <APCVel>
      <X>100.0</X>
      <Y>0.0</Y>
      <Z>0.0</Z>
    </APCVel>
    <SideOfTrack>R</SideOfTrack>
    <SlantRange>10000.0</SlantRange>
    <GroundRange>9000.0</GroundRange>
    <DopplerConeAngle>45.0</DopplerConeAngle>
    <SquintAngle>0.0</SquintAngle>
    <AzimuthAngle>90.0</AzimuthAngle>
    <GrazeAngle>10.0</GrazeAngle>
    <IncidenceAngle>30.0</IncidenceAngle>
  </RcvParameters>
</ReferenceGeometry>

<SupportArray>
  <AntGainPhase>
    <Identifier>AGP1</Identifier>
    <ElementFormat>Gain=F4;Phase=F4;</ElementFormat>
    <X0>0.0</X0>
    <Y0>0.0</Y0>
    <XSS>1.0</XSS>
    <YSS>1.0</YSS>
  </AntGainPhase>
  <FxResponseArray>
    <Identifier>FXRESP1</Identifier>
    <ElementFormat>Amp=F4;Phase=F4;</ElementFormat>
    <Fx0FXR>1000000000.0</Fx0FXR>
    <FxSSFXR>1000000.0</FxSSFXR>
  </FxResponseArray>
</SupportArray>

<PPP>
  <TxTime>
    <Offset>0</Offset>
    <Size>2</Size>
    <Format>Int=I8;Frac=F8;</Format>
  </TxTime>
  <TxPos>
    <Offset>16</Offset>
    <Size>3</Size>
    <Format>X=F8;Y=F8;Z=F8;</Format>
  </TxPos>
  <TxVel>
    <Offset>40</Offset>
    <Size>3</Size>
    <Format>X=F8;Y=F8;Z=F8;</Format>
  </TxVel>
  <FX1>
    <Offset>64</Offset>
    <Size>1</Size>
    <Format>F8</Format>
  </FX1>
  <FX2>
    <Offset>72</Offset>
    <Size>1</Size>
    <Format>F8</Format>
  </FX2>
  <TXmt>
    <Offset>80</Offset>
    <Size>1</Size>
    <Format>F8</Format>
  </TXmt>
  <PhiX0>
    <Offset>88</Offset>
    <Size>2</Size>
    <Format>Int=I8;Frac=F8;</Format>
  </PhiX0>
  <FxFreq0>
    <Offset>104</Offset>
    <Size>1</Size>
    <Format>F8</Format>
  </FxFreq0>
  <FxRate>
    <Offset>112</Offset>
    <Size>1</Size>
    <Format>F8</Format>
  </FxRate>
  <TxRadInt>
    <Offset>120</Offset>
    <Size>1</Size>
    <Format>F8</Format>
  </TxRadInt>
  <TxACX>
    <Offset>128</Offset>
    <Size>3</Size>
    <Format>X=F8;Y=F8;Z=F8;</Format>
  </TxACX>
  <TxACY>
    <Offset>152</Offset>
    <Size>3</Size>
    <Format>X=F8;Y=F8;Z=F8;</Format>
  </TxACY>
  <TxEB>
    <Offset>176</Offset>
    <Size>2</Size>
    <Format>DCX=F8;DCY=F8;</Format>
  </TxEB>
  <FxResponseIndex>
    <Offset>192</Offset>
    <Size>1</Size>
    <Format>I8</Format>
  </FxResponseIndex>
</PPP>

<PVP>
  <RcvStart>
    <Offset>0</Offset>
    <Size>2</Size>
    <Format>Int=I8;Frac=F8;</Format>
  </RcvStart>
  <RcvPos>
    <Offset>16</Offset>
    <Size>3</Size>
    <Format>X=F8;Y=F8;Z=F8;</Format>
  </RcvPos>
  <RcvVel>
    <Offset>40</Offset>
    <Size>3</Size>
    <Format>X=F8;Y=F8;Z=F8;</Format>
  </RcvVel>
  <FRCV1>
    <Offset>64</Offset>
    <Size>1</Size>
    <Format>F8</Format>
  </FRCV1>
  <FRCV2>
    <Offset>72</Offset>
    <Size>1</Size>
    <Format>F8</Format>
  </FRCV2>
  <RefPhi0>
    <Offset>80</Offset>
    <Size>2</Size>
    <Format>Int=I8;Frac=F8;</Format>
  </RefPhi0>
  <RefFreq>
    <Offset>96</Offset>
    <Size>1</Size>
    <Format>F8</Format>
  </RefFreq>
  <DFIC0>
    <Offset>104</Offset>
    <Size>1</Size>
    <Format>F8</Format>
  </DFIC0>
  <FICRate>
    <Offset>112</Offset>
    <Size>1</Size>
    <Format>F8</Format>
  </FICRate>
  <RcvACX>
    <Offset>120</Offset>
    <Size>3</Size>
    <Format>X=F8;Y=F8;Z=F8;</Format>
  </RcvACX>
  <RcvACY>
    <Offset>144</Offset>
    <Size>3</Size>
    <Format>X=F8;Y=F8;Z=F8;</Format>
  </RcvACY>
  <RcvEB>
    <Offset>168</Offset>
    <Size>2</Size>
    <Format>DCX=F8;DCY=F8;</Format>
  </RcvEB>
  <SIGNAL>
    <Offset>184</Offset>
    <Size>1</Size>
    <Format>I8</Format>
  </SIGNAL>
  <AmpSF>
    <Offset>192</Offset>
    <Size>1</Size>
    <Format>F8</Format>
  </AmpSF>
  <DGRGC>
    <Offset>200</Offset>
    <Size>1</Size>
    <Format>F8</Format>
  </DGRGC>
  <TxPulseIndex>
    <Offset>208</Offset>
    <Size>1</Size>
    <Format>I8</Format>
  </TxPulseIndex>
</PVP>

<Antenna>
  <NumACFs>1</NumACFs>
  <NumAPCs>2</NumAPCs>
  <NumAntPats>1</NumAntPats>
  <AntCoordFrame>
    <Identifier>ACF1</Identifier>
  </AntCoordFrame>
  <AntPhaseCenter>
    <Identifier>APC1</Identifier>
    <ACFId>ACF1</ACFId>
    <APCXYZ>
      <X>0.0</X>
      <Y>0.0</Y>
      <Z>0.0</Z>
    </APCXYZ>
  </AntPhaseCenter>
  <AntPhaseCenter>
    <Identifier>APC2</Identifier>
    <ACFId>ACF1</ACFId>
    <APCXYZ>
      <X>0.0</X>
      <Y>0.0</Y>
      <Z>0.0</Z>
    </APCXYZ>
  </AntPhaseCenter>
  <AntPattern>
    <Identifier>ANTPAT1</Identifier>
    <FreqZero>1000000000.0</FreqZero>
    <ArrayGPId>AGP1</ArrayGPId>
    <ElementGPId>EGP1</ElementGPId>
    <EBFreqShift>
      <DCXSF>1.0</DCXSF>
      <DCYSF>1.0</DCYSF>
    </EBFreqShift>
    <MLFreqDilation>
      <DCXSF>1.0</DCXSF>
      <DCYSF>1.0</DCYSF>
    </MLFreqDilation>
    <GainBSPoly order1="1">
      <Coef exponent1="0">1.0</Coef>
    </GainBSPoly>
    <AntPolRef>
      <AmpX>1.0</AmpX>
      <AmpY>0.0</AmpY>
      <PhaseX>0.0</PhaseX>
      <PhaseY>0.0</PhaseY>
    </AntPolRef>
  </AntPattern>
</Antenna>

<ErrorParameters>
  <SARImage>
    <Monostatic>
      <PosVelError>
        <Frame>ECF</Frame>
        <PVCov>
          <Entry index1="1" index2="1">1.0</Entry>
        </PVCov>
      </PosVelError>
      <RadarSensor>
        <TimeFreqCov>
          <Entry index1="1" index2="1">1.0</Entry>
        </TimeFreqCov>
      </RadarSensor>
    </Monostatic>
  </SARImage>
</ErrorParameters>

<GeoInfo name="ExampleGeoInfo">
  <Desc name="Description">Example geographic info</Desc>
  <Point>
    <Lat>38.0</Lat>
    <Lon>-77.0</Lon>
  </Point>
</GeoInfo>
)";
    return std::string(xmlBody);
}

std::string testCRSDXML(const std::string& version)
{
    auto uri = crsd::CRSDXMLControl::getVersionUriMap().at(version);
    return "<CRSD xmlns=\""
        + uri.value
        + "\">\n"
        + testCRSDXMLBody()
        + "</CRSD>\n";
    return "";
}

void runTest(const std::string& testName, const std::string& version)
{
    auto xmlString = testCRSDXML(version);
    io::StringStream crsdStream;
    crsdStream.write(xmlString.c_str(), xmlString.size());
    
    xml::lite::MinidomParser xmlParser;
    xmlParser.preserveCharacterData(true);  
    xmlParser.parse(crsdStream, crsdStream.available());      
    const std::unique_ptr<crsd::Metadata> metadata =
            crsd::CRSDXMLControl().fromXML(xmlParser.getDocument());
    
    // ProductInfo
    TEST_ASSERT_EQ(metadata->productInfo.productName, "EXAMPLE_PRODUCT");
    TEST_ASSERT_EQ(metadata->productInfo.classification, "UNCLASSIFIED");
    TEST_ASSERT_EQ(metadata->productInfo.releaseInfo, "Approved for public release");
    TEST_ASSERT_EQ(metadata->productInfo.countryCode, "USA");
    TEST_ASSERT_EQ(metadata->productInfo.profile, "DEFAULT");
    TEST_ASSERT_EQ(metadata->productInfo.creationInfo[0].application, "ExampleApp");
    TEST_ASSERT_EQ(metadata->productInfo.creationInfo[0].dateTime.getYear(), 2024);
    TEST_ASSERT_EQ(metadata->productInfo.creationInfo[0].site, "ExampleSite");
    

    // // CollectionID
    // TEST_ASSERT_EQ(metadata->collectionID.collectorName, "Collector");
    // TEST_ASSERT_EQ(metadata->collectionID.coreName, "Core");
    // TEST_ASSERT_EQ(metadata->collectionID.collectType,
    //                six::CollectType::MONOSTATIC);
    // TEST_ASSERT_EQ(metadata->collectionID.radarMode,
    //                six::RadarModeType::STRIPMAP);
    // TEST_ASSERT_EQ(metadata->collectionID.radarModeID, "Mode");
    // TEST_ASSERT_EQ(metadata->collectionID.getClassificationLevel(), "U");
    // TEST_ASSERT_EQ(metadata->collectionID.releaseInfo, "Release");
    // TEST_ASSERT_EQ(metadata->collectionID.countryCodes[0], "US");
    // TEST_ASSERT_EQ(metadata->collectionID.countryCodes[1], "GB");
    // TEST_ASSERT_EQ(metadata->collectionID.countryCodes[2], "AZ");

    // TEST_ASSERT_EQ(metadata->collectionID.parameters[0].getName(), "param1");
    // TEST_ASSERT_EQ(metadata->collectionID.parameters[0].str(), "val");

    // // Global
    // const crsd::Global& global = metadata->global;
    // TEST_ASSERT_EQ(global.domainType, crsd::DomainType::FX);
    // TEST_ASSERT_EQ(global.sgn, crsd::PhaseSGN::PLUS_1);
    // const crsd::Timeline& timeline = global.timeline;
    // TEST_ASSERT_EQ(timeline.collectionStart.getYear(), 2013);
    // TEST_ASSERT_EQ(timeline.rcvCollectionStart.getYear(), 2014);
    // TEST_ASSERT_EQ(timeline.txTime1, 1.3);
    // TEST_ASSERT_EQ(timeline.txTime2, 1.5);
    // TEST_ASSERT_EQ(global.fxBand.fxMin, 0.9);
    // TEST_ASSERT_EQ(global.fxBand.fxMax, 1.7);
    // TEST_ASSERT_EQ(global.toaSwath.toaMin, 3.4);
    // TEST_ASSERT_EQ(global.toaSwath.toaMax, 6.1);
    // TEST_ASSERT_TRUE(global.tropoParameters.get() != nullptr);
    // TEST_ASSERT_EQ(global.tropoParameters->n0, 65.2);
    // TEST_ASSERT_EQ(global.tropoParameters->refHeight, crsd::RefHeight::IARP);
    // TEST_ASSERT_TRUE(global.ionoParameters.get() != nullptr);
    // TEST_ASSERT_EQ(global.ionoParameters->tecv, 5.8);
    // TEST_ASSERT_EQ(global.ionoParameters->f2Height, 3);

    // // Scene Coordinates
    // const crsd::SceneCoordinates& scene = metadata->sceneCoordinates;
    // TEST_ASSERT_EQ(scene.earthModel, crsd::EarthModelType::WGS_84);
    // TEST_ASSERT_EQ(scene.iarp.ecf[0], 1.2);
    // TEST_ASSERT_EQ(scene.iarp.ecf[1], 2.3);
    // TEST_ASSERT_EQ(scene.iarp.ecf[2], 3.4);

    // TEST_ASSERT_EQ(scene.iarp.llh.getLat(), 45);
    // TEST_ASSERT_EQ(scene.iarp.llh.getLon(), -102);
    // TEST_ASSERT_EQ(scene.iarp.llh.getAlt(), 3.4);

    // TEST_ASSERT_TRUE(scene.referenceSurface.hae.get() != nullptr);
    // TEST_ASSERT_EQ(scene.referenceSurface.hae->uIax.getLat(), 12);
    // TEST_ASSERT_EQ(scene.referenceSurface.hae->uIax.getLon(), 24);
    // TEST_ASSERT_EQ(scene.referenceSurface.hae->uIay.getLat(), 36);
    // TEST_ASSERT_EQ(scene.referenceSurface.hae->uIay.getLon(), 48);

    // TEST_ASSERT_EQ(scene.imageArea.x1y1[0], 3.5);
    // TEST_ASSERT_EQ(scene.imageArea.x1y1[1], 5.3);
    // TEST_ASSERT_EQ(scene.imageArea.x2y2[0], 5.3);
    // TEST_ASSERT_EQ(scene.imageArea.x2y2[1], 3.5);
    // TEST_ASSERT_EQ(scene.imageArea.polygon.size(), static_cast<size_t>(3));
    // TEST_ASSERT_EQ(scene.imageArea.polygon[0][0], .1);
    // TEST_ASSERT_EQ(scene.imageArea.polygon[0][1], .3);
    // TEST_ASSERT_EQ(scene.imageArea.polygon[1][0], .4);
    // TEST_ASSERT_EQ(scene.imageArea.polygon[1][1], .2);
    // TEST_ASSERT_EQ(scene.imageArea.polygon[2][0], .5);
    // TEST_ASSERT_EQ(scene.imageArea.polygon[2][1], .9);

    // TEST_ASSERT_EQ(scene.imageAreaCorners.upperLeft.getLat(), 10);
    // TEST_ASSERT_EQ(scene.imageAreaCorners.upperLeft.getLon(), 11);

    // TEST_ASSERT_EQ(scene.imageAreaCorners.upperRight.getLat(), 20);
    // TEST_ASSERT_EQ(scene.imageAreaCorners.upperRight.getLon(), 21);

    // TEST_ASSERT_EQ(scene.imageAreaCorners.lowerRight.getLat(), 30);
    // TEST_ASSERT_EQ(scene.imageAreaCorners.lowerRight.getLon(), 31);

    // TEST_ASSERT_EQ(scene.imageAreaCorners.lowerLeft.getLat(), 40);
    // TEST_ASSERT_EQ(scene.imageAreaCorners.lowerLeft.getLon(), 41);

    // TEST_ASSERT_TRUE(scene.imageGrid.get() != nullptr);
    // TEST_ASSERT_EQ(scene.imageGrid->identifier, "Grid");
    // TEST_ASSERT_EQ(scene.imageGrid->iarpLocation.line, 1.23);
    // TEST_ASSERT_EQ(scene.imageGrid->iarpLocation.sample, 3.21);
    // TEST_ASSERT_EQ(scene.imageGrid->xExtent.lineSpacing, 3.14);
    // TEST_ASSERT_EQ(scene.imageGrid->xExtent.firstLine, 4);
    // TEST_ASSERT_EQ(scene.imageGrid->xExtent.numLines, static_cast<size_t>(50));
    // TEST_ASSERT_EQ(scene.imageGrid->yExtent.sampleSpacing, 6.28);
    // TEST_ASSERT_EQ(scene.imageGrid->yExtent.firstSample, 8);
    // TEST_ASSERT_EQ(scene.imageGrid->yExtent.numSamples, static_cast<size_t>(100));
    // TEST_ASSERT_EQ(scene.imageGrid->segments.size(), static_cast<size_t>(2));
    // TEST_ASSERT_EQ(scene.imageGrid->segments[0].startLine, 0);
    // TEST_ASSERT_EQ(scene.imageGrid->segments[0].startSample, 1);
    // TEST_ASSERT_EQ(scene.imageGrid->segments[0].endLine, 2);
    // TEST_ASSERT_EQ(scene.imageGrid->segments[0].endSample, 3);
    // TEST_ASSERT_EQ(scene.imageGrid->segments[0].polygon.size(), static_cast<size_t>(3));
    // TEST_ASSERT_EQ(scene.imageGrid->segments[0].polygon[0].line, 0.4);
    // TEST_ASSERT_EQ(scene.imageGrid->segments[0].polygon[0].sample, 0.6);
    // TEST_ASSERT_EQ(scene.imageGrid->segments[0].polygon[1].line, 0.8);
    // TEST_ASSERT_EQ(scene.imageGrid->segments[0].polygon[1].sample, 1.2);
    // TEST_ASSERT_EQ(scene.imageGrid->segments[0].polygon[2].line, 1.2);
    // TEST_ASSERT_EQ(scene.imageGrid->segments[0].polygon[2].sample, 1.8);

    // // Data
    // const crsd::Data& data = metadata->data;
    // TEST_ASSERT_EQ(data.signalArrayFormat, crsd::SignalArrayFormat::CI4);
    // TEST_ASSERT_EQ(data.numBytesPVP, static_cast<size_t>(24));
    // TEST_ASSERT_EQ(data.channels.size(), static_cast<size_t>(2));
    // TEST_ASSERT_EQ(data.channels[0].identifier, "Channel");
    // TEST_ASSERT_EQ(data.channels[0].numVectors, static_cast<size_t>(2));
    // TEST_ASSERT_EQ(data.channels[0].numSamples, static_cast<size_t>(3));
    // TEST_ASSERT_EQ(data.channels[0].signalArrayByteOffset, static_cast<size_t>(0));
    // TEST_ASSERT_EQ(data.channels[0].pvpArrayByteOffset, static_cast<size_t>(1));
    // TEST_ASSERT_EQ(data.channels[0].compressedSignalSize, static_cast<size_t>(3));
    // TEST_ASSERT_EQ(data.signalCompressionID, "Compress");
    // TEST_ASSERT_EQ(data.supportArrayMap.size(), static_cast<size_t>(3));
    // const std::string identifier = "1.0";
    // TEST_ASSERT_EQ(data.supportArrayMap.count(identifier), static_cast<size_t>(1));
    // TEST_ASSERT_EQ(data.supportArrayMap.find(identifier)->second.numRows, static_cast<size_t>(3));
    // TEST_ASSERT_EQ(data.supportArrayMap.find(identifier)->second.numCols, static_cast<size_t>(4));
    // TEST_ASSERT_EQ(data.supportArrayMap.find(identifier)->second.bytesPerElement, static_cast<size_t>(8));
    // TEST_ASSERT_EQ(data.supportArrayMap.find(identifier)->second.arrayByteOffset, static_cast<size_t>(0));

    // // Channel
    // const crsd::Channel& channel = metadata->channel;
    // TEST_ASSERT_EQ(channel.refChId, "ChId");
    // TEST_ASSERT_EQ(channel.fxFixedCrsd, six::BooleanType::IS_TRUE);
    // TEST_ASSERT_EQ(channel.toaFixedCrsd, six::BooleanType::IS_FALSE);
    // TEST_ASSERT_EQ(channel.srpFixedCrsd, six::BooleanType::IS_TRUE);
    // TEST_ASSERT_EQ(channel.parameters.size(), static_cast<size_t>(1));
    // TEST_ASSERT_EQ(channel.parameters[0].identifier, "CPI");
    // TEST_ASSERT_EQ(channel.parameters[0].refVectorIndex, static_cast<size_t>(1));
    // TEST_ASSERT_EQ(channel.parameters[0].fxFixed,
    //                six::BooleanType::IS_FALSE);
    // TEST_ASSERT_EQ(channel.parameters[0].toaFixed,
    //                six::BooleanType::IS_TRUE);
    // TEST_ASSERT_EQ(channel.parameters[0].srpFixed,
    //                six::BooleanType::IS_TRUE);
    // TEST_ASSERT_EQ(channel.parameters[0].signalNormal,
    //                six::BooleanType::IS_FALSE);

    // TEST_ASSERT_EQ(channel.parameters[0].polarization.txPol,
    //                crsd::PolarizationType::X);
    // TEST_ASSERT_EQ(channel.parameters[0].polarization.rcvPol,
    //                crsd::PolarizationType::RHC);

    // TEST_ASSERT_EQ(channel.parameters[0].fxC, 1.3);
    // TEST_ASSERT_EQ(channel.parameters[0].fxBW, 0.8);
    // TEST_ASSERT_EQ(channel.parameters[0].toaSaved, 2.7);
    // TEST_ASSERT_EQ(channel.parameters[0].toaExtended->toaExtSaved, 1.0);
    // TEST_ASSERT_EQ(channel.parameters[0].toaExtended->lfmEclipse->fxEarlyLow, 1.0);
    // TEST_ASSERT_EQ(channel.parameters[0].toaExtended->lfmEclipse->fxEarlyHigh, 2.0);
    // TEST_ASSERT_EQ(channel.parameters[0].toaExtended->lfmEclipse->fxLateLow, 1.0);
    // TEST_ASSERT_EQ(channel.parameters[0].toaExtended->lfmEclipse->fxLateHigh, 2.0);

    // TEST_ASSERT_EQ(channel.parameters[0].dwellTimes.codId, "CODPolynomial");
    // TEST_ASSERT_EQ(channel.parameters[0].dwellTimes.dwellId, "DwellPolynomial");

    // TEST_ASSERT_EQ(channel.parameters[0].imageArea.x1y1[0], 3.5);
    // TEST_ASSERT_EQ(channel.parameters[0].imageArea.x1y1[1], 5.3);
    // TEST_ASSERT_EQ(channel.parameters[0].imageArea.x2y2[0], 5.3);
    // TEST_ASSERT_EQ(channel.parameters[0].imageArea.x2y2[1], 3.5);

    // TEST_ASSERT_EQ(channel.parameters[0].imageArea.polygon[0][0], 0.1);
    // TEST_ASSERT_EQ(channel.parameters[0].imageArea.polygon[0][1], 0.3);
    // TEST_ASSERT_EQ(channel.parameters[0].imageArea.polygon[1][0], 0.4);
    // TEST_ASSERT_EQ(channel.parameters[0].imageArea.polygon[1][1], 0.2);
    // TEST_ASSERT_EQ(channel.parameters[0].imageArea.polygon[2][0], 0.5);
    // TEST_ASSERT_EQ(channel.parameters[0].imageArea.polygon[2][1], 0.9);

    // TEST_ASSERT_EQ(channel.parameters[0].antenna->txAPCId, "TxAPCId");
    // TEST_ASSERT_EQ(channel.parameters[0].antenna->txAPATId, "TxAPATId");
    // TEST_ASSERT_EQ(channel.parameters[0].antenna->rcvAPCId, "RcvAPCId");
    // TEST_ASSERT_EQ(channel.parameters[0].antenna->rcvAPATId, "RcvAPATId");

    // TEST_ASSERT_EQ(channel.parameters[0].txRcv->txWFId[0], "TxWFId");
    // TEST_ASSERT_EQ(channel.parameters[0].txRcv->rcvId[0], "RcvId");

    // TEST_ASSERT_EQ(channel.parameters[0].tgtRefLevel->ptRef, 12.0);

    // TEST_ASSERT_EQ(channel.parameters[0].noiseLevel->pnRef, 0.5);
    // TEST_ASSERT_EQ(channel.parameters[0].noiseLevel->bnRef, 0.8);

    // TEST_ASSERT_EQ(channel.parameters[0].noiseLevel->fxNoiseProfile->point.size(), static_cast<size_t>(2));
    // TEST_ASSERT_EQ(channel.parameters[0].noiseLevel->fxNoiseProfile->point[0].fx, 0.3);
    // TEST_ASSERT_EQ(channel.parameters[0].noiseLevel->fxNoiseProfile->point[0].pn, 2.7);
    // TEST_ASSERT_EQ(channel.parameters[0].noiseLevel->fxNoiseProfile->point[1].fx, 0.5);
    // TEST_ASSERT_EQ(channel.parameters[0].noiseLevel->fxNoiseProfile->point[1].pn, 2.7);

    // TEST_ASSERT_EQ(channel.addedParameters[0].getName(), "AddedParameter1");
    // TEST_ASSERT_EQ(channel.addedParameters[0].str(), "Param");
    // TEST_ASSERT_EQ(channel.addedParameters[1].getName(), "AddedParameter2");
    // TEST_ASSERT_EQ(channel.addedParameters[0].str(), "Param");


    // //PVP
    // const crsd::Pvp& pvp = metadata->pvp;
    // TEST_ASSERT_EQ(pvp.txTime.getOffset(), static_cast<size_t>(0));
    // TEST_ASSERT_EQ(pvp.txTime.getSize(), static_cast<size_t>(1));
    // TEST_ASSERT_EQ(pvp.txTime.getFormat(), "F8");
    // TEST_ASSERT_EQ(pvp.txPos.getOffset(), static_cast<size_t>(1));
    // TEST_ASSERT_EQ(pvp.txPos.getSize(), static_cast<size_t>(3));
    // TEST_ASSERT_EQ(pvp.txPos.getFormat(), "X=F8;Y=F8;Z=F8;");
    // TEST_ASSERT_EQ(pvp.rcvVel.getOffset(), static_cast<size_t>(11));
    // TEST_ASSERT_EQ(pvp.rcvVel.getSize(), static_cast<size_t>(3));
    // TEST_ASSERT_EQ(pvp.rcvVel.getFormat(), "X=F8;Y=F8;Z=F8;");
    // TEST_ASSERT_EQ(pvp.addedPVP.size(), static_cast<size_t>(2));
    // TEST_ASSERT_EQ(pvp.addedPVP.find("newParam1")->second.getName(), "newParam1");
    // TEST_ASSERT_EQ(pvp.addedPVP.find("newParam1")->second.getOffset(), static_cast<size_t>(27));
    // TEST_ASSERT_EQ(pvp.addedPVP.find("newParam2")->second.getName(), "newParam2");
    // TEST_ASSERT_EQ(pvp.addedPVP.find("newParam2")->second.getOffset(), static_cast<size_t>(28));

    // //Dwell
    // const crsd::Dwell& dwell = metadata->dwell;
    // TEST_ASSERT_EQ(dwell.cod.size(), static_cast<size_t>(1));
    // TEST_ASSERT_EQ(dwell.dtime.size(), static_cast<size_t>(1));
    // TEST_ASSERT_EQ(dwell.cod[0].identifier, "codPolynomial1");
    // TEST_ASSERT_EQ(dwell.dtime[0].identifier, "dwellPolynomial1");
    // TEST_ASSERT_EQ(dwell.cod[0].codTimePoly.orderX(), static_cast<size_t>(1));
    // TEST_ASSERT_EQ(dwell.cod[0].codTimePoly.orderY(), static_cast<size_t>(1));
    // TEST_ASSERT_EQ(dwell.dtime[0].dwellTimePoly.orderX(), static_cast<size_t>(1));
    // TEST_ASSERT_EQ(dwell.dtime[0].dwellTimePoly.orderY(), static_cast<size_t>(1));

    // // ReferenceGeometry
    // const crsd::ReferenceGeometry& ref = metadata->referenceGeometry;
    // TEST_ASSERT_EQ(ref.srp.ecf[0], 1.0);
    // TEST_ASSERT_EQ(ref.srp.ecf[1], 2.0);
    // TEST_ASSERT_EQ(ref.srp.ecf[2], 3.5);
    // TEST_ASSERT_EQ(ref.srp.iac[0], 1.5);
    // TEST_ASSERT_EQ(ref.srp.iac[1], 2.5);
    // TEST_ASSERT_EQ(ref.srp.iac[2], 4.0);
    // TEST_ASSERT_EQ(ref.referenceTime, 0.0);
    // TEST_ASSERT_EQ(ref.srpCODTime, 23.0);
    // TEST_ASSERT_EQ(ref.srpDwellTime, 25.0);
    // TEST_ASSERT_EQ(ref.monostatic->arpPos[0], 10);
    // TEST_ASSERT_EQ(ref.monostatic->arpPos[1], 10);
    // TEST_ASSERT_EQ(ref.monostatic->arpPos[2], 10);
    // TEST_ASSERT_EQ(ref.monostatic->arpVel[0], 10);
    // TEST_ASSERT_EQ(ref.monostatic->arpVel[1], 10);
    // TEST_ASSERT_EQ(ref.monostatic->arpVel[2], 10);
    // TEST_ASSERT(ref.monostatic->sideOfTrack.toString() == "LEFT");
    // TEST_ASSERT_EQ(ref.monostatic->sideOfTrack, six::SideOfTrackType::LEFT);
    // TEST_ASSERT_EQ(ref.monostatic->azimuthAngle, 30.0);
    // TEST_ASSERT_EQ(ref.monostatic->grazeAngle, 30.0);
    // TEST_ASSERT_EQ(ref.monostatic->twistAngle, 30.0);
    // TEST_ASSERT_EQ(ref.monostatic->slopeAngle, 30.0);
    // TEST_ASSERT_EQ(ref.monostatic->layoverAngle, 30.0);
    // TEST_ASSERT_EQ(ref.monostatic->dopplerConeAngle, 30.0);
}
}

TEST_CASE(testVersions)
{
    auto versionUriMap = crsd::CRSDXMLControl::getVersionUriMap();
    for (auto version : {"1.0.0"})
    {
        TEST_ASSERT_TRUE(
            versionUriMap.find(version) != versionUriMap.end());
    }
}

TEST_CASE(testReadXML)
{
    for (auto pair : crsd::CRSDXMLControl::getVersionUriMap())
    {
        auto& version = pair.first;
        runTest("testReadXML" + version, version);
    }
}

TEST_MAIN(
    TEST_CHECK(testVersions);
    TEST_CHECK(testReadXML);
)
