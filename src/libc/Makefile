ifeq (,$(SRCDIR))
ROOTDIR:=$(dir $(shell p=;for d in $$(pwd|sed 's/[^/]*/ /g');do find $${p}Makefile.root && break; p+=../; done 2>/dev/null))
include $(ROOTDIR)/Makefile.root
else


# Local variables

TGTS_$(d)       := $(d)libc.a
OBJS_$(d)       := $(patsubst $(SRCDIR)/%,%,$(patsubst %.c,%.o,$(wildcard $(SRCDIR)/$(d)*.c)))

DEPS_$(d)       := $(OBJS_$(d):%=%.d)
TGTDEPS_$(d)    := $(TGTS_$(d):%=%.d)
CLEAN_$(d)      := $(OBJS_$(d)) $(DEPS_$(d)) $(TGTDEPS_$(d)) $(TGTS_$(d))


# Local rules

$(d)libc.a: $(OBJS_$(d))

$(OBJS_$(d)):   CF_TGT := -I$(SRCDIR)/$(d)

TGT_BIN         := $(TGT_BIN) $(TGTS_$(d))

endif
