package main

import (
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"runtime"
	"strconv"
	"strings"

	"github.com/magefile/mage/mg"
	"github.com/magefile/mage/sh"
)

var (
	TOOLPREFIX = detectToolPrefix()
	CC         = TOOLPREFIX + "gcc"
	LD         = TOOLPREFIX + "ld"

	CFLAGS  = initCflags()
	LDFLAGS = initLdflags()
)

var (
	GOTAGS    = "nes phy"
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

func Kernel() error {
	mg.Deps(pranaOS)

	detectGoVersion()
	return rundir("app", nil, pranaosBin, "build", "-o", "../kernel.elf",
		"-gcflags", GOGCFLAGS,
		"-tags", GOTAGS,
		"./kmain")
}

func Boot64() error {
	compileCfile("boot/boot64.S", "-m64")
	compileCfile("boot/boot64main.c", "-m64")
	ldflags := "-Ttext 0x3200000 -m elf_x86_64 -o boot64.elf boot64.o boot64main.o"
	ldArgs := append([]string{}, LDFLAGS...)
	ldArgs = append(ldArgs, strings.Fields(ldflags)...)
	return sh.RunV(LD, ldArgs...)
}

func Multiboot() error {
	mg.Deps(Boot64)
	compileCfile("boot/multiboot.c", "-m32")
	compileCfile("boot/multiboot_header.S", "-m32")
	ldflags := "-Ttext 0x3300000 -m elf_i386 -o multiboot.elf multiboot.o multiboot_header.o -b binary boot64.elf"
	ldArgs := append([]string{}, LDFLAGS...)
	ldArgs = append(ldArgs, strings.Fields(ldflags)...)
	err := sh.RunV(LD, ldArgs...)
	if err != nil {
		return err
	}
	return sh.Copy(
		filepath.Join("cmd", "pranaos", "assets", "boot", "multiboot.elf"),
		"multiboot.elf",
	)
}

func Test() error {
	mg.Deps(pranaOS)

	envs := map[string]string{
		"QEMU_OPTS": quoteArgs(QEMU_OPT),
	}
	return rundir("tests", envs, pranaosBin, "test")
}

func TestDebug() error {
	mg.Deps(pranaOS)

	envs := map[string]string{
		"QEMU_OPTS": quoteArgs(QEMU_DEBUG_OPT),
	}
	return rundir("tests", envs, pranaosBin, "test")
}

func Qemu() error {
	mg.Deps(Kernel)

	detectQemu()
	return pranaosrun(QEMU_OPT, "kernel.elf")
}

func QemuDebug() error {
	GOGCFLAGS += " -N -l"
	mg.Deps(Kernel)

	detectQemu()
	return pranaosrun(QEMU_DEBUG_OPT, "kernel.elf")
}

func Iso() error {
	mg.Deps(Kernel)
	return sh.RunV(pranaosBin, "pack", "-o", "pranaos.iso", "-k", "kernel.elf")
}

func Graphic() error {
	detectQemu()

	mg.Deps(Iso)
	return pranaosrun(QEMU_OPT, "pranaos.iso")
}

func GraphicDebug() error {
	detectQemu()

	GOGCFLAGS += " -N -l"
	mg.Deps(Iso)
	return pranaosrun(QEMU_DEBUG_OPT, "pranaos.iso")
}

func pranaOS() error {
	err := rundir("cmd", nil, "go", "build", "-o", "../pranaos", "./pranaos")
	if err != nil {
		return err
	}
	current, _ := os.Getwd()
	pranaosBin = filepath.Join(current, "pranaos")
	return nil
}

func Clean() {
	rmGlob("*.o")
	rmGlob("kernel.elf")
	rmGlob("multiboot.elf")
	rmGlob("qemu.log")
	rmGlob("qemu.pcap")
	rmGlob("pranaos.iso")
	rmGlob("pranaos")
	rmGlob("boot64.elf")
	rmGlob("bochs.log")
}

func detectToolPrefix() string {
	prefix := os.Getenv("TOOLPREFIX")
	if prefix != "" {
		return prefix
	}

	if hasOutput("elf32-i386", "x86_64-elf-objdump", "-i") {
		return "x86_64-elf-"
	}

	if hasOutput("elf32-i386", "i386-elf-objdump", "-i") {
		return "i386-elf-"
	}

	if hasOutput("elf32-i386", "objdump", "-i") {
		return ""
	}
	panic(`
	** Error: Couldn't find an i386-*-elf or x86_64-*-elf version of GCC/binutils **
	`)
}

var goVersionRegexp = regexp.MustCompile(`go(\d+)\.(\d+)\.?(\d?)`)

func goVersion() (string, int, int, error) {
	versionBytes, err := cmdOutput(gobin(), "version")
	if err != nil {
		return "", 0, 0, err
	}
	version := strings.TrimSpace(string(versionBytes))
	result := goVersionRegexp.FindStringSubmatch(version)
	if len(result) < 3 {
		return "", 0, 0, fmt.Errorf("use of unreleased go version `%s`, may not work", version)
	}
	major, _ := strconv.Atoi(result[1])
	minor, _ := strconv.Atoi(result[2])
	return version, major, minor, nil
}

func detectGoVersion() {
	version, major, minor, err := goVersion()
	if err != nil {
		fmt.Printf("warning: %s\n", err)
		return
	}
	if !(major == goMajorVersionSupported && minor <= maxGoMinorVersionSupported) {
		fmt.Printf("warning: max supported go version go%d.%d.x, found go version `%s`, may not work\n",
			goMajorVersionSupported, maxGoMinorVersionSupported, version,
		)
		return
	}
}

func gobin() string {
	goroot := os.Getenv("PRANAOS_GOROOT")
	if goroot != "" {
		return filepath.Join(goroot, "bin", "go")
	}
	return "go"
}

func detectQemu() {
	if !hasCommand(QEMU64) {
		panic(QEMU64 + ` command not found`)
	}
}

func accelArg() []string {
	switch runtime.GOOS {
	case "darwin":
		return []string{"-M", "accel=hvf"}
	default:
		return nil
	}
}

func initCflags() []string {
	cflags := strings.Fields("-fno-pic -static -fno-builtin -fno-strict-aliasing -O2 -Wall -Werror -fno-omit-frame-pointer -I. -nostdinc")
	if hasOutput("-fno-stack-protector", CC, "--help") {
		cflags = append(cflags, "-fno-stack-protector")
	}
	if hasOutput("[^f]no-pie", CC, "-dumpspecs") {
		cflags = append(cflags, "-fno-pie", "-no-pie")
	}
	if hasOutput("[^f]nopie", CC, "-dumpspecs") {
		cflags = append(cflags, "-fno-pie", "-nopie")
	}
	return cflags
}

func initLdflags() []string {
	ldflags := strings.Fields("-N -e _start")
	return ldflags
}

func initQemuOpt() []string {
	var opts []string
	if os.Getenv("QEMU_ACCEL") != "" {
		opts = append(opts, accelArg()...)
	}
	if os.Getenv("QEMU_GRAPHIC") == "" {
		opts = append(opts, "-nographic")
	}
	return opts
}

func initQemuDebugOpt() []string {
	opts := `
	-d int -D qemu.log
	-object filter-dump,id=f1,netdev=eth0,file=qemu.pcap
	-s -S
	`
	ret := append([]string{}, initQemuOpt()...)
	ret = append(ret, strings.Fields(opts)...)
	return ret
}

func compileCfile(file string, extFlags ...string) {
	args := append([]string{}, CFLAGS...)
	args = append(args, extFlags...)
	args = append(args, "-c", file)
	err := sh.RunV(CC, args...)
	if err != nil {
		panic(err)
	}
}

func rundir(dir string, envs map[string]string, cmd string, args ...string) error {
	current, _ := os.Getwd()
	os.Chdir(dir)
	defer os.Chdir(current)
	return sh.RunWithV(envs, cmd, args...)
}

func pranaosrun(qemuArgs []string, flags ...string) error {
	qemuOpts := quoteArgs(qemuArgs)
	var args []string
	args = append(args, "run")
	args = append(args, "-p", "8080:80")
	args = append(args, flags...)
	envs := map[string]string{
		"QEMU_OPTS": qemuOpts,
	}
	return sh.RunWithV(envs, pranaosBin, args...)
}

func cmdOutput(cmd string, args ...string) ([]byte, error) {
	return exec.Command(cmd, args...).CombinedOutput()
}

func quoteArgs(args []string) string {
	var ret []string
	for _, s := range args {
		if strings.Index(s, " ") != -1 {
			ret = append(ret, strconv.Quote(s))
		} else {
			ret = append(ret, s)
		}
	}
	return strings.Join(ret, " ")
}

func hasCommand(cmd string) bool {
	_, err := exec.LookPath(cmd)
	if err != nil {
		return false
	}
	return true
}

func hasOutput(regstr, cmd string, args ...string) bool {
	out, err := cmdOutput(cmd, args...)
	if err != nil {
		return false
	}
	match, err := regexp.Match(regstr, []byte(out))
	if err != nil {
		return false
	}
	return match
}

func rmGlob(patten string) error {
	match, err := filepath.Glob(patten)
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
