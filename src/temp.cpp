#include <secrets.h>
#include <string.h>
#include <EEPROM.h>
#include <config.h>
#include <telnet.h>
#include <motor.h>
#include <util.h>
#include <math.h>
#include <net.h>

#define DEBUG_MODE

#define UPDATE_INTERVAL_SECS 10

#ifdef DEBUG_MODE
#define POST_SWITCH_WAIT_TIME 1
#else
// Don't keep switching back and forth
#define POST_SWITCH_WAIT_TIME 300
#endif

typedef struct data
{
	bool on;
} data_t;

namespace Temp
{
	char *last_state = NULL;
	data_t data = {0};

	void setup()
	{
		EEPROM.begin(512);
		EEPROM.get(0, data);
	}

	String get_state()
	{
		return Net::req(BASE_NAME, 80, "/temperature/advise/");
	}

	void apply_state(bool is_on)
	{
		data.on = is_on;

		EEPROM.put(0, data);
		EEPROM.commit();

		if (is_on)
		{
			Motor::move_right();
		}
		else
		{
			Motor::move_left();
		}
	}

	long last_switch = -1;
	bool is_in_cooldown()
	{
		if (last_switch == -1)
		{
			return false;
		}
		if (millis() - last_switch >= (POST_SWITCH_WAIT_TIME * 1000))
		{
			return false;
		}
		return true;
	}

	void update_mode(bool is_on, bool force)
	{
		if (is_on != data.on)
		{
			if (!is_in_cooldown())
			{
				LOGF("Changing state to %s\n", is_on ? "on" : "off");
				apply_state(is_on);

				last_switch = millis();
			}
		}
	}

	void update_state()
	{
		String state = get_state();
		char **strings = Util::split_string((char *)state.c_str(), ' ');
		bool is_on = strcmp(strings[0], "on") == 0;

		if (strcmp(strings[0], "?") != 0)
		{
			update_mode(is_on, strcmp(strings[1], "auto") != 0);
		}

		Util::free_split(strings);
	}

	int loop_counts = 0;
	unsigned int last_loop = 0;
	void loop()
	{
		if (millis() > last_loop + 1000)
		{
			last_loop = millis();

			if (loop_counts == 0)
			{
				update_state();
			}

			loop_counts = (loop_counts + 1) % UPDATE_INTERVAL_SECS;
		}
	}
} // namespace Temp