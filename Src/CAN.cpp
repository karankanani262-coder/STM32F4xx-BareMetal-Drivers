#include "stm32f407xx_1.h"
#include <stm32f407xx.h>
#include "CAN.h"
#include "GPIO.h"

double can_timeout=0;
void CAN_Periclock_control(CAN_TypeDef *pCANx, uint8_t ENorDI)
{
	if(ENorDI==ENABLE)
	{
		switch((int)pCANx)
		{
			case (int)CAN1:
			CAN1_PCLK_EN;
			break;
			
			case (int)CAN2:
			CAN2_PCLK_EN;	
			break;
		}
	}
	else
	{
		switch((int)pCANx)
		{
			case (int)CAN1:
			CAN1_PCLK_DI;
			break;
			
			case (int)CAN2:
			CAN2_PCLK_DI;	
			break;
		}
	}
}

void CAN_Init(CAN_Handle_t *pCANHandle)
{
	CAN_Periclock_control(pCANHandle->pCANx, ENABLE);
	
//	pCANHandle->pCANx->MCR &= ~CAN_MCR_DBF;

	pCANHandle->pCANx->MCR |= pCANHandle->CAN_Config.Transmit_Priority << CAN_MCR_TXFP_Pos;
	
	pCANHandle->pCANx->MCR |= pCANHandle->CAN_Config.AutoBusOff << CAN_MCR_ABOM_Pos;
	
	pCANHandle->pCANx->MCR |= pCANHandle->CAN_Config.AutoRetransmission << CAN_MCR_NART_Pos;

	pCANHandle->pCANx->MCR |= pCANHandle->CAN_Config.RecieveFifoLocked << CAN_MCR_RFLM_Pos;
	
	if(pCANHandle->CAN_Config.CAN_Interrupt_Name != No_Interrupt)
	{
		CAN_Interrupt_Init(pCANHandle->pCANx, pCANHandle->CAN_Config.CAN_Interrupt_Name);
	}
	
//	CAN1->IER |= CAN_IER_FMPIE0;
	
	if(pCANHandle->CAN_Filter_Config.Filter_Active == ENABLE)
	{
		Filter_Init(pCANHandle);
	}
	
	pCANHandle->pCANx->MCR &= ~CAN_MCR_SLEEP;
	
	Initialisation_Mode(pCANHandle);										// Completes initialisation and now bus in Normal Mode
}

void Initialisation_Mode(CAN_Handle_t *pCANHandle)
{
	pCANHandle->pCANx->MCR |= CAN_MCR_INRQ;							// To put CAN in initialisation mode
	pCANHandle->pCANx->BTR &= ~CAN_BTR_RESET;
	
	switch(pCANHandle->CAN_Config.MODE)
	{
		case CAN_Mode_Normal:
			break;
		case CAN_Mode_Silent:
			Silent_Mode(pCANHandle->pCANx);
			break;
		case CAN_Mode_Loop_Back:
			Loop_Back_Mode(pCANHandle->pCANx);
			break;
		case CAN_Mode_Loop_Back_With_Silent:
			Loop_Back_Silent_Mode(pCANHandle->pCANx);
			break;
	}
	
	pCANHandle->pCANx->BTR |= pCANHandle->CAN_Config.BTR_Value_from_Website;			// to define one tq
	
	while(!(pCANHandle->pCANx->MSR & CAN_MSR_INAK));		// To wait for completion of intialisation

	Normal_Mode(pCANHandle->pCANx);
}

void Normal_Mode(CAN_TypeDef *pCANx)
{
	pCANx->MCR &= ~(CAN_MCR_INRQ);											// To get CAN out of initialisation mode					// To Get in Normal Mode
	while(!(pCANx->MSR & CAN_MSR_INAK));									// To wait for CAN to leave intialisation mode		// Now in Normal Mode
}

void Silent_Mode(CAN_TypeDef *pCANx)
{
	pCANx->BTR |= CAN_BTR_SILM;													// To Configure for Silent Mode
}

void Loop_Back_Mode(CAN_TypeDef *pCANx)
{
	pCANx->BTR |= CAN_BTR_LBKM;													// To Configure for Loop Back Mode
}

