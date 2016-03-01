#include <SDL2/SDL.h>
#include "FpsTracker.h"
#include "Puddi.h"

namespace puddi
{
	void FpsTracker::Update()
	{
		// calculate milliseconds elapsed since last call
		int current_time = SDL_GetTicks();
		frameTimeMs = current_time - last_frame_time;
		last_frame_time = current_time;

		// increment frame counter, update fps once per second
		frame_counter++;
		frame_time_accum += frameTimeMs;
		if (frame_time_accum >= 1000)
		{
			fps = frame_counter;
			frame_counter = frame_time_accum = 0;
		}
	}

	int FpsTracker::GetFrameTimeMs()
	{
		return frameTimeMs;
	}

	int FpsTracker::GetFps()
	{
		return fps;
	}

	void FpsTracker::Reset()
	{
		int current_time = SDL_GetTicks();
		last_frame_time = current_time;
		frame_counter = frame_time_accum = 0;
	}

	int FpsTracker::frameTimeMs;

	int FpsTracker::last_frame_time = 0;
	int FpsTracker::frame_counter = 0;
	int FpsTracker::frame_time_accum = 0;
	int FpsTracker::fps = 0;
}
