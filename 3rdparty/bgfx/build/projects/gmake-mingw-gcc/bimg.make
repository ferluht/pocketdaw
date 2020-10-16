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

CC  = $(MINGW)/bin/x86_64-w64-mingw32-gcc
CXX = $(MINGW)/bin/x86_64-w64-mingw32-g++
AR  = $(MINGW)/bin/ar

ifndef RESCOMP
  ifdef WINDRES
    RESCOMP = $(WINDRES)
  else
    RESCOMP = windres
  endif
endif

MAKEFILE = bimg.make

ifeq ($(config),debug32)
  OBJDIR              = ../../win32_mingw-gcc/obj/x32/Debug/bimg
  TARGETDIR           = ../../win32_mingw-gcc/bin
  TARGET              = $(TARGETDIR)/libbimgDebug.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DWIN32 -DMINGW_HAS_SECURE_API=1
  INCLUDES           += -I"../../../../bx/include/compat/mingw" -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../../bimg/3rdparty/astc-codec" -I"../../../../bimg/3rdparty/astc-codec/include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/win32_mingw-gcc" -m32 -Wl,--gc-sections -static -static-libgcc -static-libstdc++
  LIBDEPS            +=
  LDDEPS             +=
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  -rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/astc_file.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/endpoint_codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/footprint.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/integer_sequence_codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/intermediate_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/logical_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/partition.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/physical_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/quantization.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/weight_infill.o \
	$(OBJDIR)/bimg/src/image.o \
	$(OBJDIR)/bimg/src/image_gnf.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release32)
  OBJDIR              = ../../win32_mingw-gcc/obj/x32/Release/bimg
  TARGETDIR           = ../../win32_mingw-gcc/bin
  TARGET              = $(TARGETDIR)/libbimgRelease.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG -DWIN32 -DMINGW_HAS_SECURE_API=1
  INCLUDES           += -I"../../../../bx/include/compat/mingw" -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../../bimg/3rdparty/astc-codec" -I"../../../../bimg/3rdparty/astc-codec/include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/win32_mingw-gcc" -m32 -Wl,--gc-sections -static -static-libgcc -static-libstdc++
  LIBDEPS            +=
  LDDEPS             +=
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  -rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/astc_file.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/endpoint_codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/footprint.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/integer_sequence_codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/intermediate_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/logical_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/partition.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/physical_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/quantization.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/weight_infill.o \
	$(OBJDIR)/bimg/src/image.o \
	$(OBJDIR)/bimg/src/image_gnf.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),debug64)
  OBJDIR              = ../../win64_mingw-gcc/obj/x64/Debug/bimg
  TARGETDIR           = ../../win64_mingw-gcc/bin
  TARGET              = $(TARGETDIR)/libbimgDebug.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DWIN32 -DMINGW_HAS_SECURE_API=1
  INCLUDES           += -I"../../../../bx/include/compat/mingw" -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../../bimg/3rdparty/astc-codec" -I"../../../../bimg/3rdparty/astc-codec/include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/win64_mingw-gcc" -m64 -Wl,--gc-sections -static -static-libgcc -static-libstdc++
  LIBDEPS            +=
  LDDEPS             +=
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  -rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/astc_file.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/endpoint_codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/footprint.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/integer_sequence_codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/intermediate_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/logical_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/partition.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/physical_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/quantization.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/weight_infill.o \
	$(OBJDIR)/bimg/src/image.o \
	$(OBJDIR)/bimg/src/image_gnf.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release64)
  OBJDIR              = ../../win64_mingw-gcc/obj/x64/Release/bimg
  TARGETDIR           = ../../win64_mingw-gcc/bin
  TARGET              = $(TARGETDIR)/libbimgRelease.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG -DWIN32 -DMINGW_HAS_SECURE_API=1
  INCLUDES           += -I"../../../../bx/include/compat/mingw" -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../../bimg/3rdparty/astc-codec" -I"../../../../bimg/3rdparty/astc-codec/include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/win64_mingw-gcc" -m64 -Wl,--gc-sections -static -static-libgcc -static-libstdc++
  LIBDEPS            +=
  LDDEPS             +=
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  -rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/astc_file.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/endpoint_codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/footprint.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/integer_sequence_codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/intermediate_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/logical_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/partition.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/physical_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/quantization.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/weight_infill.o \
	$(OBJDIR)/bimg/src/image.o \
	$(OBJDIR)/bimg/src/image_gnf.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),debug)
  OBJDIR              = obj/Debug/bimg
  TARGETDIR           = ../../../../bimg/scripts
  TARGET              = $(TARGETDIR)/libbimgDebug.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DWIN32 -DMINGW_HAS_SECURE_API=1
  INCLUDES           += -I"../../../../bx/include/compat/mingw" -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../../bimg/3rdparty/astc-codec" -I"../../../../bimg/3rdparty/astc-codec/include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -Wl,--gc-sections -static -static-libgcc -static-libstdc++
  LIBDEPS            +=
  LDDEPS             +=
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  -rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/astc_file.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/endpoint_codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/footprint.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/integer_sequence_codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/intermediate_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/logical_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/partition.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/physical_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/quantization.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/weight_infill.o \
	$(OBJDIR)/bimg/src/image.o \
	$(OBJDIR)/bimg/src/image_gnf.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release)
  OBJDIR              = obj/Release/bimg
  TARGETDIR           = ../../../../bimg/scripts
  TARGET              = $(TARGETDIR)/libbimgRelease.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG -DWIN32 -DMINGW_HAS_SECURE_API=1
  INCLUDES           += -I"../../../../bx/include/compat/mingw" -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../../bimg/3rdparty/astc-codec" -I"../../../../bimg/3rdparty/astc-codec/include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -Wl,--gc-sections -static -static-libgcc -static-libstdc++
  LIBDEPS            +=
  LDDEPS             +=
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  -rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/astc_file.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/endpoint_codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/footprint.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/integer_sequence_codec.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/intermediate_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/logical_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/partition.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/physical_astc_block.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/quantization.o \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/weight_infill.o \
	$(OBJDIR)/bimg/src/image.o \
	$(OBJDIR)/bimg/src/image_gnf.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

