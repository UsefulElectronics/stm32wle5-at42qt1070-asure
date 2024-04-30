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

/* MACROS --------------------------------------------------------------------*/
#define AT42QT1070_SLAVE_ADDRESS		0x1B << 1


#define AT42QT1070_CHIPID 					0 //!< should be 0x2E
#define AT42QT1070F_FIRMWARE		 		1 //!< could be 0x15 ;)
#define AT42QT1070F_DETECTIONSTATUS			2 //!<Bit7=calibrate, Bit6=Overflow, Bit0=Touch
#define AT42QT1070F_KEYSTATUS 				3 //!< Bit=Key
#define AT42QT1070F_KEYSIGNAL				4 //!< MSB,LSB of signal
#define AT42QT1070F_REFERENCE				18//!<MSB,LSB of reference value (after calibration these change)
												// subsequent are R/W
#define AT42QT1070F_NEGTHRESHOLD 			32//!< don't set 0, default 20
#define AT42QT1070F_AVERAGE_ADJACENTKEYSUPPRESSIONLEVEl 39
												//  BIT7-BIT2 average of samples (allowed 1,2,4,8,16,32) default 8
												// BIT2-BIT0=Define AKS group (0..3)
#define AT42QT1070F_DETECTIONINTEGRATORCOUNTER 46 // min is 2, number of consecutive threshold
#define AT42QT1070F_FOMODI_MAXCAL_GUARDCHANNEL 53
											   // Bit5 Fas out DI ,
											   //Bit4 calibrate all when Max On Reached if bit 0
											   // Bit3-Bit0 Guard channel (0..6)
#define AT42QT1070F_LOWPOWERMODE			54 // 0=8ms,1=8ms,2=16ms,3=24ms, 4=32ms, 254=2.032,255=2.040 (default 2!)
#define AT42QT1070F_MAXONDURATION			55 // 0 = off 1=160ms, 2=320ms... 255=40.8 seconds
#define AT42QT1070F_CALIBRATE				56 // write non zero for calibration, check status register
#define AT42QT1070F_RESET 					57 // write non zero for reset

#define AT42QT1070F_REGISTER_COUNT			15 // Accessible register count
/// Detection status Register bit fields
#define AT42QT1070F_TOUCHBIT				0x01
#define AT42QT1070F_OVERFLOWBIT 			0x40
#define AT42QT1070F_CALIBRATEBIT			0x80
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/



#endif /* SRC_AT42QT1070_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
