#include <Arduino.h>

// put function declarations here:

int buttonState13 = 0;
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
    if (buttonState13 == 0)
    {
      Serial.println("BUTTON_1_OFF");
      buttonState13 = 1;
      delay(100);
    }

  }
  else
  {
    if (digitalRead(13) == HIGH)
    {
      if (buttonState13 == 1)
      {
        Serial.println("BUTTON_1_ON");
        buttonState13 = 0;
        delay(100);
      }

    }
  }
}
