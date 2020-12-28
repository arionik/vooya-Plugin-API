
clang -dynamiclib -std=gnu99 examples/gamma9.c -g -o examples/gamma9.dylib
clang -dynamiclib -std=gnu99 examples/zoneplate.c -g -o examples/zoneplate.dylib
clang -dynamiclib -ggdb examples/osx_hud.m -o examples/osx_hud.dylib -framework Cocoa
clang -dynamiclib -ggdb examples/diff.c -o examples/diff.dylib -framework Cocoa

if [ -x "$(command -v rustc)" ]; then
	rustc --crate-type dylib examples/rustplugin.rs -g -o examples/rust.dylib
fi

