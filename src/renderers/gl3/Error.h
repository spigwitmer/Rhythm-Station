#pragma once

#define CheckError()\
{\
	for (GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError())\
	{\
		switch (err)\
		{\
			case GL_INVALID_ENUM: LOG->Fatal("GL_INVALID_ENUM"); break;\
			case GL_INVALID_OPERATION: LOG->Fatal("GL_INVALID_OPERATION"); break;\
			case GL_INVALID_VALUE: LOG->Fatal("GL_INVALID_VALUE"); break;\
			case GL_OUT_OF_MEMORY: LOG->Fatal("GL_OUT_OF_MEMORY"); break;\
			default: break;\
		}\
	}\
}
