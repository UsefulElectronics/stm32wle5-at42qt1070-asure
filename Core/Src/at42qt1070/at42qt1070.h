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
#define AT42QT1070_LONG_PRESSED_PERIOD	200	// 2 seconds

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
	SENSOR_KEY_IDLE,
	SENSOR_KEY_PRESSED,
	SENSOR_KEY_LONG_PRESSED,
	SENSOR_KEY_RELEASE,
}at42qt1070_event_e;

typedef enum
{
	SENSOR_CHANGE_PIN_LOW	= 0,	//When new event is active
	SENSOR_CHANGE_PIN_HIGH,			//While the sensor is IDLE
}at42qt1070_change_pin_e;
/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
/**
 * @brief Initializes the AT42QT1070 touch sensor handler.
 *
 * This function sets up the AT42QT1070 touch sensor handler by assigning the provided function pointers
 * for I2C communication, state change detection, and tick retrieval. It also initializes the handler's state,
 * sets the register address, and performs a reset and calibration of the touch sensor.
 *
 * @param send_function Pointer to the function used to send data via I2C.
 * @param receive_function Pointer to the function used to receive data via I2C.
 * @param change_state_read Pointer to the function used to read the state change pin.
 * @param get_tick Pointer to the function used to get the current tick count.
 */
void at42qt1070_init(uint8_t* send_function, uint8_t* receive_fucntion, uint8_t* change_state_read, uint32_t* get_tick);
/**
 * @brief Callback function to handle AT42QT1070 touch sensor events.
 *
 * This function checks the state of the change pin using the provided read function. If a change is detected,
 * it determines the type of key event and, if the pin state indicates a new event, it reads the key state.
 * The function returns a boolean indicating whether a new event has occurred.
 *
 * @return true if a new event has occurred, false otherwise.
 */
bool at42qt1070_callback(void);
/**
 * @brief Retrieves the key state from the circular buffer.
 *
 * This function dequeues a key number from the AT42QT1070 handler's circular buffer and updates the
 * unread event flag based on the number of arrays remaining in the buffer. It returns the dequeued key number.
 *
 * @return The key number retrieved from the circular buffer.
 */
uint8_t at42qt1070_key_stete_get(void);
/**
 * @brief Calibrates the AT42QT1070 touch sensor.
 *
 * This function sends a calibration command to the AT42QT1070 touch sensor using the I2C send function
 * provided during initialization. The calibration command is constructed and sent to the sensor's
 * slave address.
 */
void at42qt1070_calibrate(void);
/**
 * @brief Resets the AT42QT1070 touch sensor.
 *
 * This function sends a reset command to the AT42QT1070 touch sensor using the I2C send function
 * provided during initialization. The reset command is constructed and sent to the sensor's
 * slave address.
 */
void at42qt1070_reset(void);


#endif /* SRC_AT42QT1070_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
