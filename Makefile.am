#
# Stuff to install
#
noinst_PROGRAMS = rstation
noinst_LIBRARIES = libglfw3.a libslb.a

#
# RStation sources
#
RSTATION = \
src/AudioManager.cpp \
src/AudioManager.h \
src/FileManager.cpp \
src/FileManager.h \
src/Font.cpp \
src/Font.h \
src/GameManager.cpp \
src/GameManager.h \
src/globals.h \
src/HandleArguments.cpp \
src/HandleArguments.h \
src/INILoader.cpp \
src/INILoader.h \
src/InputManager.cpp \
src/InputManager.h \
src/Logger.cpp \
src/Logger.h \
src/LuaManager.cpp \
src/LuaManager.h \
src/main.cpp \
src/Matrix.cpp \
src/Matrix.h \
src/Object.cpp \
src/Object.h \
src/OBJLoader.cpp \
src/OBJLoader.h \
src/PNGLoader.cpp \
src/PNGLoader.h \
src/ResourceManager.cpp \
src/ResourceManager.h \
src/SceneManager.cpp \
src/SceneManager.h \
src/Screen.cpp \
src/Screen.h \
src/Shader.cpp \
src/Shader.h \
src/StringUtils.cpp \
src/StringUtils.h \
src/Timer.cpp \
src/Timer.h \
src/Tween.cpp \
src/Tween.h \
src/Type.cpp \
src/Type.h

#
# Submodule of glfw3 (no packaged release available)
#  - git source can be updated with 'git submodule update'
#  - cmake extern/glfw3/ must be run before trying to make rstation
#
libglfw3_a_SOURCES = \
extern/glfw3/src/config.h \
extern/glfw3/src/enable.c \
extern/glfw3/src/error.c \
extern/glfw3/src/fullscreen.c \
extern/glfw3/src/gamma.c \
extern/glfw3/src/glext.c \
extern/glfw3/src/init.c \
extern/glfw3/src/input.c \
extern/glfw3/src/internal.h \
extern/glfw3/src/joystick.c \
extern/glfw3/src/time.c \
extern/glfw3/src/window.c \
extern/glfw3/src/x11/platform.h \
extern/glfw3/src/x11/x11_enable.c \
extern/glfw3/src/x11/x11_fullscreen.c \
extern/glfw3/src/x11/x11_gamma.c \
extern/glfw3/src/x11/x11_glext.c \
extern/glfw3/src/x11/x11_init.c \
extern/glfw3/src/x11/x11_joystick.c \
extern/glfw3/src/x11/x11_keysym2unicode.c \
extern/glfw3/src/x11/x11_time.c \
extern/glfw3/src/x11/x11_window.c \
extern/glfw3/include/GL/glext.h \
extern/glfw3/include/GL/glfw3.h \
extern/glfw3/include/GL/wglext.h
libglfw3_a_CPPFLAGS = -Iextern/glfw3/src -Iextern/glfw3/src/x11

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
extern/lua-5.1.4/src/lapi.h \
extern/lua-5.1.4/src/lauxlib.c \
extern/lua-5.1.4/src/lauxlib.h \
extern/lua-5.1.4/src/lbaselib.c \
extern/lua-5.1.4/src/lcode.c \
extern/lua-5.1.4/src/lcode.h \
extern/lua-5.1.4/src/ldblib.c \
extern/lua-5.1.4/src/ldebug.c \
extern/lua-5.1.4/src/ldebug.h \
extern/lua-5.1.4/src/ldo.c \
extern/lua-5.1.4/src/ldo.h \
extern/lua-5.1.4/src/ldump.c \
extern/lua-5.1.4/src/lfunc.c \
extern/lua-5.1.4/src/lfunc.h \
extern/lua-5.1.4/src/lgc.c \
extern/lua-5.1.4/src/lgc.h \
extern/lua-5.1.4/src/linit.c \
extern/lua-5.1.4/src/liolib.c \
extern/lua-5.1.4/src/llex.c \
extern/lua-5.1.4/src/llex.h \
extern/lua-5.1.4/src/llimits.h \
extern/lua-5.1.4/src/lmathlib.c \
extern/lua-5.1.4/src/lmem.c \
extern/lua-5.1.4/src/lmem.h \
extern/lua-5.1.4/src/loadlib.c \
extern/lua-5.1.4/src/lobject.c \
extern/lua-5.1.4/src/lobject.h \
extern/lua-5.1.4/src/lopcodes.c \
extern/lua-5.1.4/src/lopcodes.h \
extern/lua-5.1.4/src/loslib.c \
extern/lua-5.1.4/src/lparser.c \
extern/lua-5.1.4/src/lparser.h \
extern/lua-5.1.4/src/lstate.c \
extern/lua-5.1.4/src/lstate.h \
extern/lua-5.1.4/src/lstring.c \
extern/lua-5.1.4/src/lstring.h \
extern/lua-5.1.4/src/lstrlib.c \
extern/lua-5.1.4/src/ltable.c \
extern/lua-5.1.4/src/ltable.h \
extern/lua-5.1.4/src/ltablib.c \
extern/lua-5.1.4/src/ltm.c \
extern/lua-5.1.4/src/ltm.h \
extern/lua-5.1.4/src/luaconf.h \
extern/lua-5.1.4/src/lua.h \
extern/lua-5.1.4/src/lualib.h \
extern/lua-5.1.4/src/lundump.c \
extern/lua-5.1.4/src/lundump.h \
extern/lua-5.1.4/src/lvm.c \
extern/lua-5.1.4/src/lvm.h \
extern/lua-5.1.4/src/lzio.c \
extern/lua-5.1.4/src/lzio.h \
extern/lua-5.1.4/src/print.c
#extern/lua-5.1.4/src/lua.c 
#extern/lua-5.1.4/src/luac.c

#
# RStation binary
# 
rstation_LDADD  = $(EXT_DEPENDS_LIBS) -lz libglfw3.a libslb.a
rstation_SOURCES = $(RSTATION)
rstation_CPPFLAGS = $(EXT_DEPENDS_CFLAGS) -Iextern/glfw3/include \-Iextern/glfw/src/x11 -DSLB_EXTERNAL_LUA -Iextern/slb-1.63/include

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
rstation_CPPFLAGS += -Iextern/lua-5.1.4/src
libslb_a_LIBADD = liblua.a
libslb_a_CPPFLAGS += -Iextern/lua-5.1.4/src
endif
