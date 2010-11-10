ifeq (,$(SRCDIR))
ROOTDIR:=$(dir $(shell p=;for d in $$(pwd|sed 's/[^/]*/ /g');do find $${p}Makefile.root && break; p+=../; done 2>/dev/null))
include $(ROOTDIR)/Makefile.root
else

# include Subdirectories
DIRS    := $(patsubst $(SRCDIR)/$(d)%/Makefile,%,$(shell ls $(SRCDIR)/$(d)*/Makefile 2>/dev/null))
ifneq (,$(DIRS))
  $(foreach DIR,$(DIRS),$(eval $(call SUBDIR_template,$(DIR),$(SRCDIR)/$(d)$(DIR)/Makefile)))
endif


# Local variables

TGT_BIN_$(d)	:=
OBJ_$(d)	:=

# Local rules

MOUNTDIR = /mnt/floppy
FLOPPYDIR = FLOPPY
SSENDIR = $(MOUNTDIR)/SSEN
KERNELBINDST = $(SSENDIR)/KERNEL.BIN
KERNELBINSRC = kernel/main
BOOTBIN = boot/boot.bin
FLOPPYDEV = /dev/disk/by-id/usb-MITSUMI_MITSUMI_USB_FDD_061M# /dev/fd0
IMG = ssen

#MOUNTFLOPPYDIR = /mnt/floppy

.PHONY: help
help:
	@echo "try: boot/boot.bin, kernel/main, ssen.img, ssen_grub.img, [g]floppy, bochs[d], qemu, clean"


OBJ_$(d)	+= $(IMG)_boot.img
$(IMG)_boot.img:	$(BOOTBIN)
	@test $(S) || echo '[01;32m$@:[0m '
	cp $(BOOTBIN) $@
	printf "\\760\\777\\777\\000" | dd of=$@ ibs=1 obs=512 seek=1 2>/dev/null
	printf "\\760\\777\\777\\000" | dd of=$@ ibs=1 obs=512 seek=10 2>/dev/null
	dd if=/dev/zero of=$@ bs=512 count=1 seek=2879 2>/dev/null

TGT_BIN_$(d)	+= $(IMG).img
$(IMG).img:	$(IMG)_boot.img $(KERNELBINSRC)
	@test $(S) || echo '[01;32m$@:[0m '
	cp $< $@
	mount $@
	mkdir -p $(SSENDIR)
	cp $(KERNELBINSRC) $(KERNELBINDST)
	-cp -r $(FLOPPYDIR)/* $(MOUNTDIR)
	umount $@

OBJ_$(d)	+= $(IMG)_grub.img
$(IMG)_grub.img:	$(KERNELBINSRC)
	@test $(S) || echo '[01;32m$@:[0m '
	cp $(SRCDIR)/../grub/grub.img $@
	mount $@
	mkdir -p $(SSENDIR)
	cp $(KERNELBINSRC) $(KERNELBINDST)
	-cp -r $(FLOPPYDIR)/* $(MOUNTDIR)
	umount $@

.PHONY: floppy
floppy: $(BOOTBIN) $(KERNELBINSRC)
	@test $(S) || echo '[01;32m$@:[0m '
	sudo dd if=$(BOOTBIN) of=$(FLOPPYDEV) 2>/dev/null
	printf "\\360\\377\\377\\000" | sudo dd of=$(FLOPPYDEV) ibs=1 obs=512 seek=1 2>/dev/null
	printf "\\360\\377\\377\\000" | sudo dd of=$(FLOPPYDEV) ibs=1 obs=512 seek=10 2>/dev/null
	mount $(FLOPPYDEV)
	mkdir -p $(SSENDIR)
	cp $(KERNELBINSRC) $(KERNELBINDST)
	-cp -r $(FLOPPYDIR)/* $(MOUNTDIR)
	umount $(FLOPPYDEV)

.PHONY: gfloppy
gfloppy: ssen_grub.img
	@echo "write image to floppy"
	sudo dd if=$< of=$(FLOPPYDEV) bs=9216 2>/dev/null
	@echo

.PHONY: umount
umount:
	umount $(MOUNTDIR)


OBJ_$(d)	+= bochslog
bochslog:
	@test $(S) || echo '[01;33m$@:[0m '
	mknod $@ p
OBJ_$(d)	+= bochsrc
bochsrc: Makefile
	@test $(S) || echo '[01;33m$@:[0m '
	echo -e "boot:a \nfloppya:1_44=$(IMG).img,status=inserted \nconfig_interface:textconfig \ndisplay_library:x \nlog:bochslog" >$@
OBJ_$(d)	+= bochsrcDebug
bochsrcDebug: Makefile
	@test $(S) || echo '[01;33m$@:[0m '
	echo -e "boot:a \nfloppya:1_44=$(IMG).img,status=inserted \nconfig_interface:textconfig \ndisplay_library:x, options=gui_debug \nlog:bochslog" >$@
OBJ_$(d)	+= bochsDebugContinue
bochsDebugContinue: Makefile
	@test $(S) || echo '[01;33m$@:[0m '
	echo "continue" >$@

.PHONY: bochs
bochs: $(IMG).img bochsrc | bochslog
	@test $(S) || echo '[01;33m$@:[0m '
	-trap "" SIGINT; sed '1,/Booting from/d' bochslog & bochs -qf bochsrc
.PHONY: bochsd
bochsd: $(IMG).img bochsrcDebug bochsDebugContinue | bochslog
	@test $(S) || echo '[01;33m$@:[0m '
	-trap "" SIGINT; sed '1,/Booting from/d' bochslog & bochsdebug -qf bochsrcDebug -rc bochsDebugContinue

.PHONY: qemu
qemu: $(IMG).img
	@test $(S) || echo '[01;33m$@:[0m '
	-qemu -m 32 -fda $(IMG).img

endif
