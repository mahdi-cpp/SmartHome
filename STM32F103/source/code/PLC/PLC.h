

#ifndef PLC_H
#define PLC_H

//-------------------------------------------------	


#define 	PLC_OUT_1   		    PBout(14)
#define 	PLC_OUT_2   		    PBout(15)



#define 	PLC_IN_1   		    PBin(14)
#define 	PLC_IN_2   		    PBin(15)
#define 	PLC_IN_3   		    PBin(6)
#define 	PLC_IN_4   		    PBin(7)
#define 	PLC_IN_5   		    PBin(8)
#define 	PLC_IN_6  		    PBin(9)


extern unsigned char 	A_PhoneNumber[11];
extern unsigned char    B_PhoneNumber[11];
extern unsigned char    C_PhoneNumber[11];
extern unsigned char 	D_PhoneNumber[11];
extern unsigned char    E_PhoneNumber[11];
extern unsigned char    F_PhoneNumber[11];
extern unsigned char    G_PhoneNumber[11];
extern unsigned char 	H_PhoneNumber[11];
extern unsigned char    I_PhoneNumber[11];
extern unsigned char    J_PhoneNumber[11];

void PLC_Init();



#endif