OBJDIRS := \
	$(OBJDIR) \
	$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder \
	$(OBJDIR)/bimg/src \

RESOURCES := \

.PHONY: clean prebuild prelink

all: $(OBJDIRS) $(TARGETDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LIBDEPS) $(EXTERNAL_LIBS) $(RESOURCES) $(OBJRESP) $(LDRESP) | $(TARGETDIR) $(OBJDIRS)
	@echo Archiving bimg
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
	@echo Cleaning bimg
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

$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/astc_file.o: ../../../../bimg/3rdparty/astc-codec/src/decoder/astc_file.cc $(GCH) $(MAKEFILE) | $(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/codec.o: ../../../../bimg/3rdparty/astc-codec/src/decoder/codec.cc $(GCH) $(MAKEFILE) | $(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/endpoint_codec.o: ../../../../bimg/3rdparty/astc-codec/src/decoder/endpoint_codec.cc $(GCH) $(MAKEFILE) | $(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/footprint.o: ../../../../bimg/3rdparty/astc-codec/src/decoder/footprint.cc $(GCH) $(MAKEFILE) | $(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/integer_sequence_codec.o: ../../../../bimg/3rdparty/astc-codec/src/decoder/integer_sequence_codec.cc $(GCH) $(MAKEFILE) | $(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/intermediate_astc_block.o: ../../../../bimg/3rdparty/astc-codec/src/decoder/intermediate_astc_block.cc $(GCH) $(MAKEFILE) | $(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/logical_astc_block.o: ../../../../bimg/3rdparty/astc-codec/src/decoder/logical_astc_block.cc $(GCH) $(MAKEFILE) | $(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/partition.o: ../../../../bimg/3rdparty/astc-codec/src/decoder/partition.cc $(GCH) $(MAKEFILE) | $(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/physical_astc_block.o: ../../../../bimg/3rdparty/astc-codec/src/decoder/physical_astc_block.cc $(GCH) $(MAKEFILE) | $(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/quantization.o: ../../../../bimg/3rdparty/astc-codec/src/decoder/quantization.cc $(GCH) $(MAKEFILE) | $(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder/weight_infill.o: ../../../../bimg/3rdparty/astc-codec/src/decoder/weight_infill.cc $(GCH) $(MAKEFILE) | $(OBJDIR)/bimg/3rdparty/astc-codec/src/decoder
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/bimg/src/image.o: ../../../../bimg/src/image.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/bimg/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/bimg/src/image_gnf.o: ../../../../bimg/src/image_gnf.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/bimg/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(OBJDIR)/$(notdir $(PCH)).d
  -include $(OBJDIR)/$(notdir $(PCH))_objc.d
endif
