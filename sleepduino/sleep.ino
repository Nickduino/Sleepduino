/* You have to open sleepduino.ino in your Arduino IDE. */

// More about sleep: page 39 and following of the datasheet
// More about watchdog timer: page 50 and following

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>


  ISR(WDT_vect) { // Watchdog Interrupt Service Routine
   wdt_disable();  // Disable watchdog then resume sketch
}

void sleep() {
#ifdef debug
  Serial.println("ZZzzz...");
  Serial.println();
  delay(30);
#endif

  ADCSRA = 0; // Analog to Digital Converter off 

  MCUSR = 0; /* For debug purpose. The MCU Status register
                stores the source that caused an MCU reset */
                
  //  WDTCSR = Watchdog Timer Control Register
  //  WDE    = Watchdog System Reset Enable (it's a safety net)
  //  WDIE   = Watchdog Interrupt Enable (will awake the MCU)
  WDTCSR = bit (WDCE) | bit (WDE);
  // Interrupt mode and an interval
  // Setting WDP3 and WDP0 is equal to setting the WDTCSR to 0bxx1xx001 => about 8 sec timeout at 5V
  WDTCSR = bit (WDIE) | bit (WDP3) | bit (WDP0);
  /* To reinitialize the timer, making sure that every
     interrupt occur right when it oughts to. I'm
     just being cocky with ASM here. You should use
     wdt_reset() from <avr/wdt.h>. */
  __asm__ __volatile__ ("wdr"); 

  // Choosing the sleepmode and activating it
  SMCR = bit(SM1); // similar to set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  SMCR |= bit(SE); // activate sleep (similar to sleep_enable())
 
 /* To turn the Brown-Out Detector off. It will be enabled back
    upon wake-up. What the fuck is the BOD? It prevents EEPROM
    corruption by resetting the MCU if the voltage drops below
    the threshold.
   Page 44: the sequence is a headache for morons like me:
   - First you need to set BODS and BODSE (i.e. BODS Enable) to 1
   - Then you set BODS to 1 and BODSE to 0 within 4 clock cycles */
  MCUCR = bit (BODS) | bit (BODSE); // BODS and BODSE to logical 1
  MCUCR = bit (BODS); // BODS (stands for BOD Sleep) only to 1
  
  sleep_cpu (); // Finally!
} // End of sleep

void wake() {
  SMCR &= ~bit(SE);// Cancel sleep (similar to sleep_disable())
#ifdef debug
  Serial.println("Bipbipbipbip bipbipbipbip!");
  delay(30);
#endif
  power_all_enable(); // Not sure we need everything but it doesn't hurt.
  cycle++;
}
