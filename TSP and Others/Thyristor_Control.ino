  /*
  See http://arduino.cc for help

  This code is for cold spray device control.
  It defines AC (power) zero crossing events and generates control pulse 
  for power TRIAC depending on the analog input voltage level
 
  modified 01 February 2017
  modified 14 February 2017:  control_duration reassaignment added
  by Sergiy Zhelnakov
  */

#define triacPulse A0

boolean enable = false;
int timer1_counter;
int counter = 0;
int counter_hold = 0;
int counter_half_period = 0;
int ctrl_value = 0;
int ctrl_value1 = 0;
int ctrl_value2 = 0;
float tickPerCtrlUnit;
float ctrl_value_x;       // intermediate variable used for calculations
int control_duration = 5;

void set_timer1_interrupt()   //initializes timer1 
{
// initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64886;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  timer1_counter = 65071; //17143; //34286;   // preload timer 65536-16MHz/256/2Hz
        // 65071 for TCNT1 and prescaler 1 give us 255 timer interrupts for 1 half period (16.67 mS)
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS10);    // prescaler = 1
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER1_OVF_vect)        // interrupt service routine (to be taken automatically
                            // from Arduino libraries
{
  TCNT1 = timer1_counter;
    digitalWrite(8, (digitalRead(8) ^ 1));  // just for test purposes now
    counter = counter + 1;
    
    if (enable)
    {
      if (counter == ctrl_value1)
      {
        digitalWrite(triacPulse, HIGH);
      }
      if (counter == ctrl_value2)
      {
        digitalWrite(triacPulse, LOW);
      }
    }
    else
    {
      digitalWrite(triacPulse, LOW);
    }
    
    
}


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(7, INPUT);
  pinMode(9, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(A6, INPUT);
  
  digitalWrite(9, LOW);   // turn OFF everything

//  digitalWrite(7, HIGH);

  digitalWrite(10, LOW);
  digitalWrite(0, LOW);
  digitalWrite(1, LOW);
  digitalWrite(A0, LOW);
  attachInterrupt(4, acon, CHANGE);
  set_timer1_interrupt();
  Serial.begin(9600);      // for test purposes

//    cli();          // disable global interrupts
//    sei();
}

// the loop function runs over and over again forever
// for now, nothing important for actual functionality of the
// device 
void loop() {
  if (counter_half_period / 10 == 0)
  {
    digitalWrite(9, (digitalRead(9) ^ 1));   // change D9 for test purposes
  }
if (counter_half_period > 100)     // also for test purposes
  {
    Serial.print(counter_hold);
    Serial.print("  ");
    Serial.print(ctrl_value);
    Serial.print("  ");
    Serial.print(ctrl_value_x);
    Serial.print("  ");
    Serial.print(tickPerCtrlUnit);
    Serial.print("  ");
    Serial.print(ctrl_value1);
    Serial.print("  ");
    Serial.println(ctrl_value2);
    counter_half_period = 0;
  }
  
}

// begin zero crossing interrupt routine
void acon() 
{
//  noInterrupts();           // disable all interrupts

   counter_hold = counter;
   counter = -1;                         //initialization ov counter for the next half period calculations\
   tickPerCtrlUnit = counter_hold/255.0;   //calculation of number of timer ticks peer unit of control
   digitalWrite(8, LOW);
   counter_half_period = counter_half_period + 1;
   ctrl_value = analogRead(A6)/4;     //the result: integer range 0 ... 255
   ctrl_value = 255 - ctrl_value;     // 255 ... 0  now: "0" -> min phase
                                      //                 "255" -> max phase
  
   if (ctrl_value >248)
   {
     enable = false;    //disable TRIAC control pulse, because its duration is defined as 5 timer ticks
   }                    //so minimal phase corresponds approximately 6 timer ticks out of approx.255.
   else
   {
     enable = true;
   }

  ctrl_value_x = ctrl_value*tickPerCtrlUnit;

  if (ctrl_value < 20)
 {
   control_duration = 15;
 }
 else
 {
   control_duration = 5;
 }


  ctrl_value1 = (int)ctrl_value_x;
  ctrl_value2 = ctrl_value1 + control_duration;    

//  ctrl_value1 = 255 - ctrl_value;
//  ctrl_value2 = ctrl_value1 + 5;    

//  interrupts();             // enable all interrupts
