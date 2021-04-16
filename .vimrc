let termdebugger="mipsel-unknown-elf-gdb"

function DebugTriangleFunc()
	Termdebug
	source "samples/triangle/triangle.gdb"
endfunction

command DebugTriangle call DebugTriangleFunc()
