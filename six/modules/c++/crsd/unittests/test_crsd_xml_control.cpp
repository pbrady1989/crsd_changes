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
std::string testCRSDsarXMLBody()
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

std::string testCRSDtxXMLBody()
{
    const char* xmlBody = R"(<ProductInfo>
    <ProductName>EXAMPLE_TX_PRODUCT</ProductName>
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

  <TransmitInfo>
    <SensorName>TX_SENSOR</SensorName>
    <EventName>TX_EVENT</EventName>
  </TransmitInfo>

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

  <Global>
    <CollectionRefTime>2024-12-30T12:00:00Z</CollectionRefTime>
    <Transmit>
      <TxTime1>0.0</TxTime1>
      <TxTime2>1.0</TxTime2>
      <FxMin>950000000.0</FxMin>
      <FxMax>1050000000.0</FxMax>
    </Transmit>
  </Global>

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

  <Antenna>
    <NumACFs>1</NumACFs>
    <NumAPCs>1</NumAPCs>
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
  </Data>

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
  </ReferenceGeometry>

  <ErrorParameters>
    <Tx>
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
    </Tx>
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

std::string testCRSDrcvXMLBody()
{
    const char* xmlBody = R"(<ProductInfo>
    <ProductName>EXAMPLE_RCV_PRODUCT</ProductName>
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

  <ReceiveInfo>
    <SensorName>RX_SENSOR</SensorName>
    <EventName>RX_EVENT</EventName>
  </ReceiveInfo>

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

  <Global>
    <CollectionRefTime>2024-12-30T12:00:00Z</CollectionRefTime>
    <Receive>
      <RcvStartTime1>0.0</RcvStartTime1>
      <RcvStartTime2>1.0</RcvStartTime2>
      <FrcvMin>950000000.0</FrcvMin>
      <FrcvMax>1050000000.0</FrcvMax>
    </Receive>
  </Global>

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

  <Antenna>
    <NumACFs>1</NumACFs>
    <NumAPCs>1</NumAPCs>
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
        <AmpX>0.0</AmpX>
        <AmpY>1.0</AmpY>
        <PhaseX>0.0</PhaseX>
        <PhaseY>0.0</PhaseY>
      </AntPolRef>
    </AntPattern>
  </Antenna>

  <ErrorParameters>
    <Rcv>
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
    </Rcv>
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

std::string testCRSDsarXML(const std::string& version)
{
    auto uri = crsd::CRSDXMLControl::getVersionUriMap().at(version);
    return "<CRSD xmlns=\""
        + uri.value
        + "\">\n"
        + testCRSDsarXMLBody()
        + "</CRSD>\n";
    return "";
}

std::string testCRSDtxXML(const std::string& version)
{
    auto uri = crsd::CRSDXMLControl::getVersionUriMap().at(version);
    return "<CRSD xmlns=\""
        + uri.value
        + "\">\n"
        + testCRSDtxXMLBody()
        + "</CRSD>\n";
    return "";
}

std::string testCRSDrcvXML(const std::string& version)
{
    auto uri = crsd::CRSDXMLControl::getVersionUriMap().at(version);
    return "<CRSD xmlns=\""
        + uri.value
        + "\">\n"
        + testCRSDrcvXMLBody()
        + "</CRSD>\n";
    return "";
}

void runTestSAR(const std::string& testName, const std::string& version)
{
    auto xmlString = testCRSDsarXML(version);
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
    
    //SARInfo
    TEST_ASSERT_EQ(metadata->sarInfo->collectType,crsd::CollectType::MONOSTATIC);
    TEST_ASSERT_EQ(metadata->sarInfo->radarMode.toString(), "SPOTLIGHT");
    TEST_ASSERT_EQ(metadata->sarInfo->radarModeID, "MODE1");

    // TransmitInfo
    TEST_ASSERT_EQ(metadata->transmitInfo->sensorName, "TX_SENSOR");
    TEST_ASSERT_EQ(metadata->transmitInfo->eventName, "TX_EVENT");

    // ReceiveInfo
    TEST_ASSERT_EQ(metadata->receiveInfo->sensorName, "RX_SENSOR");
    TEST_ASSERT_EQ(metadata->receiveInfo->eventName, "RX_EVENT");

    // Global
    TEST_ASSERT_EQ(metadata->global.collectionRefTime.getYear(), 2024);
    TEST_ASSERT_EQ(metadata->global.transmitParameters->txTime1, 0.0);
    TEST_ASSERT_EQ(metadata->global.transmitParameters->txTime2, 1.0);
    TEST_ASSERT_EQ(metadata->global.transmitParameters->fxMin, 950000000.0);
    TEST_ASSERT_EQ(metadata->global.transmitParameters->fxMax, 1050000000.0);
    TEST_ASSERT_EQ(metadata->global.receiveParameters->rcvStartTime1, 0.0);
    TEST_ASSERT_EQ(metadata->global.receiveParameters->rcvStartTime2, 1.0);
    TEST_ASSERT_EQ(metadata->global.receiveParameters->fRcvMin, 950000000.0);
    TEST_ASSERT_EQ(metadata->global.receiveParameters->fRcvMax, 1050000000.0);

    // SceneCoordinates
    TEST_ASSERT_EQ(metadata->sceneCoordinates.earthModel, crsd::EarthModelType::WGS_84);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.ecf[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.ecf[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.ecf[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.llh.getLat(), 38.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.llh.getLon(), -77.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.llh.getAlt(), 100.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.referenceSurface.planar->uIax[0], 1.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.referenceSurface.planar->uIax[1], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.referenceSurface.planar->uIay[0], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.referenceSurface.planar->uIay[1], 1.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.x1y1[0], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.x1y1[1], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.x2y2[0], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.x2y2[1], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon.size(), 4);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[0][0], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[0][1], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[1][0], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[1][1], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[2][0], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[2][1], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[3][0], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[3][1], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.upperLeft.getLat(), 38.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.upperLeft.getLon(), -77.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.upperRight.getLat(), 38.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.upperRight.getLon(), -76.9);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.lowerRight.getLat(), 38.1);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.lowerRight.getLon(), -76.9);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.lowerLeft.getLat(), 38.1);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.lowerLeft.getLon(), -77.0);
    TEST_ASSERT_TRUE(metadata->sceneCoordinates.imageGrid.get() == nullptr);
 
    // Data
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.size(), 1);
    TEST_ASSERT_TRUE(metadata->data.supportArrayMap.find("SUPPORT1") !=
                   metadata->data.supportArrayMap.end());
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.at("SUPPORT1").numRows, 10);
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.at("SUPPORT1").numCols, 10);
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.at("SUPPORT1").bytesPerElement, 4);
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.at("SUPPORT1").arrayByteOffset, 0);
    TEST_ASSERT_EQ(metadata->data.transmitParameters->numBytesPPP, 128);
    TEST_ASSERT_EQ(metadata->data.transmitParameters->txSequence.size(), 1);
    TEST_ASSERT_TRUE(metadata->data.transmitParameters->txSequence[0].identifier == "TXSEQ1");
    TEST_ASSERT_EQ(metadata->data.transmitParameters->txSequence[0].numPulses, 64);
    TEST_ASSERT_EQ(metadata->data.transmitParameters->txSequence[0].pppArrayByteOffset, 0);
    TEST_ASSERT_EQ(metadata->data.receiveParameters->signalArrayFormat, crsd::SignalArrayFormat::CI2);
    TEST_ASSERT_EQ(metadata->data.receiveParameters->numBytesPVP, 256);
    TEST_ASSERT_EQ(metadata->data.receiveParameters->getNumCRSDChannels(), 1);
    TEST_ASSERT_TRUE(metadata->data.receiveParameters->channels[0].identifier  == "CHAN1");
    TEST_ASSERT_EQ(metadata->data.receiveParameters->channels[0].numVectors, 128);
    TEST_ASSERT_EQ(metadata->data.receiveParameters->channels[0].numSamples, 1024);
    TEST_ASSERT_EQ(metadata->data.receiveParameters->channels[0].signalArrayByteOffset, 0);
    TEST_ASSERT_EQ(metadata->data.receiveParameters->channels[0].pvpArrayByteOffset, 0);

    // TxSequence
    TEST_ASSERT_TRUE(metadata->txSequence->refTxId == "TXSEQ1");
    TEST_ASSERT_TRUE(metadata->txSequence->txWFType.toString() == "LFM");
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].identifier, "TXSEQ1_PARAM");
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].refPulseIndex, 0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].fxResponseId, "FXRESP1");
    TEST_ASSERT_TRUE(metadata->txSequence->parameters[0].fxBWFixed);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].fxC, 1000000000.0);  
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].fxBW, 10000000.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txmtMin, 0.0001);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txmtMax, 0.001);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txTime1, 0.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txTime2, 1.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txAPCId, "APC1");
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txAPATId, "APAT1");
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefPoint.ecf[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefPoint.ecf[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefPoint.ecf[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefPoint.iac[0], 0.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefPoint.iac[1], 0.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txPolarization.polarizationID.toString(), "H");
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txPolarization.ampH, 1.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txPolarization.ampV, 0.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txPolarization.phaseH, 0.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txPolarization.phaseV, 0.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefRadIntensity, 1.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefRadIntensityStdDev, 0.01);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefLAtm, 0.0);

    // Channel
    TEST_ASSERT_TRUE(metadata->channel->refChId == "CHAN1");
    TEST_ASSERT_EQ(metadata->channel->parameters[0].identifier, "CHAN1");
    TEST_ASSERT_EQ(metadata->channel->parameters[0].refVectorIndex, 0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].refFreqFixed, true);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].fRcvFixed, true);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].signalNormal, true);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].f0Ref, 1000000000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].fs, 20000000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].bwInst, 10000000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvStartTime1, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvStartTime2, 1.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].fRcvMin, 950000000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].fRcvMax, 1050000000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvAPCId, "APC2");
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvAPATId, "APAT2");
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefPoint.ecf[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefPoint.ecf[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefPoint.ecf[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefPoint.iac[0], 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefPoint.iac[1], 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvPolarization.polarizationID.toString(), "V");
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvPolarization.ampH, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvPolarization.ampV, 1.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvPolarization.phaseH, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvPolarization.phaseV, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefIrradiance, 1.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefIrradianceErrorStdDev, 0.01);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefLAtm, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].pncrsd, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].bncrsd, 1.0);
    TEST_ASSERT_TRUE(metadata->channel->parameters[0].sarImage.get() != nullptr);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->txID, "TXSEQ1");
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->refVectorPulseIndex, 0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->txPolarization.polarizationID.toString(), "H");
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->txPolarization.ampH, 1.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->txPolarization.ampV, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->txPolarization.phaseH, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->txPolarization.phaseV, 0.0);  
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->dwellTime.codId, "COD1");    
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->dwellTime.dwellId, "DWELL1");
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->imageArea.x1y1[0], 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->imageArea.x1y1[1], 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->imageArea.x2y2[0], 1000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->imageArea.x2y2[1], 1000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->imageArea.polygon.size(), 4);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->imageArea.polygon[0][0], 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->imageArea.polygon[0][1], 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->imageArea.polygon[1][0], 1000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->imageArea.polygon[1][1], 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->imageArea.polygon[2][0], 1000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->imageArea.polygon[2][1], 1000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->imageArea.polygon[3][0], 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].sarImage->imageArea.polygon[3][1], 1000.0);

    // ReferenceGeometry
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.ecf[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.ecf[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.ecf[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.iac[0], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.iac[1], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->codTime, 0.5);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->dwellTime, 0.001);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->referenceTime, 0.5);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->arpPos[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->arpPos[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->arpPos[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->arpVel[0], 100.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->arpVel[1], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->arpVel[2], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->bistaticAngle, 30.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->bistaticAngleRate, 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->sideOfTrack.toString(), "RIGHT");
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->slantRange, 10000.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->groundRange, 9000.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->dopplerConeAngle, 45.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->squintAngle, 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->azimuthAngle, 90.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->grazeAngle, 10.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->incidenceAngle, 30.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->twistAngle, 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->slopeAngle, 5.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.sarParameters->layoverAngle, 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->time, 0.5);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->apcPos[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->apcPos[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->apcPos[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->apcVel[0], 100.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->apcVel[1], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->apcVel[2], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->sideOfTrack.toString(), "RIGHT");
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->slantRange, 10000.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->groundRange, 9000.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->dopplerConeAngle, 45.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->squintAngle, 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->azimuthAngle, 90.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->grazeAngle, 10.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->incidenceAngle, 30.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->time, 0.5);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->apcPos[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->apcPos[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->apcPos[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->apcVel[0], 100.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->apcVel[1], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->apcVel[2], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->sideOfTrack.toString(), "RIGHT");
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->slantRange, 10000.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->groundRange, 9000.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->dopplerConeAngle, 45.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->squintAngle, 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->azimuthAngle, 90.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->grazeAngle, 10.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->incidenceAngle, 30.0);

    // SupportArray
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase.size(), 1);
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].getIdentifier(), "AGP1");
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].elementFormat, "Gain=F4;Phase=F4;");
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].x0, 0.0);
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].y0, 0.0);
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].xSS, 1.0);
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].ySS, 1.0);
    TEST_ASSERT_EQ(metadata->supportArray.fxResponseArray.size(), 1);
    TEST_ASSERT_EQ(metadata->supportArray.fxResponseArray[0].getIdentifier(), "FXRESP1");
    TEST_ASSERT_EQ(metadata->supportArray.fxResponseArray[0].elementFormat, "Amp=F4;Phase=F4;");
    TEST_ASSERT_EQ(metadata->supportArray.fxResponseArray[0].fx0FXR, 1000000000.0);
    TEST_ASSERT_EQ(metadata->supportArray.fxResponseArray[0].fxSSFXR, 1000000.0);

    // PPP
    TEST_ASSERT_EQ(metadata->ppp->txTime.getSize(), 2);
    TEST_ASSERT_EQ(metadata->ppp->txTime.getOffset(), 0);
    TEST_ASSERT_EQ(metadata->ppp->txTime.getFormat(), "Int=I8;Frac=F8;");
    TEST_ASSERT_EQ(metadata->ppp->txPos.getSize(), 3);
    TEST_ASSERT_EQ(metadata->ppp->txPos.getOffset(), 16);
    TEST_ASSERT_EQ(metadata->ppp->txPos.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->ppp->txVel.getSize(), 3);
    TEST_ASSERT_EQ(metadata->ppp->txVel.getOffset(), 40);
    TEST_ASSERT_EQ(metadata->ppp->txVel.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->ppp->fx1.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->fx1.getOffset(), 64);
    TEST_ASSERT_EQ(metadata->ppp->fx1.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->ppp->fx2.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->fx2.getOffset(), 72);
    TEST_ASSERT_EQ(metadata->ppp->fx2.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->ppp->txmt.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->txmt.getOffset(), 80);
    TEST_ASSERT_EQ(metadata->ppp->txmt.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->ppp->phiX0.getSize(), 2);
    TEST_ASSERT_EQ(metadata->ppp->phiX0.getOffset(), 88);
    TEST_ASSERT_EQ(metadata->ppp->phiX0.getFormat(), "Int=I8;Frac=F8;");
    TEST_ASSERT_EQ(metadata->ppp->fxFreq0.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->fxFreq0.getOffset(), 104);
    TEST_ASSERT_EQ(metadata->ppp->fxFreq0.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->ppp->fxRate.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->fxRate.getOffset(), 112);
    TEST_ASSERT_EQ(metadata->ppp->fxRate.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->ppp->txRadInt.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->txRadInt.getOffset(), 120);
    TEST_ASSERT_EQ(metadata->ppp->txRadInt.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->ppp->txACX.getSize(), 3);
    TEST_ASSERT_EQ(metadata->ppp->txACX.getOffset(), 128);
    TEST_ASSERT_EQ(metadata->ppp->txACX.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->ppp->txACY.getSize(), 3);
    TEST_ASSERT_EQ(metadata->ppp->txACY.getOffset(), 152);
    TEST_ASSERT_EQ(metadata->ppp->txACY.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->ppp->txEB.getSize(), 2);
    TEST_ASSERT_EQ(metadata->ppp->txEB.getOffset(), 176);
    TEST_ASSERT_EQ(metadata->ppp->txEB.getFormat(), "DCX=F8;DCY=F8;");
    TEST_ASSERT_EQ(metadata->ppp->fxResponseIndex.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->fxResponseIndex.getOffset(), 192);
    TEST_ASSERT_EQ(metadata->ppp->fxResponseIndex.getFormat(), "I8");

    // PVP
    TEST_ASSERT_EQ(metadata->pvp->rcvStart.getSize(), 2);
    TEST_ASSERT_EQ(metadata->pvp->rcvStart.getOffset(), 0);
    TEST_ASSERT_EQ(metadata->pvp->rcvStart.getFormat(), "Int=I8;Frac=F8;");
    TEST_ASSERT_EQ(metadata->pvp->rcvPos.getSize(), 3);
    TEST_ASSERT_EQ(metadata->pvp->rcvPos.getOffset(), 16);
    TEST_ASSERT_EQ(metadata->pvp->rcvPos.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->pvp->rcvVel.getSize(), 3);
    TEST_ASSERT_EQ(metadata->pvp->rcvVel.getOffset(), 40);
    TEST_ASSERT_EQ(metadata->pvp->rcvVel.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->pvp->frcv1.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->frcv1.getOffset(), 64);
    TEST_ASSERT_EQ(metadata->pvp->frcv1.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->frcv2.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->frcv2.getOffset(), 72);
    TEST_ASSERT_EQ(metadata->pvp->frcv2.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->refPhi0.getSize(), 2);
    TEST_ASSERT_EQ(metadata->pvp->refPhi0.getOffset(), 80);
    TEST_ASSERT_EQ(metadata->pvp->refPhi0.getFormat(), "Int=I8;Frac=F8;");
    TEST_ASSERT_EQ(metadata->pvp->refFreq.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->refFreq.getOffset(), 96);
    TEST_ASSERT_EQ(metadata->pvp->refFreq.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->dfiC0.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->dfiC0.getOffset(), 104);
    TEST_ASSERT_EQ(metadata->pvp->dfiC0.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->ficRate.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->ficRate.getOffset(), 112);
    TEST_ASSERT_EQ(metadata->pvp->ficRate.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->rcvACX.getSize(), 3);
    TEST_ASSERT_EQ(metadata->pvp->rcvACX.getOffset(), 120);
    TEST_ASSERT_EQ(metadata->pvp->rcvACX.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->pvp->rcvACY.getSize(), 3);
    TEST_ASSERT_EQ(metadata->pvp->rcvACY.getOffset(), 144);
    TEST_ASSERT_EQ(metadata->pvp->rcvACY.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->pvp->rcvEB.getSize(), 2);
    TEST_ASSERT_EQ(metadata->pvp->rcvEB.getOffset(), 168);
    TEST_ASSERT_EQ(metadata->pvp->rcvEB.getFormat(), "DCX=F8;DCY=F8;");
    TEST_ASSERT_EQ(metadata->pvp->signal.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->signal.getOffset(), 184);
    TEST_ASSERT_EQ(metadata->pvp->signal.getFormat(), "I8");
    TEST_ASSERT_EQ(metadata->pvp->ampSF.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->ampSF.getOffset(), 192);
    TEST_ASSERT_EQ(metadata->pvp->ampSF.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->dgrgc.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->dgrgc.getOffset(), 200);
    TEST_ASSERT_EQ(metadata->pvp->dgrgc.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->txPulseIndex.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->txPulseIndex.getOffset(), 208);
    TEST_ASSERT_EQ(metadata->pvp->txPulseIndex.getFormat(), "I8");

    // Antenna
    TEST_ASSERT_EQ(metadata->antenna.antCoordFrame.size(), 1);
    TEST_ASSERT_EQ(metadata->antenna.antCoordFrame[0].identifier, "ACF1");
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter.size(), 2);
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].identifier, "APC1");
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].acfId, "ACF1");
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].apcXYZ[0], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].apcXYZ[1], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].apcXYZ[2], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[1].identifier, "APC2");
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[1].acfId, "ACF1");
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[1].apcXYZ[0], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[1].apcXYZ[1], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[1].apcXYZ[2], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern.size(), 1);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].identifier, "ANTPAT1");
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].freqZero, 1000000000.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].arrayGPId, "AGP1");
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].elementGPId, "EGP1");
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].ebFreqShift.first, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].ebFreqShift.second, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].mlFreqDilation.first, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].mlFreqDilation.second, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].gainBSPoly.size(), 2);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].gainBSPoly[0], 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].gainBSPoly[1], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].antPolRef.ampX, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].antPolRef.ampY, 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].antPolRef.phaseX, 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].antPolRef.phaseY, 0.0);
}

