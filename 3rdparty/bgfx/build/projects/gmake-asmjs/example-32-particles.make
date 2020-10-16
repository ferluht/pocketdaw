# GNU Make project makefile autogenerated by GENie
ifndef config
  config=debug32
endif

ifndef verbose
  SILENT = @
endif

SHELLTYPE := msdos
ifeq (,$(ComSpec)$(COMSPEC))
  SHELLTYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(SHELL)))
  SHELLTYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(MAKESHELL)))
  SHELLTYPE := posix
endif

ifeq (posix,$(SHELLTYPE))
  MKDIR = $(SILENT) mkdir -p "$(1)"
  COPY  = $(SILENT) cp -fR "$(1)" "$(2)"
  RM    = $(SILENT) rm -f "$(1)"
else
  MKDIR = $(SILENT) mkdir "$(subst /,\\,$(1))" 2> nul || exit 0
  COPY  = $(SILENT) copy /Y "$(subst /,\\,$(1))" "$(subst /,\\,$(2))"
  RM    = $(SILENT) del /F "$(subst /,\\,$(1))" 2> nul || exit 0
endif

CC  = "$(EMSDK)/fastcomp/bin/emcc"
CXX = "$(EMSDK)/fastcomp/bin/em++"
AR  = "$(EMSDK)/fastcomp/bin/emar"

ifndef RESCOMP
  ifdef WINDRES
    RESCOMP = $(WINDRES)
  else
    RESCOMP = windres
  endif
endif

MAKEFILE = example-32-particles.make

ifeq ($(config),debug32)
  OBJDIR              = ../../asmjs/obj/x32/Debug/example-32-particles
  TARGETDIR           = ../../asmjs/bin
  TARGET              = $(TARGETDIR)/example-32-particlesDebug.bc
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DENTRY_CONFIG_IMPLEMENT_MAIN=1
  INCLUDES           += -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../include" -I"../../../3rdparty" -I"../../../examples/common"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -m32 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -m32 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -m32 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -m32 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -m32 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/asmjs" -L"../../asmjs/bin" -m32
  LIBDEPS            += ../../asmjs/bin/libexample-glueDebug.a ../../asmjs/bin/libexample-commonDebug.a ../../asmjs/bin/libbgfxDebug.a ../../asmjs/bin/libbimg_decodeDebug.a ../../asmjs/bin/libbimgDebug.a ../../asmjs/bin/libbxDebug.a
  LDDEPS             += ../../asmjs/bin/libexample-glueDebug.a ../../asmjs/bin/libexample-commonDebug.a ../../asmjs/bin/libbgfxDebug.a ../../asmjs/bin/libbimg_decodeDebug.a ../../asmjs/bin/libbimgDebug.a ../../asmjs/bin/libbxDebug.a
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(CXX) -o $(TARGET) $(LINKOBJS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/examples/32-particles/particles.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
	@echo Running post-build commands
	$(SILENT) echo Running asmjs finalize.
	$(SILENT) "$(EMSDK)/fastcomp/bin/emcc" -O2 -s PRECISE_F32=1 -s TOTAL_MEMORY=268435456 --memory-init-file 1 "$(TARGET)" -o "$(TARGET)".html 
  endef
endif

ifeq ($(config),release32)
  OBJDIR              = ../../asmjs/obj/x32/Release/example-32-particles
  TARGETDIR           = ../../asmjs/bin
  TARGET              = $(TARGETDIR)/example-32-particlesRelease.bc
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG -DENTRY_CONFIG_IMPLEMENT_MAIN=1
  INCLUDES           += -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../include" -I"../../../3rdparty" -I"../../../examples/common"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -m32 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -m32 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -m32 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -m32 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -m32 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/asmjs" -L"../../asmjs/bin" -m32
  LIBDEPS            += ../../asmjs/bin/libexample-glueRelease.a ../../asmjs/bin/libexample-commonRelease.a ../../asmjs/bin/libbgfxRelease.a ../../asmjs/bin/libbimg_decodeRelease.a ../../asmjs/bin/libbimgRelease.a ../../asmjs/bin/libbxRelease.a
  LDDEPS             += ../../asmjs/bin/libexample-glueRelease.a ../../asmjs/bin/libexample-commonRelease.a ../../asmjs/bin/libbgfxRelease.a ../../asmjs/bin/libbimg_decodeRelease.a ../../asmjs/bin/libbimgRelease.a ../../asmjs/bin/libbxRelease.a
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(CXX) -o $(TARGET) $(LINKOBJS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/examples/32-particles/particles.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
	@echo Running post-build commands
	$(SILENT) echo Running asmjs finalize.
	$(SILENT) "$(EMSDK)/fastcomp/bin/emcc" -O2 -s PRECISE_F32=1 -s TOTAL_MEMORY=268435456 --memory-init-file 1 "$(TARGET)" -o "$(TARGET)".html 
  endef
endif

ifeq ($(config),debug64)
  OBJDIR              = ../../asmjs/obj/x64/Debug/example-32-particles
  TARGETDIR           = ../../asmjs/bin
  TARGET              = $(TARGETDIR)/example-32-particlesDebug.bc
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DENTRY_CONFIG_IMPLEMENT_MAIN=1
  INCLUDES           += -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../include" -I"../../../3rdparty" -I"../../../examples/common"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -m64 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -m64 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -m64 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -m64 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -m64 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/asmjs" -L"../../asmjs/bin" -m64
  LIBDEPS            += ../../asmjs/bin/libexample-glueDebug.a ../../asmjs/bin/libexample-commonDebug.a ../../asmjs/bin/libbgfxDebug.a ../../asmjs/bin/libbimg_decodeDebug.a ../../asmjs/bin/libbimgDebug.a ../../asmjs/bin/libbxDebug.a
  LDDEPS             += ../../asmjs/bin/libexample-glueDebug.a ../../asmjs/bin/libexample-commonDebug.a ../../asmjs/bin/libbgfxDebug.a ../../asmjs/bin/libbimg_decodeDebug.a ../../asmjs/bin/libbimgDebug.a ../../asmjs/bin/libbxDebug.a
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(CXX) -o $(TARGET) $(LINKOBJS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/examples/32-particles/particles.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
	@echo Running post-build commands
	$(SILENT) echo Running asmjs finalize.
	$(SILENT) "$(EMSDK)/fastcomp/bin/emcc" -O2 -s PRECISE_F32=1 -s TOTAL_MEMORY=268435456 --memory-init-file 1 "$(TARGET)" -o "$(TARGET)".html 
  endef
endif

ifeq ($(config),release64)
  OBJDIR              = ../../asmjs/obj/x64/Release/example-32-particles
  TARGETDIR           = ../../asmjs/bin
  TARGET              = $(TARGETDIR)/example-32-particlesRelease.bc
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG -DENTRY_CONFIG_IMPLEMENT_MAIN=1
  INCLUDES           += -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../include" -I"../../../3rdparty" -I"../../../examples/common"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -m64 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -m64 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -m64 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -m64 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -m64 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/asmjs" -L"../../asmjs/bin" -m64
  LIBDEPS            += ../../asmjs/bin/libexample-glueRelease.a ../../asmjs/bin/libexample-commonRelease.a ../../asmjs/bin/libbgfxRelease.a ../../asmjs/bin/libbimg_decodeRelease.a ../../asmjs/bin/libbimgRelease.a ../../asmjs/bin/libbxRelease.a
  LDDEPS             += ../../asmjs/bin/libexample-glueRelease.a ../../asmjs/bin/libexample-commonRelease.a ../../asmjs/bin/libbgfxRelease.a ../../asmjs/bin/libbimg_decodeRelease.a ../../asmjs/bin/libbimgRelease.a ../../asmjs/bin/libbxRelease.a
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(CXX) -o $(TARGET) $(LINKOBJS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/examples/32-particles/particles.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
	@echo Running post-build commands
	$(SILENT) echo Running asmjs finalize.
	$(SILENT) "$(EMSDK)/fastcomp/bin/emcc" -O2 -s PRECISE_F32=1 -s TOTAL_MEMORY=268435456 --memory-init-file 1 "$(TARGET)" -o "$(TARGET)".html 
  endef
endif

ifeq ($(config),debug)
  OBJDIR              = ../../asmjs/obj/Debug/example-32-particles
  TARGETDIR           = ../../asmjs/bin
  TARGET              = $(TARGETDIR)/example-32-particlesDebug.bc
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DENTRY_CONFIG_IMPLEMENT_MAIN=1
  INCLUDES           += -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../include" -I"../../../3rdparty" -I"../../../examples/common"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Werror -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/asmjs" -L"../../asmjs/bin"
  LIBDEPS            += ../../asmjs/bin/libexample-glueDebug.a ../../asmjs/bin/libexample-commonDebug.a ../../asmjs/bin/libbgfxDebug.a ../../asmjs/bin/libbimg_decodeDebug.a ../../asmjs/bin/libbimgDebug.a ../../asmjs/bin/libbxDebug.a
  LDDEPS             += ../../asmjs/bin/libexample-glueDebug.a ../../asmjs/bin/libexample-commonDebug.a ../../asmjs/bin/libbgfxDebug.a ../../asmjs/bin/libbimg_decodeDebug.a ../../asmjs/bin/libbimgDebug.a ../../asmjs/bin/libbxDebug.a
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(CXX) -o $(TARGET) $(LINKOBJS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/examples/32-particles/particles.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
	@echo Running post-build commands
	$(SILENT) echo Running asmjs finalize.
	$(SILENT) "$(EMSDK)/fastcomp/bin/emcc" -O2 -s PRECISE_F32=1 -s TOTAL_MEMORY=268435456 --memory-init-file 1 "$(TARGET)" -o "$(TARGET)".html 
  endef
endif

ifeq ($(config),release)
  OBJDIR              = ../../asmjs/obj/Release/example-32-particles
  TARGETDIR           = ../../asmjs/bin
  TARGET              = $(TARGETDIR)/example-32-particlesRelease.bc
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG -DENTRY_CONFIG_IMPLEMENT_MAIN=1
  INCLUDES           += -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../include" -I"../../../3rdparty" -I"../../../examples/common"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Werror -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/asmjs" -L"../../asmjs/bin"
  LIBDEPS            += ../../asmjs/bin/libexample-glueRelease.a ../../asmjs/bin/libexample-commonRelease.a ../../asmjs/bin/libbgfxRelease.a ../../asmjs/bin/libbimg_decodeRelease.a ../../asmjs/bin/libbimgRelease.a ../../asmjs/bin/libbxRelease.a
  LDDEPS             += ../../asmjs/bin/libexample-glueRelease.a ../../asmjs/bin/libexample-commonRelease.a ../../asmjs/bin/libbgfxRelease.a ../../asmjs/bin/libbimg_decodeRelease.a ../../asmjs/bin/libbimgRelease.a ../../asmjs/bin/libbxRelease.a
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(CXX) -o $(TARGET) $(LINKOBJS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/examples/32-particles/particles.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
	@echo Running post-build commands
	$(SILENT) echo Running asmjs finalize.
	$(SILENT) "$(EMSDK)/fastcomp/bin/emcc" -O2 -s PRECISE_F32=1 -s TOTAL_MEMORY=268435456 --memory-init-file 1 "$(TARGET)" -o "$(TARGET)".html 
  endef
endif

OBJDIRS := \
	$(OBJDIR) \
	$(OBJDIR)/examples/32-particles \

RESOURCES := \

.PHONY: clean prebuild prelink

all: $(OBJDIRS) $(TARGETDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LIBDEPS) $(EXTERNAL_LIBS) $(RESOURCES) $(OBJRESP) $(LDRESP) | $(TARGETDIR) $(OBJDIRS)
	@echo Linking example-32-particles
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
	-$(call MKDIR,$(TARGETDIR))

$(OBJDIRS):
	@echo Creating $(@)
	-$(call MKDIR,$@)

clean:
	@echo Cleaning example-32-particles
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(GCH): $(PCH) $(MAKEFILE) | $(OBJDIR)
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) -x c++-header $(DEFINES) $(INCLUDES) -o "$@" -c "$<"

$(GCH_OBJC): $(PCH) $(MAKEFILE) | $(OBJDIR)
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_OBJCPPFLAGS) -x objective-c++-header $(DEFINES) $(INCLUDES) -o "$@" -c "$<"
endif

ifneq (,$(OBJRESP))
$(OBJRESP): $(OBJECTS) | $(TARGETDIR) $(OBJDIRS)
	$(SILENT) echo $^
	$(SILENT) echo $^ > $@
endif

ifneq (,$(LDRESP))
$(LDRESP): $(LDDEPS) | $(TARGETDIR) $(OBJDIRS)
	$(SILENT) echo $^
	$(SILENT) echo $^ > $@
endif

$(OBJDIR)/examples/32-particles/particles.o: ../../../examples/32-particles/particles.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/examples/32-particles
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(OBJDIR)/$(notdir $(PCH)).d
  -include $(OBJDIR)/$(notdir $(PCH))_objc.d
endif
