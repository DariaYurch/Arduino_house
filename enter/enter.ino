//..................................................................
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
//.............................ИМПОРТИРУЕМ ВСЕ НЕОБХОДИМЫЕ БИБЛИОТЕКИ
//...................................................................
#define BUZZER_PIN 2
String kod="562",num;
int a,err,piezoPin = 13;
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS] [COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {12,11,10,9};
byte colPins[COLS] = {4,5,6,7};
//..............................ИНИЦИАЛИЗИРУЕМ ВСЕ НУЖНЫЕ ПЕРЕМЕННЫЕ
//..................................................................
Keypad keypad = Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);
Servo servo; 
LiquidCrystal_I2C lcd(0x27,16,2);
//..............................ОБЪЯВЛЯЕМ ВСЕ ИСПОЛЬЗУЕМЫЕ УСТРОЙСТВА

//..............................НАСТРАИВАЕМ ВСЕ НЕОБХОДИМЫЕ ПИНЫ(НА ВХОД ИЛИ ВЫХОД)
void setup() {
  servo.attach(8);
  lcd.init();                     
  lcd.backlight();
  lcd.setCursor(0, 0);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(13,OUTPUT);
}
//.................................................................................

void loop() {
  //....................................................ЧАСТЬ КОДА,КОТОРАЯ ВСЕГДА ОЖИДАЕТ ВВОДА КОДА ПОЛЬЗОВАТЕЛЕМ
  digitalWrite(13,HIGH);
  digitalWrite(3,HIGH);
  char key = keypad.getKey();
  Serial.print(key);
  a = num.length();
  lcd.setCursor(0,0);
  lcd.print("Enter a code:");
  servo.write(0);
  //.........................................................................................................
  //.....................................................ЕСЛИ ПОЛЬЗОВАТЕЛЬ НАЧАЛ ЧТО-ТО ВВОДИТЬ,АКТИВИРУЕТСЯ ЭТА ЧАСТЬ ПРОГРАММЫ
  if(key)
  {
    if(key && key != '*' && key != '#')//................ЕСЛИ НАЖАТА ЛЮБАЯ КНОПКА,КРОМЕ * И #,ТО ВЫВОДИМ НАЖАТЫЙ СИМВОЛ НА ЭКРАН
    {
      num = num + key;
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print(num);
    }
    //......................................................................................................................
    if(key == '#')//.............................ЕСЛИ НАЖАТА КНОПКА #,СТИРАЕМ ЭЛЕМЕНТ С ЭКРАНА
    {
      lcd.clear();
      num.remove(a - 1);
      lcd.setCursor(0,1);
      lcd.print(num);
    }
    //........................................................................................
    if(key == '*')//.............................ЕСЛИ НАЖАТА КНОПКА *,НАЧИНАЕМ ПРОВЕРКУ ВВЕДЕННОГО КОДА
    {
    num = num + "";
    lcd.clear();
    if(num == kod)//............................ЕСЛИ КОД ВЕРНЫЙ ВЫВОДИМ ПРИГЛАШЕНИЕ КО ВХОДУ НА ЭКРАН И ОТКРЫВАЕМ ДВЕРИ
    {
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("You are welcome!");
      servo.write(180);
      delay(5000);
      lcd.clear();
      
    }
    //.................................................................................................................
    else//....................................ЕСЛИ КОД НЕВЕРНЫЙ,ДАЕМ ПОВТОРНУЮ ПОПЫТКУ
    {
      lcd.print("Error!!");
      lcd.setCursor(0,1);
      lcd.print("Try again");
      err = err + 1;
      delay(3000);
      num = "";
      lcd.clear();
    }
    //.................................................................................
    if (err >= 3)//......................ЕСЛИ НЕУДАЧНЫХ ПОПЫТОК БОЛЬШЕ 3,АКТИВИРУЕМ СИГНАЛИЗАЦИЮ,ДО ВВЕДЕНИЯ СЕКРЕТНОЙ ПОСЛЕДОВАТЕЛЬНОСТИ      
        {
        lcd.clear();
        lcd.setCursor(0,1);
        while(key != 'A')
        {
          key = keypad.getKey();
          lcd.print("ALARM ACTIVATED");
          tone(2,3000,1000);
          tone(2,1000,1000);
          digitalWrite(13,LOW);
          digitalWrite(3,HIGH);
          delay(500);
          digitalWrite(13,HIGH);
          digitalWrite(3,LOW);
          delay(500);
        }
        lcd.clear();
        err = 0;
      }
      //...............................................................................................................................
  }
  //.........................................................
}
//...............................................................
}
