#pragma once

namespace Library
{
	inline const std::chrono::microseconds& StopWatch::Elapsed() const
	{
		return mElapsedTime;
	}

	inline std::chrono::milliseconds StopWatch::ElapsedMilliseconds() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(mElapsedTime);
	}

	inline std::chrono::seconds StopWatch::ElapsedSeconds() const
	{
		return std::chrono::duration_cast<std::chrono::seconds>(mElapsedTime);
	}

	inline bool StopWatch::IsRunning() const
	{
		return mIsRunning;
	}

	inline void StopWatch::Reset()
	{
		mIsRunning = false;
		mStartTime = std::chrono::high_resolution_clock::time_point();
		mElapsedTime = std::chrono::microseconds::zero();
	}

	inline void StopWatch::Restart()
	{
		Reset();
		Start();
	}

	inline void StopWatch::Start()
	{
		mIsRunning = true;
		mStartTime = std::chrono::high_resolution_clock::now();
	}

	inline void StopWatch::Stop()
	{
		mIsRunning = false;
		mElapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - mStartTime);
	}
}