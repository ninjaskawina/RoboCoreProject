#include "hFramework.h"
#include <stdio.h>
#include <string>

using namespace hFramework;

    int power = 500;
    int angle1 = -50;
    int angle2 = 90;
    bool block = 1;
    bool locked = false;


void lock_toggle(){
    if(locked){
            //Open
    hMot1.rotAbs(angle1, power, block);
    hMot1.stopRegulation();
    LED1.on();
    } else{
    hMot1.rotAbs(angle2, power, block);
    hMot1.stopRegulation();
    LED1.off();
    }
    locked = !locked;
}

void hMain(void)
{

    std::string buffer;
    bool inRange = false;
    std::string tocomp;
    tocomp = std::string(1, (char)0x02)+"110D04B3F5F2A448801859"+std::string(1, (char)0x03);
	sys.setLogDev(&Serial);
	hExt1.serial.init(57600, NONE, ONE);
	hExt1.serial.selectSerial();

	hMot1.rotAbs(angle1, power, block);
	for (;;)
	{
	    while(hExt1.serial.available() > 0)
        {
            char c =  hExt1.serial.getch();
            buffer += c;
            if(c == (char)0x03){
                //Parse
                //Check length
                //Check checksum
                if(true){
                    if(buffer.length() >= 10){
                        if(!buffer.substr(3, 2).compare("0D")){
                                if(buffer.substr((buffer.length()-5), 2) == "0A"){
                                    inRange = false;
                                   LED3.off();
                                }else{
                                    LED3.on();
                                    if(!buffer.substr(5, (buffer.length()-10)).compare("04B3F5F2A44880")){
                                        if(!inRange){
                                            inRange = true;
                                            lock_toggle();
                                        }
                                    }
                                    //hExt1.serial.printf("\r\n4\r\n");
                                }
                        }
                    }
                if(!buffer.compare(tocomp)){

                   //hExt1.serial.printf("\r\nOK\r\n");
                }else if(!buffer.compare(std::string(1, (char)0x02)+"110D0A4C"+std::string(1, (char)0x03))){
                    //LED1.off();
                }
                }
                //hExt1.serial.printf("\r\n%s\r\n", tocomp.c_str());
                //hExt1.serial.printf("\r\n%s\r\n", buffer.c_str());
                buffer = "";
            }
            //110D6BB143D51209
        }

		hExt1.serial.printf("%c11090A41%c", (char)0x02, (char)0x03);
		sys.delay_ms(100);
	}
}

