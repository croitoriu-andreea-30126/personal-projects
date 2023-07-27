#include <Servo.h> 
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
#include <Keypad.h> 
#include <Password.h>

#define buzzer 10 
int relay_pin = 12; 
#define ledVerde 13 
#define ledRosu 14 
Servo servo; 
byte a = 5;

Password password = Password("1234"); //Enter your password

byte maxPasswordLength = 6; 
byte currentPasswordLength = 0; 
const byte ROWS = 4; 
const byte COLS = 4;

char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}

};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() 
{ 
  pinMode(buzzer, OUTPUT); 
  pinMode(ledVerde,OUTPUT); 
  pinMode(ledRosu,OUTPUT); 
  pinMode(relay_pin,OUTPUT); 
  lcd.begin(16,2); 
  lcd.backlight(); 
  servo.attach(11); 
  digitalWrite(relay_pin,HIGH); 
  servo.write(0); 
  lcd.setCursor(0, 0); 
  lcd.print("WELCOME TO"); 
  lcd.setCursor(0, 1); 
  lcd.print("DOOR LOCK SYSTEM"); 
  delay(3000);
  lcd.clear(); 
}

void loop() 
{ 
  lcd.setCursor(1, 0); 
  lcd.print("ENTER PASSWORD");
  digitalWrite(ledVerde,LOW); 
  digitalWrite(ledRosu,HIGH); 
  char key = keypad.getKey(); 
  if (key != NO_KEY) 
  { 
    delay(60); 
    if (key == 'C') 
    { 
      resetPassword(); 
    } 
    else if (key == 'D') 
    { 
      dooropen(); 
    } 
    else 
    { 
      processNumberKey(key); 
    } 
  } 
}

void processNumberKey(char key) 
{ 
  lcd.setCursor(a, 1); 
  lcd.print("*"); 
  a++; 
  if (a == 11) 
  { 
    a = 5; 
  } 
  currentPasswordLength++; 
  password.append(key); 
}

void dooropen() 
{ 
  if (password.evaluate()) 
  { 
    lcd.setCursor(0, 0); 
    lcd.print("CORRECT PASSWORD"); 
    lcd.setCursor(0, 1); 
    lcd.print(" DOOR IS OPEN "); 
    delay(100);
    digitalWrite(ledVerde,HIGH); 
    digitalWrite(ledRosu,LOW); 
    digitalWrite(buzzer, HIGH); 
    delay(300); 
    digitalWrite(buzzer, LOW); 
    delay(100); 
    digitalWrite(relay_pin,LOW); 
    delay(500); 
    servo.write(100); 
    delay(3000); 
    digitalWrite(relay_pin,HIGH); 
    delay(2000); 
    servo.write(0); 
    digitalWrite(ledVerde,LOW); 
    digitalWrite(ledRosu,HIGH); 
    a = 5; 
  }  
  else 
  { 
    digitalWrite(relay_pin,HIGH); 
    lcd.setCursor(0, 0); 
    lcd.print("WRONG PASSWORD!"); 
    lcd.setCursor(0, 1); 
    lcd.print("PLEASE TRY AGAIN"); 
    delay(100); 
    digitalWrite(ledVerde,LOW); 
    digitalWrite(ledRosu,HIGH); 
    digitalWrite(buzzer, HIGH); 
    delay(200); 
    digitalWrite(buzzer, LOW); 
    delay(200); 
    digitalWrite(buzzer, HIGH); 
    delay(200); 
    digitalWrite(buzzer, LOW); 
    delay(200); 
    digitalWrite(buzzer, HIGH); 
    delay(200); 
    digitalWrite(buzzer, LOW); 
    delay(200); 
    a = 5; 
  }
  resetPassword();
}

void resetPassword() 
{ 
  password.reset(); 
  currentPasswordLength = 0; 
  lcd.clear();
   a = 5; 
}