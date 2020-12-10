/*!
 * COPYRIGHT (C) 2020 Emeric Grange - All Rights Reserved
 *
 * This file is part of MiniVideo.
 *
 * MiniVideo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MiniVideo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with MiniVideo.  If not, see <http://www.gnu.org/licenses/>.
 *
 * \file      mp4_gopro.c
 * \author    Emeric Grange <emeric.grange@gmail.com>
 * \date      2017
 */

// minivideo headers
#include "mp4_gopro.h"
#include "mp4_box.h"
#include "mp4_struct.h"
#include "../xml_mapper.h"
#include "../../minivideo_fourcc.h"
#include "../../minivideo_typedef.h"
#include "../../bitstream.h"
#include "../../bitstream_utils.h"
#include "../../minitraces.h"

// C standard libraries
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>

/* ************************************************************************** */

int parse_firm(Bitstream_t *bitstr, Mp4Box_t *box_header, Mp4_t *mp4)
{
    TRACE_INFO(MP4, BLD_GREEN "parse_firm()" CLR_RESET);
    int retcode = SUCCESS;

    print_box_header(box_header);
    write_box_header(box_header, mp4->xml, "GoPro Firmware");

    if (! mp4->gopro) mp4->gopro = (metadata_gopro_t*)calloc(1, sizeof(metadata_gopro_t));

    char *firmware = read_mp4_string(bitstr, (box_header->size - 8), mp4->xml, "Firmware");
    strncpy(mp4->gopro->camera_firmware, firmware, 32);
    free(firmware);

    if (mp4->xml) fprintf(mp4->xml, "  </a>\n");

    return retcode;
}

int parse_lens(Bitstream_t *bitstr, Mp4Box_t *box_header, Mp4_t *mp4)
{
    TRACE_INFO(MP4, BLD_GREEN "parse_lens()" CLR_RESET);
    int retcode = SUCCESS;

    print_box_header(box_header);
    write_box_header(box_header, mp4->xml, "GoPro Lens");

    char *lens = read_mp4_string(bitstr, (box_header->size - 8), mp4->xml, "Lens");
    free(lens);

    if (mp4->xml) fprintf(mp4->xml, "  </a>\n");

    return retcode;
}

int parse_came(Bitstream_t *bitstr, Mp4Box_t *box_header, Mp4_t *mp4)
{
    TRACE_INFO(MP4, BLD_GREEN "parse_came()" CLR_RESET);
    int retcode = SUCCESS;

    print_box_header(box_header);
    write_box_header(box_header, mp4->xml, "GoPro Camera");

    if (! mp4->gopro) mp4->gopro = (metadata_gopro_t*)calloc(1, sizeof(metadata_gopro_t));

    char *serial = read_mp4_string(bitstr, (box_header->size - 8), mp4->xml, "SerialNumber");
    strncpy(mp4->gopro->camera_serial, serial, 32);
    free(serial);

    if (mp4->xml) fprintf(mp4->xml, "  </a>\n");

    return retcode;
}

