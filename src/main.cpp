#include <ArduinoOTA.h>
#include <Arduino.h>
#include <config.h>
#include <telnet.h>
#include <motor.h>
#include <temp.h>
#include <ota.h>
#include <net.h>

void setup()
{
	Serial.begin(115200);
	Serial.println("Booting");

	// Setup telnet
	Telnet::setup("temp-controller-board");

	// Setup OTA and wait
	OTA::setup();
	LOGN("Booted. Waiting for possible OTAs");
	OTA::wait_for_otas();
	LOGN("Stopped waiting");

	// Setup the rest
	Motor::setup();
	Net::setup();
	Temp::setup();

	// Done
	LOGN("Booted");
}

void loop()
{
	OTA::loop();
	Temp::loop();
	Telnet::loop();
}
