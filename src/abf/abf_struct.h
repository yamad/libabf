/* Structure definitions common to Axon Binary File v1 and v2 formats */
#ifndef ABF_STRUCT_H
#define ABF_STRUCT_H

#include <stdint.h>

#define ABF_LOGFONTSIZE 40
#define ABF_SIGNALSIZE 34
#define ABF_SCOPECONFIGSIZE 769
#define ABF_TAGSIZE 64
#define ABF_SYNCHSIZE 8
#define ABF_VOICETAGINFOSIZE 32
#define ABF_DELTASIZE 12


#define ABF_ADCCOUNT           16    /* number of ADC channels supported. */
#define ABF_ADCNAMELEN         10    /* length of ADC channel name strings */
#define ABF_TAGCOMMENTLEN      56    /* length of tag comment string */

/* Indexes into the rgbColor field of ABFScopeConfig. */
#define ABF_BACKGROUNDCOLOR   0
#define ABF_GRIDCOLOR         1
#define ABF_THRESHOLDCOLOR    2
#define ABF_EVENTMARKERCOLOR  3
#define ABF_SEPARATORCOLOR    4
#define ABF_AVERAGECOLOR      5
#define ABF_OLDDATACOLOR      6
#define ABF_TEXTCOLOR         7
#define ABF_AXISCOLOR         8
#define ABF_ACTIVEAXISCOLOR   9
#define ABF_LASTCOLOR         ABF_ACTIVEAXISCOLOR
#define ABF_SCOPECOLORS       (ABF_LASTCOLOR+1)

/* Extended colors for rgbColorEx field in ABFScopeConfig */
#define ABF_STATISTICS_REGION0 0
#define ABF_STATISTICS_REGION1 1
#define ABF_STATISTICS_REGION2 2
#define ABF_STATISTICS_REGION3 3
#define ABF_STATISTICS_REGION4 4
#define ABF_STATISTICS_REGION5 5
#define ABF_STATISTICS_REGION6 6
#define ABF_STATISTICS_REGION7 7
#define ABF_BASELINE_REGION    8
#define ABF_STOREDSWEEPCOLOR   9
#define ABF_LASTCOLOR_EX       ABF_STOREDSWEEPCOLOR
#define ABF_SCOPECOLORS_EX     (ABF_LASTCOLOR+1)

enum ABF_FILETYPE {
    ABF_FILETYPE_ABF = 1,
    ABF_FILETYPE_FETCHEX = 2,
    ABF_FILETYPE_CLAMPEX = 3
};

enum ABF_DATAFORMAT {
    ABF_DATAFORMAT_INTEGER = 0,
    ABF_DATAFORMAT_FLOAT = 1
};

/* Constants for nTagType in the ABFTag structure. */
enum ABF_TAGTYPE {
    ABF_TAGTYPE_TIME = 0,
    ABF_TAGTYPE_COMMENT = 1,
    ABF_TAGTYPE_EXTERNAL = 2,
    ABF_TAGTYPE_VOICE = 3,
    ABF_TAGTYPE_NEWFILE = 4,
    ABF_TAGTYPE_ANNOTATION = 5
};

/* aliases for compatibility with Axon's constants */
#define ABF_TIMETAG              ABF_TAGTYPE_TIME
#define ABF_COMMENTTAG           ABF_TAGTYPE_COMMENT
#define ABF_EXTERNALTAG          ABF_TAGTYPE_EXTERNAL
#define ABF_VOICETAG             ABF_TAGTYPE_VOICE
#define ABF_NEWFILETAG           ABF_TAGTYPE_NEWFILE
#define ABF_ANNOTATIONTAG        ABF_TAGTYPE_ANNOTATION

#define ABF_INTEGERDATA          ABF_DATAFORMAT_INTEGER
#define ABF_FLOATDATA            ABF_DATAFORMAT_FLOAT


#define ABF_FACESIZE 32
struct abf_logfont
{
    int16_t nHeight;            /* Height of the font in pixels. */
    int16_t nWeight;            /* MSWindows font weight value. */
    uint8_t cPitchAndFamily;    /* MSWindows pitch and family mask. */
    uint8_t Unused[3];  /* Unused space to maintain 4-byte packing. */
    uint8_t szFaceName[ABF_FACESIZE]; /* Face name of the font. */
    /* size = 40 bytes */
};

struct abf_signal
{
    uint8_t szName[ABF_ADCNAMELEN+2]; /* ABF name length + '\0' + 1 for alignment. */
    int16_t nMxOffset; /* Offset of the signal in the sampling sequence. */
    uint32_t rgbColor; /* Pen color used to draw trace. */
    uint8_t nPenWidth; /* Pen width in pixels. */
    uint8_t bDrawPoints;    /* TRUE = Draw disconnected points */
    uint8_t bHidden;        /* TRUE = Hide the trace. */
    uint8_t bFloatData;     /* TRUE = Floating point pseudo channel */
    float fVertProportion; /* Relative proportion of client area to use */
    float fDisplayGain;    /* Display gain of trace in UserUnits */
    float fDisplayOffset;  /* Display offset of trace in UserUnits */
    /* size = 34 bytes */
};

