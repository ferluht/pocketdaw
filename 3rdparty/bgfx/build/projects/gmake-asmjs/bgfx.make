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

MAKEFILE = bgfx.make

ifeq ($(config),debug32)
  OBJDIR              = ../../asmjs/obj/x32/Debug/bgfx
  TARGETDIR           = ../../asmjs/bin
  TARGET              = $(TARGETDIR)/libbgfxDebug.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DBGFX_CONFIG_DEBUG=1
  INCLUDES           += -I"../../../3rdparty" -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../3rdparty/khronos" -I"../../../include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m32 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/asmjs" -L"../../asmjs/bin" -m32
  LIBDEPS            += ../../asmjs/bin/libbxDebug.a
  LDDEPS             += ../../asmjs/bin/libbxDebug.a
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/src/bgfx.o \
	$(OBJDIR)/src/debug_renderdoc.o \
	$(OBJDIR)/src/dxgi.o \
	$(OBJDIR)/src/glcontext_egl.o \
	$(OBJDIR)/src/glcontext_glx.o \
	$(OBJDIR)/src/glcontext_html5.o \
	$(OBJDIR)/src/glcontext_wgl.o \
	$(OBJDIR)/src/nvapi.o \
	$(OBJDIR)/src/renderer_d3d11.o \
	$(OBJDIR)/src/renderer_d3d12.o \
	$(OBJDIR)/src/renderer_d3d9.o \
	$(OBJDIR)/src/renderer_gl.o \
	$(OBJDIR)/src/renderer_gnm.o \
	$(OBJDIR)/src/renderer_noop.o \
	$(OBJDIR)/src/renderer_nvn.o \
	$(OBJDIR)/src/renderer_vk.o \
	$(OBJDIR)/src/shader.o \
	$(OBJDIR)/src/shader_dx9bc.o \
	$(OBJDIR)/src/shader_dxbc.o \
	$(OBJDIR)/src/shader_spirv.o \
	$(OBJDIR)/src/topology.o \
	$(OBJDIR)/src/vertexdecl.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release32)
  OBJDIR              = ../../asmjs/obj/x32/Release/bgfx
  TARGETDIR           = ../../asmjs/bin
  TARGET              = $(TARGETDIR)/libbgfxRelease.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG
  INCLUDES           += -I"../../../3rdparty" -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../3rdparty/khronos" -I"../../../include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m32 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/asmjs" -L"../../asmjs/bin" -m32
  LIBDEPS            += ../../asmjs/bin/libbxRelease.a
  LDDEPS             += ../../asmjs/bin/libbxRelease.a
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/src/bgfx.o \
	$(OBJDIR)/src/debug_renderdoc.o \
	$(OBJDIR)/src/dxgi.o \
	$(OBJDIR)/src/glcontext_egl.o \
	$(OBJDIR)/src/glcontext_glx.o \
	$(OBJDIR)/src/glcontext_html5.o \
	$(OBJDIR)/src/glcontext_wgl.o \
	$(OBJDIR)/src/nvapi.o \
	$(OBJDIR)/src/renderer_d3d11.o \
	$(OBJDIR)/src/renderer_d3d12.o \
	$(OBJDIR)/src/renderer_d3d9.o \
	$(OBJDIR)/src/renderer_gl.o \
	$(OBJDIR)/src/renderer_gnm.o \
	$(OBJDIR)/src/renderer_noop.o \
	$(OBJDIR)/src/renderer_nvn.o \
	$(OBJDIR)/src/renderer_vk.o \
	$(OBJDIR)/src/shader.o \
	$(OBJDIR)/src/shader_dx9bc.o \
	$(OBJDIR)/src/shader_dxbc.o \
	$(OBJDIR)/src/shader_spirv.o \
	$(OBJDIR)/src/topology.o \
	$(OBJDIR)/src/vertexdecl.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),debug64)
  OBJDIR              = ../../asmjs/obj/x64/Debug/bgfx
  TARGETDIR           = ../../asmjs/bin
  TARGET              = $(TARGETDIR)/libbgfxDebug.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DBGFX_CONFIG_DEBUG=1
  INCLUDES           += -I"../../../3rdparty" -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../3rdparty/khronos" -I"../../../include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -m64 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/asmjs" -L"../../asmjs/bin" -m64
  LIBDEPS            += ../../asmjs/bin/libbxDebug.a
  LDDEPS             += ../../asmjs/bin/libbxDebug.a
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/src/bgfx.o \
	$(OBJDIR)/src/debug_renderdoc.o \
	$(OBJDIR)/src/dxgi.o \
	$(OBJDIR)/src/glcontext_egl.o \
	$(OBJDIR)/src/glcontext_glx.o \
	$(OBJDIR)/src/glcontext_html5.o \
	$(OBJDIR)/src/glcontext_wgl.o \
	$(OBJDIR)/src/nvapi.o \
	$(OBJDIR)/src/renderer_d3d11.o \
	$(OBJDIR)/src/renderer_d3d12.o \
	$(OBJDIR)/src/renderer_d3d9.o \
	$(OBJDIR)/src/renderer_gl.o \
	$(OBJDIR)/src/renderer_gnm.o \
	$(OBJDIR)/src/renderer_noop.o \
	$(OBJDIR)/src/renderer_nvn.o \
	$(OBJDIR)/src/renderer_vk.o \
	$(OBJDIR)/src/shader.o \
	$(OBJDIR)/src/shader_dx9bc.o \
	$(OBJDIR)/src/shader_dxbc.o \
	$(OBJDIR)/src/shader_spirv.o \
	$(OBJDIR)/src/topology.o \
	$(OBJDIR)/src/vertexdecl.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release64)
  OBJDIR              = ../../asmjs/obj/x64/Release/bgfx
  TARGETDIR           = ../../asmjs/bin
  TARGET              = $(TARGETDIR)/libbgfxRelease.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG
  INCLUDES           += -I"../../../3rdparty" -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../3rdparty/khronos" -I"../../../include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -m64 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/asmjs" -L"../../asmjs/bin" -m64
  LIBDEPS            += ../../asmjs/bin/libbxRelease.a
  LDDEPS             += ../../asmjs/bin/libbxRelease.a
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/src/bgfx.o \
	$(OBJDIR)/src/debug_renderdoc.o \
	$(OBJDIR)/src/dxgi.o \
	$(OBJDIR)/src/glcontext_egl.o \
	$(OBJDIR)/src/glcontext_glx.o \
	$(OBJDIR)/src/glcontext_html5.o \
	$(OBJDIR)/src/glcontext_wgl.o \
	$(OBJDIR)/src/nvapi.o \
	$(OBJDIR)/src/renderer_d3d11.o \
	$(OBJDIR)/src/renderer_d3d12.o \
	$(OBJDIR)/src/renderer_d3d9.o \
	$(OBJDIR)/src/renderer_gl.o \
	$(OBJDIR)/src/renderer_gnm.o \
	$(OBJDIR)/src/renderer_noop.o \
	$(OBJDIR)/src/renderer_nvn.o \
	$(OBJDIR)/src/renderer_vk.o \
	$(OBJDIR)/src/shader.o \
	$(OBJDIR)/src/shader_dx9bc.o \
	$(OBJDIR)/src/shader_dxbc.o \
	$(OBJDIR)/src/shader_spirv.o \
	$(OBJDIR)/src/topology.o \
	$(OBJDIR)/src/vertexdecl.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),debug)
  OBJDIR              = ../../asmjs/obj/Debug/bgfx
  TARGETDIR           = ../../asmjs/bin
  TARGET              = $(TARGETDIR)/libbgfxDebug.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_DEBUG -DBGFX_CONFIG_DEBUG=1
  INCLUDES           += -I"../../../3rdparty" -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../3rdparty/khronos" -I"../../../include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/asmjs" -L"../../asmjs/bin"
  LIBDEPS            += ../../asmjs/bin/libbxDebug.a
  LDDEPS             += ../../asmjs/bin/libbxDebug.a
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/src/bgfx.o \
	$(OBJDIR)/src/debug_renderdoc.o \
	$(OBJDIR)/src/dxgi.o \
	$(OBJDIR)/src/glcontext_egl.o \
	$(OBJDIR)/src/glcontext_glx.o \
	$(OBJDIR)/src/glcontext_html5.o \
	$(OBJDIR)/src/glcontext_wgl.o \
	$(OBJDIR)/src/nvapi.o \
	$(OBJDIR)/src/renderer_d3d11.o \
	$(OBJDIR)/src/renderer_d3d12.o \
	$(OBJDIR)/src/renderer_d3d9.o \
	$(OBJDIR)/src/renderer_gl.o \
	$(OBJDIR)/src/renderer_gnm.o \
	$(OBJDIR)/src/renderer_noop.o \
	$(OBJDIR)/src/renderer_nvn.o \
	$(OBJDIR)/src/renderer_vk.o \
	$(OBJDIR)/src/shader.o \
	$(OBJDIR)/src/shader_dx9bc.o \
	$(OBJDIR)/src/shader_dxbc.o \
	$(OBJDIR)/src/shader_spirv.o \
	$(OBJDIR)/src/topology.o \
	$(OBJDIR)/src/vertexdecl.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release)
  OBJDIR              = ../../asmjs/obj/Release/bgfx
  TARGETDIR           = ../../asmjs/bin
  TARGET              = $(TARGETDIR)/libbgfxRelease.a
  DEFINES            += -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DNDEBUG
  INCLUDES           += -I"../../../3rdparty" -I"../../../../bx/include" -I"../../../../bimg/include" -I"../../../3rdparty/khronos" -I"../../../include"
  ALL_CPPFLAGS       += $(CPPFLAGS) -MMD -MP -MP $(DEFINES) $(INCLUDES)
  ALL_ASMFLAGS       += $(ASMFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CFLAGS         += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_CXXFLAGS       += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCFLAGS      += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_OBJCPPFLAGS    += $(CXXFLAGS) $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -Wall -Wextra -ffast-math -fomit-frame-pointer -g -O3 -std=c++14 -fno-rtti -fno-exceptions -isystem "$(EMSDK)/fastcomp/emscripten" -Wunused-value -Wundef
  ALL_RESFLAGS       += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS        += $(LDFLAGS) -L"../../../3rdparty/lib/asmjs" -L"../../asmjs/bin"
  LIBDEPS            += ../../asmjs/bin/libbxRelease.a
  LDDEPS             += ../../asmjs/bin/libbxRelease.a
  LDRESP              =
  LIBS               += $(LDDEPS)
  EXTERNAL_LIBS      +=
  LINKOBJS            = $(OBJECTS)
  LINKCMD             = $(AR)  rcs $(TARGET)
  OBJRESP             =
  OBJECTS := \
	$(OBJDIR)/src/bgfx.o \
	$(OBJDIR)/src/debug_renderdoc.o \
	$(OBJDIR)/src/dxgi.o \
	$(OBJDIR)/src/glcontext_egl.o \
	$(OBJDIR)/src/glcontext_glx.o \
	$(OBJDIR)/src/glcontext_html5.o \
	$(OBJDIR)/src/glcontext_wgl.o \
	$(OBJDIR)/src/nvapi.o \
	$(OBJDIR)/src/renderer_d3d11.o \
	$(OBJDIR)/src/renderer_d3d12.o \
	$(OBJDIR)/src/renderer_d3d9.o \
	$(OBJDIR)/src/renderer_gl.o \
	$(OBJDIR)/src/renderer_gnm.o \
	$(OBJDIR)/src/renderer_noop.o \
	$(OBJDIR)/src/renderer_nvn.o \
	$(OBJDIR)/src/renderer_vk.o \
	$(OBJDIR)/src/shader.o \
	$(OBJDIR)/src/shader_dx9bc.o \
	$(OBJDIR)/src/shader_dxbc.o \
	$(OBJDIR)/src/shader_spirv.o \
	$(OBJDIR)/src/topology.o \
	$(OBJDIR)/src/vertexdecl.o \

  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

OBJDIRS := \
	$(OBJDIR) \
	$(OBJDIR)/src \

RESOURCES := \

.PHONY: clean prebuild prelink

all: $(OBJDIRS) $(TARGETDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LIBDEPS) $(EXTERNAL_LIBS) $(RESOURCES) $(OBJRESP) $(LDRESP) | $(TARGETDIR) $(OBJDIRS)
	@echo Archiving bgfx
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
	@echo Cleaning bgfx
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

$(OBJDIR)/src/bgfx.o: ../../../src/bgfx.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/debug_renderdoc.o: ../../../src/debug_renderdoc.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/dxgi.o: ../../../src/dxgi.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/glcontext_eagl.o: ../../../src/glcontext_eagl.mm $(GCH_OBJC) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_OBJCPPFLAGS) $(FORCE_INCLUDE_OBJC) -o "$@" -c "$<"

