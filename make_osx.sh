
clang -dynamiclib -std=gnu99 examples/gamma9.c -g -o examples/gamma9.dylib
clang -dynamiclib -std=gnu99 examples/zoneplate.c -g -o examples/zoneplate.dylib

# rustc --crate-type dylib examples/rustplugin.rs -g -o examples/rust.dylib

