#include <AccelStepper.h>

#define step_top 10
#define dir_top 11
#define step_bot 6 
#define dir_bot 5 

AccelStepper stepper1(AccelStepper::DRIVER, step_top, dir_top);
AccelStepper stepper2(AccelStepper::DRIVER, step_bot, dir_bot);

void setup()
{  
    stepper1.setMaxSpeed(-2000.0);
    stepper1.setAcceleration(-1600.0);
    stepper1.moveTo(-1000000);
    
    stepper2.setMaxSpeed(2000.0);
    stepper2.setAcceleration(1600.0);
    stepper2.moveTo(1000000);

}

void loop()
{
    stepper1.run();
    stepper2.run();   
}
