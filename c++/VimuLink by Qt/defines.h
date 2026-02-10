#ifndef DEFINES_H
#define DEFINES_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include <QString>

static const QString ch1Name = "CH1";
static const QString ch2Name = "CH2";
static const QString ch3Name = "CH3";
static const QString ch4Name = "CH4";
static const QString dio0Name = "DIO0";
static const QString dio1Name = "DIO1";
static const QString dio2Name = "DIO2";
static const QString dio3Name = "DIO3";
static const QString dio4Name = "DIO4";
static const QString dio5Name = "DIO5";
static const QString dio6Name = "DIO6";
static const QString dio7Name = "DIO7";
static const QString dio8Name = "DIO8";
static const QString dio9Name = "DIO9";
static const QString dio10Name = "DIO10";
static const QString dio11Name = "DIO11";
static const QString dio12Name = "DIO12";
static const QString dio13Name = "DIO13";
static const QString dio14Name = "DIO14";
static const QString dio15Name = "DIO15";

static const QString filterName = "FLT";
static const QString fftName = "FFT";

typedef enum OscWorkMode    {
    OscRealTime = 0,
    OscStream = 1
}OscWorkMode;

typedef enum TriggerMode    {
    TriggerModeAuto = 0,
    TriggerModeNormal = 1
}TriggerMode;

typedef enum TriggerStyle    {
    TriggerStyleNone = 0,
    TriggerStyleRising = 1,
    TriggerStyleFalling,
    TriggerStyleEdge,
    TriggerStylePosPulseMore,
    TriggerStylePosPulseLess,
    TriggerStylePosPulse,
    TriggerStyleNegPulseMore,
    TriggerStyleNegPulseLess,
    TriggerStyleNegPulse
}TriggerStyle;

typedef enum TriggerSource    {
    TriggerSourceCh1 = 0,
    TriggerSourceCh2 = 1,
    TriggerSourceCh3 = 2,
    TriggerSourceCh4 = 3,
    TriggerSourceLogic0 = 16,
    TriggerSourceLogic1 = 17,
    TriggerSourceLogic2 = 18,
    TriggerSourceLogic3 = 19,
    TriggerSourceLogic4 = 20,
    TriggerSourceLogic5 = 21,
    TriggerSourceLogic6 = 22,
    TriggerSourceLogic7 = 23,
    TriggerSourceLogic8 = 24,
    TriggerSourceLogic9 = 25,
    TriggerSourceLogic10 = 26,
    TriggerSourceLogic11 = 27,
    TriggerSourceLogic12 = 28,
    TriggerSourceLogic13 = 29,
    TriggerSourceLogic14 = 30,
    TriggerSourceLogic15 = 31,
}TriggerSource;

typedef enum IOOutput    {
    IOOutput0 = 0x0000,
    IOOutput1 = 0x0001,
    IOOutputZ = 0x0002,
    IOOutputPulse = 0x0003,
    IOOutputDDSGate = 0x0004,
}IOOutput;

typedef enum DdsMode    {
    DdsModeContinuous = 0x0000,
    DdsModeSweep = 0x0001,
    DdsModeBurst = 0x0002
}DdsMode;

typedef enum BurstStyle    {
    BurstStyleNLoops = 0x0000,
    BurstStyleGate = 0x0001
}BurstStyle;

typedef enum DdsWave    {
    DdsWaveSine = 0x0001,
    DdsWaveSquare = 0x0002,
    DdsWaveRamp = 0x0004,
    DdsWavePulse = 0x0008,
    DdsWaveNoise = 0x0010,
    DdsWaveDc = 0x0020,
    DdsWaveArb = 0x0040
}DdsWave;

typedef enum DdsTriggerSourceEdge    {
    DdsTriggerEdgeRising = 0,
    DdsTriggerEdgeFalling= 1
}DdsTriggerSourceEdge;


typedef enum DdsTriggerSource    {
    DdsTriggerSourceInternal = 0,
    DdsTriggerSourceExternal = 1,
    DdsTriggerSourceManual = 2
}DdsTriggerSource;

typedef enum DdsOutput    {
    DdsOutputClose = 0,
    DdsOutputRising = 1,
    DdsOutputFalling = 2
}DdsOutput;

static unsigned int const FFTN[] =
{
    1024,
    2048,
    4096,
    8192,
    16384,
    32768,
    65536,
    131072,
    262144,
    524288,
    1048576
};



#endif // DEFINES_H
