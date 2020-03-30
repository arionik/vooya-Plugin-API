
#import <Cocoa/Cocoa.h>
#include "../voo_plugin.h"

// (compile with) clang -dynamiclib -ggdb osx_hud.m -o osx_hud.dylib -framework Cocoa


// This class handles our small HUD window
////////////////////////////////////////////////////////////
@interface OSXHUDPlugin : NSObject
@property (assign) NSWindow *vooya;
@property (strong) NSPanel *hud_window;
@property (strong) NSSlider *slider;
@property NSUInteger value;
- (void)setAppInfo:(voo_app_info_t *)p_app_info;
@end

@implementation OSXHUDPlugin
{
@private
	void *p_reload_cargo;
	int (*pf_trigger_reload)(void *);
	BOOL b_active;
}
- (instancetype)init{
	p_reload_cargo = 0x0;
	pf_trigger_reload = 0x0;
	b_active = NO;
	_value = 25;
	return self;
}
- (void)setAppInfo:(voo_app_info_t *)p_app_info{

	// This is the (current) vooya window
	self.vooya = (NSWindow *)p_app_info->p_handle;

	// To trigger reloads, we get vooya's internal data and a callback function
	p_reload_cargo = p_app_info->p_reload_cargo;
	pf_trigger_reload = p_app_info->pf_trigger_reload;

	// Everything UI-related needs to happen on the UI-Thread, hence
	dispatch_async(dispatch_get_main_queue(), ^{

		// Create our HUD-mini window
		NSRect frame = [_vooya frame];
		frame = NSMakeRect( frame.origin.x - 20, frame.origin.y - 20, 280, 120 );
		_hud_window = [[NSPanel alloc] initWithContentRect:frame
							 styleMask:NSWindowStyleMaskHUDWindow | NSWindowStyleMaskUtilityWindow | NSWindowStyleMaskTitled | NSWindowStyleMaskResizable
							   backing:NSBackingStoreBuffered
							     defer:YES];

		// route user input to vooya ...
		[_hud_window makeFirstResponder:self.vooya];
		[_hud_window setTitle:@"OSX Test Window"];
		// ... which is our parent
		if( b_active )
			[_vooya addChildWindow:_hud_window ordered:NSWindowAbove];


		// create a slider control
		_slider = [[NSSlider alloc] initWithFrame:NSMakeRect(20.0, 20.0, 200.0, 48.0)];
		[_slider setTarget:self];
		[_slider setAction:@selector(sliderAction:)];
		_slider.minValue = 1.0;
		_slider.maxValue = 100.0;
		_slider.continuous = NO;
		_slider.altIncrementValue = 1.;
		[_slider setIntValue:self.value];
		[_hud_window.contentView addSubview:_slider];

		// make it fit into the (resizeable) window
		[_slider setTranslatesAutoresizingMaskIntoConstraints:NO];
		[NSLayoutConstraint activateConstraints:[NSLayoutConstraint
							constraintsWithVisualFormat:@"H:|-[_slider]-|"
									    options:0
									    metrics:nil
									      views:NSDictionaryOfVariableBindings(_slider)]];
		[NSLayoutConstraint activateConstraints:[NSLayoutConstraint
							 constraintsWithVisualFormat:@"V:|-[_slider]-|"
									     options:0
									     metrics:nil
									       views:NSDictionaryOfVariableBindings(_slider)]];
	});
}

// when the slider is moved, this is called
- (void)sliderAction:(id)sender{

	// slider value will be our noise amount
	self.value = [sender integerValue];

	if( self->pf_trigger_reload ){
		// Try to trigger reload. vooya does not allow calling it too often, so if it fails, try again later.
		if( !self->pf_trigger_reload( self->p_reload_cargo ) ){
			dispatch_after(dispatch_time(DISPATCH_TIME_NOW, .75 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
				self->pf_trigger_reload( self->p_reload_cargo );
			});
		}
	}
}

// The window will be activated along with the native callback
- (void)activate:(BOOL)activate{
	if( activate ){
		[self.vooya addChildWindow:self.hud_window ordered:NSWindowAbove];
	} else {
		dispatch_after(DISPATCH_TIME_NOW, dispatch_get_main_queue(), ^{
			[self.hud_window close];
			[self.hud_window release];
		});
	}
	b_active = activate;
}

// we are non-ARC
- (void)dealloc{
	if( b_active ){
		[self activate:NO];
	}
	[super dealloc];
}
@end




// C-Functions called by vooya
////////////////////////////////////////////////////////////

// The native callback for adding noise
void add_noise( unsigned char *p_data, voo_video_frame_metadata_t *p_metadata ){

	int n, scale = 1;
	voo_sequence_t *p_seq_info = p_metadata->p_info;
	OSXHUDPlugin *osx_plugin = (__bridge OSXHUDPlugin *)p_metadata->p_user_video;

	if( p_metadata->flags & VOOPerFrameFlag_YouAlreadyProcessed )
		return;

	if( p_metadata->flags & VOOPerFrameFlag_IsDifference ){
		scale = 10;
		return;
	}

	// The value from our slider
	int value = [osx_plugin value];
	for( n=0; n<p_seq_info->frame_size; n++ ){
		int val = p_data[ n ] + scale*rand()%value - value/2;
		p_data[ n ] = val > 255 ? 255 : val < 0 ? 0 : val;
	}
}

// Called when a user selects "Native Output" in the plugin menu
VP_API void on_select( voo_sequence_t *p_info, voo_app_info_t *p_app_info, void *p_user, void **pp_user_video ){

	NSLog(@"on_select_native for %s", p_info->filename);
	OSXHUDPlugin *osx_plugin = [[OSXHUDPlugin alloc] init];
	[osx_plugin setAppInfo:p_app_info];
	*pp_user_video = osx_plugin;
	// show our window
	[osx_plugin activate:YES];
}

// Called when a user deselects "Native Output" in the plugin menu
VP_API void on_deselect( void *p_user, void *p_user_video ){

	NSLog(@"on_deselect_native");
	// when the video window is closed, it is safe to release our HUD
	OSXHUDPlugin *osx_plugin = (__bridge OSXHUDPlugin *)p_user_video;

	[osx_plugin activate:NO];
	[osx_plugin release];
}

VP_API void on_unload_plugin( void *p_user ){
	// do something here when vooya signals you're about to be killed
}







VP_API void voo_describe( voo_plugin_t *p_plugin )
{
	p_plugin->voo_version = VOO_PLUGIN_API_VERSION;

	// plugin main properties
	p_plugin->name = "OSX Test Plugin";
	p_plugin->description = "This is a plugin for vooya testing OSX/Cocoa capabilities.";
	p_plugin->copyright = "\n\xA9 2017 Arion Neddens";
	p_plugin->version = "\nver0.0.1 (beta)";

	// p_user could point to static data we need everywhere.
	p_plugin->p_user = NULL;
	p_plugin->on_unload_plugin = on_unload_plugin;
	
	p_plugin->callbacks[0].uid = "voo.osxtest.native_out.1";
	p_plugin->callbacks[0].name = "Native Noise Adder";
	p_plugin->callbacks[0].description = "Adds some noise and demonstrates adding a Cocoa HUD panel to vooya.";
	p_plugin->callbacks[0].cb_type = vooCallback_Native;
	p_plugin->callbacks[0].method_native = add_noise;
	p_plugin->callbacks[0].on_select = on_select;
	p_plugin->callbacks[0].on_deselect = on_deselect;
}

