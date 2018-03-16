#include <ThreadController.h>
#include <Thread.h>
#include <SPI.h>
#include "mcp_can.h"  

Thread Sender;

MCP_CAN CAN(10);                           

String data = "";

unsigned char flagRecv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];

void setup() {
	Serial.begin(115200);
	while (!Serial) delay(100);

	while (CAN_OK != CAN.begin(CAN_250KBPS, MCP_8MHz)){
		Serial.println("CAN BUS Shield init fail");
		Serial.println(" Init CAN BUS Shield again");
		delay(100);
	}
	Serial.println("CAN BUS Shield init ok!");

	attachInterrupt(0, MCP2515_ISR, FALLING); // start interrupt
}

void MCP2515_ISR() {
	flagRecv = 1;
}

void loop() {
	if(flagRecv) {        
		flagRecv = 0;    
		while(CAN_MSGAVAIL == CAN.checkReceive()) {
			Serial.print("ID -> 0x");
			Serial.print(CAN.getCanId(), HEX);
			Serial.print(" | Data -> ");

			CAN.readMsgBuf(&len, buf);

			data = String((char*)buf);

			Serial.println(data.substring(0, 8));

		}
	}
}