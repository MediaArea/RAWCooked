/*  Copyright (c) MediaArea.net SARL & AV Preservation by reto.ch.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifndef MatroskaH
#define MatroskaH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Lib/FFV1/FFV1_Frame.h"
#include "Lib/Input_Base.h"
#include <cstdint>
#include <string>
#include <vector>
#include <condition_variable>
using namespace std;
//---------------------------------------------------------------------------

class matroska_mapping;
class ThreadPool;
class flac_info;

class matroska : public input_base
{
public:
    matroska();
    ~matroska();

    bool                        Parse(bool AcceptTruncated = false, bool FullCheck = false);
    void                        Shutdown();

    string                      OutputDirectoryName;
    bool                        Quiet;

    // Theading relating functions
    void                        ProgressIndicator_Show();

    // libFLAC related helping functions
    void                        FLAC_Read(uint8_t buffer[], size_t* bytes);
    void                        FLAC_Tell(uint64_t* absolute_byte_offset);
    void                        FLAC_Metadata(uint8_t channels, uint8_t bits_per_sample);
    void                        FLAC_Write(const uint32_t* buffer[], size_t blocksize);

private:
    typedef void (matroska::*call)();
    typedef call(matroska::*name)(uint64_t);

    static const size_t Levels_Max = 16;
    struct levels_struct
    {
        name SubElements;
        uint64_t Offset_End;
    };
    levels_struct Levels[Levels_Max];
    size_t Level;
    bool IsList;

    #define MATROSKA_ELEMENT(_NAME) \
        void _NAME(); \
        call SubElements_##_NAME(uint64_t Name);
    
    MATROSKA_ELEMENT(_);
    MATROSKA_ELEMENT(Segment);
    MATROSKA_ELEMENT(Segment_Attachments);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileName);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedBlock);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedBlock_AfterData);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedBlock_BeforeData);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedBlock_FileName);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedBlock_MaskAdditionAfterData);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedBlock_MaskAdditionBeforeData);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedBlock_MaskAdditionFileName);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedSegment);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedSegment_LibraryName);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedSegment_LibraryVersion);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedSegment_PathSeparator);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedTrack);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedTrack_AfterData);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedTrack_BeforeData);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedTrack_FileName);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedTrack_LibraryName);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedTrack_LibraryVersion);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedTrack_MaskBaseAfterData);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedTrack_MaskBaseBeforeData);
    MATROSKA_ELEMENT(Segment_Attachments_AttachedFile_FileData_RawCookedTrack_MaskBaseFileName);
    MATROSKA_ELEMENT(Segment_Cluster);
    MATROSKA_ELEMENT(Segment_Cluster_SimpleBlock);
    MATROSKA_ELEMENT(Segment_Cluster_Timestamp);
    MATROSKA_ELEMENT(Segment_Tracks);
    MATROSKA_ELEMENT(Segment_Tracks_TrackEntry);
    MATROSKA_ELEMENT(Segment_Tracks_TrackEntry_CodecID);
    MATROSKA_ELEMENT(Segment_Tracks_TrackEntry_CodecPrivate);
    MATROSKA_ELEMENT(Segment_Tracks_TrackEntry_Video);
    MATROSKA_ELEMENT(Segment_Tracks_TrackEntry_Video_PixelWidth);
    MATROSKA_ELEMENT(Segment_Tracks_TrackEntry_Video_PixelHeight);
    MATROSKA_ELEMENT(Void);

    enum format
    {
        Format_None,
        Format_FFV1,
        Format_FLAC,
        Format_PCM,
        Format_Max,
    };

    string                      RAWcooked_LibraryName;
    string                      RAWcooked_LibraryVersion;
    struct trackinfo
    {
        uint8_t*                Mask_FileName;
        uint8_t*                Mask_Before;
        uint8_t*                Mask_After;
        uint8_t**               DPX_FileName;
        uint8_t**               DPX_Before;
        uint8_t**               DPX_After;
        size_t                  Mask_FileName_Size;
        size_t                  Mask_Before_Size;
        size_t                  Mask_After_Size;
        size_t*                 DPX_FileName_Size;
        size_t*                 DPX_Before_Size;
        size_t*                 DPX_After_Size;
        size_t                  DPX_Buffer_Pos;
        size_t                  DPX_Buffer_Count;
        raw_frame*              R_A;
        raw_frame*              R_B;
        flac_info*              FlacInfo;
        frame                   Frame;
        bool                    Unique;
        format                  Format;

        trackinfo() :
            Mask_FileName(NULL),
            Mask_Before(NULL),
            Mask_After(NULL),
            DPX_FileName(NULL),
            DPX_Before(NULL),
            DPX_After(NULL),
            Mask_FileName_Size(0),
            Mask_Before_Size(0),
            Mask_After_Size(0),
            DPX_FileName_Size(0),
            DPX_Before_Size(0),
            DPX_After_Size(0),
            DPX_Buffer_Pos(0),
            DPX_Buffer_Count(0),
            R_A(NULL),
            R_B(NULL),
            FlacInfo(NULL),
            Unique(false),
            Format(Format_None)
            {
            }
    };
    vector<trackinfo*>          TrackInfo;
    size_t                      TrackInfo_Pos;
    vector<uint8_t>             ID_to_TrackOrder;
    string                      AttachedFile_FileName;
    ThreadPool*                 FramesPool;
    condition_variable          ProgressIndicator_IsEnd;
    uint64_t                    Timestampscale;
    uint64_t                    Cluster_Timestamp;
    int16_t                     Block_Timestamp;

    //Utils
    void Uncompress(uint8_t* &Output, size_t &Output_Size);
    void SanitizeFileName(uint8_t* &FileName, size_t &FileName_Size);
    void RejectIncompatibleVersions();
    void ProcessCodecPrivate_FFV1();
    void ProcessCodecPrivate_FLAC();
    void ProcessFrame_FLAC();
};

//---------------------------------------------------------------------------
#endif
