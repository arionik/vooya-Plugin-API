
// compile with
//  rustc --crate-type dylib rustplugin.rs
// on windows:
//  rustc --crate-type cdylib -C opt-level=3 -C link-args=-s  -C prefer-dynamic rustplugin.rs

use std::os::raw::{c_void,c_char,c_uchar,c_int,c_uint,c_double};


const VOO_PLUGIN_API_VERSION: i32 = 3;


// display pixel data type
#[allow(dead_code)]
#[allow(non_camel_case_types)]
#[cfg(target_os = "macos")]
#[repr(C)]
pub struct voo_target_space_t
{
	x: c_uchar,
	r: c_uchar,
	g: c_uchar,
	b: c_uchar,
}
#[allow(dead_code)]
#[allow(non_camel_case_types)]
#[cfg(not(target_os = "macos"))]
#[repr(C)]
pub struct voo_target_space_t
{
	b: c_uchar,
	g: c_uchar,
	r: c_uchar,
	x: c_uchar,
}


#[allow(dead_code)]
#[allow(non_camel_case_types)]
pub enum voo_colorSpace_t {
	vooColorSpace_Unknown = -1,
	vooCS_YUV,
	vooCS_XYZ,
	vooCS_YIQ,
	vooCS_RGB,
	vooCS_Gray,
	vooCS_HSV,
	vooCS_YCgCo,
	vooCS_ICtCp
}

#[allow(dead_code)]
#[allow(non_camel_case_types)]
#[allow(non_snake_case)]
pub enum voo_dataArrangement_t {
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
	vooNumDataArrangements
}


#[allow(dead_code)]
#[allow(non_camel_case_types)]
#[allow(non_snake_case)]
pub enum voo_channelOrder_t
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

	vooNumChannelOrders
}



#[allow(dead_code)]
#[allow(non_camel_case_types)]
#[allow(non_snake_case)]
#[repr(C)]
pub struct voo_sequence_t {

	pub filename: *const c_char,

	// video resolution
	pub width: c_int,
	pub height: c_int,

	// frames per seconds
	pub fps: c_double,

	// Color space, such as YUV, RGB etc.
	pub colorSpace: voo_colorSpace_t,

	// How the channels are packed or interleaved
	arrangement: voo_dataArrangement_t,

	// The order in which color channels are written
	channel_order: voo_channelOrder_t,

	// size in bytes of a single video frame in native format
	framesize: c_uint,

	// Bits per channel is normally 8 or 10-16 (valid bit depths are 1-16) (if integer)
	bitsPerChannel: c_int,

	// Whether the video shall be played upside down
	b_flipped: c_int,
	// Whether 16bit words shall be byte-swapped
	b_toggle_endian: c_int,
	// Whether the values (if integer) shall be treated as signed integers
	b_signed: c_int,


	reserved: [c_char; 32],

}


// structure vooya gives you in on_load_video( ... ).
#[allow(dead_code)]
#[allow(non_camel_case_types)]
#[allow(non_snake_case)]
#[repr(C)]
pub struct voo_app_info_t {

	// a handle to vooya's current window (what it is, is platform dependent)
	p_handle: *const c_void,

	// to trigger vooya to a reload a frame, use these like:
	//     p_app_info.pf_trigger_reload( app_info.p_reload_cargo )
	// note that this should happen not too often.
	p_reload_cargo: *const c_void,
	pf_trigger_reload: extern fn(p_reload_cargo: *const c_void) -> c_int,

	// send a message to the console window in vooya
	p_message_cargo: *const c_void,
	pf_console_message: extern fn(p_message_cargo: *const c_void, message: *const c_char ) -> c_void,

	reserved: [c_char; 32],
}




// Structure you get in per-frame callback functions.
#[allow(dead_code)]
#[allow(non_camel_case_types)]
#[allow(non_snake_case)]
#[repr(C)]
pub struct voo_video_frame_metadata_t {

	// user data you might have provided in voo_describe( ... ) as voo_plugin_t::p_user
	p_user: *const c_void,

	// per-sequence user data you might have provided in voo_plugin_t::on_load_video( ... )
	p_user_video: *const c_void,

	// per-frame user data you might have provided in input_plugin_t::load( ... )
	p_user_frame: *const c_void,

	p_info: *const voo_sequence_t,  // info about the current sequence

	// frame number, beginning at zero
	frame_idx: c_uint,

	flags: c_int,

	reserved: [c_char; 32],
}

