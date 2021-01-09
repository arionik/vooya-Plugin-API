set -x
gcc -Wall -shared -fpic examples/gamma9.c -o examples/gamma9.so
gcc -Wall -shared -fpic examples/zoneplate.c -o examples/zoneplate.so

if [ -x "$(command -v rustc)" ]; then
	rustc --crate-type cdylib examples/rustplugin.rs -g -o examples/rust.so
fi

