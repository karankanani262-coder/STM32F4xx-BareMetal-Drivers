#include <stm32f407xx.h>

#ifndef _CAN_H
#define _CAN_H

/*
Calculation of time quanta:-

APB1 = 42MHz
CAN prescalar = x
Duration of 1 time quanta (1tq) = 42/x = 42MHz 				// This is frequency in reference manual it is clock time.
																			 = 0.023 us

website :- http://www.bittiming.can-wiki.info/ 
										
Bit Rate			accuracy			Pre-scaler				Number of time quanta				Seg 1(Prop_Seg+Phase_Seg1)			Seg 2				Sample Point at				Register CAN_BTR
	1000					0.0000					3											14															11										2								85.7								0x001a0002

*/

// Baud Rates
typedef enum {
	CAN_BAUD_1000KBPS = 0x001A0002,
	CAN_BAUD_500KBPS  = 0x001A0005,
	CAN_BAUD_250KBPS  = 0x001A000B,
	CAN_BAUD_125KBPS  = 0x001C0014,
	CAN_BAUD_100KBPS  = 0x001B001B,
	CAN_BAUD_50KBPS   = 0x001B0037,
	CAN_BAUD_20KBPS   = 0x001B008B,
	CAN_BAUD_10KBPS   = 0x001B0117,
} CAN_BAUD;

// CAN Interrupts
typedef enum {
	TMEIE = 0,
	FMPIE0 = 1,
	FFIE0 = 2,
	FOVIE0 = 3,
	FMPIE1 = 4,
	FFIE1 = 5,
	FOVIE1 = 6,
	EWGIE = 8,
	EPVIE = 9,
	BOFIE = 10,
	LECIE = 11,
	ERRIE = 15,
	WKUIE = 16,
	SLKIE = 17,

	// ? ADDED COMBO
	FMPIE0_FMPIE1 = 0xFF,   // special flag (we handle manually)

	No_Interrupt = 18,
} CAN_Interrupt;

typedef enum {
	Dual_16_bit = 0,
	Single_32_bit = 1,
} Filter_Scale_Options;

typedef enum {
	Identifier_Mode = 0,
	List_Mode = 1,
} Filter_Mode_Options;

typedef enum {
	FIFO_0 = 0,
	FIFO_1 = 1,
} Filter_No_Options;

typedef struct
{
	uint8_t MODE;                              								//@
	CAN_BAUD BTR_Value_from_Website;													//@BTR value for baud http://www.bittiming.can-wiki.info/
	bool AutoBusOff;          		                           	//@
//	bool AutoWakeUp;          		                           	//@
	bool AutoRetransmission;  	                              //@NART value
	bool RecieveFifoLocked;   	                              //@RFLM value FIFO lock disabled => last message overwritten by new incoming message
																														// 						FIFO lock enabled  => new incoming message is discarded last message is locked
	bool Transmit_Priority;	                              		//@Set Priority according to arbitration ID or sequential FIFO
	CAN_Interrupt CAN_Interrupt_Name;													//@Name of Interrupt
}CAN_Config_t;

typedef struct
{
	bool Filter_Active;																				//@To Activate filter
	uint8_t CAN2_Start_Bank;																	//@CAN2SB for assigning Filters to CAN2 Value => 0d to 27d
	uint8_t Number_of_filters;																//@Number of filters
	bool Filter_On_ID_Type[54];																//@To determine shift in Filter registers // Std_Ardbitration or // Std_Ardbitration of to be received
//	bool Filter_On_ID_Type_2;																	//@To determine shift in Filter registers // Std_Ardbitration or // Std_Ardbitration of to be received
	Filter_No_Options Filter_FIFO_Assignment[54];									//@CAN_FIFO_0 //CAN_FIFO_1
//	uint8_t Filter_Bank;																			//@CAN Filter Bank
	Filter_Mode_Options Filter_Mode[54];											//@Identifier_Mask // List_mode
	uint32_t Filter_Value[54];																//@Filter Values
	Filter_Scale_Options Filter_Scale[54];										//@Dual_16_bit //Single_32_bit
}CAN_Filter_Config_t;

typedef struct
{
  CAN_TypeDef *pCANx;
	CAN_Config_t CAN_Config;
	CAN_Filter_Config_t CAN_Filter_Config;
}CAN_Handle_t;

typedef struct
{
	CAN_TypeDef *pCANx;                        								//@CAN No
	bool FIFO_No;																							//@FIFO No
	uint8_t Received_Data[8];																	//@Received Data array
	uint8_t Data_len;																					//@Length of data received
	uint32_t Ardbitration_ID;          		                    //@Variable to read Arbitration ID
	bool Arbitration_Type;          		                      //@Variable to see type of arbitration id
	bool Frame_Type;          		                           	//@Remote Frame/Data Frame
}CAN_Receive_t;

//CAN Enable Clock Macros
#define CAN1_PCLK_EN      RCC_APB1ENR|=RCC_APB1ENR_CAN1EN
#define CAN2_PCLK_EN      RCC_APB1ENR|=RCC_APB1ENR_CAN2EN