#[allow(dead_code)]
#[allow(non_upper_case_globals)]
const VOOPerFrameFlag_YouAlreadyProcessed: i32 = 0x01; // this frame has already been processed by you
#[allow(dead_code)]
#[allow(non_upper_case_globals)]
const VOOPerFrameFlag_IsFromCache:         i32 = 0x02; // this one comes from RGB-display cache
#[allow(dead_code)]
#[allow(non_upper_case_globals)]
const VOOPerFrameFlag_IsDifference:        i32 = 0x04; // this frame is a difference frame

// PLUGIN CALLBACK FUNCTION STRUCT
//
// This struct shall contain user-defined callback functions along with some metadata.
// First the callback types:
#[allow(dead_code)]
#[allow(non_camel_case_types)]
enum vooya_callback_type_t {
	vooCallback_Native,
	vooCallback_RGBOut,
	vooCallback_EOTF,
	vooCallback_Histogram
}


#[allow(dead_code)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub struct vooya_callback_t
{
	// The following strings must be set and be persistent throughout plugin's linkage.
	// uid must not be empty or NULL.
	uid: *const c_char,         // a unique string, e.g. "myplugin.rgb_invert.1",
	                            // at most 63 chars in length, ANSI without any whitespace
	name: *const c_char,        // a user-friendly, descriptive name
	description: *const c_char, // a more in-depth description

	// Functions vooya will call upon user's (de)selection of this callback (optional)
	on_select: unsafe extern fn( p_info: *const voo_sequence_t, p_app_info: *const voo_app_info_t, p_user: *const c_void, pp_user_video: *const *mut c_void ) -> (),
	on_deselect: unsafe extern fn( p_user: *const c_void, p_user_video: *const c_void ) -> (),

	// Flags to signal something to vooya (for future use)
	flags: i32,

	// type determines which callback signature will be called
	e_type: vooya_callback_type_t,

	// actual callback function (required, see below)
	method: *const c_void,

	// For type == vooCallback_RGBOut:
	// Called by vooya for each video frame with rgb data ready to be rendered,
	// i.e. color-converted, range-shifted to 8bit and with EOTF and image
	// adjustments applied. Can be used to feed the data outside of vooya as
	// well as to alter the data right before display.
	// Stride in bytes is equal to width*sizeof(voo_target_space_t).
	//   method shall be:
	// unsafe extern fn( p_data: *mut voo_target_space_t, p_metadata: *const voo_video_frame_metadata_t ) -> (),

	// For type == vooCallback_Native:
	// Called by vooya for each video frame with native data before color
	// conversion to RGB 8bit, and without image adjustments. Can be used to
	// feed the data outside of vooya. Properties like resolution
	// and data format are given beforehand in on_load_video( ... ); you can
	// save them in p_metadata->p_user_video. "p_data" is the image data.
	//   method shall be
	// unsafe extern fn( p_data: *mut c_uchar, p_metadata: *const voo_video_frame_metadata_t ) -> (),

	// For type == vooCallback_EOTF:
	// Called by vooya when a lookup-table for the transfer function is being made.
	// "value" is in the range of 0-1, representing an RGB channel value of input bit
	// depth ("bits"). "p_user" might be provided by you from within voo_describe(...)
	// and can be NULL or any custom data. The call of this function happens before
	// application of brightness, contrast, gamma and exposure user settings.
	//   method shall be:
	// unsafe extern fn( value: c_double, bits: c_int, p_user: *const c_void ) -> c_double,

	// For type == vooCallback_Histogram:
	// Called by vooya for each frame if histogram calculation (and display) is enabled.
	// The three pointers contain the histograms for each channel respectively. Their
	// length is (1<<bit_depth)-1 (floating point data is put into 12bits).
	//   method shall be:
	// unsafe extern fn( p_h1: *const c_uint, p_h2: *const c_uint, p_h3: *const c_uint,
	//                               p_metadata: *const voo_video_frame_metadata_t ) -> (),
}







// INPUT DESCRIPTION STRUCT
//
// Container to provide custom input to vooya from file or from "nowhere".
#[allow(dead_code)]
#[allow(non_camel_case_types)]
#[allow(non_snake_case)]
#[repr(C)]
struct input_plugin_t {

	uid: *const c_char,         // a unique string, e.g. "myplugin.text.input",
	                            // at most 63 chars in length, ANSI without any whitespace
	name: *const c_char,        // a user-friendly, descriptive name (mandatory)
	description: *const c_char, // a more in-depth description

