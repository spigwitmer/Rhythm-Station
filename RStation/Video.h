#ifndef _VIDEO_H_
#define _VIDEO_H_

namespace Video
{
	namespace Window
	{
		void Open();
		void Close();
		void PollEvents();
	}
	namespace Matrix
	{
		void Push();
		void Pop();
		void Translate(float x, float y, float z);
		void Scale(float x, float y, float z);
		void Rotate(float angle, float x, float y, float z);
		bool Invert4(float inverse[4][4], float mat[4][4]);
		void f16_m4(float m4[4][4], float f16[16]);
		void m4_f16(float f16[16], float m4[4][4]);
	}
	namespace Draw
	{
		void Begin();
		void End();
	}
}

#endif
