//
// A basic static timer class made with chrono
// To use for frame timing, simply call StartFrame and EndFrame at the beginning and end of each frame
// Then, a simple call to GetFrameTime(void) will return your value!
// You can also time custom local points using Start(), End() and GetElapsedTime()
// Justin Bonczek
//

#include "Timer.h"

std::chrono::time_point<std::chrono::system_clock> Timer::programStart;
std::chrono::time_point<std::chrono::system_clock> Timer::localStart;
std::chrono::time_point<std::chrono::system_clock> Timer::localEnd;
std::chrono::time_point<std::chrono::system_clock> Timer::frameStart;
std::chrono::time_point<std::chrono::system_clock> Timer::frameEnd;
std::chrono::duration<float> Timer::elapsedTime;
std::chrono::duration<float> Timer::frameTime;
int Timer::frameRate;

float Timer::GetFrameTime(void) { return frameTime.count(); }
float Timer::GetElapsedTime(void) { return elapsedTime.count(); }
float Timer::GetTotalTime(void){ return (std::chrono::system_clock::now() - programStart).count(); }

void Timer::Initialize(void)
{
	programStart = std::chrono::system_clock::now();
}

void Timer::Start(void)
{
	localStart = std::chrono::system_clock::now();
}

void Timer::StartFrame(void)
{
	frameStart = std::chrono::system_clock::now();
}

void Timer::Stop(void)
{
	localEnd = std::chrono::system_clock::now();

	elapsedTime = localEnd - localStart;
}

void Timer::StopFrame(void)
{
	frameEnd = std::chrono::system_clock::now();

	frameTime = frameEnd - frameStart;
}

