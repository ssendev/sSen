# GCC options
GCC = /usr/local/cross/bin/i586-elf-gcc
LD = /usr/local/cross/bin/i586-elf-ld
K_OPT = -O -fstrength-reduce -fomit-frame-pointer -finline-functions
INCLUDE = -nostdinc -fno-builtin -I./include
WALL = -Wall

# build variables
build_p = ../build
mount_p = ../mount
floppy_p = $(build_p)/FLOPPY
ssen_p = $(floppy_p)/SSEN
mnt_floppy_p = /mnt/floppy

boot_f = $(build_p)/boot.bin
image_f = $(build_p)/ssen.img
kernel_f = $(ssen_p)/KERNEL.BIN
mnt_floppy_f = /dev/disk/by-id/usb-MITSUMI_MITSUMI_USB_FDD_061M# /dev/fd0


# Make routines
info:
	@echo "try: boot, core, [g]image, [g]floppy, bochs[d], qemu, vmware, b|d|q|v = image emu, [man]debug, test, clean, Setup(the Makefile)"

msg:
	@echo
	@echo "### sSen #####################################################################"
	@echo
dirs: msg
	@[ -e "$(ssen_p)" ] || { echo "create build path"; mkdir -p $(ssen_p); echo;}

boot: dirs
	@echo "compile bootloader"
	@nasm -f bin -o $(boot_f) boot/boot.asm
	@echo

core_Compile:
	@echo "Now assembling, compiling, and linking the kernel"
	@nasm -f elf -o kernel/start.o kernel/start.asm
	@e=0;for f in `ls kernel/*.c`;do $(GCC) $(WALL) $(K_OPT) $(INCLUDE) -c -o $${f%.c}.o $$f || { e="1";break;} ;done; [ 0 -eq $$e ]

core_Link:
	@$(LD) -T link.ld `cat debug-oformat 2>/dev/null && rm debug-oformat` -o $(kernel_f) kernel/start.o `ls kernel/*.o|grep -v "kernel/start.o"`

core: dirs core_Compile core_Link
	@b=`cat $(kernel_f)|wc -c`;echo "size: $$((b/1024/1024)) mb  $$((b/1024%1024)) kb  $$((b%1024)) b"
	@echo

image_raw:
	@echo create floppy image
	@cp $(boot_f) $(image_f)
	@printf "\\760\\777\\777\\000" | dd of=$(image_f) ibs=1 obs=512 seek=1 2>/dev/null
	@printf "\\760\\777\\777\\000" | dd of=$(image_f) ibs=1 obs=512 seek=10 2>/dev/null
	@dd if=/dev/zero of=$(image_f) bs=512 count=1 seek=2879 2>/dev/null