	// If b_fileBased is TRUE, vooya will ask for file suffixes supported by this input,
	// call file_suffixes( ... ), responsible( ... ) and open( ... ), and will include
	// this input in the file open dialog. If b_fileBased is FALSE, an entry for this input
	// will be displayed in the plugins-menu that the user can select as current input.
	// In that case, vooya will call open_nowhere( ... ).
	b_fileBased: i32,

	// Flags to signal something to vooya (for future use)
	flags: i32,

	reserved1: [c_char; 8],

	// If the input is file-based, responsible will be called with the file name and the
	// first sixteen bytes of data, which e.g. might contain magic data. p_user is
	// voo_plugin_t::p_user. If responsible returns TRUE, open will be called.
	// Only if input comes from stdin and "--container [your input UID]" is specified,
	// responsible will not be called, but open( ... ) directly.
	// For stdin, the filename is simply "-".
	//       FIXME: filename not a c_char in Windows
	responsible: unsafe extern fn( filename: *const c_char, sixteen_bytes: *const c_char, p_user: *const c_void ) -> c_int,
	// The global p_user pointer you may have set in voo_describe( ... )
	// is given here as *pp_user_seq, but you can alter it. In that case, subsequent
	// calls to methods of this struct will have the new, per-sequence value. This is
	// important on macOS, where multiple instances of this input may exist.
	open: unsafe extern fn( filename: *const c_char, p_app_info: *const voo_app_info_t, pp_user_seq: *const *mut c_void ) -> c_int,

	// If the input is not based on file input (b_fileBased is FALSE),
	// open_nowhere will be called. The global p_user pointer you may have set in
	// voo_describe( ... ) is given here as *pp_user_seq, but you can alter it.
	// In that case, subsequent calls to methods of this struct will have the new,
	// per-sequence value. This is important on macOS, where multiple instances
	// of this input may exist.
	open_nowhere: unsafe extern fn( p_app_info: *const voo_app_info_t, pp_user_seq: *const *mut c_void ) -> c_int,

	// Called by vooya to get information about the video you provide.
	// You should fill p_info with correct information to make vooya play.
	get_properties: unsafe extern fn( p_info: *const voo_sequence_t, p_user_seq: *const c_void ) -> c_int,

	// Client shall return the number of frames available, or ~0U if no
	// framecount can be given (e.g. stdin).
	framecount: unsafe extern fn( p_user_seq: *const c_void ) -> c_uint,

	// Shall issue a seek by the client plugin to frame number "frame"
	seek: unsafe extern fn( frame: c_uint, p_user_seq: *const c_void ) -> c_int,

	// Load contents of frame number "frame" into p_buffer. p_buffer has a size
	// appropriate to the format given by the client in get_properties( ... ).
	// "pb_skipped" shall be set by the client to FALSE if the p_buffer has been filled
	// with data, or to TRUE if client decided to no reload the frame if e.g. "frame" is
	// repeated. "pp_user_frame" can hold custom data and is later available
	// in voo_video_frame_metadata_t::p_user_frame.
	load: unsafe extern fn( frame: c_uint, p_buffer: *const c_char, pb_skipped: *const c_int, pp_user_frame: *const *mut c_void, p_user_seq: *const c_void ) -> c_int,

	eof: unsafe extern fn( p_user_seq: *const c_void ) -> c_uint,
	good: unsafe extern fn( p_user_seq: *const c_void ) -> c_uint,
	reload: unsafe extern fn( p_user_seq: *const c_void ) -> c_uint,
	close: unsafe extern fn( p_user_seq: *const c_void ) -> (),

	// After open( ... ) or open_nowhere( ... ), this is called.
	// Set pp_err to an appropriate, persistent error message or to NULL.
	error_msg: unsafe extern fn( pp_err: *const *mut c_char, p_user_seq: *const c_void ) -> (),

	// Called by vooya to get supported file extensions. Those are then displayed in
	// the "Open file" dialog. vooya will start with idx=0, then increment idx and
	// call this again as long as you return TRUE. (only called when b_fileBased is true)
	file_suffixes: unsafe extern fn( idx: c_int, pp_suffix: *const *mut c_char, p_user_seq: *const c_void ) -> c_int,

