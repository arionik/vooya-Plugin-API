/*

 diff.c

 Copyright (c) 2020 Arion Neddens. All rights reserved.

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 
*/

// compile at least with:
// (macOS)   clang -dynamiclib -std=gnu99 diff.c -o diff.dylib
// (Linux)   gcc -Wall -shared -fpic diff.c -o diff.so
// (Windows) cl.exe /LD diff.c /Fediff.dll


#include <math.h>
#include <stdio.h>
#include "../voo_plugin.h"

VP_API void my_diff( voo_diff_t *p_diff )
{
	int x,y,k;
	int pels[3];
	voo_video_frame_metadata_t *p_md = p_diff->p_metadata;
	voo_sequence_t *p_info = p_md->p_info;
	float max = (float)(1 << p_info->bits_per_channel) - 1;
	pels[0] = p_info->width*p_info->height;
	pels[1] = pels[2] = p_info->width/p_info->chroma_subsampling_hor
		* p_info->height/p_info->chroma_subsampling_ver;

	float v[3] = {0,0,0};
	for (y=0; y<p_info->height; y++) {
		for (x=0; x<p_info->width; x++) {
			float d = p_diff->c1_a[x+p_diff->stride*y] - p_diff->c1_b[x+p_diff->stride*y];
			v[0] += d * d;
		}
	}
	for (y=0; y<p_info->height/p_info->chroma_subsampling_ver; y++) {
		for (x=0; x<p_info->width/p_info->chroma_subsampling_hor; x++) {
			float d2 = p_diff->c2_a[x+p_diff->stride*y] - p_diff->c2_b[x+p_diff->stride*y];
			float d3 = p_diff->c3_a[x+p_diff->stride*y] - p_diff->c3_b[x+p_diff->stride*y];
			v[1] += d2 * d2;
			v[2] += d3 * d3;
		}
	}

	float psnr[3];
	for (k=0; k<3; k++) {
		v[k] = v[k] / pels[k];
		psnr[k] = 10.f * log10f( max*max / v[k] );
	}

	char buffer[200];
	sprintf(buffer, "psnr %1.2f |  %1.2f |  %1.2f", psnr[0], psnr[1], psnr[2]);
	p_md->pfun_add_text(p_md->p_textfun_cargo, buffer, vooPluginTextFlag_AlignRight, p_info->width - 20, 180);
}

VP_API void voo_describe( voo_plugin_t *p_plugin )
{
	p_plugin->voo_version = VOO_PLUGIN_API_VERSION;

	// plugin main properties
	p_plugin->name = "Diff Test Plugin";
	p_plugin->description = "Adds a difference calculation entry to the tools menu.";

	p_plugin->callbacks[0].uid = "my.diff.0";
	p_plugin->callbacks[0].name = "PSNR";
	p_plugin->callbacks[0].description = "Diff Demo";
	p_plugin->callbacks[0].cb_type = vooCallback_Diff;
	p_plugin->callbacks[0].method_diff = my_diff;
}
