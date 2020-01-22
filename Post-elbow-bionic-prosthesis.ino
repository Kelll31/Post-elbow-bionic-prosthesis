/*
  MMMMMMMMMMMMMMMNc    NMMk     kMMN    cNMMMMMMMMMMMMMMMM
  MMMMMMMMMMMMMMMNc    kOOl     lOOk    cNMMN00OKNMMWKO0OO
  MMMMMMMMMMMMMMMNc                     cNMMk    0MMX    X
  MMMMMMMMMMMMMMMNc                     cNMMO    xKKO    X
  MMMMMMMMMMMMMMMNc                     cNMMWXXXk    dXXXX
  MMMMMMMMMMMMNXXKc                     cKXXNMMM0    xMMMM
  MMMMMMMMMMMXl                             lXMM0    xMMMM
  MMMMMMMMWWW0                               0WWO    xMMMM
  MMMMMMMKc       kkOkk             kkOkk            xMMMM
  MMMMMMMO        oWWWd             oWWWd            xMMMM
  MMM0c           kkOkk             kkOkk        cxxkXMMMM
  MMMx                                           OMMMMMMMM
  MMMx    coox                               cooxXMMMMMMMM
  MMMX    0MMK                               KMMMMMMMMMMMM
  lllX    0MMK     clllllllllllllllllllc     KMMMMMMMMMMMM
        0MMK     WMMMMMMMMMMMMMMMMMMMW     KMMMMMMMMMMMM
        0MMN     dddddddONMMMNOddddddd     NMMMMMMMMMMMM
  MMMX    0MMMMMMNc        KMMMK        cNMMMMMMMMMMMMMMMM
  MMMX    0MMMMMMNc        KMMM0        cNMMMMMMMMMMMMMMMM
                   Kelll31 technology
*/



#include <Servo.h> // Подключаем библиотеку
#include <Wire.h> // Подключаем библиотеку
#include <EEPROM.h> // Подключаем библиотеку
#include <SPI.h> // Подключаем библиотеку
#include <Adafruit_GFX.h> // Подключаем библиотеку
#include <Adafruit_SSD1306.h> // Подключаем библиотеку
#include "FreeMono7.h" // Русский шрифт
#include "bmp.h" // Подключаем лого

#define korector 12 // Задаем корректор чистки
#define ElectrodeR 10
#define ElectrodeL 11
#define Pishalka 12
#define AllServo 1
#define TranzistorsA 2
#define TranzistorsB 3
#define TranzistorsC 4
#define KeyA 7
#define KeyB 8

#define potmizinec A1
#define potbezimani A2
#define potfuck A3
#define potukozatel A4
#define potbig A5

Servo mizinec;
Servo bezimani;
Servo fuck;
Servo ukozatel;
Servo big;
Servo ladon;
Servo povorot;
//========================================================================================
#define FASTADC 1
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) // Для лютого ускореня Аналогово чтения
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
//========================================================================================
#define OLED_MOSI   2
#define OLED_CLK   6
#define OLED_DC    4
#define OLED_CS    20
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS); // Задаем адрес Oled монитора

int SignalSDatchika; // Аналоговый сигнал с датчика
int i; // Сдвиговый регистр
byte x; //Сдвиговый регистр для Датчика
byte y; //Сдвиговый регистр для транзисторов

byte index; // Индекс для очистки
int val_0[3]; // Первый этап очистки (Масcив)
int val_1[3]; // Среднее арифметические 2 (Массив)
int Srednie_0; // Среднее арифметические 1 (Целое единственное число)
int Srednie_1; // Среднее арифметические 2 (Целое единственное число)
int Summa1[10]; //Сумма для работы с датчиком
float SummaDo1; //Сумма для работы с памятью (действие 1)
float SummaDo2; //Сумма для работы с памятью (действие 2)
float SummaDo3; //Сумма для работы с памятью (действие 3)
float SummaDo4; //Сумма для работы с памятью (действие 4)
float SummaDoALL;
int ServoMemory[20];
byte  flash[3000]; //Значения во flash
byte KorektorSravnenia;
byte levo8bit[] = {  B00010,  B00011,  B00111,  B01101,  B11111,  B10111,  B10100,  B10011};
byte pravo8bit[] = {   B01010,  B11001,  B11101,  B10111,  B11110,  B11100,  B00100,  B11000};


