# Global CFLAGS
AM_CFLAGS= -Wall -Wno-unknown-pragmas
AM_CXXFLAGS= $(AM_CFLAGS)

#
# Stuff to install (make install not working yet as there's no reason
# to install rstation to a system yet..)
#
TestSuite = rs-test-threads

noinst_PROGRAMS = rstation $(TestSuite)
noinst_LIBRARIES = libglfw3.a libslb.a libgl3w.a libglsw.a

#
# Submodule of glfw3 (no packaged release available)
#  - git source can be updated with 'git submodule update'
#  - cmake extern/glfw3/ must be run before trying to make rstation
#
libglfw3_a_SOURCES = \
	extern/glfw3/src/enable.c \
	extern/glfw3/src/error.c \
	extern/glfw3/src/fullscreen.c \
	extern/glfw3/src/gamma.c \
	extern/glfw3/src/opengl.c \
	extern/glfw3/src/init.c \
	extern/glfw3/src/input.c \
	extern/glfw3/src/joystick.c \
	extern/glfw3/src/time.c \
	extern/glfw3/src/window.c \
	extern/glfw3/src/x11_fullscreen.c \
	extern/glfw3/src/x11_enable.c \
	extern/glfw3/src/x11_gamma.c \
	extern/glfw3/src/x11_opengl.c \
	extern/glfw3/src/x11_init.c \
	extern/glfw3/src/x11_joystick.c \
	extern/glfw3/src/x11_keysym2unicode.c \
	extern/glfw3/src/x11_time.c \
	extern/glfw3/src/x11_window.c

libglfw3_a_CPPFLAGS = -Iextern/glfw3/include -Iextern/glfw3/src

libgl3w_a_CPPFLAGS = -Iextern/gl3w/include -lGL -lX11
libgl3w_a_SOURCES = extern/gl3w/src/gl3w.c

#
# Bundled SLB
#
libslb_a_SOURCES = \
	extern/slb-1.63/src/Error.cpp \
	extern/slb-1.63/src/Hybrid.cpp \
	extern/slb-1.63/src/Object.cpp \
	extern/slb-1.63/src/LuaCall.cpp \
	extern/slb-1.63/src/Manager.cpp \
	extern/slb-1.63/src/Debug.cpp \
	extern/slb-1.63/src/ClassInfo.cpp \
	extern/slb-1.63/src/Table.cpp \
	extern/slb-1.63/src/FuncCall.cpp \
	extern/slb-1.63/src/Instance.cpp \
	extern/slb-1.63/src/Script.cpp \
	extern/slb-1.63/src/Iterator.cpp

libslb_a_CPPFLAGS = -DSLB_EXTERNAL_LUA -Iextern/slb-1.63/include

#
# Bundled lua sources
#
liblua_a_SOURCES = \
	extern/lua-5.1.4/src/lapi.c \
	extern/lua-5.1.4/src/lauxlib.c \
	extern/lua-5.1.4/src/lbaselib.c \
	extern/lua-5.1.4/src/lcode.c \
	extern/lua-5.1.4/src/ldblib.c \
	extern/lua-5.1.4/src/ldebug.c \
	extern/lua-5.1.4/src/ldo.c \
	extern/lua-5.1.4/src/ldump.c \
	extern/lua-5.1.4/src/lfunc.c \
	extern/lua-5.1.4/src/lgc.c \
	extern/lua-5.1.4/src/linit.c \
	extern/lua-5.1.4/src/liolib.c \
	extern/lua-5.1.4/src/llex.c \
	extern/lua-5.1.4/src/lmathlib.c \
	extern/lua-5.1.4/src/lmem.c \
	extern/lua-5.1.4/src/loadlib.c \
	extern/lua-5.1.4/src/lobject.c \
	extern/lua-5.1.4/src/lopcodes.c \
	extern/lua-5.1.4/src/loslib.c \
	extern/lua-5.1.4/src/lparser.c \
	extern/lua-5.1.4/src/lstate.c \
	extern/lua-5.1.4/src/lstring.c \
	extern/lua-5.1.4/src/lstrlib.c \
	extern/lua-5.1.4/src/ltable.c \
	extern/lua-5.1.4/src/ltablib.c \
	extern/lua-5.1.4/src/ltm.c \
	extern/lua-5.1.4/src/lundump.c \
	extern/lua-5.1.4/src/lvm.c \
	extern/lua-5.1.4/src/lzio.c \
	extern/lua-5.1.4/src/print.c

