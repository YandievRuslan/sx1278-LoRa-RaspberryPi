#include "LoRa.h"
#include <stdio.h>

void tx_f(txData *tx){
    LoRa_ctl *modem = (LoRa_ctl *)(tx->userPtr);
    fprintf(stderr, "SENT: %s", tx->buf); 
    fflush(stderr);
    LoRa_receive(modem);
}

void rx_f(rxData *rx){
    LoRa_ctl *modem = (LoRa_ctl *)(rx->userPtr);
    fprintf(stderr, "SNR: %f\t", rx->SNR);
    fprintf(stderr, "RSSI: %d\t", rx->RSSI);
    fprintf(stderr, "CRC: %d\t", rx->CRC);
    fprintf(stderr, "Size: %d\n", rx->size);
    fflush(stderr);
    fprintf(stdout, "%s", rx->buf); 
    fflush(stdout);
}

int main(){

    char txbuf[255];
    char rxbuf[255];
    LoRa_ctl modem;

    //See for typedefs, enumerations and there values in LoRa.h header file
    modem.spiCS = 0;//Raspberry SPI CE pin number
    modem.tx.callback = tx_f;
    modem.tx.data.buf = txbuf;
    modem.rx.callback = rx_f;
    modem.rx.data.buf = rxbuf;
    modem.rx.data.userPtr = (void *)(&modem);//To handle with chip from rx callback
    modem.tx.data.userPtr = (void *)(&modem);//To handle with chip from tx callback
    modem.eth.preambleLen=6;
    modem.eth.bw = BW62_5;//Bandwidth 62.5KHz
    modem.eth.sf = SF12;//Spreading Factor 12
    modem.eth.ecr = CR8;//Error coding rate CR4/8
    modem.eth.CRC = 1;//Turn on CRC checking
    modem.eth.freq = 434800000;// 434.8MHz
    modem.eth.resetGpioN = 4;//GPIO4 on lora RESET pin
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

    while(LoRa_get_op_mode(&modem) != SLEEP_MODE){
        char mybuf[BUFSIZ];
        fgets(mybuf, sizeof mybuf, stdin);
        if (mybuf[strlen(mybuf)-1] == '\n') {
            memcpy(modem.tx.data.buf, mybuf, strlen(mybuf) + 1);//copy data we'll sent to buffer
            modem.tx.data.size = strlen(mybuf) + 1;//Payload len
            while(LoRa_get_op_mode(&modem) != RXCONT_MODE){
                fprintf(stderr, "Waiting for last transmit to finish.\n"); 
                sleep(3);
            }
            LoRa_send(&modem);
            sleep(1);
            memset(mybuf,0,strlen(mybuf));
        }
        sleep(1);
    }

    printf("end\n");

    LoRa_end(&modem);
}
