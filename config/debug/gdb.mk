gdb: strip-symbols
	@printf "`cat config/debug/gdb.commands)`" "$(DEBUG_SYMBOL_FILE)" "$(DEBUG_BREAKPOINT)" > /tmp/tmp_gdb.commands
	@gdb -x /tmp/tmp_gdb.commands