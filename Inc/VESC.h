#include <stm32f407xx.h>
#include "CAN.h"
#include "buffer.h"

#ifndef _VESC_H
#define _VESC_H

// CMD_Type
#define Set													0
#define Get													1

// CMD_IDs
//#define CAN_PACKET_SET_POS					4

// CAN commands
typedef enum {
	CAN_PACKET_SET_DUTY											= 0,
	CAN_PACKET_SET_CURRENT									= 1,
	CAN_PACKET_SET_CURRENT_BRAKE						= 2,
	CAN_PACKET_SET_RPM											= 3,
	CAN_PACKET_SET_POS											= 4,
	CAN_PACKET_FILL_RX_BUFFER								= 5,
	CAN_PACKET_FILL_RX_BUFFER_LONG					= 6,
	CAN_PACKET_PROCESS_RX_BUFFER						= 7,
	CAN_PACKET_PROCESS_SHORT_BUFFER					= 8,
	CAN_PACKET_STATUS												= 9,
	CAN_PACKET_SET_CURRENT_REL							= 10,
	CAN_PACKET_SET_CURRENT_BRAKE_REL				= 11,
	CAN_PACKET_SET_CURRENT_HANDBRAKE				= 12,
	CAN_PACKET_SET_CURRENT_HANDBRAKE_REL		= 13,
	CAN_PACKET_STATUS_2											= 14,
	CAN_PACKET_STATUS_3											= 15,
	CAN_PACKET_STATUS_4											= 16,
	CAN_PACKET_PING													= 17,
	CAN_PACKET_PONG													= 18,
	CAN_PACKET_DETECT_APPLY_ALL_FOC					= 19,
	CAN_PACKET_DETECT_APPLY_ALL_FOC_RES			= 20,
	CAN_PACKET_CONF_CURRENT_LIMITS					= 21,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS		= 22,
	CAN_PACKET_CONF_CURRENT_LIMITS_IN				= 23,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS_IN	= 24,
	CAN_PACKET_CONF_FOC_ERPMS								= 25,
	CAN_PACKET_CONF_STORE_FOC_ERPMS					= 26,
	CAN_PACKET_STATUS_5											= 27,
	CAN_PACKET_POLL_TS5700N8501_STATUS			= 28,
	CAN_PACKET_CONF_BATTERY_CUT							= 29,
	CAN_PACKET_CONF_STORE_BATTERY_CUT				= 30,
	CAN_PACKET_SHUTDOWN											= 31,
	CAN_PACKET_IO_BOARD_ADC_1_TO_4					= 32,
	CAN_PACKET_IO_BOARD_ADC_5_TO_8					= 33,
	CAN_PACKET_IO_BOARD_ADC_9_TO_12					= 34,
	CAN_PACKET_IO_BOARD_DIGITAL_IN					= 35,
	CAN_PACKET_IO_BOARD_SET_OUTPUT_DIGITAL	= 36,
	CAN_PACKET_IO_BOARD_SET_OUTPUT_PWM			= 37,
	CAN_PACKET_BMS_V_TOT										= 38,
	CAN_PACKET_BMS_I												= 39,
	CAN_PACKET_BMS_AH_WH										= 40,
	CAN_PACKET_BMS_V_CELL										= 41,
	CAN_PACKET_BMS_BAL											= 42,
	CAN_PACKET_BMS_TEMPS										= 43,
	CAN_PACKET_BMS_HUM											= 44,
	CAN_PACKET_BMS_SOC_SOH_TEMP_STAT				= 45,
	CAN_PACKET_PSW_STAT											= 46,
	CAN_PACKET_PSW_SWITCH										= 47,
	CAN_PACKET_BMS_HW_DATA_1								= 48,
	CAN_PACKET_BMS_HW_DATA_2								= 49,
	CAN_PACKET_BMS_HW_DATA_3								= 50,
	CAN_PACKET_BMS_HW_DATA_4								= 51,
	CAN_PACKET_BMS_HW_DATA_5								= 52,
	CAN_PACKET_BMS_AH_WH_CHG_TOTAL					= 53,
	CAN_PACKET_BMS_AH_WH_DIS_TOTAL					= 54,
	CAN_PACKET_UPDATE_PID_POS_OFFSET				= 55,
	CAN_PACKET_POLL_ROTOR_POS								= 56,
	CAN_PACKET_NOTIFY_BOOT									= 57,
	CAN_PACKET_STATUS_6											= 58,
	CAN_PACKET_GNSS_TIME										= 59,
	CAN_PACKET_GNSS_LAT											= 60,
	CAN_PACKET_GNSS_LON											= 61,
	CAN_PACKET_GNSS_ALT_SPEED_HDOP					= 62,
	CAN_PACKET_MAKE_ENUM_32_BITS 						= 0xFFFFFFFF
} CAN_PACKET_ID;

