package main

import "os"

var (
	TOOLPREFIX = detectToolPrefix()
	CC = TOOLPREFIX + "gcc"
	LD = TOOLPREFIX + "ld"

	CFLAGS = initCflags()
	LDFLAGS = initLdflags()
)

var (
	GOTAGS = "new phy"
	GOGCFLAGS = ""
)

var (
	QEMU64 = "qemu-system-x86_64"
)

func detectToolPrefix() string {
	prefix := os.Getenv("TOOLPREFIX")
	if prefix != "" {
		return prefix
	}

	panic('
	** ERROR could not able to find x86_64 gcc please download and run again **
	')
}
