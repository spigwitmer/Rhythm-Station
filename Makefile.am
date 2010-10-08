#
# Stuff to install
#
noinst_PROGRAMS = rstation

#
# RStation sources
#
RSTATION = \
src/Actor.cpp \
src/Actor.h \
src/AudioManager.cpp \
src/AudioManager.h \
src/FileManager.cpp \
src/FileManager.h \
src/GameLoop.cpp \
src/GameLoop.h \
src/ImageLoader.cpp \
src/ImageLoader.h \
src/INILoader.cpp \
src/INILoader.h \
src/InputManager.cpp \
src/InputManager.h \
src/KeyboardHandler.cpp \
src/KeyboardHandler.h \
src/Logger.cpp \
src/Logger.h \
src/LuaManager.cpp \
src/LuaManager.h \
src/MathUtils.cpp \
src/MathUtils.h \
src/Matrix.cpp \
src/Matrix.h \
src/MessageManager.cpp \
src/MessageManager.h \
src/MouseHandler.cpp \
src/MouseHandler.h \
src/OBJLoader.cpp \
src/OBJLoader.h \
src/PNGLoader.cpp \
src/PNGLoader.h \
src/Primitives.cpp \
src/Primitives.h \
src/RStation.cpp \
src/RStation.h \
src/RSUtil.cpp \
src/RSUtil.h \
src/SceneManager.cpp \
src/SceneManager.h \
src/Screen.cpp \
src/Screen.h \
src/ShaderLoader.cpp \
src/ShaderLoader.h \
src/ShaderManager.cpp \
src/ShaderManager.h \
src/Sound.cpp \
src/Sound.h \
src/Sprite.cpp \
src/Sprite.h \
src/TextureManager.cpp \
src/TextureManager.h \
src/Timer.cpp \
src/Timer.h \
src/TimingData.h \
src/Tween.cpp \
src/Tween.h \
src/Type.cpp \
src/Type.h \
src/Video.h \
src/Video_Draw.cpp \
src/Video_Window.cpp

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

BUNDLED_LUABIND = \
extern/luabind-0.8.1/src/link_compatibility.cpp \
extern/luabind-0.8.1/src/implicit_cast.cpp \
extern/luabind-0.8.1/src/class_rep.cpp \
extern/luabind-0.8.1/src/stack_content_by_name.cpp \
extern/luabind-0.8.1/src/ref.cpp \
extern/luabind-0.8.1/src/scope.cpp \
extern/luabind-0.8.1/src/find_best_match.cpp \
extern/luabind-0.8.1/src/object_rep.cpp \
extern/luabind-0.8.1/src/class_info.cpp \
extern/luabind-0.8.1/src/function.cpp \
extern/luabind-0.8.1/src/open.cpp \
extern/luabind-0.8.1/src/class_registry.cpp \
extern/luabind-0.8.1/src/weak_ref.cpp \
extern/luabind-0.8.1/src/error.cpp \
extern/luabind-0.8.1/src/class.cpp \
extern/luabind-0.8.1/src/exception_handler.cpp \
extern/luabind-0.8.1/src/wrapper_base.cpp \
extern/luabind-0.8.1/src/create_class.cpp \
extern/luabind-0.8.1/src/pcall.cpp \
extern/luabind-0.8.1/src/overload_rep.cpp

#
# RStation binary
# 
rstation_LDADD  = $(EXT_DEPENDS_LIBS) -lz
rstation_SOURCES = $(RSTATION)
rstation_CPPFLAGS = $(EXT_DEPENDS_CFLAGS)

#
# Optional bundled lua
#
if !WITH_BUNDLED_LUA
rstation_LDADD += $(EXT_LUA_LIBS)
rstation_CPPFLAGS += $(EXT_LUA_CFLAGS)
else # WITH_BUNDLED_LUA (include bundled luabind too)
rstation_SOURCES += $(BUNDLED_LUA)
rstation_CPPFLAGS += -Iextern/lua-5.1.4/src
endif

#
# Optional bundled luabind (forced when using bundled lua)
#
if !WITH_BUNDLED_LUABIND
rstation_LDADD += $(EXT_LUABIND_LIBS)
rstation_CPPFLAGS += $(EXT_LUABIND_CFLAGS)
else # WITH_BUNDLED_LUABIND
rstation_SOURCES += $(BUNDLED_LUABIND)
rstation_CPPFLAGS += -Iextern/luabind-0.8.1/
endif
