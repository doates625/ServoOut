/**
 * @file ServoOut.h
 * @brief Class for Arduino and Mbed servo control
 * @author Dan Oates (WPI Class of 2020)
 */
#pragma once
#include <Platform.h>

/**
 * Conditional Dependencies
 */
#if defined(PLATFORM_ARDUINO)
	#include <Servo.h>
#endif

/**
 * Class Declaration
 */
class ServoOut
{
public:
	ServoOut(
		Platform::pin_t pin,
		float cmd_min,
		float cmd_max,
		float pw_min = 1e-3,
		float pw_max = 2e-3);
	void set_enabled(bool enabled);
	void set_cmd(float cmd);
protected:
	#if defined(PLATFORM_ARDUINO)
		Servo servo;
		uint8_t pin;
	#elif defined(PLATFORM_MBED)
		PwmOut pwm_out;
	#endif
	bool enabled;
	float cmd;
	float pw_min;
	float pw_max;
	float pw_per_cmd;
	float pw_zero;
};