void Loop_Back_Silent_Mode(CAN_TypeDef *pCANx)
{
	Silent_Mode(pCANx);																	// To Configure for Silent Mode
	Loop_Back_Mode(pCANx);															// To Configure for Loop Back Mode
}

void CAN_Send_Data(CAN_TypeDef *pCANx, bool Arbitration_Type, uint32_t Arbitration_ID, bool Frame_Type, uint8_t Data_Len, uint8_t Data[])									// To Transmit Data
{
//	pCANx->MSR |=  CAN_MSR_RXM;													// Reset Receive Mode
//	pCANx->MSR |=  CAN_MSR_TXM;													// Set Transmit Mode
	can_timeout=0;
	if((pCANx->TSR & CAN_TSR_TME2) >> CAN_TSR_TME2_Pos)
	{
		Mailbox(pCANx, Arbitration_Type, Arbitration_ID, Frame_Type, Data_Len, Data, 2);		// = 2;
		while(!(((pCANx->TSR & CAN_TSR_TME2)>>CAN_TSR_TME2_Pos)||can_timeout>15000)){
			can_timeout++;
		}
	}
	else if((pCANx->TSR & CAN_TSR_TME1) >> CAN_TSR_TME1_Pos)
	{
		Mailbox(pCANx, Arbitration_Type, Arbitration_ID, Frame_Type, Data_Len, Data, 1);		// = 1;
		while(!(((pCANx->TSR & CAN_TSR_TME1)>>CAN_TSR_TME1_Pos)||can_timeout>15000)){
			can_timeout++;
		}
	}
	else if((pCANx->TSR & CAN_TSR_TME0) >> CAN_TSR_TME0_Pos)
	{
		Mailbox(pCANx, Arbitration_Type, Arbitration_ID, Frame_Type, Data_Len, Data, 0);		// = 0;
		while(!(((pCANx->TSR & CAN_TSR_TME0)>>CAN_TSR_TME0_Pos)||can_timeout>15000)){
			can_timeout++;
		}
	}
}
void CAN_SendData_array(CAN_TypeDef *pCANx,uint32_t Arbitration_ID, uint8_t Data[]){
	CAN_Send_Data(pCANx, Std_Ardbitration, Arbitration_ID, Data_Frame, 8, Data);
}
void CAN_SendData(CAN_TypeDef *pCANx, uint32_t Arbitration_ID,uint8_t Data)									// To Transmit only 1 Data
{
	uint8_t Data_Send[8] = {Data, 0, 0, 0, 0, 0, 0, 0};
	CAN_Send_Data(pCANx, Std_Ardbitration, Arbitration_ID, Data_Frame, 3, Data_Send);
}
void CAN_SendData1(CAN_TypeDef *pCANx, uint32_t Arbitration_ID,uint8_t Data)									// To Transmit only 1 Data
{
	uint8_t Data_Send[8] = {0,Data, 0, 0, 0, 0, 0, 0};
	CAN_Send_Data(pCANx, Std_Ardbitration, Arbitration_ID, Data_Frame, 3, Data_Send);
}
void CAN_SendData_Any_Device(uint32_t Arbitration_ID, uint8_t Data)			// To Transmit only 1 Data to any Device
{
	uint8_t Data_Send[8] = {Data, 0, 0, 0, 0, 0, 0, 0};
	CAN_Send_Data(CAN1, Std_Ardbitration, Arbitration_ID, Data_Frame, 1, Data_Send);
}














