# Global CFLAGS
AM_CFLAGS= -Wall -Wno-unknown-pragmas
AM_CXXFLAGS= $(AM_CFLAGS)

#
# Stuff to install (make install not working yet as there's no reason
# to install rstation to a system yet..)
#
TestSuite = rs-test-threads

noinst_PROGRAMS = rstation $(TestSuite)
noinst_LIBRARIES = libglfw3.a libslb.a

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
extern/glfw3/src/x11/x11_enable.c \
extern/glfw3/src/x11/x11_fullscreen.c \
extern/glfw3/src/x11/x11_gamma.c \
extern/glfw3/src/x11/x11_opengl.c \
extern/glfw3/src/x11/x11_init.c \
extern/glfw3/src/x11/x11_joystick.c \
extern/glfw3/src/x11/x11_keysym2unicode.c \
extern/glfw3/src/x11/x11_time.c \
extern/glfw3/src/x11/x11_window.c
libglfw3_a_CPPFLAGS = -Iextern/glfw3/include -Iextern/glfw3/src -Iextern/glfw3/src/x11

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
#extern/lua-5.1.4/src/lua.c 
#extern/lua-5.1.4/src/luac.c
liblua_a_CPPFLAGS = -Iextern/lua-5.1.4/src

#
# RStation components
#

# Threads
RSTHREADS = src/ThreadGroup.cpp
RSTHREADSLD = 
#if LINUX
RSTHREADS += src/Threads_pthreads.cpp
RSTHREADSLD += -lpthread
#endif

#
# RStation binary
# 
rstation_LDADD  = $(EXT_DEPENDS_LIBS) $(FFTW3_LIBS) -lz libglfw3.a libslb.a
rstation_SOURCES = \
src/FileManager.cpp \
src/Font.cpp \
src/GameManager.cpp \
src/HandleArguments.cpp \
src/INIFile.cpp \
src/InputManager.cpp \
src/utils/Logger.cpp \
src/LuaManager.cpp \
src/main.cpp \
src/Matrix.cpp \
src/Mesh.cpp \
src/Message.cpp \
src/MessageManager.cpp \
src/NoteField.cpp \
src/Object.cpp \
src/PNGFile.cpp \
src/PreferencesFile.cpp \
src/ResourceManager.cpp \
src/Shader.cpp \
src/Sound.cpp \
src/SoundManager.cpp \
src/Sprite.cpp \
src/SQLiteDatabase.cpp \
src/StringUtils.cpp \
src/Texture.cpp \
src/Timer.cpp \
src/Tween.cpp \
src/Type.cpp \
src/Window.cpp

rstation_SOURCES += $(RSTHREADS)
rstation_LDADD += $(RSTHREADSLD)

rs_test_threads_SOURCES = $(RSTHREADS) src/RSTestThreads.cpp
rs_test_threads_LDADD = $(RSTHREADSLD)

rstation_CPPFLAGS = -DDEBUG \
$(EXT_DEPENDS_CFLAGS) \
$(FFTW3_CFLAGS) \
$(libglfw3_a_CPPFLAGS) \
$(libslb_a_CPPFLAGS) \
-Iextern/simpleini-4.13 \
-Iextern/ogl-math

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