int parse_sett(Bitstream_t *bitstr, Mp4Box_t *box_header, Mp4_t *mp4)
{
    TRACE_INFO(MP4, BLD_GREEN "parse_sett()" CLR_RESET);
    int retcode = SUCCESS;

    print_box_header(box_header);
    write_box_header(box_header, mp4->xml, "GoPro Settings");

    if (! mp4->gopro) mp4->gopro = (metadata_gopro_t*)calloc(1, sizeof(metadata_gopro_t));

    if (box_header->size >= 1)
    {
        xmlSpacer(mp4->xml, "Word 1", -1);
        mp4->gopro->startup_mode = read_mp4_uint(bitstr, 4, mp4->xml, "StartupMode");
        mp4->gopro->photo_mode = read_mp4_uint(bitstr, 4, mp4->xml, "PhotoMode");
        mp4->gopro->timelapse_interval = read_mp4_uint(bitstr, 8, mp4->xml, "TimelapseInterval");
        mp4->gopro->image_flip = read_mp4_uint(bitstr, 1, mp4->xml, "ImageFlip");
        mp4->gopro->exposure_style = read_mp4_uint(bitstr, 1, mp4->xml, "ExposureStyle");

        mp4->gopro->white_balance = read_mp4_uint(bitstr, 2, mp4->xml, "WhiteBalance");
        mp4->gopro->iso_limit = read_mp4_uint(bitstr, 2, mp4->xml, "IsoLimit");
        mp4->gopro->one_button_mode = read_mp4_uint(bitstr, 1, mp4->xml, "OneButtonMode");
        mp4->gopro->osd = read_mp4_uint(bitstr, 1, mp4->xml, "OnScreenDisplay");
        mp4->gopro->leds_active = read_mp4_uint(bitstr, 2, mp4->xml, "LEDsActive");
        mp4->gopro->beep_active = read_mp4_uint(bitstr, 2, mp4->xml, "BeepActive");
        mp4->gopro->auto_off = read_mp4_uint(bitstr, 2, mp4->xml, "AutoOff");
        mp4->gopro->stereo_mode = read_mp4_uint(bitstr, 2, mp4->xml, "StereoMode");

        mp4->gopro->protune = read_mp4_uint(bitstr, 1, mp4->xml, "Protune");
        mp4->gopro->cam_raw = read_mp4_uint(bitstr, 1, mp4->xml, "CameraRaw");
    }

    if (box_header->size >= 2)
    {
        xmlSpacer(mp4->xml, "Word 2", -1);
        mp4->gopro->broadcast_range = read_mp4_uint(bitstr, 1, mp4->xml, "Broadcast_YUV_range");
        mp4->gopro->video_mode_fov = read_mp4_uint(bitstr, 2, mp4->xml, "VideoModeFOV");
        mp4->gopro->lens_type = read_mp4_uint(bitstr, 1, mp4->xml, "LensType");
        mp4->gopro->lowlight = read_mp4_uint(bitstr, 1, mp4->xml, "Lowlight");
        mp4->gopro->superview = read_mp4_uint(bitstr, 1, mp4->xml, "Superview");
        mp4->gopro->sharpening = read_mp4_uint(bitstr, 2, mp4->xml, "Sharpening");
        mp4->gopro->color_curve = read_mp4_uint(bitstr, 1, mp4->xml, "ColorCurve");
        mp4->gopro->iso_limit2 = read_mp4_uint(bitstr, 3, mp4->xml, "IsoLimit2");
        mp4->gopro->EV_compensation = read_mp4_uint(bitstr, 4, mp4->xml, "EV_Compensation");
        mp4->gopro->white_balance2 = read_mp4_uint(bitstr, 2, mp4->xml, "WhiteBalance2");
        mp4->gopro->unnamed = read_mp4_uint(bitstr, 2, mp4->xml, "Unnamed");
        mp4->gopro->eis = read_mp4_uint(bitstr, 1, mp4->xml, "EIS");
    }

    if (box_header->size >= 3)
    {
        xmlSpacer(mp4->xml, "Word 3", -1);
        mp4->gopro->media_type = read_mp4_uint(bitstr, 4, mp4->xml, "MediaType");
        skip_bits(bitstr, 28);
    }

    if (box_header->size >= 4)
    {
        xmlSpacer(mp4->xml, "Word 4", -1);
        mp4->gopro->upload_status = read_mp4_uint32(bitstr, mp4->xml, "UploadStatus");
    }

    if (mp4->xml) fprintf(mp4->xml, "  </a>\n");

    return retcode;
}

int parse_amba(Bitstream_t *bitstr, Mp4Box_t *box_header, Mp4_t *mp4)
{
    TRACE_INFO(MP4, BLD_GREEN "parse_amba()" CLR_RESET);
    int retcode = SUCCESS;

    print_box_header(box_header);
    write_box_header(box_header, mp4->xml, "Ambarella");

    //

    if (mp4->xml) fprintf(mp4->xml, "  </a>\n");

    return retcode;
}

int parse_muid(Bitstream_t *bitstr, Mp4Box_t *box_header, Mp4_t *mp4)
{
    TRACE_INFO(MP4, BLD_GREEN "parse_muid()" CLR_RESET);
    int retcode = SUCCESS;

    print_box_header(box_header);
    write_box_header(box_header, mp4->xml, "GoPro Media Unique ID");

    uint8_t *muid = read_mp4_data(bitstr, 32, mp4->xml, "Media_Unique_ID");
    free(muid);

    if (mp4->xml) fprintf(mp4->xml, "  </a>\n");

    return retcode;
}