liblua_a_CPPFLAGS = -Iextern/lua-5.1.4/src

libglsw_a_CPPFLAGS = -Iextern/glsw
libglsw_a_SOURCES = extern/glsw/bstrlib.c extern/glsw/glsw.c

#
# RStation components
#

# Threads
RSTHREADS = src/threads/ThreadGroup.cpp
RSTHREADSLD = 
#if LINUX
RSTHREADS += src/threads/Threads_pthreads.cpp
RSTHREADSLD += -lpthread
#endif

# RStation source groups

Actors = \
	src/actors/Actor.cpp \
	src/actors/Quad.cpp

GL3 = \
	src/renderer/gl3/ShaderBase.cpp \
	src/renderer/gl3/ShaderProgram.cpp \
	src/renderer/gl3/ShaderStage.cpp

Input = \
	src/input/ButtonState.cpp

Managers = \
	src/managers/DisplayManager.cpp \
	src/managers/FileManager.cpp \
	src/managers/InputManager.cpp \
	src/managers/LuaManager.cpp \
	src/managers/MessageManager.cpp \
	src/managers/ScreenManager.cpp

Screens = \
	src/screens/Screen.cpp \
	src/screens/ScreenTestDrawing.cpp

Threads = \
	src/threads/Threads_pthreads.cpp \
	src/threads/ThreadGroup.cpp

Utils = \
	src/utils/Logger.cpp \
	src/utils/Message.cpp \
	src/utils/Timer.cpp

#
# RStation binary
# 
rstation_LDADD  = $(EXT_DEPENDS_LIBS) $(FFTW3_LIBS) -lz libglfw3.a libslb.a libgl3w.a libglsw.a
rstation_SOURCES = \
	$(Actors) \
	$(Input) \
	$(Managers) \
	$(Renderer) \
	$(Screens) \
	$(Utils) \
	$(GL3) \
	src/RStation.cpp

rstation_SOURCES += $(RSTHREADS)
rstation_LDADD += $(RSTHREADSLD)

rs_test_threads_CPPFLAGS = -I./src
rs_test_threads_SOURCES = $(RSTHREADS) src/tests/RSTestThreads.cpp
rs_test_threads_LDADD = $(RSTHREADSLD)

rstation_CPPFLAGS = \
	-DDEBUG \
	$(EXT_DEPENDS_CFLAGS) \
	$(FFTW3_CFLAGS) \
	$(libglfw3_a_CPPFLAGS) \
	$(libslb_a_CPPFLAGS) \
	$(libglsw_a_CPPFLAGS) \
	-Iextern/simpleini-4.13 \
	-Iextern/ogl-math \
	-Iextern/gl3w/include \
	-I./src

#
# Optional bundled lua
#
if !WITH_BUNDLED_LUA
rstation_LDADD += $(EXT_LUA_LIBS)
rstation_CPPFLAGS += $(EXT_LUA_CFLAGS)
libslb_a_CPPFLAGS += $(EXT_LUA_CFLAGS)
else # WITH_BUNDLED_LUA
noinst_LIBRARIES += liblua.a
rstation_LDADD += liblua.a
rstation_CPPFLAGS += $(liblua_a_CPPFLAGS)
libslb_a_LIBADD = liblua.a
libslb_a_CPPFLAGS += $(liblua_a_CPPFLAGS)
endif

# Yeah, this is strange (but convenient).
run:
	exec ./rstation
