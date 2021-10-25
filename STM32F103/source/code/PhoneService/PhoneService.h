

#ifndef PHONESERVICE_H
#define PHONESERVICE_H

//-------------------------------------------------	


#define 	MaxPasswordLen				4
#define		PasswordLen					4

#define 	MAX_INVALID_PASSWORD		3
//-------------------------------------------------
extern int IntercomSelect;
extern int SwitchToIntercom;
extern int PhoneServiceBusy;
extern int Start_Telephone;
extern int StartTelephoneSelect;


extern int PhoneServiceTimeout ;
extern int PLC_Telephone_Response;
extern int PLC_Telephone_Password;

extern char Telephone_Password[4];

void Call_Warning(unsigned char PhoneNumber[], int AlertType);

extern int honeService();
extern int IntercomPhoneService();

extern char Latest_PhoneNumber[14]; 

int MicrophoneEnable(void);


#endif