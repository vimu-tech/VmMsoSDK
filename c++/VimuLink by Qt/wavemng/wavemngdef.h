#ifndef WAVEMNGDEF_H
#define WAVEMNGDEF_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/


typedef enum {
    PhysicsFile = 0x00000000,  //硬盘文件
    EmsFile = 0x00000001   //内存
} TrackFileType;

typedef enum SHIYUBX_SOURCE {
    SHIYUBX_SOURCE_ZERO = 0x00000000,
    SHIYUBX_SOURCE_CH1 = 0x00000001,
    SHIYUBX_SOURCE_CH2 = 0x00000002,
    SHIYUBX_SOURCE_CH3 = 0x00000004,
    SHIYUBX_SOURCE_CH4 = 0x00000008,
    SHIYUBX_SOURCE_REF1 = 0x00000010,
    SHIYUBX_SOURCE_REF2 = 0x00000020,
    SHIYUBX_SOURCE_REF3 = 0x00000040,
    SHIYUBX_SOURCE_REF4 = 0x00000080
} SHIYUBX_SOURCE;

static SHIYUBX_SOURCE sourceList[4] ={SHIYUBX_SOURCE_CH1, SHIYUBX_SOURCE_CH2, SHIYUBX_SOURCE_CH3, SHIYUBX_SOURCE_CH4};

typedef enum {
    errorSample = 0x00000000,
    int8Sample = 0x00010001,
    uint8Sample = 0x00010002,
    int16Sample = 0x00020001,
    uint16Sample = 0x00020002,
    int24Sample = 0x00030001,
    int32Sample = 0x0004000A,
    uint32Sample = 0x0004000B,
    floatSample = 0x0004000F,
    doubleSample = 0x0008000F
} sampleFormat;

#define SAMPLE_SIZE(mSampleFormat) (mSampleFormat>>16)

#define m_max(a,b) ((((a) > (b)) ? (a) : (b)))
#define m_min(a,b) ((((a) < (b)) ? (a) : (b)))

#endif
