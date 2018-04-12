#include "LoRa.h"

void tx_f(txData *tx){
printf("tx done \n");
}

int main(){

char txbuf[255];
LoRa_ctl modem;

modem.spiCS = 0;
modem.tx.callback = tx_f;
modem.tx.data.buf = txbuf;
memcpy(modem.tx.data.buf, "LoRa", 5);//Data we'll sent
modem.tx.data.size = 5;//Payload len
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
modem.eth.implicitHeader = 0;
modem.eth.syncWord = 0x12;

LoRa_begin(&modem);

LoRa_send(&modem);

printf("Tsym: %f\n", modem.tx.data.Tsym);
printf("Tpkt: %f\n", modem.tx.data.Tpkt);
printf("payloadSymbNb: %u\n", modem.tx.data.payloadSymbNb);

printf("sleep %d seconds to transmitt complete\n", (int)modem.tx.data.Tpkt/1000);
sleep(((int)modem.tx.data.Tpkt/1000)+1);

printf("end\n");

LoRa_end(&modem);
}
