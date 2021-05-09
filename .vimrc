let termdebugger="mipsel-unknown-elf-gdb"

function DebugCubeFunc()
	Termdebug
	source "samples/cube/cube.gdb"
endfunction

command DebugCube call DebugCubeFunc()
