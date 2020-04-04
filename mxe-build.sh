#!/bin/sh

MINGW_ROOT=/home/$USER/mxe
# And don't touch anything below this for now.
MINGW_PREFIX=$MINGW_ROOT/usr
# MINGW_TARGET=i686-w64-mingw32.static
MINGW_TARGET=x86_64-w64-mingw32.static
MINGW_PATH="$MINGW_PREFIX/$MINGW_TARGET/bin:$MINGW_PREFIX/bin"
STD_PATH="/opt/local/bin:/opt/local/sbin:/usr/local/bin:/usr/bin:/bin:/usr/X11/bin" # Get the path for the user you are compiling this under and put it here.
# No Touchie

export PATH="$PATH:$MINGW_PATH:$STD_PATH"
PATH="$MINGW_PATH:$STD_PATH" SDL_CONFIG="$MINGW_PREFIX/$MINGW_TARGET/bin/sdl-config" ACLOCAL_FLAGS="-I$MINGW_PREFIX/$MINGW_TARGET/share/aclocal" LDFLAGS=-Wl,-S LIBS="-lfreetype -ltiff -ljpeg -lpng -lz -lstdc++ -lws2_32 -lbz2 -liphlpapi" ./autogen.sh --prefix=$MINGW_PREFIX --target=$MINGW_TARGET --host=$MINGW_TARGET
PATH="$MINGW_PATH:$STD_PATH" make -j`nproc`
PATH="$MINGW_PATH:$STD_PATH" make dist windist datadist
