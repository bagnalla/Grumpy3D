#ifndef FPSTRACKER_H
#define FPSTRACKER_H

namespace puddi
{
	class FpsTracker
	{
	public:
		/**********************************************************************
		Function:		Update
		Purpose:		Update the FpsTracker. Updates FrameTimeMs and keeps
		track of the FPS.
		**********************************************************************/
		static void Update();

		/**********************************************************************
		Function:		GetFrameTimeMs
		Purpose:		Static getter for frameTimeMs.
		**********************************************************************/
		static int GetFrameTimeMs();

		/**********************************************************************
		Function:		GetFps
		Purpose:		Static getter for fps.
		**********************************************************************/
		static int GetFps();

		static void Reset();
	private:
		static int last_frame_time, // timestamp of the previous frame
			frame_counter, // frame counter for tracking fps
			frame_time_accum, // time accumulator for tracking fps
			fps, // frames per second
			frameTimeMs; // elapsed milliseconds since previous frame
	};
}

#endif
