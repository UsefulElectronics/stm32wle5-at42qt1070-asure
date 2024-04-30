/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  at42qt1070.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Apr 27, 2024
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "at42qt1070.h"
#include "stdlib.h"
#include "stdbool.h"
#include "circ_buffer.h"
/* PRIVATE STRUCTRES ---------------------------------------------------------*/
typedef union
{
	struct
	{
		uint8_t change_pin 	: 1,
				unread_event: 1,
				reserved	: 6;
	}flag;
	uint8_t all;
}at42qt1070_state_t;
typedef struct
{
	void	(*i2c_send)(uint16_t slave_address, uint8_t *data_buffer, uint16_t size);
	void	(*i2c_receive)(uint16_t slave_address, uint8_t *data_buffer, uint16_t size);
	void	(*change_pin_read)(void);
	at42qt1070_state_t  state;
	//Event buffer
}at42qt1070_t;
/* VARIABLES -----------------------------------------------------------------*/
const uint8_t at42qt1070_reg_list[AT42QT1070F_REGISTER_COUNT] =
{
AT42QT1070_CHIPID,
AT42QT1070F_FIRMWARE,
AT42QT1070F_DETECTIONSTATUS,
AT42QT1070F_KEYSTATUS ,
AT42QT1070F_KEYSIGNAL,
AT42QT1070F_REFERENCE	,
AT42QT1070F_NEGTHRESHOLD,
AT42QT1070F_AVERAGE_ADJACENTKEYSUPPRESSIONLEVEl,
AT42QT1070F_DETECTIONINTEGRATORCOUNTER,
AT42QT1070F_FOMODI_MAXCAL_GUARDCHANNEL,
AT42QT1070F_LOWPOWERMODE,
AT42QT1070F_MAXONDURATION,
AT42QT1070F_CALIBRATE,
AT42QT1070F_RESET,
};

at42qt1070_t at42qt1070_handler = {0};
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void at42qt1070_init(void* send_function, void* receive_fucntion, void* change_state_read)
{
	at42qt1070_handler.i2c_send 		= send_function;

	at42qt1070_handler.i2c_receive	 	= receive_fucntion;

	at42qt1070_handler.change_pin_read	= change_state_read;

	at42qt1070_init(&premetive_buffer);

	ryuw122_set_mode(hRyuw122.operation_mode);


}

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
