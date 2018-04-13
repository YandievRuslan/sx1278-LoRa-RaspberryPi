#include "LoRa.h"

void rx_f(rxData *rx){
printf("rx done \n");
printf("CRC error: %d\n", rx->CRC);
printf("string: %s\n", rx->buf);//Data we'v received
printf("RSSI: %d\n", rx->RSSI);
printf("SNR: %f\n", rx->SNR);
}

int main(){

char rxbuf[255];
LoRa_ctl modem;

modem.spiCS = 0;
modem.rx.callback = rx_f;
modem.rx.data.buf = rxbuf;
modem.eth.payloadLen = 5;//payload len used in implicit header mode
modem.eth.preambleLen=6;
modem.eth.bw = BW62_5;//Bandwidth 62.5KHz
modem.eth.sf = SF12;//Spreading Factor 12
modem.eth.ecr = CR8;//Error coding rate CR4/8
modem.eth.CRC = 1;//Turn on CRC checking
modem.eth.freq = 434800000;// 434.8MHz
modem.eth.resetGpioN = 4;//GPIO4 on lora RESET pi
modem.eth.dio0GpioN = 17;//GPIO17 on lora DIO0 pin to control Rxdone and Txdone interrupts
modem.eth.outPower = OP20;//Output power
modem.eth.powerOutPin = PA_BOOST;//Power Amplifire pin
modem.eth.AGC = 1;//Auto Gain Control
modem.eth.OCP = 240;// 45 to 240 mA. 0 to turn off protection
modem.eth.implicitHeader = 1;//Implicit header mode
modem.eth.syncWord = 0x12;

LoRa_begin(&modem);
LoRa_receive(&modem);

sleep(60);
printf("end\n");
LoRa_end(&modem);
}
