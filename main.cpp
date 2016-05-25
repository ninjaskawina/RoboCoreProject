#include "hFramework.h"
#include <stdio.h>
#include <string>
#include <vector>

using namespace hFramework;

class Lock {
private:
    int power = 500;
    int angle1 = -40;
    int angle2 = 90;
    bool block = 1;
    bool locked = true;
    
    bool open(bool open) {
        if(open) {
            printf("\rOpening...\n");
            hMot1.rotAbs(this->angle1, this->power, this->block);
        } else {
            printf("\rClosing...\n");
            hMot1.rotAbs(this->angle2, this->power, this->block);
        }
        this->locked = !open;
        printf("\rDone!\n");
        return open;
    };
public:
    Lock(int power, int angle1, int angle2) {
        this->power = power;
        this->angle1 = angle1;
        this->angle2 = angle2;
    };
    Lock() {
    };
    bool lock() {
        return this->open(false);
    }
    bool unlock() {
        return this->open(true);
    }
    bool change() {
        return this->open(!this->locked);
    }
    bool isLocked() {
        return this->locked;
    }
    bool calibrate() {
        return true;
    }
    bool free() {
        return hMot1.stopRegulation(this->block, 10);
    }
};

class RFIDReader {
private:
    std::string tag = "";
    std::vector<std::string> tags = {"04B3F5F2A44880"};
    bool newTagAvaliable = false;
    void sendCommand() {
        hExt1.serial.printf("%c11090A41%c", (char)0x02, (char)0x03);
    }

    bool readId() {
        std::string buffer = "";
        while(hExt1.serial.available() > 0) {
            char c =  hExt1.serial.getch();
            buffer += c;
            if(c == (char)0x03){
                //Parse
                //Check length
                //Check checksum
                if(buffer.length() >= 10){
                    if(!buffer.substr(3, 2).compare("0D")){
                        if(buffer.substr((buffer.length()-5), 2) != "0A"){
                            LED3.on();
                            if(buffer.substr(5, (buffer.length()-10)).compare(this->tag)){
                                // New tag spotted
                                this->tag = buffer.substr(5, (buffer.length()-10));
                                this->newTagAvaliable = true;
                            }
                            return true;
                        } else {
                            LED3.off();
                        }
                    }
                }
            }
            //110D6BB143D51209
        }
        this->sendCommand();
        // No tag
        return false;
    }
public:
    RFIDReader() {
        hExt1.serial.init(57600, NONE, ONE);
        hExt1.serial.selectSerial();
    };
    bool compareTag() {
        if(this->readId()) {
            if(this->newTagAvaliable) {
                this->newTagAvaliable = false;
                for(unsigned int i=0; i<tags.size(); i++) {
                    if(!tags[i].compare(this->tag)) {
                        LED1.on();
                        return true;
                    }
                }
                return false;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
};

void hMain(void){
    sys.setLogDev(&Serial);
    
    Lock l;
    RFIDReader r;
    
    l.unlock();
    while(!hBtn1.isPressed()) {
        LED1.off();
        if(hBtn2.isPressed()) {
            l.free();
            printf("\rFreed!\n");
        }
        if(r.compareTag()) { // Check if spotted tag and it is in the list
            if(l.isLocked()) {
                LED2.on();
                l.unlock();
            } else {
                LED2.off();
                l.lock();
            }
        }
        sys.delay_ms(100);
    }
}