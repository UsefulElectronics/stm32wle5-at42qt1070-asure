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

#include "circ_buffer.h"
/* PRIVATE STRUCTRES ---------------------------------------------------------*/
typedef union
{
	struct
	{
		uint8_t change_pin 	: 1,
				unread_event: 1,
				event_type  : 2,
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
	uint8_t	(*i2c_send)(uint16_t slave_address, uint8_t *data_buffer, uint16_t size);
	uint8_t	(*i2c_receive)(uint16_t slave_address, uint8_t *data_buffer, uint16_t size);
	uint8_t	(*change_pin_read)(void);
	uint8_t	(*event_report)(void);
	uint8_t key_buffer;
	uint32_t(*get_tick)(void);
	const struct register_set_t *register_address;
	at42qt1070_state_t  state;
	circ_buffer_t circ_buffer;
	//Event buffer
}at42qt1070_t;
/* VARIABLES -----------------------------------------------------------------*/

at42qt1070_t at42qt1070_handler = {0};
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
/**
 * @brief Handles key event type based on the current state of the change pin.
 *
 * This function updates the key event type based on the current state of the change pin and the
 * duration the key has been pressed. It handles transitions between idle, pressed, released, and
 * long-pressed states.
 *
 * @param current_change_pin_state The current state of the change pin.
 * @return The current key event type.
 */
static at42qt1070_event_e at42qt1070_key_event_type_handler(bool current_change_pin_state);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void at42qt1070_init(uint8_t* send_function, uint8_t* receive_fucntion, uint8_t* change_state_read, uint32_t* get_tick)
{
	at42qt1070_handler.i2c_send 		= send_function;

	at42qt1070_handler.i2c_receive	 	= receive_fucntion;

	at42qt1070_handler.change_pin_read	= change_state_read;

	at42qt1070_handler.get_tick			= get_tick;

	at42qt1070_handler.state.all 		= 0;

	at42qt1070_handler.register_address = &register_set;

	at42qt1070_reset();

	at42qt1070_calibrate();
}


bool at42qt1070_new_event_check(void)
{
	return at42qt1070_handler.state.flag.unread_event;
}

bool at42qt1070_key_stete_read(void)
{
	if(at42qt1070_handler.i2c_send != NULL)
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
	return 0;

}

bool at42qt1070_callback(void)
{
	if(at42qt1070_handler.change_pin_read != NULL)
	{
		bool new_event = false;

		at42qt1070_change_pin_e pin_state = at42qt1070_handler.change_pin_read();

		at42qt1070_event_e event_type = at42qt1070_key_event_type_handler(pin_state);

		if(!pin_state)
		{
			new_event = at42qt1070_key_stete_read();
		}

		return new_event;
	}
	return 0;
}


uint8_t at42qt1070_key_stete_get(void)
{

	uint8_t* key_number  = circ_buffer_dequeue(&at42qt1070_handler.circ_buffer,
												NULL);
	//Update the unread event flag
	at42qt1070_handler.state.flag.unread_event = circ_buffer_getNumArrays(&at42qt1070_handler.circ_buffer) ? 1 : 0;



	return *key_number;
}

void at42qt1070_calibrate(void)
{
	const uint8_t packet_size = AT42QT1070_REGISTER_WEDTH * 2;

	uint8_t temp_buffer[2] = {at42qt1070_handler.register_address->at42qt1070_calibrate, 1};

	at42qt1070_handler.i2c_send(AT42QT1070_SLAVE_ADDRESS,
								temp_buffer,
								packet_size);
}

void at42qt1070_reset(void)
{
	const uint8_t packet_size = AT42QT1070_REGISTER_WEDTH * 2;

	uint8_t temp_buffer[2] = {at42qt1070_handler.register_address->at42qt1070_reset, 1};

	at42qt1070_handler.i2c_send(AT42QT1070_SLAVE_ADDRESS,
								temp_buffer,
								packet_size);
}

static at42qt1070_event_e at42qt1070_key_event_type_handler(bool current_change_pin_state)
{
	static at42qt1070_event_e key_event_type = SENSOR_KEY_IDLE;

	static uint32_t long_pressed_timer = 0;

	if(SENSOR_CHANGE_PIN_LOW == current_change_pin_state &&
	   SENSOR_KEY_IDLE == key_event_type)
	{
		key_event_type = SENSOR_KEY_PRESSED;
		//Update timer for SENSOR_KEY_LONG_PRESSED event detection.
		long_pressed_timer = at42qt1070_handler.get_tick();

	}
	else if(SENSOR_CHANGE_PIN_HIGH == current_change_pin_state &&
			SENSOR_KEY_PRESSED == key_event_type)
	{
		key_event_type = SENSOR_KEY_RELEASE;
	}
	else if(SENSOR_CHANGE_PIN_HIGH == current_change_pin_state &&
			SENSOR_KEY_RELEASE == key_event_type)
	{
		key_event_type = SENSOR_KEY_IDLE;
	}
	else if(SENSOR_CHANGE_PIN_LOW == current_change_pin_state &&
			SENSOR_KEY_PRESSED == key_event_type)
	{
		//check the spent time since SENSOR_KEY_PRESSED event is first detected.
		if(at42qt1070_handler.get_tick() - long_pressed_timer > AT42QT1070_LONG_PRESSED_PERIOD)
		{
			key_event_type = SENSOR_KEY_LONG_PRESSED;
		}

	}
	else if(SENSOR_CHANGE_PIN_HIGH == current_change_pin_state &&
			SENSOR_KEY_LONG_PRESSED == key_event_type)
	{
		key_event_type = SENSOR_KEY_RELEASE;
	}

	return key_event_type;
}

static at42qt1070_event_e at42qt1070_event_report(void)
{
//	uint8_t key_status = 0;
//
//	if(at42qt1070_callback())
//	{
//		key_status = at42qt1070_key_stete_get();
//	}
//
//	return key_status;
}

static at42qt1070_event_e at42qt1070_manager(void)
{
//	static at42qt1070_event_e event_type = SENSOR_KEY_IDLE;
//
//	at42qt1070_event_e temp_event_type = SENSOR_KEY_IDLE;
//
//	if(at42qt1070_handler.change_pin_read != NULL)
//	{
//		bool new_event = false;
//
//		at42qt1070_change_pin_e pin_state = at42qt1070_handler.change_pin_read();
//
//		event_type = at42qt1070_key_event_type_handler(pin_state);
//
//		if(!pin_state)
//		{
//			new_event = at42qt1070_key_stete_read();
//		}
//
//	}
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