void setup() {
  //========================================================================================
#if FASTADC
  sbi(ADCSRA, ADPS2) ;
  cbi(ADCSRA, ADPS1) ; // Для лютого ускореня Аналогово чтения
  cbi(ADCSRA, ADPS0) ;
#endif
  //========================================================================================
  y = 0;
  i = 0;
  x = 0;
  Serial.begin(115200);
  pinMode(KeyA, INPUT);
  pinMode(KeyB, INPUT);
  pinMode(ElectrodeR, INPUT);
  pinMode(ElectrodeL, INPUT);
  pinMode(Pishalka, OUTPUT);
  mizinec.attach(32);
  bezimani.attach(33);
  fuck.attach(34);
  ukozatel.attach(35);
  big.attach(36);
  ladon.attach(37);
  povorot.attach(38);
  KorektorSravnenia = 93; //Основной корректор сравнения для логики

  display.begin(SSD1306_SWITCHCAPVCC);  // Инициализируем Oled
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setFont(&FreeMono7pt8b);
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Kelll31");
  display.println("Technology");
  display.drawBitmap(80, 0, Pixel_lord, 0xD, 0x9, WHITE);
  display.display();
  delay(3000);
  if ((digitalRead(KeyA) > 0) && (digitalRead(KeyB)  > 0))  {
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Первый запуск!");
    display.display();
    digitalWrite(AllServo, LOW);
    delay(3000);
    TuningServo1();
    Pisk();
    delay(3000);
    TuningServo2();
    Pisk();
    delay(3000);
    TuningServo3();
    Pisk();
    delay(3000);
    TuningServo4();
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Первый запуск!");
    display.setCursor(10, 25);
    display.println("Выполнено");
    display.display();
    Pisk();
    digitalWrite(AllServo, HIGH);
    delay(3000);
  }
  SearchValues();
  digitalWrite(AllServo, HIGH);
  delay(1000);

}

void loop() {

  if (digitalRead(ElectrodeR) == 1 || digitalRead(ElectrodeL) == 1) {
    proverka();
  }
  Logic();

}

void Pisk() {

  digitalWrite(Pishalka, HIGH);
  delay(250);
  digitalWrite(Pishalka, LOW);
  delay(250);

}

void ServoDo1() {

  mizinec.write(ServoMemory[1]);
  bezimani.write(ServoMemory[3]);
  fuck.write(ServoMemory[5]);
  ukozatel.write(ServoMemory[7]);
  big.write(ServoMemory[9]);

}

void ServoDo2() {

  mizinec.write(ServoMemory[0]);
  bezimani.write(ServoMemory[2]);
  fuck.write(ServoMemory[4]);
  ukozatel.write(ServoMemory[6]);
  big.write(ServoMemory[8]);

}

void ServoDo3() {

  //Здесь должен быть код


}

void ServoDo4() {

  //Здесь должен быть код

}

