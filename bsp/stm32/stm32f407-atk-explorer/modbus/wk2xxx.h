/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __wk2xxx_H
#define __wk2xxx_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/****************************************************/
/*External clock=11.0592Mhz*/
#define 	WK2XXX_FREQ_CLK     (11059200)
/*TX FIFO Trigger level*/
#define 	WK2XXX_TX_FIFO_ TRI    (0X10)
/*RX FIFO Trigger level*/
#define 	WK2XXX_RX_FIFO_ TRI    (0X40)

/* WK2xxx Global register 6-bit address definitions */
#define 	WK2XXX_GENA_REG     (0X00)
#define 	WK2XXX_GRST_REG     (0X01)
#define		WK2XXX_GMUT_REG     (0X02)
#define 	WK2XXX_GIER_REG     (0X10)
#define 	WK2XXX_GIFR_REG     (0X11)
	
#define 	WK2XXX_GPDIR_REG    (0X21)	/* only on wk2168/wk2212*/
#define 	WK2XXX_GPDAT_REG    (0X31)	/* only on wk2168/wk2212*/
	 
	 
/*WK2xxx  slave rigister 4-bit address definitions*/

#define 	WK2XXX_SPAGE_REG    (0X03)
/*PAGE0*/
#define 	WK2XXX_SCR_REG      (0X04)
#define 	WK2XXX_LCR_REG      (0X05)  
#define 	WK2XXX_FCR_REG      (0X06)  /* FIFO control */
#define 	WK2XXX_SIER_REG     (0X07)  /* Interrupt enable */
#define 	WK2XXX_SIFR_REG     (0X08)
#define 	WK2XXX_TFCNT_REG    (0X09)  /* RX FIFO cnt */
#define 	WK2XXX_RFCNT_REG    (0X0A)  /* RX FIFO level */
#define 	WK2XXX_FSR_REG      (0X0B)
#define 	WK2XXX_LSR_REG      (0X0C)
#define 	WK2XXX_FDAT_REG     (0X0D)
#define 	WK2XXX_FWCR_REG     (0X0E)
#define 	WK2XXX_RS485_REG    (0X0F)
/*PAGE1*/
#define 	WK2XXX_BAUD1_REG    (0X04)
#define 	WK2XXX_BAUD0_REG    (0X05)
#define 	WK2XXX_PRES_REG     (0X06)
#define 	WK2XXX_RFTL_REG     (0X07)  /* RX FIFO Trigger level */
#define 	WK2XXX_TFTL_REG     (0X08)  /* TX FIFO Trigger level */
#define 	WK2XXX_FWTH_REG     (0X09)
#define 	WK2XXX_FWTL_REG     (0X0A)
#define 	WK2XXX_XON1_REG     (0X0B)  /* Xon1 word */
#define 	WK2XXX_XOFF1_REG    (0X0C)  /* Xoff1 word */
#define 	WK2XXX_SADR_REG     (0X0D)
#define 	WK2XXX_SAEN_REG     (0X0E)
#define 	WK2XXX_RTSDLY_REG   (0X0F)
/********************************************************/
/********************************************************/
/* GENA register bits */
#define 	GENA_UT4EN_BIT				(1 << 3)
#define 	GENA_UT3EN_BIT				(1 << 2)
#define 	GENA_UT2EN_BIT				(1 << 1)
#define 	GENA_UT1EN_BIT				(1 << 0)
//GRST
#define 	GRST_UT4SLEEP_BIT	    (1 << 7)
#define 	GRST_UT3SLEEP_BIT	    (1 << 6)
#define 	GRST_UT2SLEEP_BIT	    (1 << 5)
#define 	GRST_UT1SLEEP_BIT	    (1 << 4)
#define 	GRST_UT4RST_BIT	      (1 << 3)
#define 	GRST_UT3RST_BIT	      (1 << 2)
#define 	GRST_UT2RST_BIT	      (1 << 1)
#define 	GRST_UT1RST_BIT	      (1 << 0)
//GIER
#define 	GIER_UT4IE_BIT	      (1 << 3)
#define 	GIER_UT3IE_BIT	      (1 << 2)
#define 	GIER_UT2IE_BIT	      (1 << 1)
#define 	GIER_UT1IE_BIT	      (1 << 0)
//GIFR
#define 	GIFR_UT4INT_BIT	      (1 << 3)
#define 	GIFR_UT3INT_BIT	      (1 << 2)
#define 	GIFR_UT2INT_BIT	      (1 << 1)
#define 	GIFR_UT1INT_BIT	      (1 << 0)
/* GPDIR register bits */
/* GPDAT register bits */
/* SPAGE register */
#define 	WK2XXX_SPAGE0	        (0x00)
#define 	WK2XXX_SPAGE1         (0x01)
/* SCR register bits */
#define 	SCR_SLEEPEN_BIT	      (1 << 2)
#define 	SCR_TXEN_BIT          (1 << 1)
#define 	SCR_RXEN_BIT          (1 << 0)
/* LCR register bits */
#define 	LCR_BREAK_BIT	        (1 << 5)
#define 	LCR_IREN_BIT          (1 << 4)
#define 	LCR_PAEN_BIT          (1 << 3)
#define 	LCR_PAM1_BIT          (1 << 2)
#define 	LCR_PAM0_BIT          (1 << 1)
#define 	LCR_STPL_BIT          (1 << 0)
/* FCR register bits */
#define 	FCR_TFTRIG1_BIT	      (1 << 7)
#define 	FCR_TFTRIG0_BIT       (1 << 6)
#define 	FCR_RFTRIG1_BIT	      (1 << 5)
#define 	FCR_RFTRIG0_BIT       (1 << 4)
#define 	FCR_TFEN_BIT          (1 << 3)
#define 	FCR_RFEN_BIT          (1 << 2)
#define 	FCR_TFRST_BIT         (1 << 1)
#define 	FCR_RFRST_BIT         (1 << 0)
/* SIER register bits */
#define 	SIER_FERR_IEN_BIT     (1 << 7)
#define 	SIER_CTS_IEN_BIT      (1 << 6)
#define 	SIER_RTS_IEN_BIT      (1 << 5)
#define 	SIER_XOFF_IEN_BIT     (1 << 4)
#define 	SIER_TFEMPTY_IEN_BIT  (1 << 3)
#define 	SIER_TFTRIG_IEN_BIT   (1 << 2)
#define 	SIER_RXOUT_IEN_BIT    (1 << 1)
#define 	SIER_RFTRIG_IEN_BIT   (1 << 0)
/* SIFR register bits */
#define 	SIFR_FERR_INT_BIT     (1 << 7)
#define 	SIFR_CTS_INT_BIT      (1 << 6)
#define 	SIFR_RTS_INT_BIT      (1 << 5)
#define 	SIFR_XOFF_INT_BIT     (1 << 4)
#define 	SIFR_TFEMPTY_INT_BIT  (1 << 3)
#define 	SIFR_TFTRIG_INT_BIT   (1 << 2)
#define 	SIFR_RXOVT_INT_BIT    (1 << 1)
#define 	SIFR_RFTRIG_INT_BIT   (1 << 0)