void Mailbox(CAN_TypeDef *pCANx, bool Arbitration_Type, uint32_t Arbitration_ID, bool Frame_Type, uint8_t Data_Len, uint8_t Data[], uint8_t Mailbox_no)
{
//	uint32_t Data_0 = 0x000000FF;
	pCANx->sTxMailBox[Mailbox_no].TIR &= ~(0xFFFFFFFF);
	pCANx->sTxMailBox[Mailbox_no].TIR |= Arbitration_Type << CAN_TI0R_IDE_Pos;
	pCANx->sTxMailBox[Mailbox_no].TIR |= Frame_Type << CAN_TI0R_RTR_Pos;
	
	if(Arbitration_Type == Std_Ardbitration)
	{
		pCANx->sTxMailBox[Mailbox_no].TIR |= Arbitration_ID << CAN_TI0R_STID_Pos;
	}
	else if(Arbitration_Type == EXD_Ardbitration)
	{
		pCANx->sTxMailBox[Mailbox_no].TIR |= Arbitration_ID << CAN_TI0R_EXID_Pos;
	}
	
	pCANx->sTxMailBox[Mailbox_no].TDTR &= ~(0xFFFFFFFF);
	pCANx->sTxMailBox[Mailbox_no].TDTR |= Data_Len << CAN_TDT0R_DLC_Pos;
	
	for(int x = 0; x < Data_Len; x++)
	{
		if(x <= 3)
		{
			pCANx->sTxMailBox[Mailbox_no].TDLR &= ~(Data_0 << (x*8));
			pCANx->sTxMailBox[Mailbox_no].TDLR |= Data[x] << (x*8);
		}
		else if(x <= 7)
		{
			pCANx->sTxMailBox[Mailbox_no].TDHR &= ~(Data_0 << ((x-4)*8));
			pCANx->sTxMailBox[Mailbox_no].TDHR |= Data[x] << ((x-4)*8);
		}
	}
	
	pCANx->sTxMailBox[Mailbox_no].TIR |= CAN_TI0R_TXRQ;
}












void Filter_Init(CAN_Handle_t *pCANHandle)						// To configure all Filters of CAN
{
	pCANHandle->pCANx->FMR	&= ~(CAN_FMR_CAN2SB);
	
	for(int x = 0; x < pCANHandle->CAN_Filter_Config.Number_of_filters; x++)
	{
		pCANHandle->pCANx->FA1R	&= ~(CAN_FA1R_FACT0 << x);
	}
	
	pCANHandle->pCANx->FMR	|=  (pCANHandle->CAN_Filter_Config.CAN2_Start_Bank << CAN_FMR_CAN2SB_Pos);
	pCANHandle->pCANx->FMR	|=	 CAN_FMR_FINIT;
	
	for(int x = 0; x < pCANHandle->CAN_Filter_Config.Number_of_filters; x++)
	{
		pCANHandle->pCANx->FM1R	 &= ~(CAN_FM1R_FBM0 << x);
		pCANHandle->pCANx->FM1R	 |=	(pCANHandle->CAN_Filter_Config.Filter_Mode[x] << x);
		pCANHandle->pCANx->FS1R	 &= ~(CAN_FS1R_FSC0 << x);	
		pCANHandle->pCANx->FS1R	 |=	(pCANHandle->CAN_Filter_Config.Filter_Scale[x] << x);
		pCANHandle->pCANx->FFA1R &= ~(1 << x);
		pCANHandle->pCANx->FFA1R |=	(pCANHandle->CAN_Filter_Config.Filter_FIFO_Assignment[x] << x);
		if(x%2 == 0)
		{
			if(pCANHandle->CAN_Filter_Config.Filter_On_ID_Type[x] == Std_Ardbitration)
			{
				pCANHandle->pCANx->sFilterRegister[x].FR1 = pCANHandle->CAN_Filter_Config.Filter_Value[x] << 21;
				pCANHandle->pCANx->sFilterRegister[x].FR1 |= Std_Ardbitration << 2;
			}
			else if(pCANHandle->CAN_Filter_Config.Filter_On_ID_Type[x] == EXD_Ardbitration)
			{
				pCANHandle->pCANx->sFilterRegister[x].FR1 = pCANHandle->CAN_Filter_Config.Filter_Value[x] << 3;
				pCANHandle->pCANx->sFilterRegister[x].FR1 |= EXD_Ardbitration << 2;
			}
		}
		else
		{
			if(pCANHandle->CAN_Filter_Config.Filter_On_ID_Type[x] == Std_Ardbitration)
			{
				pCANHandle->pCANx->sFilterRegister[x].FR2 = pCANHandle->CAN_Filter_Config.Filter_Value[x] << 21;
				pCANHandle->pCANx->sFilterRegister[x].FR2 |= Std_Ardbitration << 2;
			}
			else if(pCANHandle->CAN_Filter_Config.Filter_On_ID_Type[x] == EXD_Ardbitration)
			{
				pCANHandle->pCANx->sFilterRegister[x].FR2 = pCANHandle->CAN_Filter_Config.Filter_Value[x] << 3;
				pCANHandle->pCANx->sFilterRegister[x].FR2 |= EXD_Ardbitration << 2;
			}
		}
	}

	for(int x = 0; x < pCANHandle->CAN_Filter_Config.Number_of_filters; x++)
	{
		pCANHandle->pCANx->FA1R	|= (CAN_FA1R_FACT0 << x);
	}

	pCANHandle->pCANx->FMR	&= ~(CAN_FMR_FINIT);
}

