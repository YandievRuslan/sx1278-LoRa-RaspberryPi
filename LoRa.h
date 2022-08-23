#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>

#define REG_FIFO 0x00
#define REG_OP_MODE 0x01
#define REG_FR_MSB 0x06
#define REG_FR_DIM 0x07
#define REG_FR_LSB 0x08
#define REG_PA_CONFIG 0x09
#define REG_OCP 0x0B
#define REG_LNA 0x0C
#define REG_FIFO_ADDR_PTR 0x0D
#define REG_FIFO_TX_BASE_ADDR 0x0E
#define REG_FIFO_RX_BASE_ADDR 0x0F
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS 0x12
#define REG_RX_NB_BYTES 0x13
#define REG_RX_HEADER_CNT_VALUE_MSB 0x14
#define REG_RX_HEADER_CNT_VALUE_LSB 0x15
#define REG_RX_PACKET_CNT_VALUE_MSB 0x16
#define REG_RX_PACKET_CNT_VALUE_LSB 0x17
#define REG_PKT_SNR_VALUE 0x19
#define REG_PKT_RSSI_VALUE 0x1A
#define REG_RSSI_VALUE 0x1B
#define REG_MODEM_CONFIG_1 0x1D
#define REG_MODEM_CONFIG_2 0x1E
#define REG_MODEM_CONFIG_3 0x26
#define REG_PAYLOAD_LENGTH 0x22
#define REG_FIFO_RX_BYTE_ADDR 0x25
#define REG_PA_DAC 0x4d
#define REG_DIO_MAPPING_1 0x40
#define REG_DIO_MAPPING_2 0x41
#define REG_TEMP 0x3c
#define REG_SYNC_WORD 0x39
#define REG_PREAMBLE_MSB 0x20
#define REG_PREAMBLE_LSB 0x21
#define REG_DETECT_OPTIMIZE 0x31
#define REG_DETECTION_THRESHOLD 0x37

#define TX_BASE_ADDR 0x00
#define RX_BASE_ADDR 0x00

#define LORA_MODE 0x80

#define SLEEP_MODE 0x00
#define STDBY_MODE 0x01
#define TX_MODE 0x03
#define RXCONT_MODE 0x05

#define IRQ_RXDONE 0x40
#define IRQ_TXDONE 0x08

typedef enum BandWidth{
    BW7_8 =0,
    BW10_4 = 1<<4,
    BW15_6 = 2<<4,
    BW20_8 = 3<<4,
    BW31_25 = 4<<4,
    BW41_7 = 5<<4,
    BW62_5 = 6<<4,
    BW125 = 7<<4,
    BW250 = 8<<4,
    BW500 = 9<<4,
} BandWidth;

typedef enum SpreadingFactor{
    SF7 = 7<<4,
    SF8 = 8<<4,
    SF9 = 9<<4,
    SF10 = 10<<4,
    SF11 = 11<<4,
    SF12 = 12<<4,
} SpreadingFactor;

typedef enum ErrorCodingRate{
    CR5 = 1<<1,
    CR6 = 2<<1,
    CR7 = 3<<1,
    CR8 = 4<<1,
} ErrorCodingRate;

typedef enum OutputPower{
    OP0 = 0,
    OP1 = 1,
    OP2 = 2,
    OP3 = 3,
    OP4 = 4,
    OP5 = 5,
    OP6 = 6,
    OP7 = 7,
    OP8 = 8,
    OP9 = 9,
    OP10 = 10,
    OP11 = 11,
    OP12 = 12,
    OP13 = 13,
    OP14 = 14,
    OP15 = 15,
    OP16 = 16,
    OP17 = 17,
    OP20 = 20,
} OutputPower;

typedef enum PowerAmplifireOutputPin{
    RFO = 0x70,
    PA_BOOST = 0xf0,
} PowerAmplifireOutputPin;

typedef enum LnaGain{
    G1 = 1,
    G2 = 2,
    G3 = 3,
    G4 = 4,
    G5 = 5,
    G6 = 6,
} LnaGain;

typedef struct{
    BandWidth bw;
    SpreadingFactor sf; //only from SF7 to SF12. SF6 not support yet.
    ErrorCodingRate ecr;
    double freq;// Frequency in Hz. Example 434000000
    unsigned char resetGpioN;//raspberry GPIO pin connected to RESET pin of LoRa chip
    unsigned char dio0GpioN;//raspberry GPIO pin connected to DIO0 pin of LoRa chip to detect TX and Rx done events.
    unsigned int preambleLen;
    _Bool lowDataRateOptimize;//Dont touch it sets automatically
    OutputPower outPower;
    PowerAmplifireOutputPin powerOutPin;//This chips has to outputs for signal "High power" and regular.
    int curRSSI;//Current ethereum RSSI
    unsigned char syncWord;
    LnaGain lnaGain;
    _Bool lnaBoost;//On/Off LNA boost
    _Bool AGC;// On/Off AGC. If AGC is on, LNAGain not used
    unsigned char OCP;//Over Current Protection. 0 to turn OFF. Else reduces current from 45mA to 240mA
    _Bool implicitHeader;// 1 - implicit header. 0 - Explicit header.
    unsigned char payloadLen;//Payload len that used in implicit mode. In Explicit header mode not used.
    _Bool CRC;//1 - add CRC data and checking. 0 - remove CRC data and checking
} Modem_cfg;

