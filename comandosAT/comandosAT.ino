#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8); //Seleccionamos los pines 7 como Rx y 8 como Tx

void setup(){
	SIM900.begin(9600);
	Serial.begin(9600);
	delay(1000);
}

void loop(){
	//Envíamos y recibimos datos
	if (Serial.available() > 0)
		SIM900.write(Serial.read());
	if (SIM900.available() > 0)
		Serial.write(SIM900.read());
}