int parse_hmmt(Bitstream_t *bitstr, Mp4Box_t *box_header, Mp4_t *mp4)
{
    TRACE_INFO(MP4, BLD_GREEN "parse_hmmt()" CLR_RESET);
    int retcode = SUCCESS;

    print_box_header(box_header);
    write_box_header(box_header, mp4->xml, "GoPro HiLight Tags");

    unsigned hmmt_count = read_mp4_uint32(bitstr, mp4->xml, "hmmt_count");

    mp4->chapters_count = hmmt_count;
    mp4->chapters = (Chapter_t *)calloc(mp4->chapters_count, sizeof(Chapter_t));

    for (unsigned i = 0; i < hmmt_count && i < 100; i++)
    {
        char tagname[16];
        snprintf(tagname, 16, "tag_%u", i);

        mp4->chapters[i].pts = read_mp4_uint32(bitstr, mp4->xml, tagname);
    }

    if (mp4->xml) fprintf(mp4->xml, "  </a>\n");

    return retcode;
}

int parse_bcid(Bitstream_t *bitstr, Mp4Box_t *box_header, Mp4_t *mp4)
{
    TRACE_INFO(MP4, BLD_GREEN "parse_bcid()" CLR_RESET);
    int retcode = SUCCESS;

    print_box_header(box_header);
    write_box_header(box_header, mp4->xml, "GoPro Broadcast ID");

    uint8_t *bcid = read_mp4_data(bitstr, 36, mp4->xml, "Broadcast_ID");
    free(bcid);

    if (mp4->xml) fprintf(mp4->xml, "  </a>\n");

    return retcode;
}

int parse_guri(Bitstream_t *bitstr, Mp4Box_t *box_header, Mp4_t *mp4)
{
    TRACE_INFO(MP4, BLD_GREEN "parse_guri()" CLR_RESET);
    int retcode = SUCCESS;

    print_box_header(box_header);
    write_box_header(box_header, mp4->xml, "GoPro Global Unique Rig Identifier");

    uint8_t *esn = read_mp4_data(bitstr, 8, mp4->xml, "Encrypted_Primary_BackPack_ESN");
    free(esn);
    uint8_t rig_type = read_mp4_uint8(bitstr, mp4->xml, "Rig_Type");
    uint32_t rig_firmware_version = read_mp4_uint(bitstr, 24, mp4->xml, "Rig_Firmware_Version");
    uint16_t rig_size = read_mp4_uint16(bitstr, mp4->xml, "Rig_Size");
    uint16_t node_id = read_mp4_uint16(bitstr, mp4->xml, "Node_ID");

    if (mp4->xml) fprintf(mp4->xml, "  </a>\n");

    return retcode;
}

int parse_gusi(Bitstream_t *bitstr, Mp4Box_t *box_header, Mp4_t *mp4)
{
    TRACE_INFO(MP4, BLD_GREEN "parse_gusi()" CLR_RESET);
    int retcode = SUCCESS;

    print_box_header(box_header);
    write_box_header(box_header, mp4->xml, "GoPro Global Unique Shot Identifier");

    uint8_t *esn = read_mp4_data(bitstr, 8, mp4->xml, "Encrypted_Primary_Camera_ESN");
    free(esn);
    uint32_t ts = read_mp4_uint32(bitstr, mp4->xml, "BackPack_Timestamp_at_Capture_Start");
    uint16_t chapter_number = read_mp4_uint16(bitstr, mp4->xml, "ChapterNumber");

    if (mp4->xml) fprintf(mp4->xml, "  </a>\n");

    return retcode;
}

int parse_gumi(Bitstream_t *bitstr, Mp4Box_t *box_header, Mp4_t *mp4)
{
    TRACE_INFO(MP4, BLD_GREEN "parse_gumi()" CLR_RESET);
    int retcode = SUCCESS;

    print_box_header(box_header);
    write_box_header(box_header, mp4->xml, "GoPro GUMI");

    //

    if (mp4->xml) fprintf(mp4->xml, "  </a>\n");

    return retcode;
}

int parse_gpmf(Bitstream_t *bitstr, Mp4Box_t *box_header, Mp4_t *mp4)
{
    TRACE_INFO(MP4, BLD_GREEN "parse_gpmf()" CLR_RESET);
    int retcode = SUCCESS;

    print_box_header(box_header);
    write_box_header(box_header, mp4->xml, "GoPro Metadata Format");

    //

    if (mp4->xml) fprintf(mp4->xml, "  </a>\n");

    return retcode;
}

/* ************************************************************************** */
