

#ifndef SIM900_H
#define SIM900_H

//-------------------------------------------------	


#define 	SIM_POWER   		PBout(0)
#define 	SIM_RESET   		PBout(5)

//-------------------------------------------------
extern int SIM900Timeout;
extern int SIM900_Timer_1;
extern int SIM900_Timer_2;
extern int SIM900_Power_Tiomout;
extern int SIM900_TurnOn_Config;
extern int SIM900_Connfig;




void SIM900_Init();
void SIM900_RESET();

void SIM900_ClearSMS();
void SIM900_Read_SMS();

int SIM900_POWER_ON();
void SIM900_POWER_OFF();

void SMS_Clear_Buffer();

int SIM900_TurnOn();

#endif