void Logic() {
  chistkaAndUsrednenie();
  SummaDoALL = 0;
  SummaDoALL = Summa1[0] + Summa1[1] + Summa1[2] + Summa1[3] + Summa1[4] + Summa1[5] + Summa1[6] + Summa1[7] + Summa1[8] + Summa1[9];
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Нормальный");
  display.setCursor(10, 25);
  display.println("режим");
  display.setCursor(10, 40);
  display.println(Srednie_1);
  display.setCursor(50, 40);
  display.println(SummaDoALL);
  display.setCursor(90, 40);
  display.println(SummaDo1);
  display.display();
  Serial.print("SummaDoALL = ");
  Serial.print(SummaDoALL);
  Serial.println(" ");
  Serial.print("SummaDo1 = ");
  Serial.print(SummaDo1);
  Serial.println(" ");
  Serial.print("SummaDo2 = ");
  Serial.print(SummaDo2);
  Serial.println(" ");
  i = 0;
  if ((((min(SummaDoALL,  SummaDo1) * 100) / max(SummaDoALL, SummaDo1))) >= KorektorSravnenia) {
    Serial.println("1EBATVALERA");
    while ((((min(Srednie_1,  flash[i]) * 100) / max(Srednie_1, flash[i]))) >= KorektorSravnenia) {
      ServoDo1();
      i = i++;
      if (i > 749) i = 720;
    }
  }
  i = 750;
  if ((((min(SummaDoALL,  SummaDo2) * 100) / max(SummaDoALL, SummaDo2))) >= KorektorSravnenia) {
    while ((((min(Srednie_1,  flash[i]) * 100) / max(Srednie_1, flash[i]))) >= KorektorSravnenia) {
      ServoDo2();
      i = i++;
      if (i > 1499) i = 1470;
    }
  }
  i = 1500;
  if ((((min(SummaDoALL,  SummaDo3) * 100) / max(SummaDoALL, SummaDo3))) >= KorektorSravnenia) {
    while ((((min(Srednie_1,  flash[i]) * 100) / max(Srednie_1, flash[i]))) >= KorektorSravnenia) {
      ServoDo4();
      i = i++;
      if (i > 2249) i = 2220;
    }
  }
  i = 2250;
  if ((((min(SummaDoALL,  SummaDo4) * 100) / max(SummaDoALL, SummaDo4))) >= KorektorSravnenia) {
    while ((((min(Srednie_1,  flash[i]) * 100) / max(Srednie_1, flash[i]))) >= KorektorSravnenia) {
      ServoDo3();
      i = i++;
      if (i > 2999) i = 2969;
    }
  }
  Serial.println("0GOVNOOOOOOOOOOOOOOO");
}

void TuningServo1() {
  i = 0;
  x = 0;
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Задайте");
  display.setCursor(10, 25);
  display.println("действие 1");
  display.display();
  delay(2000);
  Pisk();
  while (i < 750) {
TuningServo1:
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Считываем");
    display.setCursor(10, 25);
    display.println("значения с");
    display.setCursor(10, 40);
    display.println("датчика");
    display.setCursor(10, 55);
    display.println(i);
    display.display();
    chistkaAndUsrednenie();
    flash[i] = Srednie_1;
    i = i + 1;
    if (x >= 10) goto TuningServo1; //Счётчик до 10
    SummaDo1 = SummaDo1 + Srednie_1;
    display.setCursor(50, 55);
    display.println(SummaDo1);
    x = x++;
  }
  EEPROM.put(3040, SummaDo1);
  i = 0;
  while (i < 750) {
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Записываем");
    display.setCursor(10, 25);
    display.println("данные в");
    display.setCursor(10, 40);
    display.println("память...");
    display.setCursor(10, 55);
    display.println(i);
    display.display();
    EEPROM.update(i, flash[i]); //Кидаем во flash для ускорения обрабатывания
    i = i + 1;
  }
  Pisk();
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Установите");
  display.setCursor(10, 25);
  display.println("пальцы в ");
  display.setCursor(10, 40);
  display.println("необходимое");
  display.setCursor(10, 55);
  display.println("положение");
  display.display();
  delay(1500);
  Pisk();
  while (1) {
    //Задаём сервухам положение руками
    ServoMemory[0] = analogRead(potmizinec);
    ServoMemory[0] = map(ServoMemory[0], 0, 1024 , 0, 255);
    ServoMemory[1] = analogRead(potbezimani);
    ServoMemory[1] = map(ServoMemory[2], 0, 1024 , 0, 255);
    ServoMemory[2] = analogRead(potfuck);
    ServoMemory[2] = map(ServoMemory[4], 0, 1024 , 0, 255);
    ServoMemory[3] = analogRead(potukozatel);
    ServoMemory[3] = map(ServoMemory[6], 0, 1024 , 0, 255);
    ServoMemory[4] = analogRead(potbig);
    ServoMemory[4] = map(ServoMemory[8], 0, 1024 , 0, 255);
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("После");
    display.setCursor(10, 25);
    display.println("завершения");
    display.setCursor(10, 40);
    display.println("нажмите");
    display.setCursor(10, 55);
    display.println("кнопку А");
    display.display();
    if (digitalRead(KeyA)) break;
    delay(3);
  }
  //Кидаем значения сервоприводов в EEPROM
  EEPROM.update(3000, ServoMemory[0]);
  EEPROM.update(3001, ServoMemory[1]);
  EEPROM.update(3002, ServoMemory[2]);
  EEPROM.update(3003, ServoMemory[3]);
  EEPROM.update(3004, ServoMemory[4]);
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Действие 1=Ок!");
  display.display();

}

