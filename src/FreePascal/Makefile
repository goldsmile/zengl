# Get current platform
ifeq ($(shell uname -m), x86_64)
  ARCH = x86_64
else
  ARCH = i386
endif

# Get current OS
ifeq ($(shell uname), Linux)
  OS = linux
else ifeq ($(shell uname), Darwin)
  OS = darwin
endif

# Variables
FPC              = fpc
DEFINES          = #-dUSE_EXPORT_C -Cccdecl
FLAGS          	 = -O3 -XsX -CX -Sd $(DEFINES)
LIBS             = -Fu../../lib/jpeg/$(ARCH)-$(OS) -Fu../../lib/ogg/$(ARCH)-$(OS) -Fu../../lib/theora/$(ARCH)-$(OS) -Fu../../lib/zlib/$(ARCH)-$(OS) -Fu../../lib/zip/$(ARCH)-$(OS)
LIBSPPC          = -Fu../../lib/jpeg/powerpc-$(OS) -Fu../../lib/ogg/powerpc-$(OS) -Fu../../lib/theora/powerpc-$(OS) -Fu../../lib/zlib/powerpc-$(OS) -Fu../../lib/zip/powerpc-$(OS)
# Linux 32-bit
LIBS_LINUX32   	 = -Fu../../lib/jpeg/i386-linux -Fu../../lib/ogg/i386-linux -Fu../../lib/theora/i386-linux -Fu../../lib/zlib/i386-linux -Fu../../lib/zip/i386-linux
# Win32
BINUTILS_WIN32 	 = -XPi486-mingw32-
LIBS_WIN32     	 = -Fu../../lib/jpeg/i386-win32 -Fu../../lib/msvcrt/i386 -Fu../../lib/ogg/i386-win32 -Fu../../lib/theora/i386-win32 -Fu../../lib/zlib/i386-win32 -Fu../../lib/zip/i386-win32
# Win64
BINUTILS_WIN64 	 = -XPx86_64-w64-mingw32-
LIBS_WIN64     	 = -Fu../../lib/jpeg/x86_64-win64 -Fu../../lib/msvcrt/x86_64 -Fu../../lib/ogg/x86_64-win64 -Fu../../lib/theora/x86_64-win64 -Fu../../lib/zlib/x86_64-win64 -Fu../../lib/zip/x86_64-win64
# WinCE
WINCE_FLAGS      = -CpARMv4 -CfSOFT
BINUTILS_WINCE 	 = -XParm-wince-cegcc-
LIBS_WINCE     	 = -Fu../../lib/jpeg/arm-wince -Fu../../lib/ogg/arm-wince -Fu../../lib/theora/arm-wince -Fu../../lib/zlib/arm-wince -Fu../../lib/zip/arm-wince
# Android
ANDROID_FLAGS    = -CpARMv6 -CfVFPv2
ANDROID_LIB      = -Fu$(ANDROID_NDK)/platforms/android-3/arch-arm/usr/lib
ANDROID_STRIP    = $(ANDROID_NDK)/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-strip
BINUTILS_ANDROID = -XParm-linux-androideabi- -FD$(ANDROID_NDK)/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin
LIBS_ANDROID   	 = -Fu../../lib/jpeg/arm-linux -Fu../../lib/ogg/arm-linux -Fu../../lib/theora/arm-linux -Fu../../lib/zlib/arm-linux -Fu../../lib/zip/arm-linux -Fu../../lib/chipmunk/arm-linux

OUTPUT           = ./
DESTDIR          = /usr
TMP              = -FU../tmp
TARGET           = libZenGL.so
TARGETW          = ZenGL.dll
TARGETM          = libZenGL.dylib

# Targets
all: clean
	$(FPC) ../ZenGL.pas -Fu../ $(LIBS) -FE$(OUTPUT) $(TMP) $(FLAGS) -o$(TARGET) -TLINUX -P$(ARCH)
	strip $(OUTPUT)$(TARGET) --strip-unneeded -R .comment -R .note

linux32: clean
	$(FPC) ../ZenGL.pas -Fu../ $(LIBS_LINUX32) -FE$(OUTPUT) $(TMP) $(FLAGS) -o$(TARGET) -TLINUX -Pi386 -Xd
	strip $(OUTPUT)$(TARGET) --strip-unneeded -R .comment -R .note

android: clean
	$(FPC) ../ZenGL.pas -Fu../ $(LIBS_ANDROID) $(ANDROID_FLAGS) $(ANDROID_LIB) -FE$(OUTPUT) $(TMP) $(FLAGS) -o$(TARGET) -TLINUX -Parm $(BINUTILS_ANDROID) -Xd
	$(ANDROID_STRIP) $(OUTPUT)$(TARGET) --strip-unneeded -R .comment -R .note

win32: clean
	$(FPC) ../ZenGL.pas -Fu../ $(LIBS_WIN32) -FE$(OUTPUT) $(TMP) $(FLAGS) -o$(TARGETW) -TWIN32 -Pi386 $(BINUTILS_WIN32) -WG

win64: clean
	$(FPC) ../ZenGL.pas -Fu../ $(LIBS_WIN64) -FE$(OUTPUT) $(TMP) $(FLAGS) -o$(TARGETW) -TWIN64 -Px86_64 $(BINUTILS_WIN64) -WG

wince: clean
	$(FPC) ../ZenGL.pas -Fu../ $(LIBS_WINCE) $(WINCE_FLAGS) -FE$(OUTPUT) $(TMP) $(FLAGS) -o$(TARGETW) -TWINCE -Parm $(BINUTILS_WINCE) -WG

macos: clean
	$(FPC) ../ZenGL.pas -Fu../ $(LIBS) -FE$(OUTPUT) $(TMP) $(FLAGS) -o$(TARGETM) -TDARWIN -Pi386 -k"-no_order_inits" -k"-no_order_data" -k"-macosx_version_min 10.4"

macos_uni: clean
	$(FPC) ../ZenGL.pas -Fu../ $(LIBS) -FE$(OUTPUT) $(TMP) $(FLAGS) -o$(TARGETM)-386 -TDARWIN -Pi386 -k"-no_order_inits" -k"-no_order_data" -k"-macosx_version_min 10.4"
	$(FPC) ../ZenGL.pas -Fu../ $(LIBSPPC) -FE$(OUTPUT) $(TMP) $(FLAGS) -o$(TARGETM)-ppc -TDARWIN -Ppowerpc -k"-no_order_inits" -k"-no_order_data" -k"-macosx_version_min 10.4"
	lipo -create $(OUTPUT)$(TARGETM)-ppc $(OUTPUT)$(TARGETM)-386 -output $(OUTPUT)$(TARGETM)
	rm $(OUTPUT)$(TARGETM)-386 $(OUTPUT)$(TARGETM)-ppc

clean:
	rm -f *.*~
	rm -f -R ../tmp/*

install:
	install -m 644 $(OUTPUT)$(TARGET) $(DESTDIR)/lib

uninstall:
	rm -f $(DESTDIR)/lib/$(TARGET)
