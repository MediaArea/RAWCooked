/*  Copyright (c) MediaArea.net SARL & AV Preservation by reto.ch.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include "Lib/RAWcooked/RAWcooked.h"
#include "Lib/Config.h"
#include "zlib.h"
#include <cstring>
using namespace std;
//---------------------------------------------------------------------------

// EBML
static const uint32_t Name_EBML = 0x0A45DFA3;
static const uint32_t Name_EBML_Doctype = 0x0282;
static const uint32_t Name_EBML_DoctypeVersion = 0x0287;
static const uint32_t Name_EBML_DoctypeReadVersion = 0x0285;

// Top level
static const uint32_t Name_RawCookedSegment = 0x7273; // " rs", RAWcooked Segment part
static const uint32_t Name_RawCookedTrack = 0x7274; // " rt", RAWcooked Track part
static const uint32_t Name_RawCookedBlock = 0x7262; // " rb", RAWcooked BlockGroup part
// File data
static const uint32_t Name_RawCooked_BeforeData = 0x01;
static const uint32_t Name_RawCooked_AfterData = 0x02;
static const uint32_t Name_RawCooked_MaskBaseBeforeData = 0x03; // In BlockGroup only
static const uint32_t Name_RawCooked_MaskBaseAfterData = 0x04; // In BlockGroup only
static const uint32_t Name_RawCooked_MaskAdditionBeforeData = 0x03; // In Track only
static const uint32_t Name_RawCooked_MaskAdditionAfterData = 0x04; // In Track only
// File metadata
static const uint32_t Name_RawCooked_FileName = 0x10;
static const uint32_t Name_RawCooked_MaskBaseFileName = 0x11; // In BlockGroup only
static const uint32_t Name_RawCooked_MaskAdditionFileName = 0x12; // In Track only
// File stats
static const uint32_t Name_RawCooked_FileMD5 = 0x20;
static const uint32_t Name_RawCooked_FileSHA1 = 0x21;
static const uint32_t Name_RawCooked_FileSHA256 = 0x22;
// Global information
static const uint32_t Name_RawCooked_LibraryName = 0x70;
static const uint32_t Name_RawCooked_LibraryVersion = 0x71;

static const char* DocType = "rawcooked";
static const uint8_t DocTypeVersion = 1;
static const uint8_t DocTypeReadVersion = 1;
extern const char* LibraryName = "__RAWcooked__";
extern const char* LibraryVersionPreviousandSupported = "__NOT FOR PRODUCTION Alpha 2__";
extern const char* LibraryVersion = "__NOT FOR PRODUCTION Alpha 3__";

//---------------------------------------------------------------------------
static size_t Size_EB(uint32_t Name, uint64_t Size)
{
    size_t N_l = 1;
    while (Name >> (N_l * 7))
        N_l++;

    size_t S_l = 1;
    while (Size >> (S_l * 7))
        S_l++;

    return N_l + S_l + Size;
}

//---------------------------------------------------------------------------
static size_t Size_EB(uint64_t Size)
{
    size_t S_l = 1;
    while (Size >> (S_l * 7))
        S_l++;
    return S_l;
}

//---------------------------------------------------------------------------
static void Put_EB(unsigned char* Buffer, size_t& Offset, uint32_t Name, uint64_t Size)
{
    size_t N_l = 1;
    while (Name >> (N_l * 7))
        N_l++;
    uint32_t N2 = Name | (((uint32_t)1) << (N_l * 7));
    while (N_l)
    {
        Buffer[Offset] = (uint8_t)(N2 >> ((N_l - 1)*8));
        Offset++;
        N_l--;
    }

    size_t S_l = 1;
    while (Size >> (S_l * 7))
        S_l++;
    uint64_t S2 = Size | (((uint64_t)1) << (S_l * 7));
    while (S_l)
    {
        Buffer[Offset] = (uint8_t)(S2 >> ((S_l - 1)*8));
        Offset++;
        S_l--;
    }
}

//---------------------------------------------------------------------------
static void Put_EB(unsigned char* Buffer, size_t& Offset, uint64_t Size)
{
    size_t S_l = 1;
    while (Size >> (S_l * 7))
        S_l++;
    uint64_t S2 = Size | (((uint64_t)1) << (S_l * 7));
    while (S_l)
    {
        Buffer[Offset] = (uint8_t)(S2 >> ((S_l - 1) * 8));
        Offset++;
        S_l--;
    }
}

//---------------------------------------------------------------------------
rawcooked::rawcooked() :
    Unique(false),
    WriteFileCall(NULL),
    WriteFileCall_Opaque(NULL)
{
}

//---------------------------------------------------------------------------
void rawcooked::Parse()
{
    write_to_disk_struct* WriteToDisk_Data = (write_to_disk_struct*)WriteFileCall_Opaque;

    // Create or Use mask
    uint8_t* FileName = (uint8_t*)WriteToDisk_Data->FileNameDPX.c_str();
    size_t FileName_Size = WriteToDisk_Data->FileNameDPX.size();
    uint8_t* ToStore_FileName = FileName;
    uint8_t* ToStore_Before = Before;
    uint8_t* ToStore_After = After;
    bool IsUsingMask_FileName = false;
    bool IsUsingMask_Before = false;
    bool IsUsingMask_After = false;
    if (!Unique)
    {
        if (WriteToDisk_Data->IsFirstFrame)
        {
            if (FileName && FileName_Size)
            {
                WriteToDisk_Data->FirstFrame_FileName = new uint8_t[FileName_Size];
                memcpy(WriteToDisk_Data->FirstFrame_FileName, FileName, FileName_Size);
                WriteToDisk_Data->FirstFrame_FileName_Size = FileName_Size;
            }
            else
            {
                WriteToDisk_Data->FirstFrame_FileName = NULL;
                WriteToDisk_Data->FirstFrame_FileName_Size = 0;
            }
            if (Before && Before_Size)
            {
                WriteToDisk_Data->FirstFrame_Before = new uint8_t[Before_Size];
                memcpy(WriteToDisk_Data->FirstFrame_Before, Before, Before_Size);
                WriteToDisk_Data->FirstFrame_Before_Size = Before_Size;
            }
            else
            {
                WriteToDisk_Data->FirstFrame_Before = NULL;
                WriteToDisk_Data->FirstFrame_Before_Size = 0;
            }
            if (After && After_Size)
            {
                WriteToDisk_Data->FirstFrame_After = new uint8_t[After_Size];
                memcpy(WriteToDisk_Data->FirstFrame_After, After, After_Size);
                WriteToDisk_Data->FirstFrame_After_Size = After_Size;
            }
            else
            {
                WriteToDisk_Data->FirstFrame_After = NULL;
                WriteToDisk_Data->FirstFrame_After_Size = 0;
            }
        }
        if (FileName && WriteToDisk_Data->FileName)
        {
            ToStore_FileName = new uint8_t[FileName_Size];
            memcpy(ToStore_FileName, FileName, FileName_Size);
            for (size_t i = 0; i < FileName_Size || i < WriteToDisk_Data->FirstFrame_FileName_Size; i++)
                ToStore_FileName[i] -= WriteToDisk_Data->FirstFrame_FileName[i];
            IsUsingMask_FileName = true;
        }
        if (Before && WriteToDisk_Data->FirstFrame_Before)
        {
            ToStore_Before = new uint8_t[Before_Size];
            memcpy(ToStore_Before, Before, Before_Size);
            for (size_t i = 0; i < Before_Size || i < WriteToDisk_Data->FirstFrame_Before_Size; i++)
                ToStore_Before[i] -= WriteToDisk_Data->FirstFrame_Before[i];
            IsUsingMask_Before = true;
        }
        if (After && WriteToDisk_Data->FirstFrame_After)
        {
            ToStore_After = new uint8_t[After_Size];
            memcpy(ToStore_After, After, After_Size);
            for (size_t i = 0; i < After_Size || i < WriteToDisk_Data->FirstFrame_After_Size; i++)
                ToStore_After[i] -= WriteToDisk_Data->FirstFrame_After[i];
            IsUsingMask_After = true;
        }
    }

    // Test if it can be compressed
    uint8_t* Compressed_MaskFileName = NULL;
    uLongf Compressed_MaskFileName_Size = 0;
    if (!Unique && WriteToDisk_Data->FirstFrame_FileName)
    {
        Compressed_MaskFileName = new uint8_t[WriteToDisk_Data->FirstFrame_FileName_Size];
        Compressed_MaskFileName_Size = (uLongf)WriteToDisk_Data->FirstFrame_FileName_Size;
        if (compress((Bytef*)Compressed_MaskFileName, &Compressed_MaskFileName_Size, (const Bytef*)WriteToDisk_Data->FirstFrame_FileName, (uLong)WriteToDisk_Data->FirstFrame_FileName_Size) < 0)
        {
            Compressed_MaskFileName = WriteToDisk_Data->FirstFrame_FileName;
            Compressed_MaskFileName_Size = 0;
        }
    }
    uint8_t* Compressed_FileName = new uint8_t[FileName_Size];
    uLongf Compressed_FileName_Size = (uLongf)FileName_Size;
    if (compress((Bytef*)Compressed_FileName, &Compressed_FileName_Size, (const Bytef*)ToStore_FileName, (uLong)FileName_Size) < 0)
    {
        Compressed_FileName = ToStore_FileName;
        Compressed_FileName_Size = 0;
    }
    uint8_t* Compressed_MaskBefore = NULL;
    uLongf Compressed_MaskBefore_Size = 0;
    if (!Unique && WriteToDisk_Data->FirstFrame_Before)
    {
        Compressed_MaskBefore = new uint8_t[WriteToDisk_Data->FirstFrame_Before_Size];
        Compressed_MaskBefore_Size = (uLongf)WriteToDisk_Data->FirstFrame_Before_Size;
        if (compress((Bytef*)Compressed_MaskBefore, &Compressed_MaskBefore_Size, (const Bytef*)WriteToDisk_Data->FirstFrame_Before, (uLong)WriteToDisk_Data->FirstFrame_Before_Size) < 0)
        {
            Compressed_MaskBefore = WriteToDisk_Data->FirstFrame_Before;
            Compressed_MaskBefore_Size = 0;
        }
    }
    uint8_t* Compressed_Before = new uint8_t[Before_Size];
    uLongf Compressed_Before_Size = (uLongf)Before_Size;
    if (compress((Bytef*)Compressed_Before, &Compressed_Before_Size, (const Bytef*)ToStore_Before, (uLong)Before_Size) < 0)
    {
        Compressed_Before = ToStore_Before;
        Compressed_Before_Size = 0;
    }
    uint8_t* Compressed_MaskAfter = NULL;
    uLongf Compressed_MaskAfter_Size = 0;
    if (!Unique && WriteToDisk_Data->FirstFrame_After)
    {
        Compressed_MaskAfter = new uint8_t[WriteToDisk_Data->FirstFrame_After_Size];
        Compressed_MaskAfter_Size = (uLongf)WriteToDisk_Data->FirstFrame_After_Size;
        if (compress((Bytef*)Compressed_MaskAfter, &Compressed_MaskAfter_Size, (const Bytef*)WriteToDisk_Data->FirstFrame_After, (uLong)WriteToDisk_Data->FirstFrame_After_Size) < 0)
        {
            Compressed_MaskAfter = WriteToDisk_Data->FirstFrame_After;
            Compressed_MaskAfter_Size = 0;
        }
    }
    uint8_t* Compressed_After = new uint8_t[After_Size];
    uLongf Compressed_After_Size = (uLongf)After_Size;
    if (compress((Bytef*)Compressed_After, &Compressed_After_Size, (const Bytef*)ToStore_After, (uLong)After_Size) < 0)
    {
        Compressed_After = ToStore_After;
        Compressed_After_Size = 0;
    }

    uint64_t Out_Size = 0;

    // Size computing - EBML header
    uint64_t EBML_Size = 0;
    uint64_t EBML_DocType_Size;
    if (WriteToDisk_Data->IsFirstFile && WriteToDisk_Data->IsFirstFrame)
    {
        EBML_DocType_Size = strlen(DocType);
        EBML_Size += Size_EB(Name_EBML_Doctype, EBML_DocType_Size);
        EBML_Size += Size_EB(Name_EBML_DoctypeVersion, 1);
        EBML_Size += Size_EB(Name_EBML_DoctypeReadVersion, 1);
    }

    // Size computing - Library name/version
    uint64_t Segment_Size = 0;
    uint64_t LibraryName_Size;
    uint64_t LibraryVersion_Size;
    if (WriteToDisk_Data->IsFirstFile && WriteToDisk_Data->IsFirstFrame)
    {
        LibraryName_Size = strlen(LibraryName);
        Segment_Size += Size_EB(Name_RawCooked_LibraryName, LibraryName_Size);
        LibraryVersion_Size = strlen(LibraryVersion);
        Segment_Size += Size_EB(Name_RawCooked_LibraryVersion, LibraryVersion_Size);
    }

    // Size computing - Track part
    uint64_t Track_Size = 0;
    if (WriteToDisk_Data->IsFirstFrame)
    {
        if (!Unique && WriteToDisk_Data->FirstFrame_FileName)
            Track_Size += Size_EB(Name_RawCooked_MaskBaseFileName, Size_EB(Compressed_MaskFileName_Size ? WriteToDisk_Data->FirstFrame_FileName_Size : 0) + (Compressed_MaskFileName_Size ? Compressed_MaskFileName_Size : WriteToDisk_Data->FirstFrame_FileName_Size));
        if (!Unique && WriteToDisk_Data->FirstFrame_Before)
            Track_Size += Size_EB(Name_RawCooked_MaskBaseBeforeData, Size_EB(Compressed_MaskBefore_Size ? WriteToDisk_Data->FirstFrame_Before_Size : 0) + (Compressed_MaskBefore_Size ? Compressed_MaskBefore_Size : WriteToDisk_Data->FirstFrame_Before_Size));
        if (!Unique && WriteToDisk_Data->FirstFrame_After)
            Track_Size += Size_EB(Name_RawCooked_MaskBaseAfterData, Size_EB(Compressed_MaskAfter_Size ? WriteToDisk_Data->FirstFrame_After_Size : 0) + (Compressed_MaskAfter_Size ? Compressed_MaskAfter_Size : WriteToDisk_Data->FirstFrame_After_Size));
    }

    // Size computing - Block part
    uint64_t Block_Size = 0;
    uint64_t BeforeData_Size = 0;
    uint64_t FileNameData_Size = 0;
    if (Compressed_FileName_Size || FileName_Size)
    {
        FileNameData_Size = Size_EB(Compressed_FileName_Size ? FileName_Size : 0) + (Compressed_FileName_Size ? Compressed_FileName_Size : FileName_Size); // size then data
        Block_Size += Size_EB(Name_RawCooked_FileName, FileNameData_Size);
    }
    if (Compressed_Before_Size || Before_Size)
    {
        BeforeData_Size = Size_EB(Compressed_Before_Size ? Before_Size : 0) + (Compressed_Before_Size ? Compressed_Before_Size : Before_Size); // size then data
        Block_Size += Size_EB(Name_RawCooked_BeforeData, BeforeData_Size);
    }
    uint64_t AfterData_Size = 0;
    if (Compressed_After_Size || After_Size)
    {
        AfterData_Size = Size_EB(Compressed_After_Size ? After_Size : 0) + (Compressed_After_Size ? Compressed_After_Size : After_Size); // size then data
        Block_Size += Size_EB(Name_RawCooked_AfterData, AfterData_Size);
    }

    // Case if the file is unique
    if (Unique)
    {
        Track_Size += Block_Size;
        Block_Size = 0;
    }

    // Size computing - Headers
    if (EBML_Size)
        Out_Size += Size_EB(Name_EBML, EBML_Size);
    if (Segment_Size)
        Out_Size += Size_EB(Name_RawCookedSegment, Segment_Size);
    if (Track_Size)
        Out_Size += Size_EB(Name_RawCookedTrack, Track_Size);
    if (Block_Size)
        Out_Size += Size_EB(Name_RawCookedBlock, Block_Size);

    // Fill
    uint8_t* Out = new uint8_t[Out_Size];
    size_t Out_Offset = 0;

    // Fill - EBML part
    if (EBML_Size)
    {
        Put_EB(Out, Out_Offset, Name_EBML, EBML_Size);
        Put_EB(Out, Out_Offset, Name_EBML_Doctype, EBML_DocType_Size);
        memcpy(Out + Out_Offset, DocType, EBML_DocType_Size);
        Out_Offset += EBML_DocType_Size;
        Put_EB(Out, Out_Offset, Name_EBML_DoctypeVersion, 1);
        Out[Out_Offset]=DocTypeVersion;
        Out_Offset += 1;
        Put_EB(Out, Out_Offset, Name_EBML_DoctypeReadVersion, 1);
        Out[Out_Offset] = DocTypeReadVersion;
        Out_Offset += 1;
    }

    // Fill - Segment part
    if (Segment_Size)
    {
        Put_EB(Out, Out_Offset, Name_RawCookedSegment, Segment_Size);
        Put_EB(Out, Out_Offset, Name_RawCooked_LibraryName, LibraryName_Size);
        memcpy(Out + Out_Offset, LibraryName, LibraryName_Size);
        Out_Offset += LibraryName_Size;
        Put_EB(Out, Out_Offset, Name_RawCooked_LibraryVersion, LibraryVersion_Size);
        memcpy(Out + Out_Offset, LibraryVersion, LibraryVersion_Size);
        Out_Offset += LibraryVersion_Size;
    }

    // Fill - Track part
    if (Track_Size)
    {
        Put_EB(Out, Out_Offset, Name_RawCookedTrack, Track_Size);
        if (!Unique && WriteToDisk_Data->FirstFrame_FileName)
        {
            Put_EB(Out, Out_Offset, Name_RawCooked_MaskBaseFileName, Size_EB(Compressed_MaskFileName_Size ? WriteToDisk_Data->FirstFrame_FileName_Size : 0) + (Compressed_MaskFileName_Size ? Compressed_MaskFileName_Size : WriteToDisk_Data->FirstFrame_FileName_Size));
            Put_EB(Out, Out_Offset, Compressed_MaskFileName_Size ? WriteToDisk_Data->FirstFrame_FileName_Size : 0);
            memcpy(Out + Out_Offset, Compressed_MaskFileName, (Compressed_MaskFileName_Size ? Compressed_MaskFileName_Size : WriteToDisk_Data->FirstFrame_FileName_Size));
            Out_Offset += (Compressed_MaskFileName_Size ? Compressed_MaskFileName_Size : WriteToDisk_Data->FirstFrame_FileName_Size);
        }
        if (!Unique && WriteToDisk_Data->FirstFrame_Before)
        {
            Put_EB(Out, Out_Offset, Name_RawCooked_MaskBaseBeforeData, Size_EB(Compressed_MaskBefore_Size ? WriteToDisk_Data->FirstFrame_Before_Size : 0) + (Compressed_MaskBefore_Size ? Compressed_MaskBefore_Size : WriteToDisk_Data->FirstFrame_Before_Size));
            Put_EB(Out, Out_Offset, Compressed_MaskBefore_Size ? WriteToDisk_Data->FirstFrame_Before_Size : 0);
            memcpy(Out + Out_Offset, Compressed_MaskBefore, (Compressed_MaskBefore_Size ? Compressed_MaskBefore_Size : WriteToDisk_Data->FirstFrame_Before_Size));
            Out_Offset += (Compressed_MaskBefore_Size ? Compressed_MaskBefore_Size : WriteToDisk_Data->FirstFrame_Before_Size);
        }
        if (!Unique && WriteToDisk_Data->FirstFrame_After)
        {
            Put_EB(Out, Out_Offset, Name_RawCooked_MaskBaseAfterData, Size_EB(Compressed_MaskAfter_Size ? WriteToDisk_Data->FirstFrame_After_Size : 0) + (Compressed_MaskAfter_Size ? Compressed_MaskAfter_Size : WriteToDisk_Data->FirstFrame_After_Size));
            Put_EB(Out, Out_Offset, Compressed_MaskAfter_Size ? WriteToDisk_Data->FirstFrame_After_Size : 0);
            memcpy(Out + Out_Offset, Compressed_MaskAfter, (Compressed_MaskAfter_Size ? Compressed_MaskAfter_Size : WriteToDisk_Data->FirstFrame_After_Size));
            Out_Offset += (Compressed_MaskAfter_Size ? Compressed_MaskAfter_Size : WriteToDisk_Data->FirstFrame_After_Size);
        }
    }

    // Fill - Block part
    if (Block_Size)
        Put_EB(Out, Out_Offset, Name_RawCookedBlock, Block_Size);
    if (FileNameData_Size)
    {
        Put_EB(Out, Out_Offset, IsUsingMask_FileName ? Name_RawCooked_MaskAdditionFileName : Name_RawCooked_FileName, FileNameData_Size);
        Put_EB(Out, Out_Offset, Compressed_FileName_Size ? FileName_Size : 0);
        memcpy(Out + Out_Offset, Compressed_FileName, (Compressed_FileName_Size ? Compressed_FileName_Size : FileName_Size));
        Out_Offset += (Compressed_FileName_Size ? Compressed_FileName_Size : FileName_Size);
    }
    if (BeforeData_Size)
    {
        Put_EB(Out, Out_Offset, IsUsingMask_Before? Name_RawCooked_MaskAdditionBeforeData : Name_RawCooked_BeforeData, BeforeData_Size);
        Put_EB(Out, Out_Offset, Compressed_Before_Size ? Before_Size : 0);
        memcpy(Out + Out_Offset, Compressed_Before, (Compressed_Before_Size ? Compressed_Before_Size : Before_Size));
        Out_Offset += (Compressed_Before_Size ? Compressed_Before_Size : Before_Size);
    }
    if (AfterData_Size)
    {
        Put_EB(Out, Out_Offset, IsUsingMask_After ? Name_RawCooked_MaskAdditionAfterData : Name_RawCooked_AfterData, AfterData_Size);
        Put_EB(Out, Out_Offset, Compressed_After_Size ? After_Size : 0);
        memcpy(Out + Out_Offset, Compressed_After, (Compressed_After_Size ? Compressed_After_Size : After_Size));
        Out_Offset += (Compressed_After_Size ? Compressed_After_Size : After_Size);
    }

    // Write
    if (WriteFileCall)
        WriteFileCall(Out, Out_Size, WriteFileCall_Opaque);

    // Clean up
    if (Compressed_FileName_Size)
        delete[] Compressed_FileName;
    if (Compressed_Before_Size)
        delete[] Compressed_Before;
    if (Compressed_After_Size)
        delete[] Compressed_After;
    if (IsUsingMask_FileName)
        delete[] ToStore_FileName;
    if (IsUsingMask_Before)
        delete[] ToStore_Before;
    if (IsUsingMask_After)
        delete[] ToStore_After;
}
