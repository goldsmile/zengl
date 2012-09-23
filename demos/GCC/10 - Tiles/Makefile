# Get current platform
ifeq ($(shell uname -m), x86_64)
  ARCH  = x86_64
else
  ARCH  = i386
endif

# Get current OS
ifeq ($(shell uname), Linux)
  OS    = linux
else ifeq ($(shell uname), Darwin)
  OS    = darwin
endif

# Variables
GCC     = gcc
MINGW32 = i486-mingw32-gcc
MINGW64 = x86_64-w64-mingw32-gcc
FLAGS   = -O3 -std=c99 -Wall
INCLUDE = -I../../../headers
LIBS    = -ldl
OUTPUT  = ../../../bin/
TMP     = ../tmp/
UNIT    = demo10.c
TARGET  = demo10
TARGETW = demo10.exe

# Targets
all: clean
	$(GCC) $(UNIT) $(INCLUDE) $(LIBS) -o$(OUTPUT)$(ARCH)-$(OS)/$(TARGET) $(FLAGS)
	strip $(OUTPUT)$(ARCH)-$(OS)/$(TARGET) --strip-unneeded -R .comment -R .note

linux32: clean
	$(GCC) $(UNIT) $(INCLUDE) $(LIBS) -o$(OUTPUT)i386-linux/$(TARGET) $(FLAGS) -m32
	strip $(OUTPUT)i386-linux/$(TARGET) --strip-unneeded -R .comment -R .note

win32: clean
	$(MINGW32) $(UNIT) $(INCLUDE) -o$(OUTPUT)i386-win32/$(TARGETW) $(FLAGS)
	strip $(OUTPUT)i386-win32/$(TARGETW) --strip-unneeded -R .comment -R .note

win64: clean
	$(MINGW64) $(UNIT) $(INCLUDE) -o$(OUTPUT)x86_64-win64/$(TARGETW) $(FLAGS)
	strip $(OUTPUT)x86_64-win64/$(TARGETW) --strip-unneeded -R .comment -R .note

clean:
	rm -f *.*~
	rm -f $(TMP)*.*
