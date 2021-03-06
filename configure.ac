# Initialization
AC_INIT([Rhythm-Station], [git], [shakesoda+rstation@gmail.com], [], [http://github.com/shakesoda/Rhythm-Station])
AM_INIT_AUTOMAKE([foreign -Wall])
AM_SILENT_RULES([yes])

# Tools to use
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_RANLIB

# Config files to generate
AC_CONFIG_FILES([
	Makefile
])

# Put automake defines into config.h even though we don't use it atm.
# (Cleans up non-silent building a lot)
AC_CONFIG_HEADERS([config.h])

# Check for external dependencies
PKG_CHECK_MODULES([EXT_DEPENDS],[
		freetype2
		gl 
		glew 
		libpng 
		openal 
		sqlite3 
		vorbisfile 
		xrandr
		x11
		xxf86vm
	],[],[
	AC_MSG_ERROR([All dependencies were not fulfilled: $EXT_DEPENDS_PKG_ERRORS])
])

# zlib has to be checked the old way as ubuntu lucid doesn't have zlib.pc
AC_CHECK_HEADER([zlib.h],[],[
	AC_MSG_ERROR([All dependencies were not fulfilled: zlib was not found.])	
])

# Choose between external and bundled lua
AC_ARG_WITH([bundled_lua],
	[  --with-bundled-lua      build against bundled lua],
	[case "${withval}" in
		yes)	bundled_lua=yes ;;
		no)		bundled_lua=no ;;
		*)		AC_MSG_ERROR([bad value ${withval} for --with-bundled-lua]) ;;
	esac],[bundled_lua=no])

AM_CONDITIONAL([WITH_BUNDLED_LUA], [test x$bundled_lua = xyes])
if test x$bundled_lua != xyes; then
	PKG_CHECK_MODULES([EXT_LUA],[lua >= 5.1],[],[
		AC_MSG_WARN([External lua dependency couldn't be satisfied with 'lua', trying 'lua5.1'])
		PKG_CHECK_MODULES([EXT_LUA],[lua5.1 >= 5.1],[],[
			AC_MSG_ERROR([External lua cannot be used because: '$EXT_LUA_PKG_ERRORS'])
		])
	])
fi

# fftw-3 (default to threaded version)
# fftw-3.3 needs different linking style
PKG_CHECK_EXISTS([fftw3 >= 3.3],[
	PKG_CHECK_MODULES([FFTW3],[fftw3 >= 3.3 fftw3_threads >= 3.3],[],[
		AC_MSG_WARN([fftw3.3 dependency couldn't be satisfied with 'fftw3_threads', trying only 'fftw3'])
		PKG_CHECK_MODULES([FFTW3],[fftw3 >= 3.3],[],[
			AC_MSG_ERROR([fftw3.3 dependency was not fulfilled: '$FFTW3_PKG_ERRORS'])
		])
	])
],[
	PKG_CHECK_MODULES([FFTW3],[fftw3 >= 3.3 fftw3_threads >= fftw3_threads],[],[
		AC_MSG_WARN([fftw3 dependency couldn't be satisfied with 'fftw3_threads', trying 'fftw3'])
		PKG_CHECK_MODULES([FFTW3],[fftw3],[],[
			AC_MSG_ERROR([fftw3 dependency was not fulfilled: '$FFTW3_PKG_ERRORS'])
		])
	])
])

AC_OUTPUT
