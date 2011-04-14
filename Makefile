#
# Makefile for Streets Of Doom Arena (S.O.D.A.)
#

binary   := soda
objects  := soda.o input.o graphics.o scene.o item.o game.o misc.o constants.o
wobjects := $(patsubst %.o, %.obj, $(objects))
wdist    := soda_win32

ARCH       :=
COMP_FLAGS := -O2 -Wall -pipe -fomit-frame-pointer
LINK_FLAGS := -Wl,-s,-O1
SDL_SDK    := SDL-1.2.14

CC       ?= /usr/bin/gcc
MINGW    ?= /usr/bin/i586-mingw32msvc-gcc
CFLAGS   := $(COMP_FLAGS) $(ARCH) $(shell sdl-config --cflags)
MFLAGS   := $(COMP_FLAGS) -I$(SDL_SDK)/include/SDL -D_GNU_SOURCE=1 -Dmain=SDL_main

LDFLAGS  := $(LINK_FLAGS) $(shell sdl-config --libs)
MLDFLAGS := $(LINK_FLAGS) -L$(SDL_SDK)/lib -lmingw32 -lSDLmain -lSDL -mwindows

all: $(binary)

$(binary).exe: $(wobjects)
	@echo "LD [win32] $@" && $(MINGW) -o $@ $^ $(MLDFLAGS)

$(binary): $(objects)
	@echo "LD  $@" && $(CC) $(LDFLAGS) -o $@ $^

$(objects): constants.h soda.h
$(wobjects): constants.h soda.h

windist: $(binary).exe
	@test -d $(wdist) || mkdir $(wdist); \
	cp -rlv stuff $(wdist); \
	cp $(binary).exe $(SDL_SDK)/bin/SDL.dll $(wdist); \
	zip -r9m $(wdist).zip $(wdist)

.SUFFIXES:
.SUFFIXES: .c .o .obj .s
.c.o:
	@echo "CC  $@" && $(CC) $(CFLAGS) -c $<

.c.s:
	@echo "AS  $@" && $(CC) $(CFLAGS) -S $<

.c.obj:
	@echo "CC [win32] $@" && $(MINGW) $(MFLAGS) -o $@ -c $<

.PHONY: clean windist
clean:
	@-rm -rfv $(wobjects) $(objects) $(binary) $(binary).exe $(wdist)
	
