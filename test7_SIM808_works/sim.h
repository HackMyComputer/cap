
#if defined(__AVR__)

#ifndef __HAVE_SIM808_H 
#define __HAVE_SIM808_H 
 
// Maybe some additional features here 

#if not defined __always_inline 
#define __always_inline   __attribute__((always_inline)) 
#endif // __always_inline

__always_inline void ringBuzzer(const int buzzer)
{
  tone(buzzer, 1000);
  delay(250);
  noTone(buzzer);
  delay(250);
  noTone(buzzer);
}

__always_inline void flushSerial() {
  while (Serial.available())
    Serial.read();
}

#endif // __HAVE_SIM808_H 
#endif // __AVR__
