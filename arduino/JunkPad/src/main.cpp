#include <Arduino.h>

// put function declarations here:

bool buttonState13 = false;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, INPUT_PULLUP);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (digitalRead(13) == LOW)
  {
    if (buttonState13 == false)
    {
      Serial.println("BTN_1_OFF");
      buttonState13 = true;
      delay(100);
    }

  }
  else
  {
    if (digitalRead(13) == HIGH)
    {
      if (buttonState13 == true)
      {
        Serial.println("BTN_1_ON");
        buttonState13 = false;
        delay(100);
      }

    }
  }
}
