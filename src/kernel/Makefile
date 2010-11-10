ifeq (,$(SRCDIR))
ROOTDIR:=$(dir $(shell p=;for d in $$(pwd|sed 's/[^/]*/ /g');do find $${p}Makefile.root && break; p+=../; done 2>/dev/null))
include $(ROOTDIR)/Makefile.root
else

# include Subdirectories
DIRS    := $(patsubst $(SRCDIR)/$(d)/%/Makefile,%,$(shell ls $(SRCDIR)/$(d)/*/Makefile 2>/dev/null))
ifneq (,$(DIRS))
  $(foreach DIR,$(DIRS),$(eval $(call SUBDIR_template,$(DIR),$(DIR)/Makefile)))
endif


# Local variables

TGT_BIN_$(d)	:= $(patsubst $(SRCDIR)/%,%,$(patsubst %.c,%,$(shell grep -l '^int main *([^()]*)' $(SRCDIR)/$(d)*.c 2>/dev/null)))
OBJ_$(d)	:= $(patsubst $(SRCDIR)/%,%,$(patsubst %.c,%.o,$(wildcard $(SRCDIR)/$(d)*.c)))
OBJ_$(d)        += $(patsubst $(SRCDIR)/%,%,$(patsubst %.asm,%.o,$(wildcard $(SRCDIR)/$(d)*.asm)))

DEP_OBJ_$(d)	:= $(OBJ_$(d):%=%.d)
DEP_TGT_$(d)	:= $(TGT_BIN_$(d):%=%.dd)


# Local rules

$(d)main:	LF_TGT := -T$(SRCDIR)/link.ld
$(d)main:	$(d)start.o  libc/libc.a

$(OBJ_$(d)) $(DEP_TGT_$(d)):   CF_TGT := -I$(SRCDIR)/$(d) -isystem $(SRCDIR)/libc	-O -fstrength-reduce -fomit-frame-pointer -finline-functions

endif
