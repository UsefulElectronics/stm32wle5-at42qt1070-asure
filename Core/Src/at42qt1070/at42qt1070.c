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
	uint8_t key_buffer;
	uint8_t *register_address;
	at42qt1070_state_t  state;
	circ_buffer_t circ_buffer;
	//Event buffer
}at42qt1070_t;
/* VARIABLES -----------------------------------------------------------------*/
const uint8_t at42qt1070_reg_list[AT42QT1070_REGISTER_COUNT] =
{
AT42QT1070_CHIPID,
AT42QT1070_FIRMWARE,
AT42QT1070_DETECTIONSTATUS,
AT42QT1070_KEYSTATUS ,
AT42QT1070_KEYSIGNAL,
AT42QT1070_REFERENCE	,
AT42QT1070_NEGTHRESHOLD,
AT42QT1070_AVERAGE_ADJACENTKEYSUPPRESSIONLEVEl,
AT42QT1070_DETECTIONINTEGRATORCOUNTER,
AT42QT1070_FOMODI_MAXCAL_GUARDCHANNEL,
AT42QT1070_LOWPOWERMODE,
AT42QT1070_MAXONDURATION,
AT42QT1070_CALIBRATE,
AT42QT1070_RESET,
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

	at42qt1070_handler.state.all 		= 0;

	at42qt1070_handler.register_address = at42qt1070_reg_list;
}


bool at42qt1070_new_event_check(void)
{
	return at42qt1070_handler.state.flag.unread_event;
}

bool at42qt1070_key_stete_read(void)
{
	at42qt1070_handler.send_function(AT42QT1070_SLAVE_ADDRESS,
									&at42qt1070_handler.register_address[SENSOR_KEYSTATUS],
									AT42QT1070_REGISTER_WEDTH);

	at42qt1070_handler.send_function(AT42QT1070_SLAVE_ADDRESS,
									&at42qt1070_handler.key_buffer,
									AT42QT1070_REGISTER_WEDTH);


	at42qt1070_handler.state.flag.unread_event  = at42qt1070_handler.key_buffer ? 1 : 0;

	circ_buffer_enqueue(&at42qt1070_handler.circ_buffer,
						&at42qt1070_handler.key_buffer,
						AT42QT1070_REGISTER_WEDTH);

	//Clear the buffer content
	at42qt1070_handler.key_buffer = 0;

	return at42qt1070_handler.state.flag.unread_event;
}

uint8_t at42qt1070_key_stete_get(void)
{

	uint8_t* key_number  = circ_buffer_dequeue(&at42qt1070_handler.circ_buffer,
												AT42QT1070_REGISTER_WEDTH);
	//Update the unread event flag
	at42qt1070_handler.state.flag.unread_event = circ_buffer_getNumArrays(&at42qt1070_handler.circ_buffer) ? 1 : 0;



	return *key_number;
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
