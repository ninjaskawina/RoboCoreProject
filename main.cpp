#include "hFramework.h"
#include <stdio.h>
#include "Lego_Touch.h"

using namespace hFramework;
using namespace hSensors;
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
    bool motorOpen(bool open) {
        if(open) {
            printf("\rOpening...\n");
            hMot1.setPower(500);
            sys.delay_ms(10000);
        } else {
            printf("\rClosing...\n");
            hMot1.setPower(-500);
            sys.delay_ms(10000);
        }
        printf("\rDone!\n");
        hMot1.setPower(0);
        return on;
    }
    
    bool maindown = false;
    bool open = false;
    
    while (counter < 5)
    {
        bool s = sensor.isPressed();
        
        if(s) { // Changing mode
            if(!maindown) {
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