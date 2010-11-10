ifeq (,$(SRCDIR))
ROOTDIR:=$(dir $(shell p=;for d in $$(pwd|sed 's/[^/]*/ /g');do find $${p}Makefile.root && break; p+=../; done 2>/dev/null))
include $(ROOTDIR)/Makefile.root
else


# Local variables

TGT_LIB_$(d)	:= $(d)libc.a
OBJ_$(d)	:= $(patsubst $(SRCDIR)/%,%,$(patsubst %.c,%.o,$(wildcard $(SRCDIR)/$(d)*.c)))

DEP_OBJ_$(d)	:= $(OBJ_$(d):%=%.d)


# Local rules

$(TGT_LIB_$(d)): $(OBJ_$(d))

$(OBJ_$(d)):	CF_TGT := -I$(SRCDIR)/$(d)

endif
