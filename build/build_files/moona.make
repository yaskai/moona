# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_x64
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
INCLUDES += -I../../src -I../../include -I../external/raylib-master/src -I../external/raylib-master/src/external -I../external/raylib-master/src/external/glfw/include
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_x64)
TARGETDIR = ../../bin/Debug
TARGET = $(TARGETDIR)/moona.exe
OBJDIR = obj/x64/Debug/moona
DEFINES += -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_WIN32
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -g -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -g -std=c++17
LIBS += ../../bin/Debug/raylib.lib -lwinmm -lgdi32 -lopengl32
LDDEPS += ../../bin/Debug/raylib.lib
ALL_LDFLAGS += $(LDFLAGS) -L../../bin/Debug -L/usr/lib64 -m64

else ifeq ($(config),debug_x86)
TARGETDIR = ../../bin/Debug
TARGET = $(TARGETDIR)/moona.exe
OBJDIR = obj/x86/Debug/moona
DEFINES += -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_WIN32
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -g -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -g -std=c++17
LIBS += ../../bin/Debug/raylib.lib -lwinmm -lgdi32 -lopengl32
LDDEPS += ../../bin/Debug/raylib.lib
ALL_LDFLAGS += $(LDFLAGS) -L../../bin/Debug -L/usr/lib32 -m32

else ifeq ($(config),debug_arm64)
TARGETDIR = ../../bin/Debug
TARGET = $(TARGETDIR)/moona.exe
OBJDIR = obj/ARM64/Debug/moona
DEFINES += -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_WIN32
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -g -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -g -std=c++17
LIBS += ../../bin/Debug/raylib.lib -lwinmm -lgdi32 -lopengl32
LDDEPS += ../../bin/Debug/raylib.lib
ALL_LDFLAGS += $(LDFLAGS) -L../../bin/Debug

else ifeq ($(config),release_x64)
TARGETDIR = ../../bin/Release
TARGET = $(TARGETDIR)/moona.exe
OBJDIR = obj/x64/Release/moona
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_WIN32
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -O2 -std=c99 -Wl,--subsystem,windows
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -O2 -std=c++17 -Wl,--subsystem,windows
LIBS += ../../bin/Release/raylib.lib -lwinmm -lgdi32 -lopengl32
LDDEPS += ../../bin/Release/raylib.lib
ALL_LDFLAGS += $(LDFLAGS) -L../../bin/Release -L/usr/lib64 -m64 -mwindows -s

else ifeq ($(config),release_x86)
TARGETDIR = ../../bin/Release
TARGET = $(TARGETDIR)/moona.exe
OBJDIR = obj/x86/Release/moona
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_WIN32
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -O2 -std=c99 -Wl,--subsystem,windows
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -O2 -std=c++17 -Wl,--subsystem,windows
LIBS += ../../bin/Release/raylib.lib -lwinmm -lgdi32 -lopengl32
LDDEPS += ../../bin/Release/raylib.lib
ALL_LDFLAGS += $(LDFLAGS) -L../../bin/Release -L/usr/lib32 -m32 -mwindows -s

else ifeq ($(config),release_arm64)
TARGETDIR = ../../bin/Release
TARGET = $(TARGETDIR)/moona.exe
OBJDIR = obj/ARM64/Release/moona
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_WIN32
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -O2 -std=c99 -Wl,--subsystem,windows
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -O2 -std=c++17 -Wl,--subsystem,windows
LIBS += ../../bin/Release/raylib.lib -lwinmm -lgdi32 -lopengl32
LDDEPS += ../../bin/Release/raylib.lib
ALL_LDFLAGS += $(LDFLAGS) -L../../bin/Release -mwindows -s

else ifeq ($(config),debug_rgfw_x64)
TARGETDIR = ../../bin/Debug_RGFW
TARGET = $(TARGETDIR)/moona.exe
OBJDIR = obj/x64/Debug_RGFW/moona
DEFINES += -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33 -D_WIN32
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -std=c++17
LIBS += ../../bin/Debug_RGFW/raylib.lib -lwinmm -lgdi32 -lopengl32
LDDEPS += ../../bin/Debug_RGFW/raylib.lib
ALL_LDFLAGS += $(LDFLAGS) -L../../bin/Debug_RGFW -L/usr/lib64 -m64 -s

