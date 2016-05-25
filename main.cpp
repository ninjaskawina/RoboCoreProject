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
    const std::string masterTag = "04B3F5F2A44880";
    bool masterMode = false;
    std::string tag = "";
    std::vector<std::string> tags = {};
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
                // Check length
                if(buffer.length() >= 10) {
                    // Check command
                    if(!buffer.substr(3, 2).compare("0D")) {
                        // Check checksum
                        if(buffer.substr((buffer.length()-5), 2) == "0A") {
                            // 
                        } else {
                            // New tag spotted
                            if(buffer.substr(5, (buffer.length()-10)).compare(this->tag)){
                                LED3.on();
                                this->tag = buffer.substr(5, (buffer.length()-10));
                                this->newTagAvaliable = true;
                            }
                            return true;
                        }
                    }
                }
            }
            //110D6BB143D51209
        }
        LED3.off();
        this->sendCommand();
        // No tag
        return false;
    }
    bool tagInList() {
        for(unsigned int i = 0; i < this->tags.size(); i++) {
            if(!this->tags[i].compare(this->tag)) {
                return true;
            }
        }
        return false;
    }
    void toggleMaster(bool enter) {
        if(enter) {
            LED2.on();
            this->masterMode = true;
        } else {
            LED2.off();
            this->masterMode = false;
        }
    }
    bool manageTag() {
        if(this->tagInList()) {
            for(unsigned int i=0; i < this->tags.size(); i++) {
                if(!this->tags[i].compare(this->tag)) {
                    this->tags.erase(this->tags.begin() + i);
                    this->toggleMaster(false);
                    return true;
                }
            }
        } else {
            this->tags.push_back(this->tag);
        }
        this->toggleMaster(false);
        return true;
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
                // Check if card is master
                if(!this->tag.compare(this->masterTag)) {
                    this->toggleMaster(!this->masterMode);
                } else {
                    if(this->masterMode) {
                        this->manageTag();
                    } else {
                        return this->tagInList();
                    }
                }
            }
        }
        return false;
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
                l.unlock();
            } else {
                l.lock();
            }
        }
        sys.delay_ms(100);
    }
}