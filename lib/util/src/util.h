#pragma once

#include <Arduino.h>
#include <string.h>

namespace Util {
	char** split_string(char *a_str, const char a_delim);
	void free_split(char** strings);
}