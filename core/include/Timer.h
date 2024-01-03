/**
 * ============================================================================
 *  Name        : Timer.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : basic high-precision timer
 * ============================================================================
**/

#pragma once

#include <cstdint>

class Timer
{
public:
	Timer();

	/**
	 * BeginTimer
	 * start calculating the time
	 */
	void BeginTimer();

	/**
	 * EndTimer
	 * stop calculatig the time
	 */
	void EndTimer();

	/**
	 * GetElapsedSeconds
	 * @return seconds between last BeginTimer - EndTimer calls.
	 */
	inline float GetElapsedSeconds() const { return m_fElapsedSeconds; }

	/**
	 * GetTicks
	 * @return system timer ticks
	 */
	static uint64_t GetTicks();

private:
	double		m_dRateToSeconds;
	uint64_t	m_uStartClock;
	uint64_t	m_uTickFrequency;

	float		m_fElapsedSeconds;
};