struct abf_scopeconfig
{
    uint32_t dwFlags;    /* Flags that are meaningful to the scope. */
    uint32_t rgbColor[ABF_SCOPECOLORS]; /* Colors for the components of the scope. */
    float fDisplayStart;       /* Start of the display area in ms. */
    float fDisplayEnd;         /* End of the display area in ms. */
    uint16_t wScopeMode;       /* Mode that the scope is in. */
    uint8_t bMaximized;        /* TRUE = Scope parent is maximized. */
    uint8_t bMinimized;        /* TRUE = Scope parent is minimized. */
    int16_t xLeft;             /* Coordinate of the left edge. */
    int16_t yTop;              /* Coordinate of the top edge. */
    int16_t xRight;            /* Coordinate of the right edge. */
    int16_t yBottom;           /* Coordinate of the bottom edge. */
    struct abf_logfont LogFont; /* Description of current font. */
    struct abf_signal TraceList[ABF_ADCCOUNT]; /* List of traces in current use. */
    int16_t nYAxisWidth;        /* Width of the YAxis region. */
    int16_t nTraceCount; /* Number of traces described in TraceList. */
    int16_t nEraseStrategy;     /* Erase strategy. */
    int16_t nDockState;         /* Docked position. */

    /* Section 2 scope configurations for file version 1.68. */
    int16_t nSizeofOldStructure; /* Unused byte to determine the offset of the version 2 data. */
    uint32_t rgbColorEx[ABF_SCOPECOLORS_EX]; /* New color settings for stored sweep and cursors. */
    int16_t nAutoZeroState;    /* Status of the autozero selection. */
    uint32_t dwCursorsVisibleState; /* Flag for visible status of cursors. */
    uint32_t dwCursorsLockedState; /* Flag for enabled status of cursors. */
    uint8_t sUnasigned[61];
    /* size = 769 bytes */
};

struct abf_tag
{
    int32_t lTagTime; /* Time at which the tag was entered in fSynchTimeUnit units. */
    uint8_t sComment[ABF_TAGCOMMENTLEN]; /* Optional tag comment. */
    int16_t nTagType; /* Type of tag ABF_TIMETAG, ABF_COMMENTTAG, ABF_EXTERNALTAG, ABF_VOICETAG, ABF_NEWFILETAG or ABF_ANNOTATIONTAG */
    union
    {
        int16_t nVoiceTagNumber;  /* If nTagType=ABF_VOICETAG, this is the number of this voice tag. */
        int16_t nAnnotationIndex; /* If nTagType=ABF_ANNOTATIONTAG, this is the index of the corresponding annotation. */
    };
    /* size = 64 bytes */
};

struct abf_synch
{
    int32_t lStart;  /* Start of the episode/event in fSynchTimeUnit units. */
    int32_t lLength; /* Length of the episode/event in multiplexed samples. */
    /* size = 8 bytes */
};

struct abf_voicetaginfo
{
   int32_t lTagNumber;          /* The tag number that corresponds to this VoiceTag */
   int32_t lFileOffset;         /* Offset to this tag within the VoiceTag block */
   int32_t lUncompressedSize;   /* Size of the voice tag expanded. */
   int32_t lCompressedSize;     /* Compressed size of the tag. */
   int16_t nCompressionType;    /* Compression method used. */
   int16_t nSampleSize;         /* Size of the samples acquired. */
   int32_t lSamplesPerSecond;   /* Rate at which the sound was acquired. */
   uint32_t dwCRC;              /* CRC used to check data integrity. */
   uint16_t wChannels;          /* Number of channels in the tag (usually 1). */
   uint16_t wUnused;            /* Unused space. */
   /* size = 32 bytes */
};

struct abf_delta
{
    int32_t lDeltaTime; /* Time at which the parameter was changed in fSynchTimeUnit units. */
    int32_t lParameterID;   /* Identifier for the parameter changed */
    union
    {
        int32_t lNewParamValue; /* Depending on the value of lParameterID */
        float fNewParamValue; /* this entry may be either a float or a int32_t. */
    };
    /* size = 12 bytes */
};

struct abf_stringcacheheader
{
    uint32_t dwSignature;
    uint32_t dwVersion;
    uint32_t uNumStrings;
    uint32_t uMaxSize;
    int32_t lTotalBytes;
    uint32_t uUnused[6];
    /* size = 44 bytes */
};

#endif /* ABF_STRUCT_H */
