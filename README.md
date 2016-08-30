# vooya Plugin API

vooya is a raw video sequencer player for Windows, macOS and Linux and has, beginning with version 1.6, an open plugin API. The plugin mechanism is based on standard shared libraries loaded at runtime, so plugins can be written in any language suitable for creating shared libraries, notably C/C++ and Objective-C.

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
All is explained in [voo_plugin.h](voo_plugin.h) and some examples are given in the examples-folder. Run the make_* scripts to build them all for your platform and be sure to let vooya's plugin directory point to our examples.

<p align="center">
	<img alt="" src="http://www.offminor.de/assets/i/6338ed0318370adfd89b7007b9004e41_290x290x0x0x290x170@2x.png" width="290">
</p>