void CAN_DeInit(CAN_TypeDef *pCANx)
{
	switch((int)pCANx)
		{
			case (int)CAN1:
			CAN1_RESET;
			break;
			
			case (int)CAN2:
			CAN2_RESET;	
			break;
		}
}

uint8_t Check_Fifo(CAN_TypeDef *pCANx, bool FIFO_No)
{
	if(FIFO_No == CAN_FIFO_0)
	{
		return ((pCANx->RF0R & CAN_RF0R_FMP0) != 0);
	}
	else
	{
		return ((pCANx->RF1R & CAN_RF1R_FMP1) != 0);
	}
}

uint8_t CAN_RecieveData(CAN_TypeDef *pCANx)				// No Polling Receive only 1 data
{
	bool FIFO_No = CAN_FIFO_0;
//	Check_Fifo(pCANx, FIFO_No);
	uint8_t Data_Len = (pCANx->sFIFOMailBox[FIFO_No].RDTR & CAN_RDT0R_DLC);
	uint8_t Data = 0;
	if(Data_Len != 0)
	{
		Data = pCANx->sFIFOMailBox[FIFO_No].RDLR & Data_0;
	}
	
	CAN_Release_FIFO(pCANx, FIFO_No);
	return Data;
}

void CAN_Recieve_Data(CAN_TypeDef *pCANx, bool FIFO_No, uint8_t Data[])
{
	Check_Fifo(pCANx, FIFO_No);
	uint8_t Data_Len =8;
	
	while(Data_Len == 0)
	{
		Data_Len = (pCANx->sFIFOMailBox[FIFO_No].RDTR & CAN_RDT0R_DLC);
	}
	
	for(int x = 0; x <= Data_Len; x++)
	{
		if(x <= 3)
		{
			Data[x] = ((pCANx->sFIFOMailBox[FIFO_No].RDLR & (Data_0 << (x*8))) >> (x*8));
		}
		else if(x <= 7)
		{
			Data[x] = ((pCANx->sFIFOMailBox[FIFO_No].RDHR & (Data_0 << ((x-4)*8))) >> ((x-4)*8));
		}
	}
	
	CAN_Release_FIFO(pCANx, FIFO_No);
}

//void CAN_Recieve_Data_All_Para(CAN_Receive_t *pCAN_Receive)
//{
//	Check_Fifo(pCAN_Receive->pCANx, pCAN_Receive->FIFO_No);
//	pCAN_Receive->Data_len = (pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RDTR & CAN_RDT0R_DLC);
//	
//	while(pCAN_Receive->Data_len == 0)
//	{
//		pCAN_Receive->Data_len = (pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RDTR & CAN_RDT0R_DLC);
//	}
//	
//	pCAN_Receive->Arbitration_Type = (pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RIR >> CAN_RI0R_IDE_Pos) & 0x01;
//	pCAN_Receive->Frame_Type = (pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RIR >> CAN_RI0R_RTR_Pos) & 0x01;
//	
//	if(pCAN_Receive->Arbitration_Type == Std_Ardbitration)
//	{
//		pCAN_Receive->Ardbitration_ID = (pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RIR & CAN_RI0R_STID) >> CAN_RI0R_STID_Pos;
//	}
//	else if(pCAN_Receive->Arbitration_Type == EXD_Ardbitration)
//	{
//		pCAN_Receive->Ardbitration_ID = (pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RIR & CAN_RI0R_EXID) >> CAN_RI0R_EXID_Pos;
//	}
//	
//	for(int x = 0; x < pCAN_Receive->Data_len; x++)
//	{
//		if(x <= 3)
//		{
//			pCAN_Receive->Received_Data[x] = (pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RDLR & (Data_0 << (x*8))) >> (x*8);
//		}
//		else if(x <= 7)
//		{
//			pCAN_Receive->Received_Data[x] = (pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RDHR & (Data_0 << ((x-4)*8))) >> ((x-4)*8);
//		}
//	}
//	
//	CAN_Release_FIFO(pCAN_Receive->pCANx, pCAN_Receive->FIFO_No);
//}

