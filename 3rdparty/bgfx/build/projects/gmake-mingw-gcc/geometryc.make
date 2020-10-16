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

MAKEFILE = geometryc.make

ifeq ($(config),debug32)
  OBJDIR              = ../../win32_mingw-gcc/obj/x32/Debug/geometryc
  TARGETDIR           = ../../win32_mingw-gcc/bin
  TARGET              = $(TARGETDIR)/geometrycDebug.exe
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DWIN32 -DMINGW_HAS_SECURE_API=1
  INCLUDES           += -I"../../../../bx/include/compat/mingw" -I"../../../../bx/include" -I"../../../include" -I"../../../3rdparty" -I"../../../examples/common"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/win32_mingw-gcc" -L"../../win32_mingw-gcc/bin" -L"." -m32 -Wl,--gc-sections -static -static-libgcc -static-libstdc++
  LIBDEPS            += ../../win32_mingw-gcc/bin/libbxDebug.a
  LDDEPS             += ../../win32_mingw-gcc/bin/libbxDebug.a
  LDRESP              =
  LIBS               += $(LDDEPS) -lpsapi
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(CXX) -o $(TARGET) $(LINKOBJS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/3rdparty/meshoptimizer/src/allocator.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/clusterizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/indexcodec.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/indexgenerator.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawoptimizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/simplifier.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/spatialorder.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/stripifier.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheoptimizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vertexcodec.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchoptimizer.o \
	$(OBJDIR)/examples/common/bounds.o \
	$(OBJDIR)/src/vertexdecl.o \
	$(OBJDIR)/tools/geometryc/geometryc.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release32)
  OBJDIR              = ../../win32_mingw-gcc/obj/x32/Release/geometryc
  TARGETDIR           = ../../win32_mingw-gcc/bin
  TARGET              = $(TARGETDIR)/geometrycRelease.exe
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG -DWIN32 -DMINGW_HAS_SECURE_API=1
  INCLUDES           += -I"../../../../bx/include/compat/mingw" -I"../../../../bx/include" -I"../../../include" -I"../../../3rdparty" -I"../../../examples/common"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m32 -mstackrealign
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/win32_mingw-gcc" -L"../../win32_mingw-gcc/bin" -L"." -m32 -Wl,--gc-sections -static -static-libgcc -static-libstdc++
  LIBDEPS            += ../../win32_mingw-gcc/bin/libbxRelease.a
  LDDEPS             += ../../win32_mingw-gcc/bin/libbxRelease.a
  LDRESP              =
  LIBS               += $(LDDEPS) -lpsapi
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(CXX) -o $(TARGET) $(LINKOBJS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/3rdparty/meshoptimizer/src/allocator.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/clusterizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/indexcodec.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/indexgenerator.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawoptimizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/simplifier.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/spatialorder.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/stripifier.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheoptimizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vertexcodec.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchoptimizer.o \
	$(OBJDIR)/examples/common/bounds.o \
	$(OBJDIR)/src/vertexdecl.o \
	$(OBJDIR)/tools/geometryc/geometryc.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
	@echo Running post-build commands
	$(SILENT) echo Stripping symbols.
	$(SILENT) $(MINGW)/bin/strip -s "$(TARGET)"
  endef
endif

ifeq ($(config),debug64)
  OBJDIR              = ../../win64_mingw-gcc/obj/x64/Debug/geometryc
  TARGETDIR           = ../../win64_mingw-gcc/bin
  TARGET              = $(TARGETDIR)/geometrycDebug.exe
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DWIN32 -DMINGW_HAS_SECURE_API=1
  INCLUDES           += -I"../../../../bx/include/compat/mingw" -I"../../../../bx/include" -I"../../../include" -I"../../../3rdparty" -I"../../../examples/common"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/win64_mingw-gcc" -L"../../win64_mingw-gcc/bin" -L"." -m64 -Wl,--gc-sections -static -static-libgcc -static-libstdc++
  LIBDEPS            += ../../win64_mingw-gcc/bin/libbxDebug.a
  LDDEPS             += ../../win64_mingw-gcc/bin/libbxDebug.a
  LDRESP              =
  LIBS               += $(LDDEPS) -lpsapi
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(CXX) -o $(TARGET) $(LINKOBJS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/3rdparty/meshoptimizer/src/allocator.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/clusterizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/indexcodec.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/indexgenerator.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawoptimizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/simplifier.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/spatialorder.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/stripifier.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheoptimizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vertexcodec.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchoptimizer.o \
	$(OBJDIR)/examples/common/bounds.o \
	$(OBJDIR)/src/vertexdecl.o \
	$(OBJDIR)/tools/geometryc/geometryc.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release64)
  OBJDIR              = ../../win64_mingw-gcc/obj/x64/Release/geometryc
  TARGETDIR           = ../../win64_mingw-gcc/bin
  TARGET              = $(TARGETDIR)/geometrycRelease.exe
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG -DWIN32 -DMINGW_HAS_SECURE_API=1
  INCLUDES           += -I"../../../../bx/include/compat/mingw" -I"../../../../bx/include" -I"../../../include" -I"../../../3rdparty" -I"../../../examples/common"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef -m64
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/win64_mingw-gcc" -L"../../win64_mingw-gcc/bin" -L"." -m64 -Wl,--gc-sections -static -static-libgcc -static-libstdc++
  LIBDEPS            += ../../win64_mingw-gcc/bin/libbxRelease.a
  LDDEPS             += ../../win64_mingw-gcc/bin/libbxRelease.a
  LDRESP              =
  LIBS               += $(LDDEPS) -lpsapi
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(CXX) -o $(TARGET) $(LINKOBJS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/3rdparty/meshoptimizer/src/allocator.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/clusterizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/indexcodec.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/indexgenerator.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawoptimizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/simplifier.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/spatialorder.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/stripifier.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheoptimizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vertexcodec.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchoptimizer.o \
	$(OBJDIR)/examples/common/bounds.o \
	$(OBJDIR)/src/vertexdecl.o \
	$(OBJDIR)/tools/geometryc/geometryc.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
	@echo Running post-build commands
	$(SILENT) echo Stripping symbols.
	$(SILENT) $(MINGW)/bin/strip -s "$(TARGET)"
  endef
endif

ifeq ($(config),debug)
  OBJDIR              = obj/Debug/geometryc
  TARGETDIR           = ../../../scripts
  TARGET              = $(TARGETDIR)/geometrycDebug.exe
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DWIN32 -DMINGW_HAS_SECURE_API=1
  INCLUDES           += -I"../../../../bx/include/compat/mingw" -I"../../../../bx/include" -I"../../../include" -I"../../../3rdparty" -I"../../../examples/common"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../../bx/scripts" -L"." -Wl,--gc-sections -static -static-libgcc -static-libstdc++
  LIBDEPS            += ../../../../bx/scripts/libbxDebug.a
  LDDEPS             += ../../../../bx/scripts/libbxDebug.a
  LDRESP              =
  LIBS               += $(LDDEPS) -lpsapi
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(CXX) -o $(TARGET) $(LINKOBJS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/3rdparty/meshoptimizer/src/allocator.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/clusterizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/indexcodec.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/indexgenerator.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawoptimizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/simplifier.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/spatialorder.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/stripifier.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheoptimizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vertexcodec.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchoptimizer.o \
	$(OBJDIR)/examples/common/bounds.o \
	$(OBJDIR)/src/vertexdecl.o \
	$(OBJDIR)/tools/geometryc/geometryc.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release)
  OBJDIR              = obj/Release/geometryc
  TARGETDIR           = ../../../scripts
  TARGET              = $(TARGETDIR)/geometrycRelease.exe
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG -DWIN32 -DMINGW_HAS_SECURE_API=1
  INCLUDES           += -I"../../../../bx/include/compat/mingw" -I"../../../../bx/include" -I"../../../include" -I"../../../3rdparty" -I"../../../examples/common"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -std=c++14 -fno-rtti -fno-exceptions -Wshadow -Wunused-value -fdata-sections -ffunction-sections -msse2 -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../../bx/scripts" -L"." -Wl,--gc-sections -static -static-libgcc -static-libstdc++
  LIBDEPS            += ../../../../bx/scripts/libbxRelease.a
  LDDEPS             += ../../../../bx/scripts/libbxRelease.a
  LDRESP              =
  LIBS               += $(LDDEPS) -lpsapi
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(CXX) -o $(TARGET) $(LINKOBJS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/3rdparty/meshoptimizer/src/allocator.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/clusterizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/indexcodec.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/indexgenerator.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawoptimizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/simplifier.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/spatialorder.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/stripifier.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheoptimizer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vertexcodec.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchanalyzer.o \
	$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchoptimizer.o \
	$(OBJDIR)/examples/common/bounds.o \
	$(OBJDIR)/src/vertexdecl.o \
	$(OBJDIR)/tools/geometryc/geometryc.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
	@echo Running post-build commands
	$(SILENT) echo Stripping symbols.
	$(SILENT) $(MINGW)/bin/strip -s "$(TARGET)"
  endef
endif

OBJDIRS := \
	$(OBJDIR) \
	$(OBJDIR)/3rdparty/meshoptimizer/src \
	$(OBJDIR)/examples/common \
	$(OBJDIR)/src \
	$(OBJDIR)/tools/geometryc \

RESOURCES := \

.PHONY: clean prebuild prelink

all: $(OBJDIRS) $(TARGETDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LIBDEPS) $(EXTERNAL_LIBS) $(RESOURCES) $(OBJRESP) $(LDRESP) | $(TARGETDIR) $(OBJDIRS)
	@echo Linking geometryc
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
	-$(call MKDIR,$(TARGETDIR))

$(OBJDIRS):
	@echo Creating $(@)
	-$(call MKDIR,$@)

clean:
	@echo Cleaning geometryc
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

$(OBJDIR)/3rdparty/meshoptimizer/src/allocator.o: ../../../3rdparty/meshoptimizer/src/allocator.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/meshoptimizer/src/clusterizer.o: ../../../3rdparty/meshoptimizer/src/clusterizer.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/meshoptimizer/src/indexcodec.o: ../../../3rdparty/meshoptimizer/src/indexcodec.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/meshoptimizer/src/indexgenerator.o: ../../../3rdparty/meshoptimizer/src/indexgenerator.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawanalyzer.o: ../../../3rdparty/meshoptimizer/src/overdrawanalyzer.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/meshoptimizer/src/overdrawoptimizer.o: ../../../3rdparty/meshoptimizer/src/overdrawoptimizer.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/meshoptimizer/src/simplifier.o: ../../../3rdparty/meshoptimizer/src/simplifier.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/meshoptimizer/src/spatialorder.o: ../../../3rdparty/meshoptimizer/src/spatialorder.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/meshoptimizer/src/stripifier.o: ../../../3rdparty/meshoptimizer/src/stripifier.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheanalyzer.o: ../../../3rdparty/meshoptimizer/src/vcacheanalyzer.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/meshoptimizer/src/vcacheoptimizer.o: ../../../3rdparty/meshoptimizer/src/vcacheoptimizer.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/meshoptimizer/src/vertexcodec.o: ../../../3rdparty/meshoptimizer/src/vertexcodec.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchanalyzer.o: ../../../3rdparty/meshoptimizer/src/vfetchanalyzer.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/3rdparty/meshoptimizer/src/vfetchoptimizer.o: ../../../3rdparty/meshoptimizer/src/vfetchoptimizer.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/3rdparty/meshoptimizer/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/examples/common/bounds.o: ../../../examples/common/bounds.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/examples/common
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/vertexdecl.o: ../../../src/vertexdecl.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/tools/geometryc/geometryc.o: ../../../tools/geometryc/geometryc.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/tools/geometryc
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(OBJDIR)/$(notdir $(PCH)).d
  -include $(OBJDIR)/$(notdir $(PCH))_objc.d
endif
