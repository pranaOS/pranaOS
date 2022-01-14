INCLUDEDIRS := kernel/include
QEMUOPTIONS := -boot d -device VGA,edid=on,xres=1024,yres=768 -trace events=../qemuTrace.txt -d cpu_reset #-readconfig qemu-usb-config.cfg -drive if=none,id=stick,file=disk.img -device usb-storage,bus=ehci.0,drive=stick

G++PARAMS := -m32 -g -D PRANAOSKERNEL -I $(INCLUDEDIRS) -Wall -fno-omit-frame-pointer -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-exceptions -fno-rtti -fno-leading-underscore -Wno-write-strings -fpermissive -Wno-unknown-pragmas
GCCPARAMS := -m32 -g -D PRANAOSKERNEL -I $(INCLUDEDIRS) -Wall -fno-omit-frame-pointer -nostdlib -fno-builtin -Wno-unknown-pragmas
ASPARAMS := --32
LDPARAMS := -m elf_i386

KRNLSRCDIR := kernel/src
KRNLOBJDIR := kernel/obj

KRNLFILES := $(shell find $(KRNLSRCDIR) -type f \( -name \*.cpp -o -name \*.s -o -name \*.asm -o -name \*.c \)) #Find all the files that end with .cpp/.s/.asm/.c
KRNLOBJS := $(patsubst %.cpp,%.o,$(patsubst %.s,%.o,$(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(KRNLFILES))))) #Replace the .cpp/.s/.asm/.c extension with .o
KRNLOBJS := $(subst $(KRNLSRCDIR),$(KRNLOBJDIR),$(KRNLOBJS)) #Replace the kernel/src part with kernel/obj

$(KRNLOBJDIR)/%.o: $(KRNLSRCDIR)/%.cpp
	mkdir -p $(@D)
	i686-elf-g++ $(G++PARAMS) -c -o $@ $<

$(KRNLOBJDIR)/%.o: $(KRNLSRCDIR)/%.c
	mkdir -p $(@D)
	i686-elf-gcc $(GCCPARAMS) -c -o $@ $<

$(KRNLOBJDIR)/gdb/i386-stub.o: $(KRNLSRCDIR)/gdb/i386-stub.c
	mkdir -p $(@D)
	i686-elf-gcc $(GCCPARAMS) -fleading-underscore -c -o $@ $<

$(KRNLOBJDIR)/%.o: $(KRNLSRCDIR)/%.s
	mkdir -p $(@D)
	i686-elf-as $(ASPARAMS) -o $@ $<

$(KRNLOBJDIR)/%.o: $(KRNLSRCDIR)/%.asm
	mkdir -p $(@D)
	nasm -f elf32 -O0 $< -o $@



pranaOSbin: kernel/linker.ld $(KRNLOBJS)
	i686-elf-ld $(LDPARAMS) -T $< -o $@ $(KRNLOBJS)

pranaOSiso: pranaOSbin
	cd lib/ && $(MAKE)
	cd apps/ && $(MAKE)
	
	nm -a pranaOSbin | sort -d > isofiles/debug.sym
	cp -r isofiles/. iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp pranaOSbin iso/boot/pranaOSbin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue --output=pranaOS.iso iso
	rm -rf iso

.PHONY: clean qemu kdbg run filelist serialDBG qemuDBG fastApps
clean:
	rm -rf $(KRNLOBJDIR) pranaOSbin pranaOS.iso
	cd lib/ && $(MAKE) clean
	cd apps/ && $(MAKE) clean
	rm -rf isofiles/apps/*.bin
	rm -rf isofiles/apps/*.sym

qemu: pranaOS.iso
	qemu-system-i386 -cdrom pranaOS.iso -serial stdio $(QEMUOPTIONS)

qemuDBG: pranaOS.iso
	qemu-system-i386 -cdrom pranaOS.iso -serial stdio $(QEMUOPTIONS) -s -S &

qemuGDB: pranaOS.iso
	qemu-system-i386 -cdrom pranaOS.iso $(QEMUOPTIONS) -serial pty &
	gdb -ex 'file pranaOSbin' -ex 'target remote /dev/pts/1' -q

run: pranaOS.iso
	vboxmanage startvm "pranaOS" -E VBOX_GUI_DBG_AUTO_SHOW=true -E VBOX_GUI_DBG_ENABLED=true &
	rm "pranaOS.log"
	echo "" > "pranaOS.log"
	tail -f "pranaOS.log"

serialDBG:
	gcc -o tools/serialDebugger/a.out tools/serialDebugger/main.c
	sudo ./tools/serialDebugger/a.out

kdbg: pranaOS.iso
	qemu-system-i386 $(QEMUOPTIONS) -cdrom pranaOS.iso -serial stdio -s -S &
	kdbg -r localhost:1234 pranaOSbin

grub-core:
	grub-mkimage -o isofiles/setup/core.img -O i386-pc -p="(hd0,msdos1)/boot/grub" --config=grubcore.cfg -v configfile biosdisk part_msdos fat normal multiboot echo

fastApps:
	rm -rf isofiles/apps/*.bin
	cd lib/ && $(MAKE) clean && $(MAKE)
	cd apps/ && $(MAKE) clean && $(MAKE)
	rm pranaOS.iso

turboApps:
	rm -rf isofiles/apps/*.bin
	cd apps/ && $(MAKE) clean && $(MAKE)
	rm pranaOS.iso

installUSB: pranaOS.iso pranaOSbin isofiles/debug.sym isofiles/apps
	rm -rf /media/remco/ISOIMAGE/apps/*.bin
	cp -r isofiles/apps/* /media/remco/ISOIMAGE/apps/
	cp isofiles/debug.sym /media/remco/ISOIMAGE/debug.sym
	cp pranaOSbin /media/remco/ISOIMAGE/boot/pranaOSbin
	umount /media/remco/ISOIMAGE

debug: pranaOS.iso
	pyuic5 tools/advancedDebugger/mainGUI.ui -o tools/advancedDebugger/mainWindow.py
	qemu-system-i386 -cdrom pranaOS.iso $(QEMUOPTIONS) -serial pty &
	/usr/bin/python3 tools/advancedDebugger/main.py


filelist:
	@echo "Source Files:"
	@echo -$(KRNLFILES)
	@echo "Object Files:"
	@echo -$(KRNLOBJS)