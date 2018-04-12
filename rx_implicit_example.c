#include "LoRa.h"

void rx_f(rxData *rx){
printf("rx done \n");
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
modem.eth.preambleLen=6;
modem.eth.bw = BW62_5;
modem.eth.sf = SF12;
modem.eth.ecr = CR8;
modem.eth.CRC = 1;
modem.eth.freq = 434800000;
modem.eth.resetGpioN = 4;
modem.eth.dio0GpioN = 17;
modem.eth.outPower = OP20;
modem.eth.powerOutPin = PA_BOOST;
modem.eth.AGC = 1;
modem.eth.OCP = 240;
modem.eth.payloadLen = 5;
modem.eth.implicitHeader = 1;
modem.eth.syncWord = 0x12;

LoRa_begin(&modem);

LoRa_receive(&modem);
printf("Tsym: %f\n", modem.tx.data.Tsym);
printf("Tpkt: %f\n", modem.tx.data.Tpkt);
printf("payloadSymbNb: %u\n", modem.tx.data.payloadSymbNb);

sleep(60);
printf("end\n");
LoRa_end(&modem);
}
