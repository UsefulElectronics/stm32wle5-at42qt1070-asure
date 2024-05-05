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
// Define a structure to hold chip IDs
struct register_set_t
{
    uint8_t at42qt1070_chipid;
    uint8_t at42qt1070_firmware;
	uint8_t at42qt1070_detectionstatus;
	uint8_t at42qt1070_keystatus;
	uint8_t at42qt1070_keysignal;
	uint8_t at42qt1070_reference;
	uint8_t at42qt1070_negthreshold;
	uint8_t at42qt1070_suppressionlevel;
	uint8_t at42qt1070_integratorcounter;
	uint8_t at42qt1070_guardchannel;
	uint8_t at42qt1070_lowpowermode;
	uint8_t at42qt1070_maxonduration;
	uint8_t at42qt1070_calibrate;
	uint8_t at42qt1070_reset;
    // Other chip IDs can be added here
};

const struct register_set_t register_set =
{
	.at42qt1070_chipid 					= AT42QT1070_CHIPID,
	.at42qt1070_firmware 				= AT42QT1070_FIRMWARE,
	.at42qt1070_detectionstatus 		= AT42QT1070_DETECTIONSTATUS,
	.at42qt1070_keystatus				= AT42QT1070_KEYSTATUS,
	.at42qt1070_keysignal				= AT42QT1070_KEYSIGNAL,
	.at42qt1070_reference				= AT42QT1070_REFERENCE,
	.at42qt1070_negthreshold			= AT42QT1070_NEGTHRESHOLD,
	.at42qt1070_suppressionlevel		= AT42QT1070_AVERAGE_ADJACENTKEYSUPPRESSIONLEVEl,
	.at42qt1070_integratorcounter		= AT42QT1070_DETECTIONINTEGRATORCOUNTER,
	.at42qt1070_guardchannel			= AT42QT1070_FOMODI_MAXCAL_GUARDCHANNEL,
	.at42qt1070_lowpowermode			= AT42QT1070_LOWPOWERMODE,
	.at42qt1070_maxonduration			= AT42QT1070_MAXONDURATION,
	.at42qt1070_calibrate				= AT42QT1070_CALIBRATE,
	.at42qt1070_reset					= AT42QT1070_RESET,
};

typedef struct
{
	void	(*i2c_send)(uint16_t slave_address, uint8_t *data_buffer, uint16_t size);
	void	(*i2c_receive)(uint16_t slave_address, uint8_t *data_buffer, uint16_t size);
	void	(*change_pin_read)(void);
	uint8_t key_buffer;
	const struct register_set_t *register_address;
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

	at42qt1070_handler.register_address = &register_set;
}


bool at42qt1070_new_event_check(void)
{
	return at42qt1070_handler.state.flag.unread_event;
}

bool at42qt1070_key_stete_read(void)
{
	at42qt1070_handler.i2c_send(AT42QT1070_SLAVE_ADDRESS,
								&at42qt1070_handler.register_address->at42qt1070_keystatus,
								AT42QT1070_REGISTER_WEDTH);

	at42qt1070_handler.i2c_receive(AT42QT1070_SLAVE_ADDRESS,
									&at42qt1070_handler.key_buffer,
									AT42QT1070_REGISTER_WEDTH);

	if(at42qt1070_handler.key_buffer)
	{
		circ_buffer_enqueue(&at42qt1070_handler.circ_buffer,
							&at42qt1070_handler.key_buffer,
							AT42QT1070_REGISTER_WEDTH);

		at42qt1070_handler.state.flag.unread_event  = true;
	}



	//Clear the buffer content
	at42qt1070_handler.key_buffer = 0;

	return at42qt1070_handler.state.flag.unread_event;
}

bool at42qt1070_callback(void)
{
	bool new_event = false;

	if(!at42qt1070_handler.change_pin_read())
	{
		new_event = at42qt1070_key_stete_read();
	}

	return new_event;
}


uint8_t at42qt1070_key_stete_get(void)
{

	uint8_t* key_number  = circ_buffer_dequeue(&at42qt1070_handler.circ_buffer,
												NULL);
	//Update the unread event flag
	at42qt1070_handler.state.flag.unread_event = circ_buffer_getNumArrays(&at42qt1070_handler.circ_buffer) ? 1 : 0;



	return *key_number;
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
