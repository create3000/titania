
CLEANFILES = natsort.1

bin_PROGRAMS = natsort
nodist_man_MANS = natsort.1
dist_doc_DATA = AUTHORS COPYING HISTORY README.md

if USE_GIT_VERSION
_VERSION = `git describe --abbrev=4 --dirty --always`
DEFS += -DGIT_VERSION=\"$(_VERSION)\"
else
_VERSION = $(PACKAGE_VERSION)
endif

dist-hook:
	cp "$(srcdir)/natsort.pod" "$(distdir)/"

AM_CFLAGS = -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
			-Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
			-Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
			-Wuninitialized -Wconversion -Wstrict-prototypes

natsort_SOURCES = main.c sort.c sort.h

natsort.1: natsort.pod
	pod2man --center="Natural Sort utility" --section=1 --release=$(_VERSION) \
		natsort.pod natsort.1
