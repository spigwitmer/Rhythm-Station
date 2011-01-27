#include "Tween.h"
#include "Logger.h"
#include <math.h>

typedef float (*TweenFn) (float,double,double);

// TODO: these should really be controlled by bezier curves rather than these equations.
float sleep(float change, double time, double elapsed)
{
	if (elapsed == 1)
		return change;
		
	return 0.0f;
}

float linear(float change, double time, double elapsed)
{
	return change * elapsed;
}

float ease_in(float change, double time, double elapsed)
{
	return change * elapsed * time;
}

float ease_out(float change, double time, double elapsed)
{
	return change * ((1-elapsed)+1) * time;
}

float smooth(float change, double time, double elapsed)
{
	// this might be wrong?
	if ((elapsed/2) < 0.5)
		return change * powf(elapsed, 2);
		
	return change * (powf(elapsed-2, 2) + 2);
}

float ease_in_cubic(float change, double time, double elapsed)
{
	return change * powf(elapsed, 3);
}

float ease_out_cubic(float change, double time, double elapsed)
{
	return change * (powf(elapsed-1, 3)+1);
}

float smooth_cubic(float change, double time, double elapsed)
{
	if ((elapsed/2) < 1)
		return change * powf(elapsed, 3);
		
	return change * (powf(elapsed-1, 3) + 1);
}

TweenFn SetTweenType(TweenType type)
{
	TweenFn Tween = NULL;
	
	switch (type)
	{
	case TWEEN_SLEEP:
		Tween = &sleep;
		break;
	case TWEEN_LINEAR:
		Tween = &linear;
		break;
	case TWEEN_EASE_IN: // 2
		Tween = &ease_in;
		break;
	case TWEEN_EASE_OUT: // 3
		Tween = &ease_out;
		break;
	case TWEEN_SMOOTH: // 4
		Tween = &smooth;
		break;
	case TWEEN_EASE_IN_CUBIC: // 5
		Tween = &ease_in_cubic;
		break;
	case TWEEN_EASE_OUT_CUBIC: // 6
		Tween = &ease_out_cubic;
		break;
	case TWEEN_SMOOTH_CUBIC: // 7
		Tween = &smooth_cubic;
		break;
	default:
		Tween = &linear;
		Log->Print("Invalid Tween. Defaulting to linear.");
		break;
	}
	
	return Tween;
}

float interpolate(TweenType tweentype, float _old, float _new, double duration, double time)
{
	TweenFn Tween = SetTweenType(tweentype);
	
	if (duration == 0 || !Tween)
		return _new; // don't divide by zero and don't bother with doing any math.
		
	float temp = _old;
	double elapsed = time / duration;
	temp -= Tween(_old - _new, time, elapsed);
	return temp;
}

// Various overloads. A bit repetetive, but convenient.
vec2 interpolate(TweenType tweentype, vec2 _old, vec2 _new, double duration, double time)
{
	TweenFn Tween = SetTweenType(tweentype);
	
	if (duration == 0 || !Tween)
		return _new;
		
	vec2 temp = _old;
	vec2 change = _old - _new;
	double elapsed = time / duration;
	temp.x -= Tween(change.x, time, elapsed);
	temp.y -= Tween(change.y, time, elapsed);
	return temp;
}

vec3 interpolate(TweenType tweentype, vec3 _old, vec3 _new, double duration, double time)
{
	TweenFn Tween = SetTweenType(tweentype);
	
	if (duration == 0 || !Tween)
		return _new;
		
	vec3 temp = _old;
	vec3 change = _old - _new;
	double elapsed = time / duration;
	temp.x -= Tween(change.x, time, elapsed);
	temp.y -= Tween(change.y, time, elapsed);
	temp.z -= Tween(change.z, time, elapsed);
	return temp;
}

rgba interpolate(TweenType tweentype, rgba _old, rgba _new, double duration, double time)
{
	TweenFn Tween = SetTweenType(tweentype);
	
	if (duration == 0 || !Tween)
		return _new;
		
	rgba temp = _old;
	rgba change = _old - _new;
	double elapsed = time / duration;
	temp.r -= Tween(change.r, time, elapsed);
	temp.g -= Tween(change.g, time, elapsed);
	temp.b -= Tween(change.b, time, elapsed);
	temp.a -= Tween(change.a, time, elapsed);
	return temp;
}

Matrix interpolate(TweenType tweentype, Matrix _new, Matrix _old, double duration, double time)
{
	TweenFn Tween = SetTweenType(tweentype);
	
	if (duration == 0 || !Tween)
		return _new;
		
	Matrix temp = _old;
	double elapsed = time / duration;
	
	for (int i = 0; i<16; i++)
		temp.matrix[i] -= Tween(_old.matrix[i] - _new.matrix[i], time, elapsed);
		
	return temp;
}