typedef struct{
    char *buf;
    unsigned char size;//Size of buffer. Used in Explicit header mode. 255 MAX size
    struct timeval last_time;
    double Tsym;
    double Tpkt;
    unsigned payloadSymbNb;
    void *userPtr;//user pointer passing to user callback
} txData;

typedef void (*UserTxDoneCallback)(txData *tx);

typedef struct{
    txData data;
    UserTxDoneCallback callback;
    pthread_t cbThread;
} LoRa_Tx;

typedef void (*txDoneISR)(int gpio_n, int level, uint32_t tick, void *userdata);

typedef struct{
    char buf[256];
    unsigned char size;
    struct timeval last_time;
    float SNR;
    int RSSI;
    _Bool CRC;
    void *userPtr;//user pointer passing to user callback
} rxData;

typedef void * (*UserRxDoneCallback)(void *rx);

typedef struct{
    rxData data;
    UserRxDoneCallback callback;
    pthread_t cbThread;
} LoRa_Rx;

typedef void (*rxDoneISR)(int gpio_n, int level, uint32_t tick, void *userdata);

typedef struct {
    unsigned char spiCS;//SPI CS number
    int spid;//SPI file descriptor
    Modem_cfg eth;//ethereum config structure
    LoRa_Rx rx;//rx structure
    LoRa_Tx tx;//tx structure
} LoRa_ctl;

int LoRa_begin(LoRa_ctl *modem);
void LoRa_send(LoRa_ctl *modem);//After sending auto switches to standby mode.
void LoRa_receive(LoRa_ctl *modem);//Continuous mode. You have to manually stop receiving if you want to. For example you can do it in callback right after catching packet.
void LoRa_calculate_packet_t(LoRa_ctl *modem);
_Bool LoRa_check_conn(LoRa_ctl *modem);
void LoRa_end(LoRa_ctl *modem);
void LoRa_stop_receive(LoRa_ctl *modem);
void LoRa_sleep(LoRa_ctl *modem);
unsigned char LoRa_get_op_mode(LoRa_ctl *modem);// Compare with SLEEP_MODE, RXCONT_MODE, SLEEP_MODE, STDBY_MODE

void lora_set_syncw(int spid, unsigned char word);
void lora_set_lora_mode(int spid);
void lora_set_sf(int spid, SpreadingFactor sf);
void lora_set_crc_on(int spid);
void lora_set_crc_off(int spid);
void lora_set_bandwidth(int spid, BandWidth bw);
void lora_set_errorcr(int spid, ErrorCodingRate cr);
void lora_set_freq(int spid, double freq);
void lora_set_payload(int spid, unsigned char payload);
void lora_reset(unsigned char gpio_n);
void lora_reset_irq_flags(int spid);
unsigned char lora_reg_read_byte(int spid, unsigned char reg);
int lora_reg_write_byte(int spid, unsigned char reg, unsigned char byte);
int lora_reg_read_bytes(int spid, unsigned char reg, char *buff, unsigned char size);
int lora_reg_write_bytes(int spid, unsigned char reg, char *buff, unsigned char size);
void lora_set_rxcont_mode(int spid);
void lora_set_tx_mode(int spid);
void lora_set_sleep_mode(int spid);
void lora_set_satandby_mode(int spid);
void lora_set_tx_power(int spid, OutputPower power, PowerAmplifireOutputPin pa_pin);
void rxDoneISRf(int gpio_n, int level, uint32_t tick, void *modem);
void txDoneISRf(int gpio_n, int level, uint32_t tick, void *modem);
void * startRxCallback(void *arg);
void * startTxCallback(void *arg);
void lora_set_dio_rx_mapping(int spid);
void lora_set_dio_tx_mapping(int spid);
void lora_set_rxdone_dioISR(int gpio_n, rxDoneISR func, LoRa_ctl *modem);
void lora_set_txdone_dioISR(int gpio_n, txDoneISR func, LoRa_ctl *modem);
void lora_remove_dioISR(int gpio_n);
unsigned char lora_get_op_mode(int spid);
void lora_set_addr_ptr(int spid, unsigned char addr);
void lora_set_lowdatarateoptimize_on(int spid);
void lora_set_lowdatarateoptimize_off(int spid);
unsigned char lora_write_fifo(int spid, char *buf, unsigned char size);
void lora_get_rssi_pkt(LoRa_ctl *modem);
void lora_get_rssi_cur(LoRa_ctl *modem);
void lora_get_snr(LoRa_ctl *modem);
void lora_set_preamble(int spid, unsigned int preambleLen);
void lora_set_agc(int spid, _Bool AGC );
void lora_set_lna(int spid, LnaGain lnaGain, _Bool lnaBoost);
void lora_set_ocp(int spid, unsigned char OCP);
void lora_set_implicit_header(int spid);
void lora_set_explicit_header(int spid);
