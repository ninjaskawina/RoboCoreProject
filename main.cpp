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
    
    bool maindown = false;
    int counter = 0;
    bool run = false;
    
    while (counter < 5)
    {
        bool s = sensor.isPressed();
        
        if(s) { // Changing mode
            if(!maindown) {
                maindown = true;
                counter++;
                printf("Counter: %d\n",counter);
                if(run) {
                    printf("Main button OFF!\n");
                    run = false;
                } else {
                    printf("Main button ON!\n");
                    run = true;
                }
            }
        } else {
            if(maindown) {
                maindown = false;
            }
        }
        sys.delay_ms(50);
        
        if(run) {
            bool b1 = hBtn1.isPressed();
            bool b2 = hBtn2.isPressed();
            
            if(b1 && !b2) {
                hMot1.setPower(1000);
                sys.delay_ms(50);
            } else if (b2 && !b1) {
                hMot1.setPower(-1000);
                sys.delay_ms(50);
            } else {
                hMot1.stop();
                sys.delay_ms(50);
            }
        }
    }
}