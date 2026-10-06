// #include <Arduino.h>
// #include <Keypad.h>

// const byte NB_ROWS = 1;
// const byte NB_COLS = 1;

// char keys[NB_ROWS][NB_COLS] = {
//   {'D'}
// };

// byte rowPins[NB_ROWS] = {A1};
// byte colPins[NB_COLS] = {D3};

// Keypad keyboard = Keypad(makeKeymap(keys), rowPins, colPins, NB_ROWS, NB_COLS);

// void setup()
// {
//   Serial.begin(9600);
//   delay(2000);
//   keyboard.setDebounceTime(20);
//   Serial.println("test keypad A1/D3");
// }

// void loop()
// {
//   char code = keyboard.getKey();
//   if (code != NO_KEY)
//   {
//     Serial.print("touche: ");
//     Serial.println(code);
//   }

//   // etat actuel (PRESSED, HOLD, RELEASED, IDLE)
//   static KeyState last = IDLE;
//   KeyState s = keyboard.getState();
//   if (s != last)
//   {
//     last = s;
//     Serial.print("etat: ");
//     switch (s)
//     {
//     case IDLE:     Serial.println("IDLE");     break;
//     case PRESSED:  Serial.println("PRESSED");  break;
//     case HOLD:     Serial.println("HOLD");     break;
//     case RELEASED: Serial.println("RELEASED"); break;
//     }
//   }
// }