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
	namespace Draw
	{
		void Begin();
		void End();
	}
}

#endif