void CAN_Recieve_Data_All_Para(CAN_Receive_t *pCAN_Receive)
{
	// ? REMOVE blocking
	// Check_Fifo(...);

	// ? NON-BLOCKING
	if(!Check_Fifo(pCAN_Receive->pCANx, pCAN_Receive->FIFO_No))
		return;

	pCAN_Receive->Data_len =
		(pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RDTR & CAN_RDT0R_DLC);

	pCAN_Receive->Arbitration_Type =
		(pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RIR >> CAN_RI0R_IDE_Pos) & 0x01;

	pCAN_Receive->Frame_Type =
		(pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RIR >> CAN_RI0R_RTR_Pos) & 0x01;

	if(pCAN_Receive->Arbitration_Type == Std_Ardbitration)
	{
		pCAN_Receive->Ardbitration_ID =
			(pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RIR & CAN_RI0R_STID) >> CAN_RI0R_STID_Pos;
	}
	else
	{
		pCAN_Receive->Ardbitration_ID =
			(pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RIR & CAN_RI0R_EXID) >> CAN_RI0R_EXID_Pos;
	}

	for(int x = 0; x < pCAN_Receive->Data_len; x++)
	{
		if(x <= 3)
		{
			pCAN_Receive->Received_Data[x] =
				(pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RDLR >> (x*8)) & 0xFF;
		}
		else
		{
			pCAN_Receive->Received_Data[x] =
				(pCAN_Receive->pCANx->sFIFOMailBox[pCAN_Receive->FIFO_No].RDHR >> ((x-4)*8)) & 0xFF;
		}
	}

	// ? release FIFO
	CAN_Release_FIFO(pCAN_Receive->pCANx, pCAN_Receive->FIFO_No);
}

void CAN_Release_FIFO(CAN_TypeDef *pCANx, bool FIFO_No)
{
	if(FIFO_No == CAN_FIFO_0)
	{
		pCANx->RF0R |= CAN_RF0R_RFOM0;
	}
	else if(FIFO_No == CAN_FIFO_1)
	{
		pCANx->RF1R |= CAN_RF1R_RFOM1;
	}
}

//void CAN_Interrupt_Init(CAN_TypeDef *pCANx, CAN_Interrupt CAN_Interrupt_Name)
//{
//	if(pCANx==CAN1)
//	{
//	pCANx->IER |= 1 << CAN_Interrupt_Name;
//	if(CAN_Interrupt_Name == TMEIE)
//	{
//		NVIC_EnableIRQ(CAN1_TX_IRQn);
//	}
//	else if((CAN_Interrupt_Name == FMPIE0) || (CAN_Interrupt_Name == FFIE0) || (CAN_Interrupt_Name == FOVIE0))
//	{
//		NVIC_EnableIRQ(CAN1_RX0_IRQn);
//	}
//	else if((CAN_Interrupt_Name == FMPIE1) || (CAN_Interrupt_Name == FFIE1) || (CAN_Interrupt_Name == FOVIE1))
//	{
//		NVIC_EnableIRQ(CAN1_RX1_IRQn);
//	}
//	else if((CAN_Interrupt_Name == ERRIE) || (CAN_Interrupt_Name == EWGIE) || (CAN_Interrupt_Name == EPVIE) || (CAN_Interrupt_Name == BOFIE) || (CAN_Interrupt_Name == LECIE) || (CAN_Interrupt_Name == WKUIE) || (CAN_Interrupt_Name == SLKIE))
//	{
//		NVIC_EnableIRQ(CAN1_SCE_IRQn);
//	}
//  }
//	else if(pCANx==CAN2)
//	{
//	  pCANx->IER |= 1 << CAN_Interrupt_Name;
//	if(CAN_Interrupt_Name == TMEIE)
//	{
//		NVIC_EnableIRQ(CAN2_TX_IRQn);
//	}
//	else if((CAN_Interrupt_Name == FMPIE0) || (CAN_Interrupt_Name == FFIE0) || (CAN_Interrupt_Name == FOVIE0))
//	{
//		NVIC_EnableIRQ(CAN2_RX0_IRQn);
//	}
//	else if((CAN_Interrupt_Name == FMPIE1) || (CAN_Interrupt_Name == FFIE1) || (CAN_Interrupt_Name == FOVIE1))
//	{
//		NVIC_EnableIRQ(CAN2_RX1_IRQn);
//	}
//	else if((CAN_Interrupt_Name == ERRIE) || (CAN_Interrupt_Name == EWGIE) || (CAN_Interrupt_Name == EPVIE) || (CAN_Interrupt_Name == BOFIE) || (CAN_Interrupt_Name == LECIE) || (CAN_Interrupt_Name == WKUIE) || (CAN_Interrupt_Name == SLKIE))
//	{
//		NVIC_EnableIRQ(CAN2_SCE_IRQn);
//	}	
//	}
//}

void CAN_Interrupt_Init(CAN_TypeDef *pCANx, CAN_Interrupt CAN_Interrupt_Name)
{
	uint32_t interrupt_mask = 0;

	/* -------- HANDLE MULTIPLE INTERRUPTS -------- */

	// FIFO0
	if((CAN_Interrupt_Name == FMPIE0) || (CAN_Interrupt_Name == FMPIE0_FMPIE1))
	{
		interrupt_mask |= (1 << FMPIE0);
		if(pCANx == CAN1) NVIC_EnableIRQ(CAN1_RX0_IRQn);
		else              NVIC_EnableIRQ(CAN2_RX0_IRQn);
	}

	// FIFO1
	if((CAN_Interrupt_Name == FMPIE1) || (CAN_Interrupt_Name == FMPIE0_FMPIE1))
	{
		interrupt_mask |= (1 << FMPIE1);
		if(pCANx == CAN1) NVIC_EnableIRQ(CAN1_RX1_IRQn);
		else              NVIC_EnableIRQ(CAN2_RX1_IRQn);
	}

	// TX
	if(CAN_Interrupt_Name == TMEIE)
	{
		interrupt_mask |= (1 << TMEIE);
		if(pCANx == CAN1) NVIC_EnableIRQ(CAN1_TX_IRQn);
		else              NVIC_EnableIRQ(CAN2_TX_IRQn);
	}

	// FIFO0 other
	if(CAN_Interrupt_Name == FFIE0 || CAN_Interrupt_Name == FOVIE0)
	{
		interrupt_mask |= (1 << CAN_Interrupt_Name);
		if(pCANx == CAN1) NVIC_EnableIRQ(CAN1_RX0_IRQn);
		else              NVIC_EnableIRQ(CAN2_RX0_IRQn);
	}

	// FIFO1 other
	if(CAN_Interrupt_Name == FFIE1 || CAN_Interrupt_Name == FOVIE1)
	{
		interrupt_mask |= (1 << CAN_Interrupt_Name);
		if(pCANx == CAN1) NVIC_EnableIRQ(CAN1_RX1_IRQn);
		else              NVIC_EnableIRQ(CAN2_RX1_IRQn);
	}

	// ERROR
	if((CAN_Interrupt_Name == ERRIE) || (CAN_Interrupt_Name == EWGIE) ||
	   (CAN_Interrupt_Name == EPVIE) || (CAN_Interrupt_Name == BOFIE) ||
	   (CAN_Interrupt_Name == LECIE) || (CAN_Interrupt_Name == WKUIE) ||
	   (CAN_Interrupt_Name == SLKIE))
	{
		interrupt_mask |= (1 << CAN_Interrupt_Name);
		if(pCANx == CAN1) NVIC_EnableIRQ(CAN1_SCE_IRQn);
		else              NVIC_EnableIRQ(CAN2_SCE_IRQn);
	}

	/* -------- APPLY INTERRUPTS -------- */
	pCANx->IER |= interrupt_mask;
}

//void Safety_Protocol()
//{
//6	
//}