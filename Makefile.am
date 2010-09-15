#
# Stuff to install
#
noinst_PROGRAMS = rstation

#
# RStation sources
#
RSTATION = \
RStation/Actor.cpp \
RStation/Actor.h \
RStation/AudioManager.cpp \
RStation/AudioManager.h \
RStation/FileManager.cpp \
RStation/FileManager.h \
RStation/GameLoop.cpp \
RStation/GameLoop.h \
RStation/ImageLoader.cpp \
RStation/ImageLoader.h \
RStation/INILoader.cpp \
RStation/INILoader.h \
RStation/InputManager.cpp \
RStation/InputManager.h \
RStation/KeyboardHandler.cpp \
RStation/KeyboardHandler.h \
RStation/Log.cpp \
RStation/Log.h \
RStation/LuaManager.cpp \
RStation/LuaManager.h \
RStation/MathUtils.cpp \
RStation/MathUtils.h \
RStation/MessageManager.cpp \
RStation/MessageManager.h \
RStation/MouseHandler.cpp \
RStation/MouseHandler.h \
RStation/PNGLoader.cpp \
RStation/PNGLoader.h \
RStation/Primitives.cpp \
RStation/Primitives.h \
RStation/RStation.cpp \
RStation/RStation.h \
RStation/RSUtil.cpp \
RStation/RSUtil.h \
RStation/SceneManager.cpp \
RStation/SceneManager.h \
RStation/Screen.cpp \
RStation/Screen.h \
RStation/ShaderLoader.cpp \
RStation/ShaderLoader.h \
RStation/ShaderManager.cpp \
RStation/ShaderManager.h \
RStation/Sound.cpp \
RStation/Sound.h \
RStation/Sprite.cpp \
RStation/Sprite.h \
RStation/TextureManager.cpp \
RStation/TextureManager.h \
RStation/Timer.cpp \
RStation/Timer.h \
RStation/TimingData.h \
RStation/Tween.cpp \
RStation/Tween.h \
RStation/Type.cpp \
RStation/Type.h

#
# Bundled lua sources
#
BUNDLED_LUA = \
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
rstation_LDADD  = $(EXT_DEPENDS_LIBS) -lz
rstation_SOURCES = $(RSTATION)

# Optional bundled lua
if !WITH_BUNDLED_LUA
rstation_LDADD += $(EXT_LUA_LIBS)
else
rstation_SOURCES += $(BUNDLED_LUA)
rstation_CPPFLAGS = -Iextern/lua-5.1.4/src
endif
