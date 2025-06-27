CC=g++
CFLAGS=-c -Wall -ansi -Wno-long-long -m64 -O2
LDFLAGS=-m64

SDIR=src
OUT_PREFIX=
OUT_POSTFIX=_bin

ODIR_PREFIX=obj
ODIRS=

SOURCES=NewickParser.cpp Soda13Impl.cpp RootedTree.cpp \
		HDTCountingLeaf.cpp HDTCountingCCToC.cpp HDTCountingIGToC.cpp HDTCountingCTransform.cpp HDTCountingG.cpp \
		HDT.cpp HDTFactory.cpp HDTListUtils.cpp RootedTreeFactory.cpp main.cpp int_stuff.cpp

ifdef QUARTETS
	CFLAGS += -DquartetsToo
	OUT := quart
else
	OUT := trip
endif

ifndef NO_N4_128
	CFLAGS += -DN4INT128
else
	OUT := $(OUT)_64bitints
endif

ifndef NOEXTRACT
	CFLAGS += -DdoExtractAndContract
else
	OUT := $(OUT)_noextract
endif

ifdef CONTRACT_NUM
	CFLAGS += -DCONTRACT_MAX_EXTRA_SIZE=$(CONTRACT_NUM)
endif

ODIR := $(ODIR_PREFIX)_$(OUT)
ODIRS := $(foreach _ODIR,$(ODIRS),$(ODIR)/$(_ODIR))
ODIRS := $(ODIR) $(ODIRS)
OUT := $(OUT)$(OUT_POSTFIX)

_OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS=$(patsubst %,$(ODIR)/%,$(_OBJECTS))

.PHONY: all clean

all: $(OUT)

clean:
	rm -rf $(ODIR_PREFIX)_* *_bin *_bin.exe

$(ODIRS):
	[ -e $(ODIR) ] || mkdir $(ODIRS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ $<

$(OUT): $(ODIR) $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)
	@echo 
	@echo --- Compiled binary $(OUT) ---