void runTestTX(const std::string& testName, const std::string& version)
{
    auto xmlString = testCRSDtxXML(version);
    io::StringStream crsdStream;
    crsdStream.write(xmlString.c_str(), xmlString.size());
    
    xml::lite::MinidomParser xmlParser;
    xmlParser.preserveCharacterData(true);  
    xmlParser.parse(crsdStream, crsdStream.available());      
    const std::unique_ptr<crsd::Metadata> metadata =
            crsd::CRSDXMLControl().fromXML(xmlParser.getDocument());
    
    // ProductInfo
    TEST_ASSERT_EQ(metadata->productInfo.productName, "EXAMPLE_TX_PRODUCT");
    TEST_ASSERT_EQ(metadata->productInfo.classification, "UNCLASSIFIED");
    TEST_ASSERT_EQ(metadata->productInfo.releaseInfo, "Approved for public release");
    TEST_ASSERT_EQ(metadata->productInfo.countryCode, "USA");
    TEST_ASSERT_EQ(metadata->productInfo.profile, "DEFAULT");
    TEST_ASSERT_EQ(metadata->productInfo.creationInfo[0].application, "ExampleApp");
    TEST_ASSERT_EQ(metadata->productInfo.creationInfo[0].dateTime.getYear(), 2024);
    TEST_ASSERT_EQ(metadata->productInfo.creationInfo[0].site, "ExampleSite");

    // TransmitInfo
    TEST_ASSERT_EQ(metadata->transmitInfo->sensorName, "TX_SENSOR");
    TEST_ASSERT_EQ(metadata->transmitInfo->eventName, "TX_EVENT");

    // Global
    TEST_ASSERT_EQ(metadata->global.collectionRefTime.getYear(), 2024);
    TEST_ASSERT_EQ(metadata->global.transmitParameters->txTime1, 0.0);
    TEST_ASSERT_EQ(metadata->global.transmitParameters->txTime2, 1.0);
    TEST_ASSERT_EQ(metadata->global.transmitParameters->fxMin, 950000000.0);
    TEST_ASSERT_EQ(metadata->global.transmitParameters->fxMax, 1050000000.0);

    // SceneCoordinates
    TEST_ASSERT_EQ(metadata->sceneCoordinates.earthModel, crsd::EarthModelType::WGS_84);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.ecf[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.ecf[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.ecf[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.llh.getLat(), 38.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.llh.getLon(), -77.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.llh.getAlt(), 100.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.referenceSurface.planar->uIax[0], 1.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.referenceSurface.planar->uIax[1], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.referenceSurface.planar->uIay[0], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.referenceSurface.planar->uIay[1], 1.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.x1y1[0], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.x1y1[1], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.x2y2[0], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.x2y2[1], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon.size(), 4);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[0][0], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[0][1], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[1][0], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[1][1], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[2][0], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[2][1], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[3][0], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[3][1], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.upperLeft.getLat(), 38.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.upperLeft.getLon(), -77.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.upperRight.getLat(), 38.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.upperRight.getLon(), -76.9);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.lowerRight.getLat(), 38.1);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.lowerRight.getLon(), -76.9);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.lowerLeft.getLat(), 38.1);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.lowerLeft.getLon(), -77.0);
    TEST_ASSERT_TRUE(metadata->sceneCoordinates.imageGrid.get() == nullptr);
 
    // Data
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.size(), 1);
    TEST_ASSERT_TRUE(metadata->data.supportArrayMap.find("SUPPORT1") !=
                   metadata->data.supportArrayMap.end());
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.at("SUPPORT1").numRows, 10);
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.at("SUPPORT1").numCols, 10);
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.at("SUPPORT1").bytesPerElement, 4);
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.at("SUPPORT1").arrayByteOffset, 0);
    TEST_ASSERT_EQ(metadata->data.transmitParameters->numBytesPPP, 128);
    TEST_ASSERT_EQ(metadata->data.transmitParameters->txSequence.size(), 1);
    TEST_ASSERT_TRUE(metadata->data.transmitParameters->txSequence[0].identifier == "TXSEQ1");
    TEST_ASSERT_EQ(metadata->data.transmitParameters->txSequence[0].numPulses, 64);
    TEST_ASSERT_EQ(metadata->data.transmitParameters->txSequence[0].pppArrayByteOffset, 0);

    // TxSequence
    TEST_ASSERT_TRUE(metadata->txSequence->refTxId == "TXSEQ1");
    TEST_ASSERT_TRUE(metadata->txSequence->txWFType.toString() == "LFM");
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].identifier, "TXSEQ1_PARAM");
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].refPulseIndex, 0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].fxResponseId, "FXRESP1");
    TEST_ASSERT_TRUE(metadata->txSequence->parameters[0].fxBWFixed);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].fxC, 1000000000.0);  
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].fxBW, 10000000.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txmtMin, 0.0001);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txmtMax, 0.001);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txTime1, 0.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txTime2, 1.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txAPCId, "APC1");
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txAPATId, "APAT1");
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefPoint.ecf[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefPoint.ecf[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefPoint.ecf[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefPoint.iac[0], 0.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefPoint.iac[1], 0.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txPolarization.polarizationID.toString(), "H");
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txPolarization.ampH, 1.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txPolarization.ampV, 0.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txPolarization.phaseH, 0.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txPolarization.phaseV, 0.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefRadIntensity, 1.0);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefRadIntensityStdDev, 0.01);
    TEST_ASSERT_EQ(metadata->txSequence->parameters[0].txRefLAtm, 0.0);

    // ReferenceGeometry
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.ecf[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.ecf[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.ecf[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.iac[0], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.iac[1], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->time, 0.5);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->apcPos[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->apcPos[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->apcPos[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->apcVel[0], 100.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->apcVel[1], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->apcVel[2], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->sideOfTrack.toString(), "RIGHT");
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->slantRange, 10000.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->groundRange, 9000.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->dopplerConeAngle, 45.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->squintAngle, 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->azimuthAngle, 90.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->grazeAngle, 10.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.txParameters->incidenceAngle, 30.0);

    // SupportArray
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase.size(), 1);
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].getIdentifier(), "AGP1");
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].elementFormat, "Gain=F4;Phase=F4;");
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].x0, 0.0);
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].y0, 0.0);
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].xSS, 1.0);
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].ySS, 1.0);
    TEST_ASSERT_EQ(metadata->supportArray.fxResponseArray.size(), 1);
    TEST_ASSERT_EQ(metadata->supportArray.fxResponseArray[0].getIdentifier(), "FXRESP1");
    TEST_ASSERT_EQ(metadata->supportArray.fxResponseArray[0].elementFormat, "Amp=F4;Phase=F4;");
    TEST_ASSERT_EQ(metadata->supportArray.fxResponseArray[0].fx0FXR, 1000000000.0);
    TEST_ASSERT_EQ(metadata->supportArray.fxResponseArray[0].fxSSFXR, 1000000.0);

    // PPP
    TEST_ASSERT_EQ(metadata->ppp->txTime.getSize(), 2);
    TEST_ASSERT_EQ(metadata->ppp->txTime.getOffset(), 0);
    TEST_ASSERT_EQ(metadata->ppp->txTime.getFormat(), "Int=I8;Frac=F8;");
    TEST_ASSERT_EQ(metadata->ppp->txPos.getSize(), 3);
    TEST_ASSERT_EQ(metadata->ppp->txPos.getOffset(), 16);
    TEST_ASSERT_EQ(metadata->ppp->txPos.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->ppp->txVel.getSize(), 3);
    TEST_ASSERT_EQ(metadata->ppp->txVel.getOffset(), 40);
    TEST_ASSERT_EQ(metadata->ppp->txVel.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->ppp->fx1.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->fx1.getOffset(), 64);
    TEST_ASSERT_EQ(metadata->ppp->fx1.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->ppp->fx2.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->fx2.getOffset(), 72);
    TEST_ASSERT_EQ(metadata->ppp->fx2.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->ppp->txmt.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->txmt.getOffset(), 80);
    TEST_ASSERT_EQ(metadata->ppp->txmt.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->ppp->phiX0.getSize(), 2);
    TEST_ASSERT_EQ(metadata->ppp->phiX0.getOffset(), 88);
    TEST_ASSERT_EQ(metadata->ppp->phiX0.getFormat(), "Int=I8;Frac=F8;");
    TEST_ASSERT_EQ(metadata->ppp->fxFreq0.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->fxFreq0.getOffset(), 104);
    TEST_ASSERT_EQ(metadata->ppp->fxFreq0.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->ppp->fxRate.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->fxRate.getOffset(), 112);
    TEST_ASSERT_EQ(metadata->ppp->fxRate.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->ppp->txRadInt.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->txRadInt.getOffset(), 120);
    TEST_ASSERT_EQ(metadata->ppp->txRadInt.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->ppp->txACX.getSize(), 3);
    TEST_ASSERT_EQ(metadata->ppp->txACX.getOffset(), 128);
    TEST_ASSERT_EQ(metadata->ppp->txACX.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->ppp->txACY.getSize(), 3);
    TEST_ASSERT_EQ(metadata->ppp->txACY.getOffset(), 152);
    TEST_ASSERT_EQ(metadata->ppp->txACY.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->ppp->txEB.getSize(), 2);
    TEST_ASSERT_EQ(metadata->ppp->txEB.getOffset(), 176);
    TEST_ASSERT_EQ(metadata->ppp->txEB.getFormat(), "DCX=F8;DCY=F8;");
    TEST_ASSERT_EQ(metadata->ppp->fxResponseIndex.getSize(), 1);
    TEST_ASSERT_EQ(metadata->ppp->fxResponseIndex.getOffset(), 192);
    TEST_ASSERT_EQ(metadata->ppp->fxResponseIndex.getFormat(), "I8");

    // Antenna
    TEST_ASSERT_EQ(metadata->antenna.antCoordFrame.size(), 1);
    TEST_ASSERT_EQ(metadata->antenna.antCoordFrame[0].identifier, "ACF1");
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter.size(), 1);
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].identifier, "APC1");
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].acfId, "ACF1");
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].apcXYZ[0], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].apcXYZ[1], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].apcXYZ[2], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern.size(), 1);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].identifier, "ANTPAT1");
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].freqZero, 1000000000.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].arrayGPId, "AGP1");
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].elementGPId, "EGP1");
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].ebFreqShift.first, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].ebFreqShift.second, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].mlFreqDilation.first, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].mlFreqDilation.second, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].gainBSPoly.size(), 2);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].gainBSPoly[0], 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].gainBSPoly[1], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].antPolRef.ampX, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].antPolRef.ampY, 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].antPolRef.phaseX, 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].antPolRef.phaseY, 0.0);
}

