/*  Copyright (c) MediaArea.net SARL & AV Preservation by reto.ch.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include "CLI/Help.h"
#include "iostream"
using namespace std;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
extern const char* LibraryName;
extern const char* LibraryVersion;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
ReturnValue Help(const char* Name)
{
    // Copy of rawcooked.1 manpage, with limit to 80 columns
    cout << "NAME" << endl;
    cout << "       RAWcooked - encode and decode audio-visual RAW data with Matroska, FFV1" << endl;
    cout << "       and FLAC" << endl;
    cout << endl;
    cout << "SYNOPSIS" << endl;
    cout << "       " << Name << " [option ...] (folder | file ...) [option ...]" << endl;
    cout << endl;
    cout << "DESCRIPTION" << endl;
    cout << "       RAWcooked encodes audio-visual RAW data into the Matroska container" << endl;
    cout << "       (MKV), using the video codec FFV1 for the image and the audio codec" << endl;
    cout << "       FLAC for the sound. The metadata accompanying the RAW data are" << endl;
    cout << "       preserved, and sidecar files, like MD5, LUT or XML, can be added into" << endl;
    cout << "       the Matroska container as attachments. This allows for the management" << endl;
    cout << "       of these audio-visual file formats in an effective and transparent way" << endl;
    cout << "       (e.g. native playback in VLC), while saving typically between  one to" << endl;
    cout << "       two thirds of the needed storage, and speeding up file writing and" << endl;
    cout << "       reading (e.g. to/from harddisk, over network or for backup on LTO)." << endl;
    cout << endl;
    cout << "       When needed, the uncompressed source is retrieved bit-by-bit, in a" << endl;
    cout << "       manner faster than uncompressed sources." << endl;
    cout << endl;
    cout << "       folder Encodes with the FFV1 video codec all single-image video files" << endl;
    cout << "              in the folder, encodes with the FLAC audio codec all audio files" << endl;
    cout << "              in the folder, and muxes these into a Matroska container (.mkv)." << endl;
    cout << "              The filenames of the single image files must end with a numbered" << endl;
    cout << "              sequence. RAWcooked will generate the regex to parse in the" << endl;
    cout << "              correct order all the frames in the folder." << endl;
    cout << endl;
    cout << "       file   contains RAW data (e.g. a .dpx or .wav file):" << endl;
    cout << "              Encodes with the FFV1 video codec all single-image video files" << endl;
    cout << "              in the folder containing the file, encodes with the FLAC audio" << endl;
    cout << "              codec all audio files in the folder containing the file, and" << endl;
    cout << "              muxes these into a Matroska container (.mkv)." << endl;
    cout << "              The filenames usually end with a numbered  sequence. Enter one" << endl;
    cout << "              frame and the tool will generate the regex to parse all the" << endl;
    cout << "              frames in the folder." << endl;
    cout << endl;
    cout << "       file   is a Matroska container (.mkv):" << endl;
    cout << "              Decodes back the video and the audio of file to its original" << endl;
    cout << "              formats. All metadata accompanying the original data are" << endl;
    cout << "              preserved bit-by-bit." << endl;
    cout << endl;
    cout << "GENERAL OPTIONS" << endl;
    cout << "       --help | -h" << endl;
    cout << "              Displays a help message." << endl;
    cout << endl;
    cout << "       --version" << endl;
    cout << "              Displays the installed version." << endl;
    cout << endl;
    cout << "       --store-license value" << endl;
    cout << "              Set the license key to value and store on hard drive." << endl;
    cout << "              (license is stored in ~/.config/RAWcooked/Config.txt on Linux/Mac," << endl;
    cout << "              %APPDATA%/RAWcooked/Config.txt on Windows)"  << endl;
    cout << endl;
    cout << "       --show-license" << endl;
    cout << "              Displays information about the installed license." << endl;
    cout << endl;
    cout << "       --attachment-max-size value | -s value" << endl;
    cout << "              Set maximum size of attachment to value (in bytes)." << endl;
    cout << "              Default value is 1048576." << endl;
    cout << endl;
    cout << "       --display-command | -d" << endl;
    cout << "              When an external encoder/decoder is used, display the command to" << endl;
    cout << "              launch instead of launching it." << endl;
    cout << endl;
    cout << "       --output-name value | -o value" << endl;
    cout << "              Set the name of the output file or folder to value." << endl;
    cout << "              Default value is ${Input}.mkv (if input is a folder)" << endl;
    cout << "              or ${Input}.RAWcooked (if input is a file)." << endl;
    cout << endl;
    cout << "       --rawcooked-file-name value | -r value" << endl;
    cout << "              Set (encoding) or get (decoding) the name of the RAWcooked" << endl;
    cout << "              reversibility data file to value." << endl;
    cout << "              Default name is ${Input}.rawcooked_reversibility_data" << endl;
    cout << "              Note: If the RAWcooked reversibility data file is included in" << endl;
    cout << "              the output A/V file during the encoding, this file is deleted" << endl;
    cout << "              after encoding." << endl;
    cout << "              Note: Not yet implemented for decoding." << endl;
    cout << endl;
    cout << "       --quiet" << endl;
    cout << "              Do not show information related to RAWcooked." << endl;
    cout << "              Encoder application may need an additional option." << endl;
    cout << "       -y" << endl;
    cout << "              Automatic yes to prompts. Assume \"yes\" as answer" << endl;
    cout << "              to all prompts and run non-interactively." << endl;
    cout << "       -n" << endl;
    cout << "              Automatic no to prompts. Assume \"no\" as answer" << endl;
    cout << "              to all prompts and run non-interactively." << endl;
    cout << endl;
    cout << "ACTIONS" << endl;
    cout << "       --all" << endl;
    cout << "              Same as --info --conch --decode --encode --hash" << endl;
    cout << "               --coherency --check-padding --check"<< endl;
    cout << "       --none" << endl;
    cout << "              Same as --no-info --no-conch --no-decode --no-encode --no-hash" << endl;
    cout << "               --no-coherency --quick-check-padding --quick-check" << endl;
    cout << endl;
    cout << "       --check" << endl;
    cout << "              Check that the encoded file can be correctly decoded." << endl;
    cout << "              If input is raw content, encode then check that output" << endl;
    cout << "              would be same as the input content." << endl;
    cout << "              If input is compressed content, check that output" << endl;
    cout << "              would be same as the original content." << endl;
    cout << "              Disable decoding." << endl;
    cout << "       --quick-check" << endl;
    cout << "              Do quick coherency checks of the encoded file." << endl;
    cout << "              It permits to check that the file seems healthy" << endl;
    cout << "              without the time and processor usage of the full check." << endl;
    cout << "              Is ignored in case of compressed content." << endl;
    cout << "              Is default (it may change in the future)" << endl;
    cout << "       --no-check" << endl;
    cout << "              Don't do checks (see above)." << endl;
    cout << endl;
    cout << "       --info" << endl;
    cout << "              Provide some extra information about the compressed file," << endl;
    cout << "              for example the presence of hash of the raw data." << endl;
    cout << "              Disable encoding and decoding." << endl;
    cout << "       --no-info" << endl;
    cout << "              Don't provide some extra information (see above)." << endl;
    cout << "              Is default (it may change in the future)" << endl;
    cout << endl;
    cout << "       --check-padding" << endl;
    cout << "              Run padding checks. Be aware check function can be demanding" << endl;
    cout << "              of time and processor usage." << endl;
    cout << "              It is a slower process but guarantees reversibility, for example" << endl;
    cout << "              with DPX files that have bits with no zero padding." << endl;
    cout << "       --quick-check-padding" << endl;
    cout << "              Switch to --check-padding or --no-check-padding depending on" << endl;
    cout << "              what is found in the first image." << endl;
    cout << "              The program will stop with an error code if --check is not" << endl;
    cout << "              used at the same time and zero-padding bits are in the content," << endl;
    cout << "              asking to choose what to do." << endl;
    cout << "              Is default (it may change in the future)" << endl;
    cout << "       --no-check-padding" << endl;
    cout << "              Do not run padding checks, as they are demanding of time" << endl;
    cout << "              and processor usage." << endl;
    cout << "              This method is quicker but may lead to partial reversibility" << endl;
    cout << "              with non conform files." << endl;
    cout << endl;
    cout << "       --coherency" << endl;
    cout << "              Coherency check of the package." << endl;
    cout << "              Currently partially implemented." << endl;
    cout << "              Is default (it may change in the future)" << endl;
    cout << "       --no-coherency" << endl;
    cout << "              Don't do coherency check (see above)." << endl;
    cout << endl;
    cout << "       --conch" << endl;
    cout << "              Conformance check of the format, when supported." << endl;
    cout << "              Currently partially implemented for DPX." << endl;
    cout << "              Disable encoding and decoding." << endl;
    cout << "       --no-conch" << endl;
    cout << "              Don't do conformance check (see above)." << endl;
    cout << "              Is default (it may change in the future)" << endl;
    cout << endl;
    cout << "       --decode" << endl;
    cout << "              Decode a compressed stream into audio-visual RAW data." << endl;
    cout << "              Is default" << endl;
    cout << "       --no-decode" << endl;
    cout << "              Don't decode (see above)." << endl;
    cout << endl;
    cout << "       --encode" << endl;
    cout << "              Encode audio-visual RAW data into a compressed stream." << endl;
    cout << "              Is default" << endl;
    cout << "       --no-encode" << endl;
    cout << "              Don't encode (see above)." << endl;
    cout << endl;
    cout << "       --hash" << endl;
    cout << "              Compute the hash of audio-visual RAW data files." << endl;
    cout << "              If input is raw content, do the hash and store hashes in RAWcooked" << endl;
    cout << "              metadata." << endl;
    cout << "              This permits a reversibility check without the original files." << endl;
    cout << "       --no-hash" << endl;
    cout << "              Don't do compute or test of hash of files. (see above)." << endl;
    cout << "              Is default (it may change in the future)" << endl;
    cout << endl;
    cout << "       --framemd5" << endl;
    cout << "              Compute the framemd5 of input frames and store it to a sidecar" << endl;
    cout << "              file." << endl;
    cout << "              See FFmpeg framemd5 documentation for more information." << endl;
    cout << "       --framemd5-name value" << endl;
    cout << "              Set the name of the framemd5 file to value." << endl;
    cout << "              Default value is ${Input}.framemd5." << endl;
    cout << "       --no-framemd5" << endl;
    cout << "              Don't do compute of framemd5 of input frames. (see above)." << endl;
    cout << "              Is default." << endl;
    cout << endl;
    cout << "INPUT RELATED OPTIONS" << endl;
    cout << "       --file" << endl;
    cout << "              Unlock compression of files (e.g. a .dpx or .wav)." << endl;
    cout << "       -framerate value" << endl;
    cout << "              Force video frame rate to value." << endl;
    cout << "              Default value is the one found in the image files if available," << endl;
    cout << "              otherwise 24." << endl;
    cout << endl;
    cout << "ENCODING RELATED OPTIONS" << endl;
    cout << "       -c:a value" << endl;
    cout << "              Force the audio encoding format to value: copy (copy PCM to PCM," << endl;
    cout << "              without modification), flac." << endl;
    cout << "              Default value is flac." << endl;
    cout << endl;
    cout << "       -c:v value" << endl;
    cout << "              Force the video encoding format value: only ffv1 is currently" << endl;
    cout << "              allowed, which is the default value." << endl;
    cout << endl;
    cout << "       -coder value" << endl;
    cout << "              If video encoding format is ffv1, set the coder to value: 0" << endl;
    cout << "              (Golomb-Rice), 1 (Range Coder), 2 (Range Coder with custom state" << endl;
    cout << "              transition table)." << endl;
    cout << "              Default value is 1." << endl;
    cout << endl;
    cout << "       -context value" << endl;
    cout << "              If video encoding format is ffv1, set the context to value: 0" << endl;
    cout << "              (small), 1 (large)." << endl;
    cout << "              Default value is 0." << endl;
    cout << endl;
    cout << "       -format value" << endl;
    cout << "              Set the container format to value: only matroska is currently" << endl;
    cout << "              allowed, which is the default value." << endl;
    cout << endl;
    cout << "       -g value" << endl;
    cout << "              If video encoding format is ffv1, set the GOP size to value any" << endl;
    cout << "              integer >=1." << endl;
    cout << "              Default value is 1." << endl;
    cout << endl;
    cout << "       -level value" << endl;
    cout << "              If video encoding format is ffv1, set the version to value: 0," << endl;
    cout << "              1, 3." << endl;
    cout << "              Default value is 3." << endl;
    cout << endl;
    cout << "       -slicecrc value" << endl;
    cout << "              If  video  encoding format is ffv1, set the CRC to value: 0 (CRC" << endl;
    cout << "              not present), 1 (CRC present)." << endl;
    cout << "              Default value is 1." << endl;
    cout << endl;
    cout << "       -slices value" << endl;
    cout << "              If video encoding format is ffv1, set the count of slices to" << endl;
    cout << "              value: any integer >=1 and making sense (2, 4, 6, 9, 16, 24...)." << endl;
    cout << "              Default value is between 16 and 512, depending on video frame" << endl;
    cout << "              size and depth." << endl;
    cout << endl;
    cout << "COPYRIGHT" << endl;
    cout << "       Copyright (c) 2018-2020 MediaArea.net SARL & AV Preservation by reto.ch" << endl;
    cout << endl;
    cout << "LICENSE" << endl;
    cout << "       RAWcooked is released under a BSD License." << endl;
    cout << endl;
    cout << "DISCLAIMER" << endl;
    cout << "       RAWcooked is provided \"as is\" without warranty or support of any kind." << endl;
    cout << endl;

  return ReturnValue_OK;
}

//---------------------------------------------------------------------------
ReturnValue Usage(const char* Name)
{
  printf("Usage: \"%s [options] DirectoryName [options]\"\n", Name);
  printf("\"%s --help\" for displaying more information\n", Name);
  printf("or \"man %s\" for displaying the man page\n", Name);

  return ReturnValue_OK;
}

//---------------------------------------------------------------------------
ReturnValue Version()
{
  printf("%s %s\n", LibraryName, LibraryVersion);

  return ReturnValue_OK;
}
