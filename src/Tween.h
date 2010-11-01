#ifndef _Tween_H_
#define _Tween_H_

#include "Type.h"
#include "Matrix.h"

enum TweenType {
	TWEEN_SLEEP = 0,
	TWEEN_LINEAR,
	TWEEN_EASE_IN, // this and the next are a little disfunctional.
	TWEEN_EASE_OUT,
	TWEEN_SMOOTH,
	TWEEN_EASE_IN_CUBIC,
	TWEEN_EASE_OUT_CUBIC,
	TWEEN_SMOOTH_CUBIC
};

float interpolate(TweenType tween, float _old, float _new, double duration, double time);
vec3 interpolate(TweenType tween, vec3 _old, vec3 _new, double duration, double time);
rgba interpolate(TweenType tween, rgba _old, rgba _new, double duration, double time);
Matrix interpolate(TweenType tween, Matrix _old, Matrix _new, double duration, double time);

#endif
