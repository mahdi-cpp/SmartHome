

#ifndef MT8888C_H
#define MT8888C_H

//-------------------------------------------------	

#define 	LED3   		    PBout(12)					

#define 	MT88_RD   		PAout(1)
#define 	MT88_RS   		PCout(4)
#define 	MT88_RW   		PCout(5)

#define 	MT88_D0   		PCout(3)
#define 	MT88_D1   		PCout(2)
#define 	MT88_D2   		PCout(1)
#define 	MT88_D3   		PCout(0)

#define 	MT8888C_D0   	PCin(3)
#define 	MT8888C_D1   	PCin(2)
#define 	MT8888C_D2   	PCin(1)
#define 	MT8888C_D3   	PCin(0)



//-------------------------------------------------

extern int RingCounter;

void  MT8888C_Init();
void  MT8888C_RESET();
void  MT8888C_CP_Receive_Init();
void  MT8888C_DTMF_Receive_Init();
void  MT8888C_DTMF_Send(unsigned char *String);
void  MT8888C_Ring(short int num);



#endif