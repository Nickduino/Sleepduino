/* This software is released under the Do whatever you want
   with it licence. Sleep.ino has to be in the same folder. */
   
   
//#define debug //Uncomment to debug through serial

#define CYCLES 7 // Number of 8-second-cycles you wish to put it asleep for
#define LED 13
byte cycle = 0;

void flash();
void sleep(); // sleep.ino
void wake();  // sleep.ino
ISR(WDT_vect);  // sleep.ino


void flash() { // <== Put the useful stuff there
  pinMode (LED, OUTPUT);
#ifdef debug
  Serial.println("Flash!");
  delay(30);
#endif
  for (byte i = 0; i < 25; i++) {
    digitalWrite (LED, HIGH);
    delay (100);
    digitalWrite (LED, LOW);
    delay (100);
    }
}  // end of flash
 
void setup () {
#ifdef debug
  Serial.begin(9600);
  delay(1000);
  Serial.print("Vous avez choisi d'endormir le microcontroleur toutes \nles ");
  Serial.print(CYCLES*8);
  Serial.print(" secondes.\n\n");
  delay(50);
#endif

  flash(); // I launch it once upon startup
}

void loop () {
  if(cycle >= CYCLES) { // Check number of cycles since last action
    cycle = 0;
    flash ();  
  }
  else {
#ifdef debug
  Serial.print("Nothing to do, back to sleep! ");
  Serial.println(cycle);
  delay(40);
#endif
  }
  
  sleep();
/* -> When the watchdog timer will trigger the interrupt,
     execution will be resumed here. */
  wake();
} // end of loop

