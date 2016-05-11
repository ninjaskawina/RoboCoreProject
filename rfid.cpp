#include "hFramework.h"
#include <stdio.h>
#include <string>

using namespace hFramework;

class RFIDReader {
private:
    std::string buffer = "";
    bool inRange = false;
public:
    RFIDReader() {
        hExt1.serial.init(57600, NONE, ONE);
        hExt1.serial.selectSerial();
    };
    bool available() {
        while(hExt1.serial.available() > 0) {
            char c =  hExt1.serial.getch();
            this->buffer += c;
            if(c == (char)0x03){
                //Parse
                //Check length
                //Check checksum
                if(true){
                    if(this->buffer.length() >= 10){
                        if(!this->buffer.substr(3, 2).compare("0D")){
                            if(this->buffer.substr((buffer.length()-5), 2) == "0A"){
                            }else{
                                if(!this->buffer.substr(5, (this->buffer.length()-10)).compare("04B3F5F2A44880")){
                                    if(!this->inRange) {
                                        this->inRange = true;
                                        return true;
                                    } else {
                                        return false;
                                    }
                                } else {
                                    this->inRange = false;
                                    return false;
                                }
                            }
                        }
                    }
                }
                this->buffer = "";
            }
            //110D6BB143D51209
        }
    }
};

void hMain(void)
{
    sys.setLogDev(&Serial);
    Lego_Touch sensor(hSens1);
    
    bool open = false;
    
//    Lock l;
    RFIDReader r;
    
    while(!hBtn1.isPressed()) {
        if(hBtn2.isPressed()) {
//            l.free();
            printf("\rFreed!\n");
        }
        if(r.available()) { // Read the rfid
            LED1.on();
//            if(l.isLocked()) {
//                l.unlock();
//            } else {
//                l.lock();
//            }
        } else {
            LED1.off();
        }
        sys.delay_ms(100);
    }
}
