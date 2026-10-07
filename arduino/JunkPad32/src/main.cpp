#include <Arduino.h>
#include <Keypad.h>

const byte NB_ROWS = 6;
const byte NB_COLS = 4;

const byte NB_LEDS = 3;

byte ledPins[NB_LEDS] = {D11, D12, D14};
byte ledMode[NB_LEDS]={2,0,0};//0:off, 1:on, 2:blink


bool connected = false;
String rxLine = "";

char keys[NB_ROWS][NB_COLS] = {
    // 4    7    9    11
    {'D', 'U', 'a', 'H'}, // 2
    {'b', 'c', 'd', 'R'}, // 3
    {'0', 'e', 'f', 'g'}, // 5
    {'8', '7', '9', 'h'}, // 6
    {'5', '4', '6', 'i'}, // 8
    {'2', '1', '3', 'L'} // 10

};

byte rowPins[NB_ROWS] = {A1, D2, D4, D5, D7, D9};
byte colPins[NB_COLS] = {D3, D6, D8, D10};


Keypad keyboard = Keypad(makeKeymap(keys), rowPins, colPins, NB_ROWS, NB_COLS);

// functions

String keyNames(char code);
void applyLed(byte i);
void processCommand(String cmd);
void readSerial();

void setup()
{
  Serial.begin(9600);
  delay(2000);          // laisse le temps d'ouvrir le moniteur
  keyboard.setDebounceTime(20);

   for (byte i = 0; i < NB_LEDS; i++) // initialise les LED
  {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], HIGH);            // éteinte au démarrage
  }
}

void loop()
{ 
 
  readSerial();
  char code = keyboard.getKey();

  if (code != NO_KEY)
  {
    String name = keyNames(code);
    if (name != "UNKNOWN") Serial.println(name);
  }

   for (byte i = 0; i < NB_LEDS; i++) applyLed(i);
  

}

void applyLed(byte i)
{
  bool on = (ledMode[i] == 1) ||
            (ledMode[i] == 2 && (millis() / 300) % 2 == 0);
  digitalWrite(ledPins[i], on ? LOW : HIGH);   // anode commune : LOW = allumé
}

void processCommand(String cmd)
{
  if (cmd == "ID_REQUEST")
  {
    Serial.println("JUNKPAD");
  }
  else if (cmd.startsWith("LED") && cmd.length() >= 6 && cmd.charAt(4) == ':')
  {
    int idx = cmd.charAt(3) - '1';             // LED1 -> 0, LED2 -> 1...
    int val = cmd.substring(5).toInt();
    if (idx >= 0 && idx < NB_LEDS && val >= 0 && val <= 2)
      ledMode[idx] = val;
  }
}
void readSerial()
{
  while (Serial.available())
  {
    char c = Serial.read();
    if (c == '\n')
    {
      rxLine.trim();
      processCommand(rxLine);
      rxLine = "";
    }
    else if (c != '\r' && rxLine.length() < 32)
    {
      rxLine += c;
    }
  }
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