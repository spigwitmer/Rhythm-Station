#include "Tween.h"
#include "utils/Logger.h"
#include <math.h>
#include <glm/glm.hpp>

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


/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
