#include "hFramework.h"
#include "Lego_Touch.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <iomanip>

using namespace hFramework;
using namespace hSensors;

class Lock {
private:
    int power = 500;
    int angle1 = -40;
    int angle2 = 90;
    bool block = 1;
    bool locked = true;
    bool isFree = false;
    bool open(bool open) {
        if(open) {
            LED1.on();
            printf("\rOpening...\n");
            hMot1.rotAbs(this->angle1, this->power, this->block);
        } else {
            LED1.off();
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
    bool isFreed(){
        return isFree;
    }
    bool calibrate() {
        return true;
    }
    bool toggleFree(bool isPressed) {
        if(isPressed){
            if(!isFree){
                isFree = true;
                return hMot1.stopRegulation();
            }
        }else{
            if(isFree){
                if(this->isLocked()) {
                    this->lock();
                } else {
                    this->unlock();
                }
               isFree = false;
            }
        }
    }

};

class RFIDReader {
private:
    std::string buffer = "";
    std::string tag = "";
    std::string masterId = "04B3F5F2A44880";
    bool masterMode = false;
    bool masterEn = false;
    std::vector<std::string> tags = {"91D24535"};
    bool newTagAvaliable = false;
    void sendCommand() {
        hExt1.serial.printf("%c11090A41%c", (char)0x02, (char)0x03);
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
    bool readId() {
        while(hExt1.serial.available() > 0) {
            char c =  hExt1.serial.getch();
            buffer += c;
            if(c == (char)0x03){
                // Check length
                if(buffer.length() >= 10) {
                    if(!buffer.substr(3, 2).compare("0D")) {
                        // Check checksum
                        if(buffer.substr((buffer.length()-5), 2) == "0A") {
                            LED3.off();
                        } else {
                            // New tag spotted
                            LED3.on();
                            if(buffer.substr(5, (buffer.length()-10)).compare(this->tag)){

                                this->tag = buffer.substr(5, (buffer.length()-10));
                                this->newTagAvaliable = true;
                            }
                            return true;
                        }
                    }
                }
                buffer = "";
            }
            //110D6BB143D51209
            //110D6BB143D51209u
            //White
            //91D24535
            //Bluewatch
            //04B3F5F2A44880
            //Purplewatch
            //04B6749A043E80
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
        bool toggleMasterMode(bool isPressed) {
        if(isPressed){
            if(!this->masterMode){
                LED2.on();
                masterMode = true;
                masterEn = true;
            }
        }else{
            if(masterMode){
                LED2.off();
                masterMode = false;
                masterEn = false;
            }
        }
        return true;
    }

    bool compareTag() {
        if(this->readId()) {
            if(this->newTagAvaliable) {
                this->newTagAvaliable = false;
                    if(this->masterEn){
                            if(this->tagInList()){
                                this->masterEn = false;

                                for(unsigned int i = 0; i < this->tags.size(); i++) {
                                    if(!this->tags[i].compare(this->tag)) {
                                        this->tags.erase(this->tags.begin() + i);
                                    }
                                }

                                LED2.off();
                            } else {
                                this->masterEn = false;
                                this->tags.push_back(this->tag);
                                LED2.off();
                            }
                    }else{
                            for(unsigned int i=0; i<this->tags.size(); i++) {
                                if(!tags[i].compare(this->tag)) {
                                    return true;
                                }
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
    Lego_Touch doorSens(hSens1);

    Lock l;
    RFIDReader r;

    l.unlock();
    while(true) {

        l.toggleFree(hBtn2.isPressed()); //Check for freing
        r.toggleMasterMode(hBtn1.isPressed());

        if(!l.isFreed()){
            if(r.compareTag()) { // Check if spotted tag and it is in the list
                if(l.isLocked()) {
                    l.unlock();
                } else {
                    if(doorSens.isPressed()){
                    l.lock();
                    }
                }
            }
        }
        sys.delay_ms(100);
    }
}