void runTestRCV(const std::string& testName, const std::string& version)
{
    auto xmlString = testCRSDrcvXML(version);
    io::StringStream crsdStream;
    crsdStream.write(xmlString.c_str(), xmlString.size());
    
    xml::lite::MinidomParser xmlParser;
    xmlParser.preserveCharacterData(true);  
    xmlParser.parse(crsdStream, crsdStream.available());      
    const std::unique_ptr<crsd::Metadata> metadata =
            crsd::CRSDXMLControl().fromXML(xmlParser.getDocument());
    
    // ProductInfo
    TEST_ASSERT_EQ(metadata->productInfo.productName, "EXAMPLE_RCV_PRODUCT");
    TEST_ASSERT_EQ(metadata->productInfo.classification, "UNCLASSIFIED");
    TEST_ASSERT_EQ(metadata->productInfo.releaseInfo, "Approved for public release");
    TEST_ASSERT_EQ(metadata->productInfo.countryCode, "USA");
    TEST_ASSERT_EQ(metadata->productInfo.profile, "DEFAULT");
    TEST_ASSERT_EQ(metadata->productInfo.creationInfo[0].application, "ExampleApp");
    TEST_ASSERT_EQ(metadata->productInfo.creationInfo[0].dateTime.getYear(), 2024);
    TEST_ASSERT_EQ(metadata->productInfo.creationInfo[0].site, "ExampleSite");

    // ReceiveInfo
    TEST_ASSERT_EQ(metadata->receiveInfo->sensorName, "RX_SENSOR");
    TEST_ASSERT_EQ(metadata->receiveInfo->eventName, "RX_EVENT");

    // Global
    TEST_ASSERT_EQ(metadata->global.collectionRefTime.getYear(), 2024);
    TEST_ASSERT_EQ(metadata->global.receiveParameters->rcvStartTime1, 0.0);
    TEST_ASSERT_EQ(metadata->global.receiveParameters->rcvStartTime2, 1.0);
    TEST_ASSERT_EQ(metadata->global.receiveParameters->fRcvMin, 950000000.0);
    TEST_ASSERT_EQ(metadata->global.receiveParameters->fRcvMax, 1050000000.0);

    // SceneCoordinates
    TEST_ASSERT_EQ(metadata->sceneCoordinates.earthModel, crsd::EarthModelType::WGS_84);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.ecf[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.ecf[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.ecf[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.llh.getLat(), 38.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.llh.getLon(), -77.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.iarp.llh.getAlt(), 100.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.referenceSurface.planar->uIax[0], 1.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.referenceSurface.planar->uIax[1], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.referenceSurface.planar->uIay[0], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.referenceSurface.planar->uIay[1], 1.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.x1y1[0], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.x1y1[1], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.x2y2[0], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.x2y2[1], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon.size(), 4);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[0][0], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[0][1], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[1][0], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[1][1], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[2][0], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[2][1], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[3][0], 0.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageArea.polygon[3][1], 1000.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.upperLeft.getLat(), 38.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.upperLeft.getLon(), -77.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.upperRight.getLat(), 38.0);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.upperRight.getLon(), -76.9);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.lowerRight.getLat(), 38.1);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.lowerRight.getLon(), -76.9);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.lowerLeft.getLat(), 38.1);
    TEST_ASSERT_EQ(metadata->sceneCoordinates.imageAreaCorners.lowerLeft.getLon(), -77.0);
    TEST_ASSERT_TRUE(metadata->sceneCoordinates.imageGrid.get() == nullptr);
 
    // Data
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.size(), 1);
    TEST_ASSERT_TRUE(metadata->data.supportArrayMap.find("SUPPORT1") !=
                   metadata->data.supportArrayMap.end());
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.at("SUPPORT1").numRows, 10);
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.at("SUPPORT1").numCols, 10);
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.at("SUPPORT1").bytesPerElement, 4);
    TEST_ASSERT_EQ(metadata->data.supportArrayMap.at("SUPPORT1").arrayByteOffset, 0);
    TEST_ASSERT_EQ(metadata->data.receiveParameters->signalArrayFormat, crsd::SignalArrayFormat::CI2);
    TEST_ASSERT_EQ(metadata->data.receiveParameters->numBytesPVP, 256);
    TEST_ASSERT_EQ(metadata->data.receiveParameters->getNumCRSDChannels(), 1);
    TEST_ASSERT_TRUE(metadata->data.receiveParameters->channels[0].identifier  == "CHAN1");
    TEST_ASSERT_EQ(metadata->data.receiveParameters->channels[0].numVectors, 128);
    TEST_ASSERT_EQ(metadata->data.receiveParameters->channels[0].numSamples, 1024);
    TEST_ASSERT_EQ(metadata->data.receiveParameters->channels[0].signalArrayByteOffset, 0);
    TEST_ASSERT_EQ(metadata->data.receiveParameters->channels[0].pvpArrayByteOffset, 0);

    // Channel
    TEST_ASSERT_TRUE(metadata->channel->refChId == "CHAN1");
    TEST_ASSERT_EQ(metadata->channel->parameters[0].identifier, "CHAN1");
    TEST_ASSERT_EQ(metadata->channel->parameters[0].refVectorIndex, 0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].refFreqFixed, true);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].fRcvFixed, true);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].signalNormal, true);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].f0Ref, 1000000000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].fs, 20000000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].bwInst, 10000000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvStartTime1, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvStartTime2, 1.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].fRcvMin, 950000000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].fRcvMax, 1050000000.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvAPCId, "APC2");
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvAPATId, "APAT2");
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefPoint.ecf[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefPoint.ecf[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefPoint.ecf[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefPoint.iac[0], 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefPoint.iac[1], 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvPolarization.polarizationID.toString(), "V");
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvPolarization.ampH, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvPolarization.ampV, 1.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvPolarization.phaseH, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvPolarization.phaseV, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefIrradiance, 1.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefIrradianceErrorStdDev, 0.01);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].rcvRefLAtm, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].pncrsd, 0.0);
    TEST_ASSERT_EQ(metadata->channel->parameters[0].bncrsd, 1.0);

    // ReferenceGeometry
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.ecf[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.ecf[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.ecf[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.iac[0], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.refPoint.iac[1], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->time, 0.5);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->apcPos[0], 1234567.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->apcPos[1], 2345678.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->apcPos[2], 3456789.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->apcVel[0], 100.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->apcVel[1], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->apcVel[2], 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->sideOfTrack.toString(), "RIGHT");
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->slantRange, 10000.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->groundRange, 9000.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->dopplerConeAngle, 45.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->squintAngle, 0.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->azimuthAngle, 90.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->grazeAngle, 10.0);
    TEST_ASSERT_EQ(metadata->referenceGeometry.rcvParameters->incidenceAngle, 30.0);

    // SupportArray
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase.size(), 1);
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].getIdentifier(), "AGP1");
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].elementFormat, "Gain=F4;Phase=F4;");
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].x0, 0.0);
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].y0, 0.0);
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].xSS, 1.0);
    TEST_ASSERT_EQ(metadata->supportArray.antGainPhase[0].ySS, 1.0);

    // PVP
    TEST_ASSERT_EQ(metadata->pvp->rcvStart.getSize(), 2);
    TEST_ASSERT_EQ(metadata->pvp->rcvStart.getOffset(), 0);
    TEST_ASSERT_EQ(metadata->pvp->rcvStart.getFormat(), "Int=I8;Frac=F8;");
    TEST_ASSERT_EQ(metadata->pvp->rcvPos.getSize(), 3);
    TEST_ASSERT_EQ(metadata->pvp->rcvPos.getOffset(), 16);
    TEST_ASSERT_EQ(metadata->pvp->rcvPos.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->pvp->rcvVel.getSize(), 3);
    TEST_ASSERT_EQ(metadata->pvp->rcvVel.getOffset(), 40);
    TEST_ASSERT_EQ(metadata->pvp->rcvVel.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->pvp->frcv1.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->frcv1.getOffset(), 64);
    TEST_ASSERT_EQ(metadata->pvp->frcv1.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->frcv2.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->frcv2.getOffset(), 72);
    TEST_ASSERT_EQ(metadata->pvp->frcv2.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->refPhi0.getSize(), 2);
    TEST_ASSERT_EQ(metadata->pvp->refPhi0.getOffset(), 80);
    TEST_ASSERT_EQ(metadata->pvp->refPhi0.getFormat(), "Int=I8;Frac=F8;");
    TEST_ASSERT_EQ(metadata->pvp->refFreq.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->refFreq.getOffset(), 96);
    TEST_ASSERT_EQ(metadata->pvp->refFreq.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->dfiC0.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->dfiC0.getOffset(), 104);
    TEST_ASSERT_EQ(metadata->pvp->dfiC0.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->ficRate.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->ficRate.getOffset(), 112);
    TEST_ASSERT_EQ(metadata->pvp->ficRate.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->rcvACX.getSize(), 3);
    TEST_ASSERT_EQ(metadata->pvp->rcvACX.getOffset(), 120);
    TEST_ASSERT_EQ(metadata->pvp->rcvACX.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->pvp->rcvACY.getSize(), 3);
    TEST_ASSERT_EQ(metadata->pvp->rcvACY.getOffset(), 144);
    TEST_ASSERT_EQ(metadata->pvp->rcvACY.getFormat(), "X=F8;Y=F8;Z=F8;");
    TEST_ASSERT_EQ(metadata->pvp->rcvEB.getSize(), 2);
    TEST_ASSERT_EQ(metadata->pvp->rcvEB.getOffset(), 168);
    TEST_ASSERT_EQ(metadata->pvp->rcvEB.getFormat(), "DCX=F8;DCY=F8;");
    TEST_ASSERT_EQ(metadata->pvp->signal.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->signal.getOffset(), 184);
    TEST_ASSERT_EQ(metadata->pvp->signal.getFormat(), "I8");
    TEST_ASSERT_EQ(metadata->pvp->ampSF.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->ampSF.getOffset(), 192);
    TEST_ASSERT_EQ(metadata->pvp->ampSF.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->dgrgc.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->dgrgc.getOffset(), 200);
    TEST_ASSERT_EQ(metadata->pvp->dgrgc.getFormat(), "F8");
    TEST_ASSERT_EQ(metadata->pvp->txPulseIndex.getSize(), 1);
    TEST_ASSERT_EQ(metadata->pvp->txPulseIndex.getOffset(), 208);
    TEST_ASSERT_EQ(metadata->pvp->txPulseIndex.getFormat(), "I8");

    // Antenna
    TEST_ASSERT_EQ(metadata->antenna.antCoordFrame.size(), 1);
    TEST_ASSERT_EQ(metadata->antenna.antCoordFrame[0].identifier, "ACF1");
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter.size(), 1);
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].identifier, "APC1");
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].acfId, "ACF1");
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].apcXYZ[0], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].apcXYZ[1], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPhaseCenter[0].apcXYZ[2], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern.size(), 1);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].identifier, "ANTPAT1");
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].freqZero, 1000000000.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].arrayGPId, "AGP1");
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].elementGPId, "EGP1");
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].ebFreqShift.first, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].ebFreqShift.second, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].mlFreqDilation.first, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].mlFreqDilation.second, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].gainBSPoly.size(), 2);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].gainBSPoly[0], 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].gainBSPoly[1], 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].antPolRef.ampX, 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].antPolRef.ampY, 1.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].antPolRef.phaseX, 0.0);
    TEST_ASSERT_EQ(metadata->antenna.antPattern[0].antPolRef.phaseY, 0.0);
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

TEST_CASE(testReadXMLSAR)
{
    for (auto pair : crsd::CRSDXMLControl::getVersionUriMap())
    {
        auto& version = pair.first;
        runTestSAR("testReadXML" + version, version);
    }
}

TEST_CASE(testReadXMLTX)
{
    for (auto pair : crsd::CRSDXMLControl::getVersionUriMap())
    {
        auto& version = pair.first;
        runTestTX("testReadXML" + version, version);
    }
}

TEST_CASE(testReadXMLRCV)
{
    for (auto pair : crsd::CRSDXMLControl::getVersionUriMap())
    {
        auto& version = pair.first;
        runTestRCV("testReadXML" + version, version);
    }
}

TEST_MAIN(
    TEST_CHECK(testVersions);
    TEST_CHECK(testReadXMLSAR);
    TEST_CHECK(testReadXMLTX);
    TEST_CHECK(testReadXMLRCV);
)
