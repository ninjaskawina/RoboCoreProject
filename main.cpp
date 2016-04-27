#include "hFramework.h"
#include <stdio.h>
#include "Lego_Touch.h"

using namespace hFramework;
using namespace hSensors;
class Lock {
private:
    int power = 1000;
    int angle1 = 0;
    int angle2 = 180;
    bool block = 1;
    bool locked = true;
    hMotor& mot;
    
    bool open(bool open) {
        if(open) {
            printf("\rOpening...\n");
            this->mot.rotAbs(this->angle1, this->power, this->block);
        } else {
            printf("\rClosing...\n");
            this->mot.rotAbs(this->angle2, this->power, this->block);
        }
        this->locked = !open;
        printf("\rDone!\n");
        return open;
    };
public:
    Lock(int power, int angle1, int angle2, hMotor& motor) {
        this->power = power;
        this->angle1 = angle1;
        this->angle2 = angle2;
        this->mot = motor;
    };
    Lock(hMotor& motor) {
        this->mot = motor;
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
        
    }
    bool free() {
        return this->mot.stopRegulation(this->block, 10);
    }
};


void hMain(void)
{
    sys.setLogDev(&Serial);
    Lego_Touch sensor(hSens1);
    
    bool maindown = false;
    bool open = false;
    
    Lock l(hMot1);
    
    while(!hBtn1.isPressed()) {
        bool s = sensor.isPressed();
        
        if(hBtn2.isPressed()) {
            l.free();
            printf("\rFreed!\n");
        }
        if(s) { // Changing mode
            if(!maindown) {
                maindown = true;
                if(open) {
                    open = l.lock();
                } else {
                    open = l.unlock();
                }
            }
        } else {
            if(maindown) {
                maindown = false;
            }
        }
        sys.delay_ms(50);
    }
}