else ifeq ($(config),debug_rgfw_x86)
TARGETDIR = ../../bin/Debug_RGFW
TARGET = $(TARGETDIR)/moona.exe
OBJDIR = obj/x86/Debug_RGFW/moona
DEFINES += -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33 -D_WIN32
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -std=c++17
LIBS += ../../bin/Debug_RGFW/raylib.lib -lwinmm -lgdi32 -lopengl32
LDDEPS += ../../bin/Debug_RGFW/raylib.lib
ALL_LDFLAGS += $(LDFLAGS) -L../../bin/Debug_RGFW -L/usr/lib32 -m32 -s

else ifeq ($(config),debug_rgfw_arm64)
TARGETDIR = ../../bin/Debug_RGFW
TARGET = $(TARGETDIR)/moona.exe
OBJDIR = obj/ARM64/Debug_RGFW/moona
DEFINES += -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33 -D_WIN32
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -std=c++17
LIBS += ../../bin/Debug_RGFW/raylib.lib -lwinmm -lgdi32 -lopengl32
LDDEPS += ../../bin/Debug_RGFW/raylib.lib
ALL_LDFLAGS += $(LDFLAGS) -L../../bin/Debug_RGFW -s

else ifeq ($(config),release_rgfw_x64)
TARGETDIR = ../../bin/Release_RGFW
TARGET = $(TARGETDIR)/moona.exe
OBJDIR = obj/x64/Release_RGFW/moona
DEFINES += -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33 -D_WIN32
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -std=c++17
LIBS += ../../bin/Release_RGFW/raylib.lib -lwinmm -lgdi32 -lopengl32
LDDEPS += ../../bin/Release_RGFW/raylib.lib
ALL_LDFLAGS += $(LDFLAGS) -L../../bin/Release_RGFW -L/usr/lib64 -m64 -s

else ifeq ($(config),release_rgfw_x86)
TARGETDIR = ../../bin/Release_RGFW
TARGET = $(TARGETDIR)/moona.exe
OBJDIR = obj/x86/Release_RGFW/moona
DEFINES += -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33 -D_WIN32
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -std=c++17
LIBS += ../../bin/Release_RGFW/raylib.lib -lwinmm -lgdi32 -lopengl32
LDDEPS += ../../bin/Release_RGFW/raylib.lib
ALL_LDFLAGS += $(LDFLAGS) -L../../bin/Release_RGFW -L/usr/lib32 -m32 -s

else ifeq ($(config),release_rgfw_arm64)
TARGETDIR = ../../bin/Release_RGFW
TARGET = $(TARGETDIR)/moona.exe
OBJDIR = obj/ARM64/Release_RGFW/moona
DEFINES += -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33 -D_WIN32
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -std=c++17
LIBS += ../../bin/Release_RGFW/raylib.lib -lwinmm -lgdi32 -lopengl32
LDDEPS += ../../bin/Release_RGFW/raylib.lib
ALL_LDFLAGS += $(LDFLAGS) -L../../bin/Release_RGFW -s

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/animation.o
GENERATED += $(OBJDIR)/audioPlayer.o
GENERATED += $(OBJDIR)/enemy.o
GENERATED += $(OBJDIR)/handler.o
GENERATED += $(OBJDIR)/main.o
GENERATED += $(OBJDIR)/pickup.o
GENERATED += $(OBJDIR)/player.o
GENERATED += $(OBJDIR)/spritesheet.o
GENERATED += $(OBJDIR)/tilemap.o
OBJECTS += $(OBJDIR)/animation.o
OBJECTS += $(OBJDIR)/audioPlayer.o
OBJECTS += $(OBJDIR)/enemy.o
OBJECTS += $(OBJDIR)/handler.o
OBJECTS += $(OBJDIR)/main.o
OBJECTS += $(OBJDIR)/pickup.o
OBJECTS += $(OBJDIR)/player.o
OBJECTS += $(OBJDIR)/spritesheet.o
OBJECTS += $(OBJDIR)/tilemap.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking moona
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning moona
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/animation.o: ../../src/animation.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/audioPlayer.o: ../../src/audioPlayer.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/enemy.o: ../../src/enemy.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/handler.o: ../../src/handler.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/main.o: ../../src/main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pickup.o: ../../src/pickup.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/player.o: ../../src/player.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/spritesheet.o: ../../src/spritesheet.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tilemap.o: ../../src/tilemap.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif