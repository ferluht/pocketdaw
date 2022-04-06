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

CC  = gcc
CXX = g++
AR  = ar

ifndef RESCOMP
  ifdef WINDRES
    RESCOMP = $(WINDRES)
  else
    RESCOMP = windres
  endif
endif

MAKEFILE = spirv-cross.make

ifeq ($(config),debug32)
  OBJDIR              = ../../linux32_gcc/obj/x32/Debug/spirv-cross
  TARGETDIR           = ../../linux32_gcc/bin
  TARGET              = $(TARGETDIR)/libspirv-crossDebug.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS
  INCLUDES           += -I"../../../3rdparty/spirv-cross/include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m32
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m32
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m32
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m32
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m32
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/linux32_gcc" -L"." -m32 -Wl,--gc-sections -Wl,--as-needed
  LIBDEPS            +=
  LDDEPS             +=
  LDRESP              =
  LIBS               += $(LDDEPS) -lrt -ldl
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  -rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cfg.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cpp.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_parsed_ir.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_util.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_glsl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_hlsl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_msl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_parser.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_reflect.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release32)
  OBJDIR              = ../../linux32_gcc/obj/x32/Release/spirv-cross
  TARGETDIR           = ../../linux32_gcc/bin
  TARGET              = $(TARGETDIR)/libspirv-crossRelease.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG -DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS
  INCLUDES           += -I"../../../3rdparty/spirv-cross/include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m32
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m32
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m32
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m32
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m32
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/linux32_gcc" -L"." -m32 -Wl,--gc-sections -Wl,--as-needed
  LIBDEPS            +=
  LDDEPS             +=
  LDRESP              =
  LIBS               += $(LDDEPS) -lrt -ldl
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  -rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cfg.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cpp.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_parsed_ir.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_util.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_glsl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_hlsl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_msl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_parser.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_reflect.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),debug64)
  OBJDIR              = ../../linux64_gcc/obj/x64/Debug/spirv-cross
  TARGETDIR           = ../../linux64_gcc/bin
  TARGET              = $(TARGETDIR)/libspirv-crossDebug.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS
  INCLUDES           += -I"../../../3rdparty/spirv-cross/include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m64
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m64
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m64
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m64
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m64
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/linux64_gcc" -L"." -m64 -Wl,--gc-sections -Wl,--as-needed
  LIBDEPS            +=
  LDDEPS             +=
  LDRESP              =
  LIBS               += $(LDDEPS) -lrt -ldl
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  -rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cfg.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cpp.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_parsed_ir.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_util.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_glsl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_hlsl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_msl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_parser.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_reflect.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release64)
  OBJDIR              = ../../linux64_gcc/obj/x64/Release/spirv-cross
  TARGETDIR           = ../../linux64_gcc/bin
  TARGET              = $(TARGETDIR)/libspirv-crossRelease.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG -DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS
  INCLUDES           += -I"../../../3rdparty/spirv-cross/include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m64
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m64
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m64
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m64
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op -m64
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/linux64_gcc" -L"." -m64 -Wl,--gc-sections -Wl,--as-needed
  LIBDEPS            +=
  LDDEPS             +=
  LDRESP              =
  LIBS               += $(LDDEPS) -lrt -ldl
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  -rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cfg.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cpp.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_parsed_ir.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_util.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_glsl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_hlsl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_msl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_parser.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_reflect.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),debug)
  OBJDIR              = obj/Debug/spirv-cross
  TARGETDIR           = ../../../scripts
  TARGET              = $(TARGETDIR)/libspirv-crossDebug.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS
  INCLUDES           += -I"../../../3rdparty/spirv-cross/include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -std=c++14 -fno-rtti -fno-exceptions -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -std=c++14 -fno-rtti -fno-exceptions -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"." -Wl,--gc-sections -Wl,--as-needed
  LIBDEPS            +=
  LDDEPS             +=
  LDRESP              =
  LIBS               += $(LDDEPS) -lrt -ldl
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  -rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cfg.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cpp.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_parsed_ir.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_util.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_glsl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_hlsl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_msl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_parser.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_reflect.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release)
  OBJDIR              = obj/Release/spirv-cross
  TARGETDIR           = ../../../scripts
  TARGET              = $(TARGETDIR)/libspirv-crossRelease.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG -DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS
  INCLUDES           += -I"../../../3rdparty/spirv-cross/include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -mfpmath=sse -msse2 -Wunused-value -Wundef -Wlogical-op
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"." -Wl,--gc-sections -Wl,--as-needed
  LIBDEPS            +=
  LDDEPS             +=
  LDRESP              =
  LIBS               += $(LDDEPS) -lrt -ldl
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  -rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cfg.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cpp.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_parsed_ir.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_util.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_glsl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_hlsl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_msl.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_parser.o \
	$(OBJDIR)/3rdparty/spirv-cross/spirv_reflect.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

OBJDIRS := \
	$(OBJDIR) \
	$(OBJDIR)/3rdparty/spirv-cross \

RESOURCES := \

.PHONY: clean prebuild prelink

all: $(OBJDIRS) $(TARGETDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LIBDEPS) $(EXTERNAL_LIBS) $(RESOURCES) $(OBJRESP) $(LDRESP) | $(TARGETDIR) $(OBJDIRS)
	@echo Archiving spirv-cross
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
endif
	$(SILENT) $(LINKCMD) $(LINKOBJS) 2>&1 > /dev/null | sed -e '/.o) has no symbols$$/d'
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
	-$(call MKDIR,$(TARGETDIR))

$(OBJDIRS):
	@echo Creating $(@)
	-$(call MKDIR,$@)

clean:
	@echo Cleaning spirv-cross
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

$(OBJDIR)/3rdparty/spirv-cross/spirv_cfg.o: ../../../3rdparty/spirv-cross/spirv_cfg.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/spirv-cross
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/spirv-cross/spirv_cpp.o: ../../../3rdparty/spirv-cross/spirv_cpp.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/spirv-cross
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/spirv-cross/spirv_cross.o: ../../../3rdparty/spirv-cross/spirv_cross.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/spirv-cross
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_parsed_ir.o: ../../../3rdparty/spirv-cross/spirv_cross_parsed_ir.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/spirv-cross
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/spirv-cross/spirv_cross_util.o: ../../../3rdparty/spirv-cross/spirv_cross_util.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/spirv-cross
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/spirv-cross/spirv_glsl.o: ../../../3rdparty/spirv-cross/spirv_glsl.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/spirv-cross
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/spirv-cross/spirv_hlsl.o: ../../../3rdparty/spirv-cross/spirv_hlsl.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/spirv-cross
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/spirv-cross/spirv_msl.o: ../../../3rdparty/spirv-cross/spirv_msl.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/spirv-cross
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/spirv-cross/spirv_parser.o: ../../../3rdparty/spirv-cross/spirv_parser.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/spirv-cross
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/spirv-cross/spirv_reflect.o: ../../../3rdparty/spirv-cross/spirv_reflect.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/spirv-cross
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(OBJDIR)/$(notdir $(PCH)).d
  -include $(OBJDIR)/$(notdir $(PCH))_objc.d
endif