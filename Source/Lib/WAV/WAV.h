/*  Copyright (c) MediaArea.net SARL & AV Preservation by reto.ch.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifndef WAVH
#define WAVH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Lib/FFV1/FFV1_Frame.h"
#include "Lib/Input_Base.h"
#include <cstdint>
#include <cstddef>
//---------------------------------------------------------------------------

namespace wav_issue
{
    namespace undecodable { enum code : uint8_t; }
    namespace unsupported { enum code : uint8_t; }
}

class wav : public input_base_uncompressed
{
public:
    wav(errors* Errors = NULL);

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
        Flavor_Max,
    };
    enum endianness
    {
        BE, // Or Signed for 8-bit
        LE, // Or Unsigned for 8-bit
    };

    // Info about formats
    uint8_t BitDepth();
    endianness Endianness();
    static uint32_t SamplesPerSec(flavor Flavor);
    static const char* SamplesPerSec_String(flavor Flavor);
    static uint8_t BitDepth(flavor Flavor);
    static const char* BitDepth_String(flavor Flavor);
    static uint8_t Channels(flavor Flavor);
    static const char* Channels_String(flavor Flavor);
    static endianness Endianness(flavor Flavor);
    static const char* Endianness_String(flavor Flavor);

private:
    void                        ParseBuffer();
    void                        BufferOverflow();
    void                        Undecodable(wav_issue::undecodable::code Code) { input_base::Undecodable((error::undecodable::code)Code); }
    void                        Unsupported(wav_issue::unsupported::code Code) { input_base::Unsupported((error::unsupported::code)Code); }
    typedef void (wav::*call)();
    typedef call (wav::*name)(uint64_t);

    static const size_t Levels_Max = 16;
    struct levels_struct
    {
        name SubElements;
        uint64_t Offset_End;
    };
    levels_struct Levels[Levels_Max];
    size_t Level;
    bool IsList;

    // Temp
    uint32_t                    BlockAlign = 0;

#define WAV_ELEMENT(_NAME) \
        void _NAME(); \
        call SubElements_##_NAME(uint64_t Name);

    WAV_ELEMENT(_);
    WAV_ELEMENT(WAVE);
    WAV_ELEMENT(WAVE_data);
    WAV_ELEMENT(WAVE_fmt_);
    WAV_ELEMENT(Void);
};

string WAV_Flavor_String(uint8_t Flavor);

//---------------------------------------------------------------------------
#endif
