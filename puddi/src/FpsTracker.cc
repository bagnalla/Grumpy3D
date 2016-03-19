#include <SDL2/SDL.h>
#include "FpsTracker.h"
#include "Puddi.h"

namespace puddi
{
    namespace FpsTracker
    {
        // PRIVATE
        namespace
        {
            int last_frame_time, // timestamp of the previous frame
			frame_counter, // frame counter for tracking fps
			frame_time_accum, // time accumulator for tracking fps
			fps, // frames per second
			frameTimeMs; // elapsed milliseconds since previous frame
        }

        // PUBLIC
        void Update()
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

        int GetFrameTimeMs()
        {
            return frameTimeMs;
        }

        int GetFps()
        {
            return fps;
        }

        void Reset()
        {
            int current_time = SDL_GetTicks();
            last_frame_time = current_time;
            frame_counter = frame_time_accum = 0;
        }
    }
}
