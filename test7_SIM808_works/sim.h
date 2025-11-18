
#if defined(__AVR__)

//Define some helper macro here 

#ifndef __HAVE_SIM808_H 
#define __HAVE_SIM808_H 

#endif 
// Maybe some additional features here 

#endif

  /*while (Serial.available())
    Serial.read();
}*/

void ringBuzzer(const int buzzer)
{
  tone(buzzer, 1000);
  delay(250);
  noTone(buzzer);
  delay(250);
  noTone(buzzer);
}

void flushSerial() {
  while (Serial.available())
    Serial.read();
}
