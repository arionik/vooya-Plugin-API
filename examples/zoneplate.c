/*

 gamma9.c

 Copyright (c) 2016 Arion Neddens. All rights reserved.

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
// (macOS)   clang -dynamiclib -std=gnu99 zoneplate.c -o zoneplate.dylib
// (Linux)   gcc -Wall -shared -fpic zoneplate.c -o zoneplate.so
// (Windows) cl.exe /LD zoneplate.c /Fezoneplate.dll


#include <math.h>

#include "../voo_plugin.h"

// struct to remember our state.
typedef struct {
	float state;
	unsigned int frame;
} my_plugin_t;

my_plugin_t my_context;

const int width  = 832;
const int height = 480;


// Functions vooya will call when we provide input.
//////////////////////////////////////////////////////////////////////////

VP_API BOOL in_open_nowhere( voo_app_info_t *p_app_info, void **pp_user ){
	my_plugin_t *p_loader = (my_plugin_t *)*pp_user;
	p_loader->state = 0.;
	p_loader->frame = 0;
	return TRUE;
}

VP_API void in_close( void *p_user ){}

VP_API BOOL in_get_properties( voo_sequence_t *p_info, void *p_user ){

	p_info->width = width;
	p_info->height = height;
	p_info->fps = 50;
	p_info->colorSpace = vooCS_RGB;
	p_info->arrangement = vooDA_interleaved_444;
	p_info->channel_order = vooCO_c123;
	p_info->bitsPerChannel = 8;

	return TRUE;
}

VP_API unsigned int in_framecount( void *p_user ){
	return 360;
}

VP_API BOOL in_seek( unsigned int frame, void *p_user ){
	my_plugin_t *p_loader = (my_plugin_t *)p_user;
	p_loader->state = (float)frame/360;
	p_loader->frame = frame;
	return TRUE;
}

VP_API BOOL in_load( unsigned int frame, char *p_buffer, BOOL *pb_skipped, void **pp_frame_user, void *p_user ){

	int x, y, cx, cy;
	unsigned char *p_rgb;
	my_plugin_t *p_loader = (my_plugin_t *)p_user;
	const float pi = 3.1415926536;

	p_loader->state = ((float)frame/360.f * pi/2) - pi/4;
	p_loader->frame = frame;

	// make a zoneplate
	cx = width >> 1;
	cy = height >> 1;
	p_rgb = (unsigned char *)p_buffer;
	for (y=0; y<height; y++)
	{
		float ynorm = -(float)(y-cy)/(float)height;
		for (x=0; x<width; x++)
		{
			float xnorm = -(float)(x-cx)/(float)width;
			float radius = sqrt(xnorm*xnorm+ynorm*ynorm);
			int val = (int)(128.0f * (1.0f+cos(p_loader->state*radius*width)) );
			val = val < 0 ? 0 : val > 255 ? 255 : val;
			p_rgb[ 3*x+0 + 3*width * y ] = val;
			p_rgb[ 3*x+1 + 3*width * y ] = val;
			p_rgb[ 3*x+2 + 3*width * y ] = val;
		}
	}

	*pb_skipped = FALSE;

	return TRUE;
}

VP_API BOOL in_eof( void *p_user ){
	my_plugin_t *p_loader = (my_plugin_t *)p_user;
	return p_loader->frame == 359;
}

VP_API BOOL in_good( void *p_user ){
	return TRUE;
}

VP_API BOOL in_reload( void *p_user ){
	return in_open_nowhere( 0x0, p_user );
}

VP_API void in_error( const char **pp_err, void *p_user ){
	*pp_err = "Example error message.";
}

VP_API void voo_describe( voo_plugin_t *p_plugin )
{
	p_plugin->voo_version = VOO_PLUGIN_API_VERSION;

	// plugin main properties
	p_plugin->name = "Zoneplate";
	p_plugin->description = "This is a plugin for vooya demoing input from \"nowhere\".";
	p_plugin->copyright = "(c) 2017 Arion Neddens";
	p_plugin->version = "ver1.0";

	// p_user points to static data we need everywhere.
	p_plugin->p_user = &my_context;

	p_plugin->input.uid = "voo.zoneplate.1";
	p_plugin->input.name = "zoneplate Test Sequence";
	p_plugin->input.description = "Just displays a zoneplate varying over time.";
	p_plugin->input.close = in_close;
	p_plugin->input.get_properties = in_get_properties;
	p_plugin->input.framecount = in_framecount;
	p_plugin->input.seek = in_seek;
	p_plugin->input.load = in_load;
	p_plugin->input.eof = in_eof;
	p_plugin->input.good = in_good;
	p_plugin->input.reload = in_reload;
	p_plugin->input.open_nowhere = in_open_nowhere;
	p_plugin->input.error_msg = in_error;
	p_plugin->input.b_fileBased = FALSE;
}


