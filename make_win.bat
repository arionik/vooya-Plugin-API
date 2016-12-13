REM run this in a visual studio command prompt
cl.exe /LD examples\gamma9.c /Feexamples\gamma9.dll
cl.exe /LD examples\zoneplate.c /Feexamples\zoneplate.dll

rustc --version >nul 2>&1 && (
	rustc --crate-type cdylib -C opt-level=3 -C link-args=-s  -C prefer-dynamic rustplugin.rs -o examples/rust.dll
) || ()
