#include "debug.h"

#include "MegaSquirt.h"

#define offset 3 // new serial protocol is offset by 3

// Constructor
MegaSquirt::MegaSquirt() {
}

byte MegaSquirt::getEngine() {
	return data[offset + 11];
}

unsigned int MegaSquirt::getRpm() {
	return ((data[offset + 6] << 8) | data[offset + 7]);
}

float MegaSquirt::getMap() {
	return ((data[offset + 18] << 8) | data[offset + 19]) * 0.1f;
}

float MegaSquirt::getMat(byte celsius) {
	if (celsius) {
		return (((data[offset + 20] << 8) | data[offset + 21]) - 320) * 0.05555f;
	}
	return ((data[offset + 20] << 8) | data[offset + 21]) * 0.1f;
}

float MegaSquirt::getClt(byte celsius) {
	if (celsius) {
		return (((data[offset + 22] << 8) | data[offset + 23]) - 320) * 0.05555f;
	}
	return ((data[offset + 22] << 8) | data[offset + 23]) * 0.1f;
}

float MegaSquirt::getTps() {
	return ((data[offset + 24] << 8) | data[offset + 25]) * 0.1f;
}

float MegaSquirt::getAfr() {
	return ((data[offset + 28] << 8) | data[offset + 29]) * 0.1f;
}

float MegaSquirt::getTaf() {
	return data[offset + 12] * 0.1f;
}

float MegaSquirt::getAdv() {
	return ((data[offset + 8] << 8) | data[offset + 9]) * 0.1f;
}

float MegaSquirt::getPwp() {
	return ((data[offset + 2] << 8) | data[offset + 3]) * 0.000666f;
}

int MegaSquirt::requestData() {
	return runCommand((byte*) (const byte[] ) { 0x0, 0x1, 0x41, 0xD3, 0xD9,
					0x9E, 0x8B }, 7, data, 170);
//        return runCommand((byte*)(const byte []){0x41},1,data,170);

}

void MegaSquirt::requestSignature() {
	runCommand((byte*) (const byte[] ) { 'S' }, 1, data, 59);
}

//private
int MegaSquirt::runCommand(byte cmd[], byte cmdLength, byte data[],
		byte dataLength) {
	// Flush any leftover data from the last command.
	// Read any leftover dat on the line.
	Serial.flush();
	while (Serial.available())
		Serial.read();

	// Send the specified command to the controller.
	byte i;
	for (i = 0; i < cmdLength; i++) {
		Serial.write(cmd[i]);
	}

	unsigned long timeOut;
	int counter;

	// The Serial buffer is 64 bytes, this is less than the largest payload
	// from the controller, as such don't wait, start reading the data right
	// away and don't stop until either the requested number of bytes has
	// been read or the timeout is reached.
	//
	counter = 0;
	timeOut = millis() + 300; // MS WAIT TIMEOUT
	while (counter < dataLength && millis() < timeOut) {
		while (counter < dataLength && Serial.available()) {
			data[counter] = Serial.read();
			++counter;
		}
	}

	// If there is still data pending to be read, raise OVERFLOW error.
	if (Serial.available() > 0 && counter >= dataLength) {
		D(debugSerial->println(F("MS_ERR_COMM_OVERFLOW"));)
		return counter; //MS_ERR_COMM_OVERFLOW
	}

	// If the wrong amount of data was read, return timeout.
	if (counter != dataLength) {
		D(debugSerial->println(F("MS_ERR_COMM_TIMEOUT"));)
		return counter; //MS_ERR_COMM_TIMEOUT
	}

	// Otherwise return success.
	return 1; //MS_COMM_SUCCESS
}
