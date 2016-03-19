/**
 * @file FpsTracker.h
 * @author Alexander Bagnall
 * @brief Provides the ability track time on a frame-to-frame basis.
 *
 * GetFrameTimeMs() can be used to scale time/movement related things
 * as to be independent of the application framerate.
 */

 #ifndef FPSTRACKER_H
#define FPSTRACKER_H

namespace puddi
{
	namespace FpsTracker
	{
		void Update();

		int GetFrameTimeMs();

		int GetFps();

		void Reset();
	};
}

#endif
