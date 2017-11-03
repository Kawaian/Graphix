#pragma once

#include "wtypes.h"
#include <chrono>
#include <thread>
#include <functional>
#include <SDL2/SDL.h>
#include "../include/Graphics/LogManager.h"
#include <boost/log/trivial.hpp>

namespace gpx
{
	namespace util
	{
		void RenderCycle
			(
			Uint32 numberOfTicks,
			bool& refRunning,
			std::function<void(float)> update,
			std::function<void(void)> render,
			unsigned& refFrames,
			bool verbose = false
			)
		{
			static auto lastTime = std::chrono::high_resolution_clock::now();
			static auto inNanoTicks = 1000000000.0f / numberOfTicks;
			static float delta = 0.0f;
			static auto timer = std::chrono::high_resolution_clock::now();
			static unsigned frames = 0;
			while (refRunning){
				auto timeNow = std::chrono::high_resolution_clock::now();
				delta += std::chrono::duration_cast<std::chrono::nanoseconds>((timeNow - lastTime) / inNanoTicks).count();
				lastTime = timeNow;
				while (delta >= 1.0){
					update(1 / delta);
					--delta;
				}
				if (refRunning)
					render();
				++frames;
				if (
					std::chrono::duration_cast<std::chrono::nanoseconds>
					(
					std::chrono::high_resolution_clock::now() - timer
					).count()
					> 1000000000.0 // == 1 second
					)
				{
					timer = timer + std::chrono::seconds(1);
					refFrames = frames;
					if(verbose)
						BOOST_LOG_TRIVIAL(trace) << "FPS : " << refFrames;
					frames = 0;
				}
			}
		}
  }
}