$(OBJDIR)/src/glcontext_egl.o: ../../../src/glcontext_egl.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/glcontext_glx.o: ../../../src/glcontext_glx.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/glcontext_html5.o: ../../../src/glcontext_html5.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/glcontext_nsgl.o: ../../../src/glcontext_nsgl.mm $(GCH_OBJC) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_OBJCPPFLAGS) $(FORCE_INCLUDE_OBJC) -o "$@" -c "$<"

$(OBJDIR)/src/glcontext_wgl.o: ../../../src/glcontext_wgl.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/nvapi.o: ../../../src/nvapi.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/renderer_d3d11.o: ../../../src/renderer_d3d11.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/renderer_d3d12.o: ../../../src/renderer_d3d12.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/renderer_d3d9.o: ../../../src/renderer_d3d9.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/renderer_gl.o: ../../../src/renderer_gl.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/renderer_gnm.o: ../../../src/renderer_gnm.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/renderer_mtl.o: ../../../src/renderer_mtl.mm $(GCH_OBJC) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_OBJCPPFLAGS) $(FORCE_INCLUDE_OBJC) -o "$@" -c "$<"

$(OBJDIR)/src/renderer_noop.o: ../../../src/renderer_noop.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/renderer_nvn.o: ../../../src/renderer_nvn.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/renderer_vk.o: ../../../src/renderer_vk.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/shader.o: ../../../src/shader.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/shader_dx9bc.o: ../../../src/shader_dx9bc.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/shader_dxbc.o: ../../../src/shader_dxbc.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/shader_spirv.o: ../../../src/shader_spirv.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/topology.o: ../../../src/topology.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

$(OBJDIR)/src/vertexdecl.o: ../../../src/vertexdecl.cpp $(GCH) $(MAKEFILE) | $(OBJDIR)/src
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(OBJDIR)/$(notdir $(PCH)).d
  -include $(OBJDIR)/$(notdir $(PCH))_objc.d
endif
