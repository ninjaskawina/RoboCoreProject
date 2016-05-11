#include "hFramework.h"
#include <stdio.h>
#include <string>

using namespace hFramework;

void hMain(void)
{
    std::string buffer;
    std::string tocomp;
    tocomp = std::string(1, (char)0x02)+"110D04B3F5F2A448801859"+std::string(1, (char)0x03);
	sys.setLogDev(&Serial);
	hExt1.serial.init(57600, NONE, ONE);
	hExt1.serial.selectSerial();
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
                                   LED1.off();
                                }else{
                                    if(!buffer.substr(5, (buffer.length()-10)).compare("04B3F5F2A44880")){
                                    LED1.on();
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
