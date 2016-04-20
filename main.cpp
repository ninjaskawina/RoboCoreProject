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
public:
    Lock(int power, int angle1, int angle2, bool block) {
        this->power = power;
        this->angle1 = angle1;
        this->angle2 = angle2;
        this->block = block;
    };
    Lock() {};
    bool open(bool open) {
        if(open) {
            printf("\rOpening...\n");
            hMot1.rotAbs(this->angle1, this->power, this->block);
        } else {
            printf("\rClosing...\n");
            hMot1.rotAbs(this->angle2, this->power, this->block);
        }
        this->locked = !open;
        hMot1.stopRegulation();
        printf("\rDone!\n");
        return open;
    };
    bool isLocked() {
        return this->locked;
    }
    bool calibrate() {
        
    }
};


void hMain(void)
{
    sys.setLogDev(&Serial);
    Lego_Touch sensor(hSens1);
    
    bool maindown = false;
    bool open = false;
    
    Lock l = new Lock();
    
    while(!hBtn1.isPressed()) {
        bool s = sensor.isPressed();
        
        if(s) { // Changing mode
            if(!maindown) {
                printf("Button down, %d times more", counter);
                maindown = true;
                if(open) {
                    open = l.open(false);
                } else {
                    open = l.open(true);
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