typedef struct {
	int id;
//systime_t rx_time;
	float rpm;
	float current;
	float duty;
} can_status_msg;

typedef struct {
	int id;
//	systime_t rx_time;
	float amp_hours;
	float amp_hours_charged;
} can_status_msg_2;

typedef struct {
	int id;
//	systime_t rx_time;
	float watt_hours;
	float watt_hours_charged;
} can_status_msg_3;

typedef struct {
	int id;
//	systime_t rx_time;
	float temp_fet;
	float temp_motor;
	float current_in;
	float pid_pos_now;
} can_status_msg_4;

typedef struct {
	int id;
//	systime_t rx_time;
	float v_in;
	int32_t tacho_value;
} can_status_msg_5;

typedef struct {
	int id;
//	systime_t rx_time;
	float adc_1;
	float adc_2;
	float adc_3;
	float ppm;
} can_status_msg_6;

typedef enum {
	CAN_MODE_VESC = 0,
	CAN_MODE_UAVCAN,
	CAN_MODE_COMM_BRIDGE,
	CAN_MODE_UNUSED,
} CAN_MODE;

// Data types
typedef enum {
	HW_TYPE_VESC = 0,
	HW_TYPE_VESC_BMS,
	HW_TYPE_CUSTOM_MODULE
} HW_TYPE;

//void Send_Data_VESC(CAN_TypeDef *pCANx, uint32_t Axis, bool CMD_Type, uint32_t CMD, uint8_t Data_Len, uint8_t Data[]);				 	// To Transmit Data
//void CMD_VESC(CAN_TypeDef *pCANx, uint32_t Axis, uint32_t CMD, uint8_t Data_Len, uint8_t Data[]);															// To Order Odrive
//void Get_VESC(CAN_TypeDef *pCANx, uint32_t Axis, uint32_t CMD, CAN_Receive_t *pCAN_Receive);	// To Get from Odrive

void comm_can_set_duty(CAN_TypeDef *pCANx, uint8_t controller_id, float duty);
void comm_can_set_current(CAN_TypeDef *pCANx, uint8_t controller_id, float current);
void comm_can_set_current_off_delay(CAN_TypeDef *pCANx, uint8_t controller_id, float current, float off_delay);
void comm_can_set_current_brake(CAN_TypeDef *pCANx, uint8_t controller_id, float current);
void comm_can_set_rpm(CAN_TypeDef *pCANx, uint8_t controller_id, int32_t rpm);
void comm_can_set_pos(CAN_TypeDef *pCANx, uint8_t controller_id, float pos);
void comm_can_set_current_rel(CAN_TypeDef *pCANx, uint8_t controller_id, float current_rel);
void comm_can_set_current_rel_off_delay(CAN_TypeDef *pCANx, uint8_t controller_id, float current_rel, float off_delay);
void comm_can_set_current_brake_rel(CAN_TypeDef *pCANx, uint8_t controller_id, float current_rel);
//bool comm_can_ping(CAN_TypeDef *pCANx, uint8_t controller_id, HW_TYPE *hw_type);
//void comm_can_detect_apply_all_foc(CAN_TypeDef *pCANx, uint8_t controller_id, bool activate_status_msgs, float max_power_loss);
void comm_can_conf_current_limits(CAN_TypeDef *pCANx, uint8_t controller_id, bool store, float min, float max);
void comm_can_conf_current_limits_in(CAN_TypeDef *pCANx, uint8_t controller_id, bool store, float min, float max);
void comm_can_conf_foc_erpms(CAN_TypeDef *pCANx, uint8_t controller_id, bool store, float foc_openloop_rpm, float foc_sl_erpm);
//int comm_can_detect_all_foc_res(unsigned int index);
//int comm_can_detect_all_foc_res_size(void);
//void comm_can_detect_all_foc_res_clear(void);
void comm_can_conf_battery_cut(CAN_TypeDef *pCANx, uint8_t controller_id, bool store, float start, float end);
void comm_can_shutdown(CAN_TypeDef *pCANx, uint8_t controller_id);
void comm_can_io_board_set_output_digital(CAN_TypeDef *pCANx, int id, int channel, bool on);
void comm_can_io_board_set_output_pwm(CAN_TypeDef *pCANx, int id, int channel, float duty);
void comm_can_update_pid_pos_offset(CAN_TypeDef *pCANx, int id, float angle_now, bool store);

#endif