//CAN Disable Clock Macros
#define CAN1_PCLK_DI      RCC_APB1ENR&=~RCC_APB1ENR_CAN1EN
#define CAN2_PCLK_DI      RCC_APB1ENR&=~RCC_APB1ENR_CAN2EN

// CAN Reset Macros
#define CAN1_RESET       do{ RCC_APB1RSTR|=RCC_APB1RSTR_CAN1RST; RCC_AHB1RSTR&=~RCC_APB1RSTR_CAN1RST;}while(0)
#define CAN2_RESET       do{ RCC_APB1RSTR|=RCC_APB1RSTR_CAN2RST; RCC_AHB1RSTR&=~RCC_APB1RSTR_CAN2RST;}while(0)

// CAN Modes Macros
#define CAN_Mode_Normal										0							
#define CAN_Mode_Silent										1
#define CAN_Mode_Loop_Back								2
#define CAN_Mode_Loop_Back_With_Silent		3

// CAN Transmit Priority Macros
#define CAN_Transmit_Priority_ID					0
#define CAN_Transmit_FIFO									1

// CAN Transmit Identifier type
#define Std_Ardbitration									0
#define EXD_Ardbitration									1

// CAN Frame Type
#define Data_Frame												0
#define Remote_Frame											1

// CAN BTR
#define CAN_BTR_RESET											(CAN_BTR_BRP | CAN_BTR_TS1 | CAN_BTR_TS2 | CAN_BTR_SJW | CAN_BTR_LBKM | CAN_BTR_SILM)

// CAN NART
#define CAN_Auto_Retransmit								0
#define CAN_No_Auto_Retransmit						1

// CAN RecieveFifoLocked
#define CAN_Recieve_Fifo_Not_Locked				0
#define CAN_Recieve_Fifo_Locked						1

// CAN FIFOs
#define CAN_FIFO_0												0
#define CAN_FIFO_1												1

// CAN Identifier Filter Modes
#define CAN_Filter_Mask										0
#define CAN_Filter_List										1

// CAN Scale
#define CAN_Filter_16_Scale								0
#define CAN_Filter_32_Scale								1

// Arbitration IDs												// Lower Arbitration ID Higher Priority
#define Drive         										0x00
#define SICK             						      0x01
#define VESC_UP														0x02
#define VESC_L                            0x03
#define VESC_R                            0x04


#define Data_0														0xFF
#define CAN2_Start_Reset_Data							0x3F

/*
*   CAN Peripheral Clock Functions
*/
void CAN_Periclock_control(CAN_TypeDef *pCANx, uint8_t ENorDI);

/*
*   CAN Init and Deinit Functions
*/
void CAN_Init(CAN_Handle_t *pCANHandle);

void Initialisation_Mode(CAN_Handle_t *pCANHandle);			// To enter Intialisation Mode
void Normal_Mode(CAN_TypeDef *pCANx);										// To enter Normal Mode
void Sleep_Mode(CAN_Handle_t *pCANHandle);

void Silent_Mode(CAN_TypeDef *pCANx);
void Loop_Back_Mode(CAN_TypeDef *pCANx);
void Loop_Back_Silent_Mode(CAN_TypeDef *pCANx);

void Filter_Init(CAN_Handle_t *pCANHandle);							// To configure all Filters of CAN

void CAN_Send_Data(CAN_TypeDef *pCANx, bool Arbitration_Type, uint32_t Arbitration_ID, bool Frame_Type, uint8_t Data_Len, uint8_t Data[]);									// To Transmit Data
void CAN_SendData(CAN_TypeDef *pCANx,uint32_t Arbitration_ID, uint8_t Data);// To Transmit only 1 Data
void CAN_SendData_array(CAN_TypeDef *pCANx,uint32_t Arbitration_ID, uint8_t Data[]);
void CAN_SendData_Any_Device(uint32_t Arbitration_ID, uint8_t Data);		// To Transmit only 1 Data to any Device
void Mailbox(CAN_TypeDef *pCANx, bool Arbitration_Type, uint32_t Arbitration_ID, bool Frame_Type, uint8_t Data_Len, uint8_t Data[], uint8_t Mailbox_no);
void CAN_SendData1(CAN_TypeDef *pCANx, uint32_t Arbitration_ID,uint8_t Data);									// To Transmit only 1 Data;

//void Check_Fifo(CAN_TypeDef *pCANx, bool FIFO_No);
uint8_t Check_Fifo(CAN_TypeDef *pCANx, bool FIFO_No);

uint8_t CAN_RecieveData(CAN_TypeDef *pCANx);				// No Polling Receive only 1 data
void CAN_Recieve_Data(CAN_TypeDef *pCANx, bool FIFO_No, uint8_t Data[]);
void CAN_Release_FIFO(CAN_TypeDef *pCANx, bool FIFO_No);
void CAN_Recieve_Data_All_Para(CAN_Receive_t *pCAN_Receive);

void CAN_Interrupt_Init(CAN_TypeDef *pCANx, CAN_Interrupt CAN_Interrupt_Name);

void CAN_DeInit(CAN_TypeDef *pCANx);

#endif