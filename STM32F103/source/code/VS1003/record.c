

 #include "record.h"
 #include "VS1003.h"
 //#include "ui.h"

 #define VS1003
 #define SPI_HDAT0       0x8   
 #define SPI_HDAT1       0x9

 extern  unsigned char SPMax;
 
 const unsigned char RIFFHeader0[] = {
   'R' , 'I' , 'F' , 'F' , // Chunk ID (RIFF)
   0x70, 0x70, 0x70, 0x70, // Chunk payload size (calculate after rec!)
   'W' , 'A' , 'V' , 'E' , // RIFF resource format type
   
   'f' , 'm' , 't' , ' ' , // Chunk ID (fmt )
   0x14, 0x00, 0x00, 0x00, // Chunk payload size (0x14 = 20 bytes)
   0x11, 0x00,             // Format Tag (IMA ADPCM)
   0x01, 0x00,             // Channels (1)
   0x80, 0x3e, 0x00, 0x00, // Sample Rate, 0x3e80 = 16.0kHz
   0xd7, 0x0f, 0x00, 0x00, // Average Bytes Per Second
   0x00, 0x01,             // Data Block Size (256 bytes) 
   0x04, 0x00,             // ADPCM encoded bits per sample (4 bits)
   0x02, 0x00,             // Extra data (2 bytes)
   0xf9, 0x01,             // Samples per Block (505 samples)
   
   'f' , 'a' , 'c' , 't' , // Chunk ID (fact)
   0xc8, 0x01, 0x00, 0x00, // Chunk payload size (456 bytes (zeropad!))
   0xff, 0xff, 0xff, 0xff  // Number of Samples (calculate after rec!)
   // Insert 448 zeroes here!
 };
 
 const unsigned char RIFFHeader504[] = {
   'd' , 'a' , 't' , 'a' , // Chunk ID (data)
   0x70, 0x70, 0x70, 0x70  // Chunk payload size (calculate after rec!)
 };
 
 







 unsigned char Record(){
 
   unsigned char blockNumber;
   unsigned long sectorCount;
   unsigned long lastSector;
   //bit stopRecording = 0;
   //bit continueRecording = 0;
 
   //char oldlevel=0;
   //blockNumber = 0;
   //sectorCount = 1;
 
   //playingState = PS_RECORDING; //Inform the world that rec mode is on.
 
   // Kick vs10xx into action!
 
   /* Set clock register, doubler etc. */
   // Set base xtal 14.7456MHZ (0x0696) | multiplier:fixed 2.5 (0x600)
   VS1003B_WriteCMD(SPI_CLOCKF,0xC0,0x00); 

   
   //Set sample rate 16khz for VS1003 (6.4kHz for VS1002 :( )
   VS1003B_WriteCMD(SPI_AICTRL0,0x00,0x09); //Set sample rate divider=9
   VS1003B_WriteCMD(SPI_AICTRL1,0x10,0x00); //AutoGain OFF, reclevel 0x1000
   VS1003B_WriteCMD(SPI_MODE,0x18,0x04);    //RECORD,NEWMODE,RESET
   Delay(10);
 
   /* Set clock register, doubler etc. */
   // Set base xtal 14.7456MHZ (0x0696) | multiplier:fixed 2.5 (0x600)
   VS1003B_WriteCMD(SPI_CLOCKF,0x66,0x96); 




  
  
 
   /*
   // Locate free space
   //freeSector = 0;
   //ScanForFreeSector();
   //sectorAddress.l = freeSector;
   //fragment[0].start = freeSector;
 
   
   for (temp.c=0; temp.c<56; temp.c++){ //temp is unsafe global temp variable.
     diskSect.raw.buf[temp.c] = RIFFHeader0[temp.c];
   }
   for (temp.i=52; temp.i<504; temp.i++){
     diskSect.raw.buf[temp.i] = 0;
   }
   for (temp.i=504; temp.i<512; temp.i++){
     diskSect.raw.buf[temp.i] = RIFFHeader504[temp.i-504];
   }
   
 
   //lastSector = freeSector;
   //WriteDiskSector(freeSector);
   
   /* Get a new free sector */
   //ScanForFreeSector();
   //sectorAddress.l = freeSector;
   //dataBufPtr = diskSect.raw.buf;
   
   /*
   Delay(10);
   //Sync to incoming audio frame...
   while (VS1003B_ReadCMD(SPI_HDAT1)>>8); //lots of data in buffer
     //Wait until buffer level restarts from 0
   
   //dataBufPtr = diskSect.raw.buf; //reset dataBufPtr to start of filebuffer
   //blockNumber = 0;
 
   printf("\rRecording, push button to stop...");
  
   while((!KEY_BUTTON 
         || (blockNumber!=0)
         || ((sectorCount)%(fatSectorsPerCluster)!=0))
         && (!stopRecording)){  
	
      
     //GREEN_LED = LED_ON;
 
     //Check for data
     if (VS1003B_ReadCMD(SPI_HDAT1) >= 128){ 
       //there is a data block to be read...
       //GREEN_LED = LED_OFF;
       blockNumber++;
 
       //dataBufPtr should point inside disk buffer, this should never fail:
       if (dataBufPtr>(diskSect.raw.buf+511)){
         printf("\rBuffer indexing error. Stop.\r");
         while(1); //stop
       }
       
       //Get the data
       for (temp.c = 0; temp.c < 128; temp.c++){
         data unsigned int i;
         i = VS1003B_ReadCMD(SPI_HDAT0);         
         *dataBufPtr++ = (i >> 8);
         *dataBufPtr++ = (i & 0xff);
       }
       
       
       { //Do basic SOUND LEVEL BAR calculation based on the 1 linear
         //sample in block[0] and [1]
         signed int soundlevel;
         // the user interface (level bar) calculation
         if (uiMode == UI_TITLE){
           soundlevel = (signed char)diskSect.raw.buf[1]<<7;
           soundlevel |= diskSect.raw.buf[0]>>1;
           if (soundlevel<0) soundlevel = -soundlevel;
           displayValue=0;
           while (soundlevel>31){
             displayValue++;         
             soundlevel>>=1;
           }
           if (soundlevel>19) displayValue++;
           if (soundlevel>12) displayValue++;
           if (soundlevel>6) displayValue++;
           displayValue-=3;
           displayValue*=13;
           if (oldlevel>displayValue){
             displayValue=oldlevel-3;
           }
           oldlevel=displayValue;       
         }     
         AvailableProcessorTime();
         
       }//SOUND LEVEL BAR calculation
       
     }//if there was data to read
     
     
     //Release SCI chip select, we might want to use MMC card
     Mp3DeselectControl();
     
     if (blockNumber==2){ //2 blocks (512 bytes) received, write to disk...
       
       //If we update the SPI_DECODE_TIME, it will show on player screen :)
       //we can do it, no problem. Here is the calculation for 16khz;
       //for 16kHz sample rate, 505 samples/block, 8 disk sectors are
       //exactly 505 milliseconds. 
       temp.l = (sectorCount/8) * 505;
       temp.l /= 1000;
       VS1003B_WriteCMD(SPI_DECODE_TIME,temp.b.b1,temp.b.b0);
       Mp3DeselectControl();
 
       blockNumber = 0;
       sectorCount++;
       WriteDiskSector(sectorAddress.l);
       lastSector = freeSector;
       ScanForFreeSector(); //this would be the proper way to go...
       sectorAddress.l = freeSector; //keep all variables in proper values
       dataBufPtr = diskSect.raw.buf; //reset data buffer pointer
       if (freeSector!=(lastSector+1)){ //end of continuous space, must break!
         stopRecording = 1;
         ConsoleWrite("\nFragment end - can't continue recording!\n");
         InitDisplay(DS_STATIC,"FRAGMENT"," LIMIT!!",0);
         continueRecording = 1;
       }
     }
     
     displayValue = 0;
     
   }//while not button - stop recording when BUTTON is pressed
   fragment[0].length = sectorCount;
   
 
   //Enter size information to RIFF header..
   {
     xdata addressType size;
 
     ReadDiskSector(fragment[0].start);
     
     //Patch in number of samples
     size.l = (sectorCount-1)*1010;
     diskSect.raw.buf[48] = size.b.b0;
     diskSect.raw.buf[49] = size.b.b1;
     diskSect.raw.buf[50] = size.b.b2;
     diskSect.raw.buf[51] = size.b.b3;
 
     //Patch in size of RIFF chunk...
     size.l = (sectorCount*512)-8;
     diskSect.raw.buf[4] = size.b.b0;
     diskSect.raw.buf[5] = size.b.b1;
     diskSect.raw.buf[6] = size.b.b2;
     diskSect.raw.buf[7] = size.b.b3;
 
     //Patch in size of DATA chunk...
     size.l = (sectorCount*512)-512;
     diskSect.raw.buf[508] = size.b.b0;
     diskSect.raw.buf[509] = size.b.b1;
     diskSect.raw.buf[510] = size.b.b2;
     diskSect.raw.buf[511] = size.b.b3;
 
     WriteDiskSector(sectorAddress.l);
   }
 
   ConsoleWrite("Registering... FStart:");
   ConsolePutUInt(fragment[0].start);
   ConsoleWrite("Registering... Size:");
   ConsolePutUInt(fragment[0].length);
   
   //Create FAT records.
   fragment[1].start = 0x0fffffff; //fragment 0 is the only fragment
   WriteClusterChain(); //register newly created file in FAT FS
 
   sectorAddress.l = 0; //force sector reload for next access
   
   Mp3Reset();
   if (continueRecording) 
   		return PS_RECORDING;
   return PS_NEXT_SONG;

   */
   
 }
 
