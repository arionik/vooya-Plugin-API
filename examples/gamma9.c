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
// (macOS)   clang -dynamiclib -std=gnu99 gamma9.c -o gamma9.dylib
// (Linux)   gcc -Wall -shared -fpic gamma9.c -o gamma9.so
// (Windows) cl.exe /LD gamma9.c /Fegamma9.dll


#include <math.h>
#include "../voo_plugin.h"

VP_API double my_gamma( double in_val, int bits, void *p_user )
{
	return pow( in_val, .9 );
}

VP_API void voo_describe( voo_plugin_t *p_plugin )
{
	p_plugin->voo_version = VOO_PLUGIN_API_VERSION;

	// plugin main properties
	p_plugin->name = "Test Plugin";
	p_plugin->description = "Adds a gamma curve to the advanced "
	                        "options in the format dialog.";

	p_plugin->callbacks[0].uid = "my.gamma.1";
	p_plugin->callbacks[0].name = "Gamma .9";
	p_plugin->callbacks[0].description = "Adds Gamma .9 EOTF";
	p_plugin->callbacks[0].type = vooCallback_EOTF;
	p_plugin->callbacks[0].method_eotf = my_gamma;
}
