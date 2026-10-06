#include <Arduino.h>
#include <Keypad.h>

const byte NB_ROWS = 6;
const byte NB_COLS = 4;

bool connected = false;


char keys[NB_ROWS][NB_COLS] = {
    // 4    7    9    11
    {'D', 'U', 'a', 'H'}, // 2
    {'b', 'c', 'd', 'R'}, // 3
    {'0', 'e', 'f', 'g'}, // 5
    {'8', '7', '9', 'h'}, // 6
    {'5', '4', '6', 'i'}, // 8
    {'2', '1', '3', 'L'} // 10

};

byte ledPins[3] = {D11, D12, D14};

byte rowPins[NB_ROWS] = {A1, D2, D4, D5, D7, D9};
byte colPins[NB_COLS] = {D3, D6, D8, D10};


Keypad keyboard = Keypad(makeKeymap(keys), rowPins, colPins, NB_ROWS, NB_COLS);

// functions

String keyNames(char code);

void setup()
{
  Serial.begin(9600);
  delay(2000);          // laisse le temps d'ouvrir le moniteur
  keyboard.setDebounceTime(20);

  Serial.println("setup");
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
    char code = keyboard.getKey();

    if (code != NO_KEY)
    {
      // Serial.print("code: ");
      // Serial.println(code);
      // Serial.print("key: ");
      Serial.println(keyNames(code));
    }
  }

   // etat actuel (PRESSED, HOLD, RELEASED, IDLE)
  // static KeyState last = IDLE;
  // KeyState s = keyboard.getState();
  // if (s != last)
  // {
  //   last = s;
  //   Serial.print("etat: ");
  //   switch (s)
  //   {
  //   case IDLE:     Serial.println("IDLE");     break;
  //   case PRESSED:  Serial.println("PRESSED");  break;
  //   case HOLD:     Serial.println("HOLD");     break;
  //   case RELEASED: Serial.println("RELEASED"); break;
  //   }
  // }

}

String keyNames(char code)
{
  switch (code)
  {
  case 'D':
    return "DOWN";
  case 'U':
    return "UP";
  case 'H':
    return "HP";
  case 'R':
    return "R";
  case 'L':
    return "LOCATE";
  case '0':
    return "M0";
  case '1':
    return "M1";
  case '2':
    return "M2";
  case '3':
    return "M3";
  case '4':
    return "M4";
  case '5':
    return "M5";
  case '6':
    return "M6";
  case '7':
    return "M7";
  case '8':
    return "M8";
  case '9':
    return "M9";
  default:
    return "UNKNOWN";
  }
}