/* FSR register bits */
#define 	FSR_RFOE_BIT          (1 << 7)
#define 	FSR_RFBI_BIT          (1 << 6)
#define 	FSR_RFFE_BIT          (1 << 5)
#define 	FSR_RFPE_BIT          (1 << 4)
#define 	FSR_RDAT_BIT          (1 << 3)
#define 	FSR_TDAT_BIT          (1 << 2)
#define 	FSR_TFULL_BIT         (1 << 1)
#define 	FSR_TBUSY_BIT         (1 << 0)
/* LSR register bits */
#define 	LSR_OE_BIT            (1 << 3)
#define 	LSR_BI_BIT            (1 << 2)
#define 	LSR_FE_BIT            (1 << 1)
#define 	LSR_PE_BIT            (1 << 0)
/* FWCR register bits */
#define 	FWCR_X2RF_BIT         (1 << 7)
#define 	FWCR_FWM2_BIT         (1 << 6)
#define 	FWCR_FWM1_BIT         (1 << 5)
#define 	FWCR_FWM0_BIT         (1 << 4)
#define 	FWCR_FWTRIG1_BIT      (1 << 3)
#define 	FWCR_FWTRIG0_BIT      (1 << 2)
#define 	FWCR_RTS_BIT          (1 << 1)
#define 	FWCR_CTS_BIT          (1 << 0)
/* RS485 register bits */
#define 	RS485_RS485_BIT       (1 << 6)
#define 	RS485_ATADD_BIT       (1 << 5)
#define 	RS485_DATEN_BIT       (1 << 4)
#define 	RS485_RTSEN_BIT       (1 << 1)
#define 	RS485_RTSINV_BIT      (1 << 0)


