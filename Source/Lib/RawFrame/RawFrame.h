/*  Copyright (c) MediaArea.net SARL & AV Preservation by reto.ch.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifndef RawFrameH
#define RawFrameH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Lib/Config.h"
#include <cstring>
#include <vector>
using namespace std;
//---------------------------------------------------------------------------

class raw_frame
{
public:
    uint64_t                    Flavor_Private; //Used by specialized flavor for marking the configuration of such flavor (e.g. endianness of DPX)
    uint8_t*                    Pre;
    size_t                      Pre_Size;
    uint8_t*                    Post;
    size_t                      Post_Size;
    uint8_t*                    In;
    size_t                      In_Size;
    uint8_t*                    Buffer;
    size_t                      Buffer_Size;
    bool                        Buffer_IsOwned;

    struct plane
    {
        uint8_t*                Buffer;
        size_t                  Buffer_Size;
        size_t                  Width;
        size_t                  Width_Padding;
        size_t                  Height;
        size_t                  BitsPerBlock;
        size_t                  PixelsPerBlock;

        plane(size_t Width_, size_t Height_, size_t BitsPerBlock_, size_t PixelsPerBlock_ = 1)
            :
            Width(Width_),
            Height(Height_),
            BitsPerBlock(BitsPerBlock_),
            PixelsPerBlock(PixelsPerBlock_)
        {
            Width_Padding=0; //TODO: option for padding size
            if (Width_Padding)
                Width_Padding-=Width%Width_Padding;
                
            Buffer_Size=(Width+Width_Padding)*Height*BitsPerBlock/PixelsPerBlock/8;
            Buffer=new uint8_t[Buffer_Size];
            memset(Buffer, 0, Buffer_Size);
        }

        ~plane()
        {
            delete[] Buffer;
        }

        size_t ValidBytesPerLine()
        {
            return Width*BitsPerBlock/PixelsPerBlock/8;
        }

        size_t AllBytesPerLine()
        {
            return (Width+Width_Padding)*BitsPerBlock/PixelsPerBlock/8;
        }
    };
    std::vector<plane*> Planes;

    enum flavor
    {
        Flavor_FFmpeg,
        Flavor_DPX,
        Flavor_TIFF,
        Flavor_Max,
    };
    flavor                       Flavor;

    raw_frame() :
        Flavor_Private(0),
        Pre(NULL),
        Post(NULL),
        In(NULL),
        Buffer(NULL),
        Flavor(Flavor_Max)
    {
    }
    
    ~raw_frame()
    {
        if (Buffer && Buffer_IsOwned)
            delete[] Buffer;

        for (size_t i = 0; i < Planes.size(); i++)
            delete Planes[i];
    }

    // Creation
    void Create(size_t colorspace_type, size_t width, size_t height, size_t bits_per_raw_sample, bool chroma_planes, bool alpha_plane, size_t h_chroma_subsample, size_t v_chroma_subsample);

    // Info
    size_t GetFrameSize();
    size_t GetTotalSize();

private:
    void FFmpeg_Create(size_t colorspace_type, size_t width, size_t height, size_t bits_per_raw_sample, bool chroma_planes, bool alpha_plane, size_t h_chroma_subsample, size_t v_chroma_subsample);
    void DPX_Create(size_t colorspace_type, size_t width, size_t height, size_t bits_per_raw_sample, bool chroma_planes, bool alpha_plane, size_t h_chroma_subsample, size_t v_chroma_subsample);
    void TIFF_Create(size_t colorspace_type, size_t width, size_t height, size_t bits_per_raw_sample, bool chroma_planes, bool alpha_plane, size_t h_chroma_subsample, size_t v_chroma_subsample);
};

//---------------------------------------------------------------------------
#endif