void TuningServo2() {
  x = 0;
  i = 750;
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Задайте");
  display.setCursor(10, 25);
  display.println("действие 2");
  display.display();
  delay(2000);
  Pisk();
  while (i < 1500) {
TuningServo2:
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Считываем");
    display.setCursor(10, 25);
    display.println("значения с");
    display.setCursor(10, 40);
    display.println("датчика");
    display.setCursor(10, 55);
    display.println(i);
    display.display();
    chistkaAndUsrednenie();
    flash[i] = Srednie_1;
    i = i + 1;
    if (x >= 10) goto TuningServo2; //Счётчик до 10
    SummaDo2 = SummaDo2 + Srednie_1;
    display.setCursor(50, 55);
    display.println(SummaDo2);
    x = x++;
  }
  EEPROM.put(3060, SummaDo2);
  i = 0;
  while (i < 1500) {
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Записываем");
    display.setCursor(10, 25);
    display.println("данные в");
    display.setCursor(10, 40);
    display.println("память...");
    display.setCursor(10, 55);
    display.println(i);
    display.display();
    EEPROM.update(i, flash[i]); //Кидаем во flash для ускорения обрабатывания
    i = i + 1;
  }
  Pisk();
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Установите");
  display.setCursor(10, 25);
  display.println("пальцы в ");
  display.setCursor(10, 40);
  display.println("необходимое");
  display.setCursor(10, 55);
  display.println("положение");
  display.display();
  delay(1500);
  Pisk();
  while (1) {
    //Задаём сервухам положение руками
    ServoMemory[5] = analogRead(potmizinec);
    ServoMemory[5] = map(ServoMemory[5], 0, 1024 , 0, 255);
    ServoMemory[6] = analogRead(potbezimani);
    ServoMemory[6] = map(ServoMemory[6], 0, 1024 , 0, 255);
    ServoMemory[7] = analogRead(potfuck);
    ServoMemory[7] = map(ServoMemory[7], 0, 1024 , 0, 255);
    ServoMemory[8] = analogRead(potukozatel);
    ServoMemory[8] = map(ServoMemory[8], 0, 1024 , 0, 255);
    ServoMemory[9] = analogRead(potbig);
    ServoMemory[9] = map(ServoMemory[9], 0, 1024 , 0, 255);
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("После");
    display.setCursor(10, 25);
    display.println("завершения");
    display.setCursor(10, 40);
    display.println("нажмите");
    display.setCursor(10, 55);
    display.println("кнопку А");
    display.display();
    if (digitalRead(KeyA)) break;
    delay(3);
  }
  //Кидаем значения сервоприводов в EEPROM
  EEPROM.update(3005, ServoMemory[5]);
  EEPROM.update(3006, ServoMemory[6]);
  EEPROM.update(3007, ServoMemory[7]);
  EEPROM.update(3008, ServoMemory[8]);
  EEPROM.update(3009, ServoMemory[9]);
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Действие 2=Ок!");
  display.display();

}

