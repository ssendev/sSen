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

TGTS_$(d)       := $(patsubst $(SRCDIR)/%,%,$(patsubst %.c,%,$(shell grep -l '^int main *([^()]*)' $(SRCDIR)/$(d)*.c 2>/dev/null)))
OBJS_$(d)       := $(patsubst $(SRCDIR)/%,%,$(patsubst %.c,%.o,$(wildcard $(SRCDIR)/$(d)*.c)) $(patsubst %.asm,%.o,$(wildcard $(SRCDIR)/$(d)*.asm)))

DEPS_$(d)       := $(OBJS_$(d):%=%.d)
TGTDEPS_$(d)    := $(TGTS_$(d):%=%.d)
CLEAN_$(d)      := $(OBJS_$(d)) $(DEPS_$(d)) $(TGTDEPS_$(d)) $(TGTS_$(d))



# Local rules

$(d)main:	LF_TGT := -T$(SRCDIR)/link.ld
$(d)main:	$(d)start.o  libc/libc.a

$(OBJS_$(d)):   CF_TGT := -I$(SRCDIR)/$(d) -isystem $(SRCDIR)/libc	-O -fstrength-reduce -fomit-frame-pointer -finline-functions

TGT_BIN         := $(TGT_BIN) $(TGTS_$(d))

endif
