/*  Copyright (c) MediaArea.net SARL & AV Preservation by reto.ch.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifndef RIFFH
#define RIFFH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Lib/FFV1/FFV1_Frame.h"
#include "Lib/Input_Base.h"
#include <cstdint>
#include <cstddef>
//---------------------------------------------------------------------------

class riff : public input_base_uncompressed
{
public:
    riff();

    bool                        Parse(bool AcceptTruncated = false, bool FullCheck = false);
    string                      Flavor_String();

    enum flavor
    {
        PCM_44100_8_1_U,
        PCM_44100_8_2_U,
        PCM_44100_8_6_U,
        PCM_44100_16_1_LE,
        PCM_44100_16_2_LE,
        PCM_44100_16_6_LE,
        PCM_44100_24_1_LE,
        PCM_44100_24_2_LE,
        PCM_44100_24_6_LE,
        PCM_48000_8_1_U,
        PCM_48000_8_2_U,
        PCM_48000_8_6_U,
        PCM_48000_16_1_LE,
        PCM_48000_16_2_LE,
        PCM_48000_16_6_LE,
        PCM_48000_24_1_LE,
        PCM_48000_24_2_LE,
        PCM_48000_24_6_LE,
        PCM_96000_8_1_U,
        PCM_96000_8_2_U,
        PCM_96000_8_6_U,
        PCM_96000_16_1_LE,
        PCM_96000_16_2_LE,
        PCM_96000_16_6_LE,
        PCM_96000_24_1_LE,
        PCM_96000_24_2_LE,
        PCM_96000_24_6_LE,
        Style_Max,
    };
    enum endianess
    {
        BE, // Or Signed for 8-bit
        LE, // Or Unsigned for 8-bit
    };

    // Info about formats
    uint8_t BitDepth();
    endianess Endianess();
    static uint32_t SamplesPerSec(flavor Flavor);
    static const char* SamplesPerSec_String(flavor Flavor);
    static uint8_t BitDepth(flavor Flavor);
    static const char* BitDepth_String(flavor Flavor);
    static uint8_t Channels(flavor Flavor);
    static const char* Channels_String(flavor Flavor);
    static endianess Endianess(flavor Flavor);
    static const char* Endianess_String(flavor Flavor);
    static string Flavor_String(uint8_t Flavor);

private:
    typedef void (riff::*call)();
    typedef call(riff::*name)(uint64_t);

    static const size_t Levels_Max = 16;
    struct levels_struct
    {
        name SubElements;
        uint64_t Offset_End;
    };
    levels_struct Levels[Levels_Max];
    size_t Level;
    bool IsList;

#define RIFF_ELEMENT(_NAME) \
        void _NAME(); \
        call SubElements_##_NAME(uint64_t Name);

    RIFF_ELEMENT(_);
    RIFF_ELEMENT(WAVE);
    RIFF_ELEMENT(WAVE_data);
    RIFF_ELEMENT(WAVE_fmt_);
    RIFF_ELEMENT(Void);
};

//---------------------------------------------------------------------------
#endif
