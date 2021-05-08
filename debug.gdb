# vi:syntax=gdb

set remote interrupt-sequence Ctrl-C

# Start emu if not already running
shell pgrep pcsx-redux
if $_shell_exitcode != 0
	shell pcsx-redux &
end

target remote 127.0.0.1:3333
set substitute-path . .
monitor reset shellhalt
load samples/cube/cube.elf
tbreak main
continue
