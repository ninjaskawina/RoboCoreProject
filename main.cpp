#include "hFramework.h"
#include <stdio.h>
#include "Lego_Touch.h"

using namespace hFramework;
using namespace hSensors;
bool motorOpen(bool open) {
    int power = 100;
    if(open) {
        printf("\rOpening...\n");
        hMot1.rotAbs(0);
    } else {
        printf("\rClosing...\n");
        hMot1.rotAbs(100);
    }
    printf("\rDone!\n");
    hMot1.setPower(0);
    return open;
}
void encoder()
{
    while (true)
    {
        // Print the current rotation of hMot1 in encoder ticks
        sys.delay(100);
    }
}
void hMain(void)
{
    sys.setLogDev(&Serial);
    Lego_Touch sensor(hSens1);
    // This creates a task that will execute `encoder` concurrently
    sys.taskCreate(encoder);
    
    bool maindown = false;
    int counter = 10;
    bool open = false;
    
    while(counter > 0) {
        bool s = sensor.isPressed();
        
        if(s) { // Changing mode
            if(!maindown) {
                counter++;
                printf("Button down, %d times more", counter);
                maindown = true;
                if(open) {
                    motorOpen(false);
                    open = false;
                } else {
                    motorOpen(true);
                    open = true;
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