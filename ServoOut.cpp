/**
 * @file ServoOut.cpp
 * @author Dan Oates (WPI Class of 2020)
 */
#include "ServoOut.h"
#include <CppUtil.h>

/**
 * @brief Constructs and disables servo output
 * @param pin Platform-specific pin ID
 * @param cmd_min Command corresponding to min pulsewidth
 * @param cmd_max Command corresponding to max pulsewidth
 * @param pw_min Minimum pulsewidth [s]
 * @param pw_max Maximum pulsewidth [s]
 */
ServoOut::ServoOut(
	Platform::pin_t pin,
	float cmd_min,
	float cmd_max,
	float pw_min,
	float pw_max)
	#if defined(PLATFORM_MBED)
		: pwm_out(pin)
	#endif
{
	#if defined(PLATFORM_ARDUINO)
		this->pin = pin;
	#endif
	this->pw_min = pw_min;
	this->pw_max = pw_max;
	this->pw_per_cmd = (pw_max - pw_min) / (cmd_max - cmd_min);
	this->pw_zero = pw_max - pw_per_cmd * cmd_max;
	this->cmd = 0.5f * (cmd_min + cmd_max);
	this->enabled = false;
}

/**
 * @brief Enables or disables servo
 * @param enabled True to enable, false to disable
 * 
 * Sets command to most recent call to set_cmd(). If set_cmd() was never called,
 * sets command to midpoint of min and max commands.
 */
void ServoOut::set_enabled(bool enabled)
{
	this->enabled = enabled;
	#if defined(PLATFORM_ARDUINO)
		if (enabled)
		{
			servo.attach(pin, 1e6 * pw_min, 1e6 * pw_max);
			set_cmd(cmd);
		}
		else
		{
			servo.detach();
		}
	#elif defined(PLATFORM_MBED)
		if (enabled)
		{
			set_cmd(cmd);
		}
		else
		{
			pwm_out.pulsewidth(0.0f);
		}
	#endif
}

/**
 * @brief Sets servo command (if enabled)
 * @param cmd Command in range [cmd_min, cmd_max]
 * 
 * Commands outside range [cmd_min, cmd_max] are automatically saturated.
 */
void ServoOut::set_cmd(float cmd)
{
	this->cmd = cmd;
	if (enabled)
	{
		float pw = pw_per_cmd * cmd + pw_zero;
		pw = CppUtil::clamp(pw, pw_min, pw_max);
		#if defined(PLATFORM_ARDUINO)
			servo.writeMicroseconds(1e6 * pw);
		#elif defined(PLATFORM_MBED)
			pwm_out.pulsewidth(pw);
		#endif
	}
}