	// Called by vooya to enumerate meta information tags about the video you provide.
	// idx is counting up for each call as long as TRUE is return. Return FALSE to finish the
	// enumeration. "buffer_k" char[64] and shall take a key, "buffer_v" char[1024] and
	// shall take a corresponding value.
	get_meta: unsafe extern fn( idx: c_int, buffer_k: *const c_char, buffer_v: *const c_char, p_user_seq: *const c_void ) -> c_int,

	// vooya gives you a callback that you might call whenever the sequence's number of frames
	// will change. Note that p_vooya_ctx must not be altered and is valid only as long as this input is bound.
//	void (*cb_seq_len_changed)( void (*seq_len_callback)( void *p_vooya_ctx, unsigned int new_len ), void *p_vooya_ctx );
	cb_seq_len_changed: unsafe extern fn( seq_len_callback: unsafe extern fn( p_vooya_ctx: *const c_void, new_len: c_uint ) -> (), p_vooya_ctx: *const c_void ) -> (),

	reserved2: [c_char; 32],
}



// Most important structure, this describes the plugin
#[allow(dead_code)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub struct voo_plugin_t
{
	voo_version: c_int, // set this always to VOO_PLUGIN_API_VERSION

	// plugin's main name, user friendly description, copyright notice and version info
	name: *const c_char,
	description: *const c_char,
	copyright: *const c_char,
	version: *const c_char,

	// Flags to signal something to vooya (for future use)
	flags: c_int,

	// any user data that shall be forwarded by vooya into other callback
	// functions ("void *p_user" argument)
	p_user: *const c_void,

	// called by vooya before the plugin is unloaded
	on_unload_plugin: extern fn( p_user: *const c_void ) -> (),

	reserved: [c_char; 48],

	// the plugin's callback functions
	callbacks: [vooya_callback_t; 10],

	// plugin's input capabilities. See input_plugin_t above.
	input: input_plugin_t
}





/*

	-------  actual plugin below -------

*/








const NAME: &'static [u8] = b"vooya Plugin Written in Rust\0";
const DESCR: &'static [u8] = b"Adds funny RGB callback to show Rust binding, hehe.\0";
const COPYRIGHT: &'static [u8] = b"(C) Arion Neddens 2016.\0";
const VERSION: &'static [u8] = b"ver 1.0\0";

const CB_UID: &'static [u8] = b"rust.callback.0\0";
const CB_NAME: &'static [u8] = b"Convert to gray (Rust)\0";
const CB_DESCR: &'static [u8] = b"Fun Function to show Rust bindings.\0";


// Main entry function that every plugin must implement to describe itself on startup.
// The "p_plugin"-structure is provided by vooya and to be filled in the implementation.
// This is the first function to be called and must be implemented.
#[no_mangle]
pub unsafe extern fn voo_describe( p_plugin: *mut voo_plugin_t )
{
	let ref mut p = *p_plugin;

	p.voo_version = VOO_PLUGIN_API_VERSION;
	p.name = NAME.as_ptr() as *const c_char;
	p.description = DESCR.as_ptr() as *const c_char;
	p.copyright = COPYRIGHT.as_ptr() as *const c_char;
	p.version = VERSION.as_ptr() as *const c_char;

	p.callbacks[0].uid = CB_UID.as_ptr() as *const c_char;
	p.callbacks[0].name = CB_NAME.as_ptr() as *const c_char;
	p.callbacks[0].description = CB_DESCR.as_ptr() as *const c_char;
	p.callbacks[0].e_type = vooya_callback_type_t::vooCallback_RGBOut;
	p.callbacks[0].method = twizzle as *const c_void;

}



// our function which does "something" with an rgb buffer.
#[no_mangle]
pub unsafe extern fn twizzle( p_data: *mut voo_target_space_t, p_metadata: *const voo_video_frame_metadata_t )
{
	let ref p_meta = *p_metadata;
	let ref p_seq_info = *(p_meta.p_info);

	if 0 != (p_meta.flags & VOOPerFrameFlag_IsFromCache) {
		return;
	}

	for y in 0..p_seq_info.height {

		for x in 0..p_seq_info.width {

			let ref mut p: voo_target_space_t = *p_data.offset( (x + p_seq_info.width * y) as isize );
			let luma : i32 = (130 * p.r as i32 + 256 * p.g as i32 + 50 * p.b as i32) >> 8;
			p.r = std::cmp::min( 255, luma ) as u8;
			p.g = std::cmp::min( 255, luma ) as u8;
			p.b = std::cmp::min( 255, luma ) as u8;
		}
	}
}
