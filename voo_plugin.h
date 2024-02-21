/*

 voo_plugin.h

 Do not change anything in this file!

 Copyright (c) 2016-2022 Arion Neddens. All rights reserved.

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


#define VOO_PLUGIN_API_VERSION 9

#ifdef __cplusplus
	#define vooEXTERN_C extern "C"
#else
	#define vooEXTERN_C
#endif

#ifdef _WIN32
	#include <wchar.h>
	typedef wchar_t vooChar_t;
	#define voo_strlen wcslen
	#define voo_strcmp wcscmp
	#define voo_strncpy wcsncpy
	#define voo_fopen _wfopen
	#define voo_snprintf _snwprintf
	#define _v(v)L##v
	#define VP_API vooEXTERN_C __declspec(dllexport)
#else
	typedef char vooChar_t;
	#define voo_strlen strlen
	#define voo_strcmp strcmp
	#define voo_strncpy strncpy
	#define voo_snprintf snprintf
	#define voo_fopen fopen
	#define _v(v)v
	#ifdef VOO_PLUGIN_SYMBOLS_HIDDEN
		#define VP_API vooEXTERN_C __attribute__ ((visibility ("default")))
	#else
		#define VP_API vooEXTERN_C
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef int vooBOOL;
#define TRUE  1
#define FALSE 0


// display pixel data type
typedef struct
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char x;
} voo_target_space_t;


typedef enum {
	vooColorSpace_Unknown = -1,
 	vooCS_YUV,
 	vooCS_XYZ,
	vooCS_YIQ,
	vooCS_RGB,
	vooCS_Gray,
	vooCS_HSV,
	vooCS_YCgCo,
	vooCS_ICtCp
} voo_colorSpace_t;


typedef enum {
    vooTF_None,
    vooTF_bt1886,
    vooTF_PQ,
    vooTF_HLG
} voo_transferFunction_t;


typedef enum {
	vooCC_None,
	vooCC_rec709,
	vooCC_rec2020,
} voo_color_spec_t;


typedef enum
{
	vooDataArrangement_Unknown = -1,
	vooDA_planar_420,
	vooDA_planar_422,
	vooDA_planar_444,
	vooDA_planar_410,
	vooDA_planar_411,
	vooDA_uyvy,
	vooDA_yuyv,
	vooDA_yuy2,
	vooDA_nv12,
	vooDA_v210,
	vooDA_interleaved_410,
	vooDA_interleaved_411,
	vooDA_reserved0,
	vooDA_interleaved_422,
	vooDA_interleaved_444,
	vooDA_single,
	vooDA_singleDouble,
	vooDA_singleFloat,
	vooDA_planar_420double,
	vooDA_planar_422double,
	vooDA_planar_444double,
	vooDA_planar_410double,
	vooDA_planar_411double,
	vooDA_planar_420float,
	vooDA_planar_422float,
	vooDA_planar_444float,
	vooDA_planar_410float,
	vooDA_planar_411float,
	vooDA_rgb565,
	vooDA_rgb555,
	vooDA_r210,
	vooDA_v410,
	vooDA_yuv10,
	vooDA_p010,
	vooDA_p016,
	vooDA_interleaved444float,
	vooDA_interleaved444Double,
	vooNumDataArrangements
} voo_dataArrangement_t;


typedef enum
{
	vooChannelOrder_Unknown = -1,

	vooCO_c123,
	vooCO_c231,
	vooCO_c312,

	vooCO_c213,
	vooCO_c321,
	vooCO_c132,

	vooCO_c123x,
	vooCO_c231x,
	vooCO_c312x,

	vooCO_c213x,
	vooCO_c321x,
	vooCO_c132x,

	vooCO_cx123,
	vooCO_cx231,
	vooCO_cx312,

	vooCO_cx213,
	vooCO_cx321,
	vooCO_cx132,

	vooNumChannelOrders
} voo_channelOrder_t;


typedef enum {
	vooOr_Top, vooOr_Right, vooOr_Bottom, vooOr_Left
} voo_orientation_t;


// structure vooya gives you in on_load_video( ... ).
typedef struct {

	// a handle to vooya's current window (what it is, is platform dependent)
	void *p_handle;

	// to trigger vooya to a reload a frame, use these like:
	//     p_app_info->pf_trigger_reload( app_info->p_reload_cargo )
	// note that this should happen not too often.
	void *p_reload_cargo;
	int (*pf_trigger_reload)( void* );

	// send a message to the console window in vooya
	void *p_message_cargo;
	void (*pf_console_message)( void*, const char* );

	char reserved[4*sizeof(void*)];

} voo_app_info_t;


// structure to describe a video sequence; you get this in on_select( ... )
// when a new video has been loaded or your plugin is selected. Or, if you provide input_plugin_t,
// you fill this structure in get_properties( ... ). Refer to the above data types.
typedef struct  
{
	const vooChar_t *filename;

	// video resolution
	int width;
	int height;

	// frames per seconds
	double fps;

	// Color space, such as YUV, RGB etc.
	voo_colorSpace_t color_space;

	// How the channels are packed or interleaved
	voo_dataArrangement_t arrangement;

	// The order in which color channels are written
	voo_channelOrder_t channel_order;

	// size in bytes of a single video frame in native format
	unsigned int frame_size;

	// Bits per channel is normally 8 or 10-16 (valid bit depths are 1-16) (if integer)
	int bits_per_channel;

	// Whether the video shall be played upside down
	vooBOOL b_flipped;
	// Whether 16bit words shall be byte-swapped
	vooBOOL b_toggle_endian;
	// Whether the values (if integer) shall be treated as signed integers
	vooBOOL b_signed;

	// number of frames in sequences
	unsigned int frame_count;

	// Chroma subsampling. Set, but never read by vooya.
	int chroma_subsampling_hor;
	int chroma_subsampling_ver;

	// narrow signal range
	vooBOOL b_narrow;
	
	// transfer function
	voo_transferFunction_t transfer;
	
	// max nits
	float max_nits, min_nits;
    
	voo_color_spec_t color_spec;
	
	// incoming orientation
	voo_orientation_t orientation;

} voo_sequence_t;


// Structure you get in per-frame callback functions.
typedef struct {

	// user data you might have provided in voo_describe( ... ) as voo_plugin_t::p_user
	void *p_user;

	// per-sequence user data you might have provided in voo_plugin_t::on_load_video( ... )
	void *p_user_video;

	// per-frame user data you might have provided in input_plugin_t::load( ... )
	void *p_user_frame;

	voo_sequence_t *p_info;  // info about the current sequence

	// frame number, beginning at zero
	unsigned int frame_idx;

#define vooPluginTextFlag_AlignRight  0x01
#define vooPluginTextFlag_AlignCenter 0x02
	// Tells vooya to display text for the given frame at the given position x,y relative to the video resolution.
	// This function can be called from within an on_frame_done callback (and only from there)
	// For "flags" see above, use p_textfun_cargo for "p_cargo"
	void (*pfun_add_text)( void *p_cargo, const vooChar_t *text, int flags, int x, int y );
	// Tells vooya to clear all text for the given frame.
	// This function can be called from within an on_frame_done callback (and only from there)
	// Use p_textfun_cargo for "p_cargo"
	void (*pfun_clear_all)(void *p_cargo);
	void *p_textfun_cargo;

#define VOOPerFrameFlag_YouAlreadyProcessed 0x01 // this frame has already been processed by you
#define VOOPerFrameFlag_IsFromCache         0x02 // this one comes from RGB-display cache
#define VOOPerFrameFlag_IsDifference        0x04 // this frame is a difference frame
	int flags;

	char reserved[4*sizeof(void*)];

} voo_video_frame_metadata_t;


// structure that is passed to frame-wise difference callbacks.
enum { vooDiffValueTypeChannel, vooDiffValueTypeFrame, vooDiffValueTypeSequence };
typedef struct {

	// Pixel buffer a and b from sequence A and B, component 1,2,3
	// (original value range and subsampling, but planar and converted to 32bit float)
	const float *c1_a;
	const float *c2_a;
	const float *c3_a;
	const float *c1_b;
	const float *c2_b;
	const float *c3_b;

	const int stride;

	voo_video_frame_metadata_t *p_metadata;
	
	const char *component_names[3];  // e.g. psnr(y), psnr(cb) psnr(cr)
	int value_type;                  // e.g. vooDiffValueTypeChannel
	double result_values[3];         // result values of the calculation
	
} voo_diff_t;


// PLUGIN CALLBACK FUNCTION STRUCT
//
// This struct shall contain user-defined callback functions along with some metadata.
// First the callback types:
typedef enum {
	vooCallback_Native,
 	vooCallback_RGBOut,
 	vooCallback_EOTF,
 	vooCallback_Histogram,
	vooCallback_Diff,
} vooya_callback_type_t;

// And the actual callback description structure:
typedef struct
{
	// The following strings must be set and be persistent throughout plugin's linkage.
	// uid must not be empty or NULL.
	const char *uid;         // a unique string, e.g. "myplugin.rgb_invert.1",
	                         // at most 63 chars in length, ANSI without any whitespace
	const char *name;        // a user-friendly, descriptive name
	const char *description; // a more in-depth description

	// Functions vooya will call upon user's (de)selection of this callback (optional)
	void (*on_select)( voo_sequence_t *p_info, voo_app_info_t *p_app_info, void *p_user, void **pp_user_video );
	void (*on_deselect)( void *p_user, void *p_user_video );

	// this function will be called when a frame has completed processing and is about to be displayed.
	// May be called multiple times for the same frame.
	void (*on_frame_done)( voo_video_frame_metadata_t * );

	char reserved[4];
	
	// type determines which callback signature will be called
	vooya_callback_type_t cb_type;

	union{
		// actual callback function (required)

		// For type == vooCallback_RGBOut:
		// Called by vooya for each video frame with rgb data ready to be rendered,
		// i.e. color-converted, range-shifted to 8bit and with EOTF and image
		// adjustments applied. Can be used to feed the data outside of vooya as
		// well as to alter the data right before display.
		// Stride in bytes is equal to width*sizeof(voo_target_space_t).
		void (*method_rgb_out)( voo_target_space_t *p_data,
							   voo_video_frame_metadata_t *p_metadata );

		// For type == vooCallback_Native:
		// Called by vooya for each video frame with native data before color
		// conversion to display format, and without image adjustments. Can be used to
		// feed the data outside of vooya. Properties like resolution
		// and data format are given beforehand in on_load_video( ... ); you can
		// save them in p_metadata->p_user_video. "p_data" is the image data.
		// Data format is 32bit float planar, with equal stride for all planes,
		// but same chroma subsampling as the input.
		void (*method_native)( float *ch1, float *ch2, float *ch3, int stride,
							  voo_video_frame_metadata_t *p_metadata );

		// For type == vooCallback_EOTF:
		// Called by vooya when a lookup-table for the transfer function is being made.
		// "value" is in the range of 0-1, representing an RGB channel value of input bit
		// depth ("bits"). "p_user" might be provided by you from within voo_describe(...)
		// and can be NULL or any custom data. The call of this function happens before
		// application of brightness, contrast, gamma and exposure user settings and
		// after conversion (including gamma) to the target RGB colorspace.
		double (*method_eotf)( double value, int bits, void *p_user );

		// For type == vooCallback_Histogram:
		// Called by vooya for each frame if histogram calculation (and display) is enabled.
		// The three pointers contain the histograms for each channel respectively. Their
		// length is (1<<bit_depth)-1 (floating point data is put into 12bits).
		void (*method_histogram)( unsigned int *p_h1, unsigned int *p_h2,
					unsigned int *p_h3, voo_video_frame_metadata_t *p_metadata );

		// For type == vooCallback_Diff:
		// Called by vooya when two sequences are being compared.
		// This method is called pixel-wise and thus not the fastest. Note that multiple threads
		// (all for the same frame) might call this function concurrently.
		// see also voo_diff_t
		void (*method_diff)( voo_diff_t *p_diff_pixel );
	};
	
} vooya_callback_t;



// PLUGIN INPUT DESCRIPTION STRUCT
//
// Container to provide custom input to vooya from file or from "nowhere".
typedef struct {

	const char *uid;         // a unique string, e.g. "myplugin.text.input",
	                         // at most 63 chars in length, ANSI without any whitespace
	const char *name;        // a user-friendly, descriptive name (mandatory)
	const char *description; // a more in-depth description
	
	// If b_fileBased is TRUE, vooya will ask for file suffixes supported by this input,
	// call file_suffixes( ... ), responsible( ... ) and open( ... ), and will include
	// this input in the file open dialog. If b_fileBased is FALSE, an entry for this input
	// will be displayed in the plugins-menu that the user can select as current input.
	// In that case, vooya will call open_nowhere( ... ).
	vooBOOL b_fileBased;

	char reserved1[12];

	// If the input is file-based, responsible will be called with the file name and the
	// first sixteen bytes of data, which e.g. might contain magic data. p_user is
	// voo_plugin_t::p_user. If responsible returns TRUE, open will be called.
	// Only if input comes from stdin and "--container [your input UID]" is specified,
	// responsible will not be called, but open( ... ) directly.
	// For stdin, the filename is simply "-".
	vooBOOL (*responsible)( const vooChar_t *filename, char *sixteen_bytes, void *p_user );
	// The global p_user pointer you may have set in voo_describe( ... )
	// is given here as *pp_user_seq, but you can alter it. In that case, subsequent
	// calls to methods of this struct will have the new, per-sequence value. This is
	// important on macOS, where multiple instances of this input may exist.
	vooBOOL (*open)( const vooChar_t *filename, voo_app_info_t *p_app_info, void **pp_user_seq );

	// If the input is not based on file input (b_fileBased is FALSE),
	// open_nowhere will be called. The global p_user pointer you may have set in
	// voo_describe( ... ) is given here as *pp_user_seq, but you can alter it.
	// In that case, subsequent calls to methods of this struct will have the new,
	// per-sequence value. This is important on macOS, where multiple instances
	// of this input may exist.
	vooBOOL (*open_nowhere)( voo_app_info_t *p_app_info, void **pp_user_seq );

	// Called by vooya to get information about the video you provide.
	// You should fill p_info with correct information to make vooya play.
	vooBOOL (*get_properties)( voo_sequence_t *p_info, void *p_user_seq );

	// Client shall return the number of frames available, or ~0U if no
	// framecount can be given (e.g. stdin).
	unsigned int (*framecount)( void *p_user_seq );

	// Shall issue a seek by the client plugin to frame number "frame"
	vooBOOL (*seek)( unsigned int frame, void *p_user_seq );

	// Load contents of frame number "frame" into p_buffer. p_buffer has a size
	// appropriate to the format given by the client in get_properties( ... ).
	// "pb_skipped" shall be set by the client to FALSE if the p_buffer has been filled
	// with data, or to TRUE if client decided to no reload the frame if e.g. "frame" is
	// repeated. "pp_user_frame" can hold custom data and is later available
	// in voo_video_frame_metadata_t::p_user_frame.
	vooBOOL (*load)( unsigned int frame, char *p_buffer, vooBOOL *pb_skipped,
				 void **pp_user_frame, void *p_user_seq );

	vooBOOL (*eof)( void *p_user_seq );
	vooBOOL (*good)( void *p_user_seq );
	vooBOOL (*reload)( void *p_user_seq );
	void (*close)( void *p_user_seq );

	// After open( ... ) or open_nowhere( ... ), this is called.
	// Set pp_err to an appropriate, persistent error message or to NULL.
	void (*error_msg)( const char **pp_err, void *p_user_seq );

	// Called by vooya to get supported file extensions. Those are then displayed in
	// the "Open file" dialog. vooya will start with idx=0, then increment idx and
	// call this again as long as you return TRUE. (only called when b_fileBased is true)
	vooBOOL (*file_suffixes)( int idx, const char **pp_suffix, void *p_user_seq );

	// Called by vooya to enumerate meta information tags about the video you provide.
	// idx is counting up for each call as long as TRUE is return. Return FALSE to finish the 
	// enumeration. "buffer_k" char[64] and shall take a key, "buffer_v" char[1024] and
	// shall take a corresponding value.
	vooBOOL (*get_meta)( int idx, char *buffer_k, char *buffer_v, void *p_user_seq );

	// vooya gives you a callback that you might call whenever the sequence's number of frames
	// will change. Note that p_vooya_ctx must not be altered and is valid only as long as this input is bound.
	void (*cb_seq_len_changed)( void (*seq_len_callback)( void *p_vooya_ctx, unsigned int new_len ), void *p_vooya_ctx, void *p_user_seq );

	// called by vooya when "settings" menu entry is clicked. May be NULL.
	// This callback will override the on_settings in voo_plugin_t if available.
	// That difference is important on macOS. Here, you get the per-sequence user data.
	void (*on_settings)( void *p_user_seq );

	char reserved2[3*sizeof(void*)];

} input_plugin_t;




// Most important structure, this describes the plugin
typedef struct
{
	int voo_version; // set this always to VOO_PLUGIN_API_VERSION

	// plugin's main name, user friendly description, copyright notice and version info
	const char *name;
	const char *description;
	const char *copyright;
	const char *version;

	// Flags to signal something to vooya
#define vooPluginFlag_DoNotCache    0x01  // tell vooya not to cache anything
#define vooPluginFlag_NeedsConsole  0x02  // tell vooya to auto-open its console window
#define vooPluginFlag_NoDiffs       0x04  // tell vooya not to open this with diffs
	int flags;

	// any user data that shall be forwarded by vooya into other callback
	// functions ("void *p_user" argument)
	void *p_user;

	// called by vooya before the plugin is unloaded
	void (*on_unload_plugin)( void *p_user );

	// called by vooya when "settings" menu entry is clicked. May be NULL.
	// This callback will not be called if an input of this plugin is active
	// that has a non-null on_settings. That difference is important on macOS.
	void (*on_settings)( void *p_user );

	char reserved1[5*sizeof(void*)];

	// the plugin's callback functions
	vooya_callback_t callbacks[10];

	// plugin's input capabilities. See input_plugin_t above.
	input_plugin_t input;

} voo_plugin_t;

#ifdef __cplusplus
}
#endif




// Main entry function that every plugin must implement to describe itself on startup.
// The "p_plugin"-structure is provided by vooya and to be filled in the implementation.
// This is the first function to be called and must be implemented.
VP_API void voo_describe( voo_plugin_t *p_plugin );


