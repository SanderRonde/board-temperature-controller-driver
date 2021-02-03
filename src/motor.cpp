#include <Arduino.h>
#include <telnet.h>

#define MOTOR_ON_PIN D1
#define MOTOR_RIGHT_PIN D2

#define FULL_MOVE_SECS 20

namespace Motor
{
	typedef enum DIR
	{
		LEFT,
		RIGHT
	} dir_t;

	void setup()
	{
		pinMode(MOTOR_ON_PIN, OUTPUT);
		pinMode(MOTOR_RIGHT_PIN, OUTPUT);
	}

	void start_motor(dir_t dir)
	{
		if (dir == LEFT)
		{
			digitalWrite(MOTOR_RIGHT_PIN, LOW);
		}
		else
		{
			digitalWrite(MOTOR_RIGHT_PIN, HIGH);
		}

		digitalWrite(MOTOR_ON_PIN, HIGH);
	}

	void stop_motor()
	{
		digitalWrite(MOTOR_ON_PIN, LOW);
		digitalWrite(MOTOR_RIGHT_PIN, LOW);
	}

	void move_left(double num_seconds)
	{
		LOGF("Moving left for %.2f seconds\n", num_seconds);
		start_motor(LEFT);
		delay(num_seconds * 1000);
		stop_motor();
		LOGF("Stopped moving\n");
	}
	void move_left()
	{
		move_left(FULL_MOVE_SECS);
	}
	void move_right(double num_seconds)
	{
		LOGF("Moving right for %.2f seconds\n", num_seconds);
		start_motor(RIGHT);
		delay(num_seconds * 1000);
		stop_motor();
		LOGF("Stopped moving\n");
	}
	void move_right()
	{
		move_right(FULL_MOVE_SECS);
	}
} // namespace Motor