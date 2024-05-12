/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  at42qt1070.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Apr 27, 2024
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef SRC_AT42QT1070_H_
#define SRC_AT42QT1070_H_


/* INCLUDES ------------------------------------------------------------------*/
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
/* MACROS --------------------------------------------------------------------*/
#define AT42QT1070_SLAVE_ADDRESS		0x1B << 1


#define AT42QT1070_CHIPID 					0 //!< should be 0x2E
#define AT42QT1070_FIRMWARE		 			1 //!< could be 0x15 ;)
#define AT42QT1070_DETECTIONSTATUS			2 //!<Bit7=calibrate, Bit6=Overflow, Bit0=Touch
#define AT42QT1070_KEYSTATUS 				3 //!< Bit=Key
#define AT42QT1070_KEYSIGNAL				4 //!< MSB,LSB of signal
#define AT42QT1070_REFERENCE				18//!<MSB,LSB of reference value (after calibration these change)
											  // subsequent are R/W
#define AT42QT1070_NEGTHRESHOLD 			32//!< don't set 0, default 20
#define AT42QT1070_AVERAGE_ADJACENTKEYSUPPRESSIONLEVEl 39
												 //  BIT7-BIT2 average of samples (allowed 1,2,4,8,16,32) default 8
												 // BIT2-BIT0=Define AKS group (0..3)
#define AT42QT1070_DETECTIONINTEGRATORCOUNTER 46 // min is 2, number of consecutive threshold
#define AT42QT1070_FOMODI_MAXCAL_GUARDCHANNEL 53
											   // Bit5 Fas out DI ,
											   //Bit4 calibrate all when Max On Reached if bit 0
											   // Bit3-Bit0 Guard channel (0..6)
#define AT42QT1070_LOWPOWERMODE				54 // 0=8ms,1=8ms,2=16ms,3=24ms, 4=32ms, 254=2.032,255=2.040 (default 2!)
#define AT42QT1070_MAXONDURATION			55 // 0 = off 1=160ms, 2=320ms... 255=40.8 seconds
#define AT42QT1070_CALIBRATE				56 // write non zero for calibration, check status register
#define AT42QT1070_RESET 					57 // write non zero for reset

#define AT42QT1070_REGISTER_COUNT			15 // Accessible register count
#define AT42QT1070_REGISTER_WEDTH			1  // byte
/// Detection status Register bit fields
#define AT42QT1070_TOUCHBIT				0x01
#define AT42QT1070_OVERFLOWBIT 			0x40
#define AT42QT1070_CALIBRATEBIT			0x80
/// Change Pin pull state
#define AT42QT1070_IDLE_EVENT			0x01
#define AT42QT1070_NEW_EVENT			0x00
/// Timing
#define AT42QT1070_LONG_PRESSED_PERIOD	1000	// 1 seconds

/* ENUMORATIONS --------------------------------------------------------------*/
typedef enum
{
	SENSOR_CHIPID,
	SENSOR_FIRMWARE,
	SENSOR_DETECTIONSTATUS,
	SENSOR_KEYSTATUS ,
	SENSOR_KEYSIGNAL,
	SENSOR_REFERENCE	,
	SENSOR_NEGTHRESHOLD,
	SENSOR_AVERAGE_ADJACENTKEYSUPPRESSIONLEVEl,
	SENSOR_DETECTIONINTEGRATORCOUNTER,
	SENSOR_FOMODI_MAXCAL_GUARDCHANNEL,
	SENSOR_LOWPOWERMODE,
	SENSOR_MAXONDURATION,
	SENSOR_CALIBRATE,
	SENSOR_RESET,
}at42qt1070_e;

typedef enum
{
	SENSOR_KEY_PRESSED,
	SENSOR_KEY_LONG_PRESSED,
	SENSOR_KEY_RELEASE,
}at42qt1070_event_e;
/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void at42qt1070_init(void* send_function, void* receive_fucntion, void* change_state_read);

bool at42qt1070_callback(void);


uint8_t at42qt1070_key_stete_get(void);


#endif /* SRC_AT42QT1070_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
