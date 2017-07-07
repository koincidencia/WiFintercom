// udpBroadcaster.h

#ifndef _UDPBROADCASTER_h
#define _UDPBROADCASTER_h

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

class UDPBroadcaster{
	private:
		static const int packetBufferLength = 256;
		static const int ipBufferLength = 256;
		WiFiUDP udp;
		char packetBuffer[packetBufferLength];
		char ipBuffer[ipBufferLength];

	public:
		UDPBroadcaster(int port);
		void BroadcastHandler(String hostname);
};


#endif
