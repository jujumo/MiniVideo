/*!
 * COPYRIGHT (C) 2017 Emeric Grange - All Rights Reserved
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
 * \file      depack.c
 * \author    Emeric Grange <emeric.grange@gmail.com>
 * \date      2017
 */

// minivideo headers
#include "depack.h"
#include "depack_struct.h"
#include "h264/depack_h264.h"
#include "../utils.h"
#include "../bitstream.h"
#include "../bitstream_utils.h"
#include "../minivideo_typedef.h"
#include "../minivideo_fourcc.h"
#include "../minitraces.h"

// C standard libraries
#include <stdio.h>
#include <string.h>

/* ************************************************************************** */

unsigned depack_file(MediaFile_t *media,
                     es_sample_t *essample_list)
{
    unsigned samplefound = 0;

    // TODO?

    return samplefound;
}

unsigned depack_sample(Bitstream_t *bitstr, MediaStream_t *track, unsigned sample_index,
                       es_sample_t *essample_list)
{
    TRACE_1(DEPAK, "depack_sample(dispatch)");

    unsigned samplefound = 0;

    // Choose a depacketizer
    if (track->stream_packetized == false)
    {
        // passthrough
        essample_list[0].offset = track->sample_offset[sample_index];
        essample_list[0].size = track->sample_size[sample_index];
        samplefound = 1;
    }
    else if (track->stream_codec == CODEC_H264)
    {
        samplefound = depack_h264_sample(bitstr, track, sample_index, essample_list);
    }

#if ENABLE_DEBUG
    // Sanity check
    if (samplefound > 0)
    {
        int64_t samplefoundsize = 0;

        for (unsigned i = 0; i < samplefound; i++)
            samplefoundsize += essample_list[i].size;

        if (samplefoundsize > track->sample_size[sample_index])
        {
            TRACE_ERROR(DEPAK, "DEPACK > That's weird, size mismatch: %lli vs %lli",
                        samplefoundsize, track->sample_size[sample_index]);
        }
    }
#endif // ENABLE_DEBUG

    return samplefound;
}

/* ************************************************************************** */
