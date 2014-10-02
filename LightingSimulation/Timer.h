//
// A basic static timer class made with chrono
// To use for frame timing, simply call StartFrame and EndFrame at the beginning and end of each frame
// Then, a simple call to GetFrameTime(void) will return your value!
// You can also time custom local points using Start(), End() and GetElapsedTime()
// Justin Bonczek
//

#ifndef TIMER_H
#define TIME_H

#include <chrono>

class Timer
{
public:
	// Initializes the static Timer
	static void Initialize(void);

	// Sets the frameStart variable to the time of call
	static void Start(void);
	static void StartFrame(void);

	// Sets the frameEnd variable to the time of call and calculates the elapsed time
	static void Stop(void);
	static void StopFrame(void);

	// Returns the time between frames
	static float GetFrameTime(void);

	// Returns the most recently calculated elapsed time (calculated in Stop(void) function)
	static float GetElapsedTime(void);

	// Returns the total time since the program began
	static float GetTotalTime(void);
private:
	static std::chrono::time_point<std::chrono::system_clock> programStart;

	static std::chrono::time_point<std::chrono::system_clock> frameStart;
	static std::chrono::time_point<std::chrono::system_clock> frameEnd;

	static std::chrono::duration<float> frameTime;

	static std::chrono::time_point<std::chrono::system_clock> localStart;
	static std::chrono::time_point<std::chrono::system_clock> localEnd;

	static std::chrono::duration<float> elapsedTime;

	static int frameRate;

};

#endif