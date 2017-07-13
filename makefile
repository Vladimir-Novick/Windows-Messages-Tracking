# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: trackdll.lib trackdll.dll

# Update the object files if necessary

trackdll.obj: trackdll.c
    $(cc) $(scall) $(cflags) $(cvarsdll) $(cdebug) trackdll.c

# Update the import library

trackdll.lib: trackdll.obj trackdll.def
    $(implib) -machine:$(CPU)     \
    -def:trackdll.def           \
    trackdll.obj                \
    -out:trackdll.lib

# Update the dynamic link library

trackdll.dll: trackdll.obj trackdll.def
    $(link) $(linkdebug) $(dlllflags)     \
    -base:0x1C000000  \
    -out:trackdll.dll   \
    trackdll.exp trackdll.obj $(guilibsdll)

