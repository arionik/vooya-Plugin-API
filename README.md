# vooya Plugin API

vooya is a raw video sequencer player for Windows, macOS and Linux and has, beginning with version 1.6, an open plugin API. The plugin mechanism is based on standard shared libraries loaded at runtime, so plugins can be written in any language suitable for creating shared libraries, notably C/C++, Rust or Objective-C.

Below you can see vooya's processing chain and where plugin callbacks can kick in (the items marked yellow). These are:
+ **Input**
Provide custom input to vooya, from any source
+ **Histogram**
Get access to the calculated histogram data
+ **Native I/O**
Callback with the raw video data for each frame
+ **RGB 32bit I/O**
Callback with the rendered data in RGB right before display
+ **EOTF**
Add custom transfer functions to vooya's existing ones
<p align="center">
	<img src="/../master/plugin_api.jpg?raw=true" width="600">
</p>

## How to begin
All is explained in [voo_plugin.h](voo_plugin.h) and some examples are given in the examples-folder. Be sure to let vooya's plugin directory point to our examples.

The whole functionality of your plugin is described by you in `voo_describe()`, where the main describing structure is to be filled:
```C
VP_API void voo_describe( voo_plugin_t *p_plugin )
{
	/* mandatory line */
	p_plugin->voo_version = VOO_PLUGIN_API_VERSION;
	p_plugin-> ...
}
```
Everything is based on callbacks you provide as function pointers. For example, you could write a function named `invert` which inverts a 32bit RGB pixel, and hand that function over to vooya like so:  
```C
	p_plugin->callbacks[0].uid = "voo.test.rgb_invert";
	p_plugin->callbacks[0].name = "RGB Inverter";
	p_plugin->callbacks[0].description = "Inverts each channel";
	p_plugin->callbacks[0].type = vooCallback_RGBOut;
	p_plugin->callbacks[0].method_rgb_out = invert;
```
Turn on _Plugin debugging_ in vooya's preferences to get some console output. **Note the plugin API is still experimental.**

###Building the examples
- on **Windows**, open a Visual Studio Command Prompt (64bit or 32bit, according to you platform), cd to the root directory and type `make_win.bat`
- on **Linux**, open a terminal, cd to the root directory and run `./make_unix.sh`
- on **macOS**, make sure you have **clang** installed (XCode command line tools) open a terminal, cd to the root directory and run `./make_osx.sh`

<p align="right">
	<img alt="" src="http://www.offminor.de/assets/i/6338ed0318370adfd89b7007b9004e41_290x290x0x0x290x170@2x.png" width="290">
</p>