void TuningServo3() {
  x = 0;
  i = 1500;
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Задайте");
  display.setCursor(10, 25);
  display.println("действие 3");
  display.display();
  delay(2000);
  Pisk();
  while (i < 2250) {
TuningServo3:
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Считываем");
    display.setCursor(10, 25);
    display.println("значения с");
    display.setCursor(10, 40);
    display.println("датчика");
    display.setCursor(10, 55);
    display.println(i);
    display.display();
    chistkaAndUsrednenie();
    flash[i] = Srednie_1;
    i = i + 1;
    if (x >= 10) goto TuningServo3; //Счётчик до 10
    SummaDo3 = SummaDo3 + Srednie_1;
    display.setCursor(50, 55);
    display.println(SummaDo3);
    x = x++;
  }
  EEPROM.put(3080, SummaDo3);
  i = 0;
  while (i < 2250) {
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Записываем");
    display.setCursor(10, 25);
    display.println("данные в");
    display.setCursor(10, 40);
    display.println("память...");
    display.setCursor(10, 55);
    display.println(i);
    display.display();
    EEPROM.update(i, flash[i]); //Кидаем во flash для ускорения обрабатывания
    i = i + 1;
  }
  Pisk();
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Установите");
  display.setCursor(10, 25);
  display.println("пальцы в ");
  display.setCursor(10, 40);
  display.println("необходимое");
  display.setCursor(10, 55);
  display.println("положение");
  display.display();
  delay(1500);
  Pisk();
  while (1) {
    //Задаём сервухам положение руками
    ServoMemory[10] = analogRead(potmizinec);
    ServoMemory[10] = map(ServoMemory[10], 0, 1024 , 0, 255);
    ServoMemory[11] = analogRead(potbezimani);
    ServoMemory[11] = map(ServoMemory[11], 0, 1024 , 0, 255);
    ServoMemory[12] = analogRead(potfuck);
    ServoMemory[12] = map(ServoMemory[12], 0, 1024 , 0, 255);
    ServoMemory[13] = analogRead(potukozatel);
    ServoMemory[13] = map(ServoMemory[13], 0, 1024 , 0, 255);
    ServoMemory[14] = analogRead(potbig);
    ServoMemory[14] = map(ServoMemory[14], 0, 1024 , 0, 255);
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("После");
    display.setCursor(10, 25);
    display.println("завершения");
    display.setCursor(10, 40);
    display.println("нажмите");
    display.setCursor(10, 55);
    display.println("кнопку А");
    display.display();
    if (digitalRead(KeyA)) break;
    delay(3);
  }
  //Кидаем значения сервоприводов в EEPROM
  EEPROM.update(3010, ServoMemory[10]);
  EEPROM.update(3011, ServoMemory[11]);
  EEPROM.update(3012, ServoMemory[12]);
  EEPROM.update(3013, ServoMemory[13]);
  EEPROM.update(3014, ServoMemory[14]);
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Действие 3=Ок!");
  display.display();

}

void TuningServo4() {

  x = 0;
  i = 2250;
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Задайте");
  display.setCursor(10, 25);
  display.println("действие 4");
  display.display();
  delay(2000);
  Pisk();
  while (i < 3000) {
TuningServo4:
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Считываем");
    display.setCursor(10, 25);
    display.println("значения с");
    display.setCursor(10, 40);
    display.println("датчика");
    display.setCursor(10, 55);
    display.println(i);
    display.display();
    chistkaAndUsrednenie();
    flash[i] = Srednie_1;
    i = i + 1;
    if (x >= 10) goto TuningServo4; //Счётчик до 10
    SummaDo4 = SummaDo4 + Srednie_1;
    display.setCursor(50, 55);
    display.println(SummaDo4);
    x = x++;
  }
  EEPROM.put(3100, SummaDo4);
  i = 0;
  while (i < 2250) {
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Записываем");
    display.setCursor(10, 25);
    display.println("данные в");
    display.setCursor(10, 40);
    display.println("память...");
    display.setCursor(10, 55);
    display.println(i);
    display.display();
    EEPROM.update(i, flash[i]); //Кидаем во flash для ускорения обрабатывания
    i = i + 1;
  }
  Pisk();
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Установите");
  display.setCursor(10, 25);
  display.println("пальцы в ");
  display.setCursor(10, 40);
  display.println("необходимое");
  display.setCursor(10, 55);
  display.println("положение");
  display.display();
  delay(1500);
  Pisk();
  while (1) {
    //Задаём сервухам положение руками
    ServoMemory[15] = analogRead(potmizinec);
    ServoMemory[15] = map(ServoMemory[15], 0, 1024 , 0, 255);
    ServoMemory[16] = analogRead(potbezimani);
    ServoMemory[16] = map(ServoMemory[16], 0, 1024 , 0, 255);
    ServoMemory[17] = analogRead(potfuck);
    ServoMemory[17] = map(ServoMemory[17], 0, 1024 , 0, 255);
    ServoMemory[18] = analogRead(potukozatel);
    ServoMemory[18] = map(ServoMemory[18], 0, 1024 , 0, 255);
    ServoMemory[19] = analogRead(potbig);
    ServoMemory[19] = map(ServoMemory[19], 0, 1024 , 0, 255);
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("После");
    display.setCursor(10, 25);
    display.println("завершения");
    display.setCursor(10, 40);
    display.println("нажмите");
    display.setCursor(10, 55);
    display.println("кнопку А");
    display.display();
    if (digitalRead(KeyA)) break;
    delay(3);
  }
  //Кидаем значения сервоприводов в EEPROM
  EEPROM.update(3015, ServoMemory[15]);
  EEPROM.update(3016, ServoMemory[16]);
  EEPROM.update(3017, ServoMemory[17]);
  EEPROM.update(3018, ServoMemory[18]);
  EEPROM.update(3019, ServoMemory[19]);
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Действие 4=Ок!");
  display.display();

}

