#include "LoRa.h"

void rx_f(rxData *rx){
    printf("rx done \n");
    printf("CRC error: %d\n", rx->CRC);
    printf("Data size: %d\n", rx->size);
    printf("string: %s\n", rx->buf);//Data we'v received
    printf("RSSI: %d\n", rx->RSSI);
    printf("SNR: %f\n", rx->SNR);
}

int main(){

    char rxbuf[255];
    LoRa_ctl modem;

    //See for typedefs, enumerations and there values in LoRa.h header file
    modem.spiCS = 0;//Raspberry SPI CE pin number
    modem.rx.callback = rx_f;
    modem.rx.data.buf = rxbuf;
    modem.eth.preambleLen=6;
    modem.eth.bw = BW62_5;//Bandwidth 62.5KHz
    modem.eth.sf = SF12;//Spreading Factor 12
    modem.eth.ecr = CR8;//Error coding rate CR4/8
    modem.eth.freq = 434800000;// 434.8MHz
    modem.eth.resetGpioN = 4;//GPIO4 on lora RESET pi
    modem.eth.dio0GpioN = 17;//GPIO17 on lora DIO0 pin to control Rxdone and Txdone interrupts
    modem.eth.outPower = OP20;//Output power
    modem.eth.powerOutPin = PA_BOOST;//Power Amplifire pin
    modem.eth.AGC = 1;//Auto Gain Control
    modem.eth.OCP = 240;// 45 to 240 mA. 0 to turn off protection
    modem.eth.implicitHeader = 0;//Explicit header mode
    modem.eth.syncWord = 0x12;
    //For detail information about SF, Error Coding Rate, Explicit header, Bandwidth, AGC, Over current protection and other features refer to sx127x datasheet https://www.semtech.com/uploads/documents/DS_SX1276-7-8-9_W_APP_V5.pdf

    LoRa_begin(&modem);
    LoRa_receive(&modem);

    sleep(60);
    printf("end\n");
    LoRa_end(&modem);
}