image: boot core image_raw
	@mount $(mount_p)
	@cp -r $(floppy_p)/* $(mount_p)
	@umount $(mount_p)
	@echo

gimage: core
	@echo "create floppy image (with grub bootloader)"
	@cp ../grub/grub.img $(image_f)
	@mount $(mount_p)
	@cp -r $(floppy_p)/* $(mount_p)
	@umount $(mount_p)
	@echo

floppy: boot core
	@echo "write sSen to floppy"
	@sudo dd if=$(boot_f) of=$(mnt_floppy_f) 2>/dev/null
	@printf "\\360\\377\\377\\000" | sudo dd of=$(mnt_floppy_f) ibs=1 obs=512 seek=1 2>/dev/null
	@printf "\\360\\377\\377\\000" | sudo dd of=$(mnt_floppy_f) ibs=1 obs=512 seek=10 2>/dev/null
	@mount $(mnt_floppy_f)
	@cp -r $(floppy_p)/* $(mnt_floppy_p)
	@umount $(mnt_floppy_f)
	@echo

gfloppy: gimage
	@echo "write image to floppy"
	@sudo dd if=$(image_f) of=$(mnt_floppy_f) bs=9216 2>/dev/null
	@echo

../.bochslog:
	@echo "creating ../.bochslog"
	@mknod ../.bochslog p
../.bochsrc: Makefile
	@echo "creating ../.bochsrc"
	@echo -e "boot:a \nfloppya:1_44=$(image_f),status=inserted \nconfig_interface:textconfig \ndisplay_library:x \nlog:../.bochslog" >../.bochsrc
../.bochsrcDebug: Makefile
	@echo "creating ../.bochsrcDebug"
	@echo -e "boot:a \nfloppya:1_44=$(image_f),status=inserted \nconfig_interface:textconfig \ndisplay_library:x, options=gui_debug \nlog:../.bochslog" >../.bochsrcDebug
../.bochsDebugContinue: Makefile
	@echo "creating ../.bochsDebugContinue"
	@echo "continue" > ../.bochsDebugContinue

bochs: msg ../.bochsrc | ../.bochslog
	@echo "running bochs"
	@-trap "" SIGINT; sed '1,/Booting from/d' ../.bochslog & bochs -qf ../.bochsrc
	@echo

bochsd: msg ../.bochsrcDebug ../.bochsDebugContinue | ../.bochslog
	@echo "running bochsdebug"
	@-trap "" SIGINT; sed '1,/Booting from/d' ../.bochslog & bochsdebug -qf ../.bochsrcDebug -rc ../.bochsDebugContinue
	@echo

qemu: msg
	@echo "running qemu"
	@- qemu -m 32 -fda $(image_f)
	@echo

vmware: msg
	@echo "running vmware"
	@vmplayer ~/.0vmware/Floppy/Floppy.vmx
	@echo

b: image bochs
d: image bochsd
q: image qemu
v: image vmware

debug: debug_ core
	@echo "debugging"
	@echo "please enter function[s] to dissassemble:"
	@echo "exact caseinsensitive matching. wildcards * and ? are supported"
	@read funct && objdump $(kernel_f) --dissassemble --line-nunbers --prefix-addresses --show-raw-insn --wide -M intel | sed -ns "s/^\(........ <\|\)\(`echo "$$funct" | sed 's/[ ,|]\+/\\\|/g;s/*/[^(>+]*/g;s/\?/[^(>+]/g'`\)\([(>+].*\)/&/ip" | sed -s 's/<[^>]*+\([^>]*\)> /\1\t/; s/<[^>]*> /0x0\t/; s/\(.*\):$$/\n&/'
	@echo

mandebug: debug_ core
	@echo "debugging"
	@objdump || echo >/dev/null
	@#printf "\nobjdump $(kernel_f) -M intel -\n\n"
	@echo
	@read -p "objdump $(kernel_f) -M intel " cmd && eval objdump $(kernel_f) -M intel $$cmd
debug_:
	@echo "--oformat elf32-i386" > debug-oformat

test: msg
	$(GCC) -o test test.c
	@echo "running test"
	./test
	@echo

clean: msg
	@echo "cleaning build area"
	@rm -rf $(build_p)/*
	@echo

Setup_first:
	@-bochs --help 2&>/dev/null || echo "no bochs"
	@-bochsdebug --help 2&>/dev/null || echo "no bochsdebug"
	@-qemu --help 2&>/dev/null || echo "no qemu"
	@-vmplayer --help 2&>/dev/null || echo "no vmplayer"
	@-[ -e "../grub/grub.img" ] || echo "no grub-image"
	@-objdump -v 2&>/dev/null || echo "no objdump (debug)"
	@nasm -v 2&>/dev/null || { echo "please install nasm"; exit 1; }
	@[ -e $(GCC) ] || { echo "$(GCC) dosn't exist please install gcc or set \$$(GCC) in this Makefile correctly"; exit 1; }
	@[ -e $(LD) ] || { echo "$(LD) dosn't exist please install ld or set \$$(LD) in this Makefile correctly"; exit 1; }
	@echo

Setup: msg Setup_first boot image_raw
	@echo
	@out="`mount $(mount_p) 2>&1`" || { echo $$out | grep -v fstab || echo "put \"`echo $$PWD/$(image_f)|sed ':beg;s#[^/]*/../##;t beg'`  `echo $$PWD/$(mount_p)|sed ':beg;s#[^/]*/../##;t beg'`  vfat  loop,user,noauto,shortname=mixed  0 0\" into /etc/fstab"; exit 1; }
	@umount $(mount_p)
	@out="`mount $(mnt_floppy_f) 2>&1`" || { echo $$out | grep -v fstab || printf "put \"$(mnt_floppy_f)  $(mnt_floppy_p)  vfat  user,noauto,shortname=mixed  0 0\"\nand eventually \"/dev/sdb                /mnt/floppy             vfat    user,noauto,shortname=mixed    0 0\" into /etc/fstab and create \"$(mnt_floppy_p)\""; exit 1; }
	@umount $(mnt_floppy_f)
	@printf "\\760\\777\\777\\000" | sudo dd  of=$(mnt_floppy_f) ibs=1 obs=512 seek=1 2>/dev/null || printf "you probably want to set \$$\(mnt_floppy_f\) inside the Makefile to /dev/fd0 before continuing.\nadd \"\n\n`whoami`	`echo $$HOSTNAME | cut -d "." -f 1`=NOPASSWD: /bin/dd if=$(boot_f) of=$(mnt_floppy_f), /bin/dd of=$(mnt_floppy_f) ibs=1 obs=512 seek=1, /bin/dd of=$(mnt_floppy_f) ibs=1 obs=512 seek=10, /bin/dd if=$(image_f) of=$(mnt_floppy_f) bs=9216\n\n\" to /etc/sudoers. note that there should be a tab after your username, if you feel too insecure the NOPASSWD: can be removed"
	@echo "Makefile Setup succesfully completed."
	@echo