void SearchValues() {

  i = 0;
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Роемся в ");
  display.setCursor(10, 25);
  display.println("памяти...");
  display.display();
  SummaDo1 = EEPROM.get(3040, SummaDo1); //Первые 10 значений при отладке
  SummaDo2 = EEPROM.get(3060, SummaDo2); //Первые 10 значений при отладке
  SummaDo3 = EEPROM.get(3080, SummaDo3); //Первые 10 значений при отладке
  SummaDo4 = EEPROM.get(3100, SummaDo4); //Первые 10 значений при отладке
  while (i < 2999) {
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Роемся в ");
    display.setCursor(10, 25);
    display.println("памяти...");
    display.setCursor(10, 40);
    display.println(i);
    display.display();
    flash[i] = EEPROM.read(i);
    Serial.print(i);
    Serial.print("\t");
    Serial.println(flash[i]);
    i = i + 1;
  }
  SummaDo1 = EEPROM.get(3040, SummaDo1); //Первые 10 значений при отладке X2
  SummaDo2 = EEPROM.get(3060, SummaDo2); //Первые 10 значений при отладке X2
  SummaDo3 = EEPROM.get(3080, SummaDo3); //Первые 10 значений при отладке X2
  SummaDo4 = EEPROM.get(3100, SummaDo4); //Первые 10 значений при отладке X2
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Роемся в ");
  display.setCursor(10, 25);
  display.println("памяти.");
  display.setCursor(10, 40);
  display.println("Выполнено!");
  display.display();
  Pisk();
  delay(250);
  Pisk();

}

void proverka() {

  while (digitalRead(ElectrodeR) == 1 || digitalRead(ElectrodeL) == 1) {
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Электроды не");
    display.setCursor(10, 25);
    display.println("обнаружены,");
    display.setCursor(10, 40);
    display.println("Ожидается");
    display.setCursor(10, 55);
    display.println("подключение!");
    display.display();
  }
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println("Электроды");
  display.setCursor(10, 25);
  display.println("найдены!");
  display.display();
  delay(3000);

}

void chistkaAndUsrednenie() {

  SignalSDatchika = analogRead(A0);

  if (index > 2) index = 0; // переключаем индекс с 0 до 2 (0, 1, 2, 0, 1, 2…)
  val_0[index] = SignalSDatchika; // записываем значение с датчика в массив
  val_1[index] = Srednie_0;
  index = index + 1;

  if (SignalSDatchika >= korector) {
    Srednie_0 = (val_0[0] + val_0[1] + val_0[2]) / 3; //Первый этап усреднения
  }
  if (val_1[0] >= 1 && val_1[1] >= 1 && val_1[2] >= 1) {
    Srednie_1 = (val_1[0] + val_1[1] + val_1[2]) / 3; //Второй этап усреднения
  }
  Srednie_1 = map(Srednie_1, 0, 900, 0, 255);
  if (x > 9) x = 0;
  Summa1[x] = Srednie_1;
  x = x++;

}
