package main

import (
	"os"
	"regexp"
)

var (
	TOOLPREFIX = detectToolPrefix()
	CC         = TOOLPREFIX + "gcc"
	LD         = TOOLPREFIX + "ld"

	CFLAGS  = initCflags()
	LDFLAGS = initLdflags()
)

var (
	GOTAGS    = "nes phy prometheus"
	GOGCFLAGS = ""
)

var (
	QEMU64 = "qemu-system-x86_64"
	QEMU32 = "qemu-system-i386"

	QEMU_OPT       = initQemuOpt()
	QEMU_DEBUG_OPT = initQemuDebugOpt()
)

var (
	pranaosBin string
)

const (
	goMajorVersionSupported    = 1
	maxGoMinorVersionSupported = 16
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

func hasOutput(regstr, cmd string, args ...string) bool {
	out, err := cmdOutput(cmd, args...)
	if err != nil{
		return false
	}
	
	match, err := regexp.Match(regstr, []byte(out))
	if err != nil {
		return false
	}

	return match
}


func rmGlob(pattern string) error {
	match, err := filepath.Glob(pattern)
	if err != nil {
		return err
	}

	for _, file := range match {
		err = os.Remove(file)
		if err != nil {
			return err
		}
	}

	return nil
}