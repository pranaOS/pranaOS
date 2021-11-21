package main

import "os"

var (
	TOOLPREFIX = detectToolPrefix()
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