#define WK2XXX_UART_STOPBITS_1                     0x00
#define WK2XXX_UART_STOPBITS_2                     ((uint8_t)LCR_STPL_BIT)


/**UART_Parity : UART Parity
  *@ref 
  */
#define WK2XXX_UART_PARITY_NONE                    0x00
#define WK2XXX_UART_PARITY_SPACE                   ((uint8_t)( LCR_PAEN_BIT)) 
#define WK2XXX_UART_PARITY_EVEN                    ((uint8_t)(LCR_PAM1_BIT | LCR_PAEN_BIT))
#define WK2XXX_UART_PARITY_ODD                     ((uint8_t)(LCR_PAM0_BIT | LCR_PAEN_BIT)) 
#define WK2XXX_UART_PARITY_MARK                    ((uint8_t)(LCR_PAM0_BIT | LCR_PAM1_BIT|LCR_PAEN_BIT)) 


/**********************Variable definition**************************/
#ifndef uint8_t
typedef unsigned char                 uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short  int           uint16_t;
#endif

#ifndef uint32_t
typedef unsigned    int           uint32_t;
#endif
/////º¯Êý²¿·Ö
void Wk1xxxRstInit(void);
void Wk1234WriteGlobalRegister(uint8_t greg,uint8_t dat);
uint8_t Wk1234ReadGlobalRegister(uint8_t greg);
void Wk1234WriteSlaveRegister(uint8_t port,uint8_t sreg,uint8_t dat);
uint8_t Wk1234ReadSlaveRegister(uint8_t port,uint8_t sreg);
uint8_t Wk1234WriteSlaveFifo(uint8_t port,uint8_t *dat,uint16_t num);
uint8_t Wk1234ReadSlaveFifo(uint8_t port,uint8_t *rec,uint16_t num);
uint8_t Wk1234MasterUartBaudAdaptive(void);
void Wk1234UartInit(uint8_t port);
void Wk1234UartDeInit(uint8_t port);

uint8_t Wk1234UartSetBaud(uint8_t port,uint32_t baudrate);
uint8_t Wk1234UartFormatSet(uint8_t port,uint8_t stopformat,uint8_t paritytype);

uint16_t Wk1234UartTxLen(uint8_t port);
uint16_t Wk1234UartTxChars(uint8_t port,int len,uint8_t *sendbuf);

uint16_t Wk1234UartRxChars(uint8_t port,uint8_t *recbuf);
void Wk1234UartRS485(uint8_t port);
void Wk1234UartRTSCTS(uint8_t port);
//uint8_t Wk1234UartProcess(void);



void Wk2xxxRstInit(void);
void Wk5678WriteGlobalRegister(uint8_t greg,uint8_t dat);
uint8_t Wk5678ReadGlobalRegister(uint8_t greg);
void Wk5678WriteSlaveRegister(uint8_t port,uint8_t sreg,uint8_t dat);
uint8_t Wk5678ReadSlaveRegister(uint8_t port,uint8_t sreg);
uint8_t Wk5678WriteSlaveFifo(uint8_t port,uint8_t *dat,uint16_t num);
uint8_t Wk5678ReadSlaveFifo(uint8_t port,uint8_t *rec,uint16_t num);
uint8_t Wk5678MasterUartBaudAdaptive(void);
void Wk5678UartInit(uint8_t port);
void Wk5678UartDeInit(uint8_t port);

uint8_t Wk5678UartSetBaud(uint8_t port,uint32_t baudrate);
uint8_t Wk5678UartFormatSet(uint8_t port,uint8_t stopformat,uint8_t paritytype);

uint16_t Wk5678UartTxLen(uint8_t port);
uint16_t Wk5678UartTxChars(uint8_t port,int len,uint8_t *sendbuf);

uint16_t Wk5678UartRxChars(uint8_t port,uint8_t *recbuf);
void Wk5678UartRS485(uint8_t port);
void Wk5678UartRTSCTS(uint8_t port);
uint8_t Wk5678UartProcess(void);

/**END**/
#ifdef __cplusplus
}
#endif

#endif /*__ wk2xxx_H */
