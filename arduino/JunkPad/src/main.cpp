#include <Arduino.h>

// put function declarations here:
bool connected = false;
bool buttonState2 = false;


void setup()
{
  // put your setup code here, to run once:
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600);
  
}

void loop()
{

  if (!connected)
  {
    String cmd = Serial.readStringUntil('\n');
    if (cmd == "ID_REQUEST")
    {
      Serial.println("JUNKPAD");
      delay(100);
    }
    connected = true;
  }
  else
  {

    // put your main code here, to run repeatedly:
    if (digitalRead(3) == HIGH)
    {
      Serial.println("BTN_2_ON");
      delay(100);
    }
    else{
      Serial.println("BTN_2_OFF");
      delay(100);
    }
    if (digitalRead(2) == LOW)
    {
      if (buttonState2 == false)
      {
        Serial.println("BTN_1_OFF");
        buttonState2 = true;
        delay(100);
      }
    }
    else
    {
      if (digitalRead(2) == HIGH)
      {
        if (buttonState2 == true)
        {
          Serial.println("BTN_1_ON");
          buttonState2 = false;
          delay(100);
        }
      }
    }
  }
}
