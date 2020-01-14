#include <Servo.h> // Подключаем библиотеку
#include <microWire.h> // Подключаем библиотеку
#include <microLiquidCrystal_I2C.h> // Подключаем библиотеку
#include <EEPROM.h> // Подключаем библиотеку

#define korector 12 // Задаем корректор чистки
#define ElectrodeR 10
#define ElectrodeL 11
#define Pishalka 12
#define AllServo 5
#define TranzistorsA 2
#define TranzistorsB 3
#define TranzistorsC 4
#define KeyA 8
#define KeyB 7

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

LiquidCrystal_I2C lcd(0x3F, 20, 4); // Задаем адрес Lcd монитора

int SignalSDatchika; // Аналоговый сигнал с датчика
int i; // Сдвиговый регистр
byte x; //Сдвиговый регистр для Датчика

byte index; // Индекс для очистки
int val_0[3]; // Первый этап очистки (Масcив)
int val_1[3]; // Среднее арифметические 2 (Массив)
int Srednie_0; // Среднее арифметические 1 (Целое единственное число)
int Srednie_1; // Среднее арифметические 2 (Целое единственное число)
int Summa1[10]; //Сумма для работы с датчиком
int SummaDo1; //Сумма для работы с памятью (действие 1)
int SummaDo2; //Сумма для работы с памятью (действие 2)
int SummaDo3; //Сумма для работы с памятью (действие 3)
int SummaDo4; //Сумма для работы с памятью (действие 4)
int ServoMemory[20];
byte  flash[3000]; //Значения во flash
byte KorektorSravnenia;

byte levo8bit[] = {  B00010,  B00011,  B00111,  B01101,  B11111,  B10111,  B10100,  B10011};
byte pravo8bit[] = {   B01010,  B11001,  B11101,  B10111,  B11110,  B11100,  B00100,  B11000};

void setup() {

  i = 0;
  x = 0;
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

  lcd.init();  // Инициализируем Lcd
  lcd.init();  // Инициализируем Lcd X2
  lcd.backlight();
  lcd.setCursor(0, 1);
  lcd.print("Kelll31 Technology");
  lcd.createChar(0, levo8bit);
  lcd.createChar(1, pravo8bit);
  lcd.setCursor(18, 1);
  lcd.write(0);
  lcd.write(1);
  delay(3000);
  if ((digitalRead(KeyA) == 1) && (digitalRead(KeyB) == 1))  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("First Launch");
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
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("First Launch");
    lcd.setCursor(8, 1);
    lcd.print("Ok");
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

  lcd.clear();
  lcd.home();
  lcd.print("Normal mode");
  lcd.setCursor(0, 1);
  lcd.print(Srednie_1);
  delay(3);
  chistka();
  Usrednenie();
  i = 0;
  if ((((min(Summa1[0] + Summa1[1] + Summa1[2] + Summa1[3] + Summa1[4] + Summa1[5] + Summa1[6] + Summa1[7] + Summa1[8] + Summa1[9],  SummaDo1) * 100) / max(Summa1[0] + Summa1[1] + Summa1[2] + Summa1[3] + Summa1[4] + Summa1[5] + Summa1[6] + Summa1[7] + Summa1[8] + Summa1[9], SummaDo1))) >= KorektorSravnenia) {
    while ((((min(Srednie_1,  flash[i]) * 100) / max(Srednie_1, flash[i]))) >= KorektorSravnenia) {
      ServoDo1();
      i = i++;
      if (i > 749) i = 720;
    }
  }
  i = 750;
  if ((((min(Summa1[0] + Summa1[1] + Summa1[2] + Summa1[3] + Summa1[4] + Summa1[5] + Summa1[6] + Summa1[7] + Summa1[8] + Summa1[9],  SummaDo2) * 100) / max(Summa1[0] + Summa1[1] + Summa1[2] + Summa1[3] + Summa1[4] + Summa1[5] + Summa1[6] + Summa1[7] + Summa1[8] + Summa1[9], SummaDo2))) >= KorektorSravnenia) {
    while ((((min(Srednie_1,  flash[i]) * 100) / max(Srednie_1, flash[i]))) >= KorektorSravnenia) {
      ServoDo2();
      i = i++;
      if (i > 1499) i = 1470;
    }
  }
  i = 1500;
  if ((((min(Summa1[0] + Summa1[1] + Summa1[2] + Summa1[3] + Summa1[4] + Summa1[5] + Summa1[6] + Summa1[7] + Summa1[8] + Summa1[9],  SummaDo3) * 100) / max(Summa1[0] + Summa1[1] + Summa1[2] + Summa1[3] + Summa1[4] + Summa1[5] + Summa1[6] + Summa1[7] + Summa1[8] + Summa1[9], SummaDo3))) >= KorektorSravnenia) {
    while ((((min(Srednie_1,  flash[i]) * 100) / max(Srednie_1, flash[i]))) >= KorektorSravnenia) {
      ServoDo4();
      i = i++;
      if (i > 2249) i = 2220;
    }
  }
  i = 2250;
  if ((((min(Summa1[0] + Summa1[1] + Summa1[2] + Summa1[3] + Summa1[4] + Summa1[5] + Summa1[6] + Summa1[7] + Summa1[8] + Summa1[9],  SummaDo4) * 100) / max(Summa1[0] + Summa1[1] + Summa1[2] + Summa1[3] + Summa1[4] + Summa1[5] + Summa1[6] + Summa1[7] + Summa1[8] + Summa1[9], SummaDo4))) >= KorektorSravnenia) {
    while ((((min(Srednie_1,  flash[i]) * 100) / max(Srednie_1, flash[i]))) >= KorektorSravnenia) {
      ServoDo3();
      i = i++;
      if (i > 2999) i = 2969;
    }
  }

}

void TuningServo1() {
  i = 0;
  x = 0;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Szhozmite hand");
  delay(3000);
  Pisk();
  lcd.setCursor(1, 2);
  lcd.print("Write to memory...");
  while ((i < 750) && (SignalSDatchika >= korector)) {
TuningServo1:
    delay(2);
    chistka();
    Usrednenie();
    delay(3);
    EEPROM.update(i, Srednie_1);
    i = i + 1;
    SummaDo1 = SummaDo1 + Srednie_1;
    if (x >= 10) goto TuningServo1; //Счётчик до 10
    x = x++;
  }
  Pisk();
  lcd.clear();
  lcd.home();
  lcd.print("Zadaite znachenie Fingers");
  delay(1500);
  Pisk();
  while (KeyA < 1) {
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
    lcd.setCursor(0, 2);
    lcd.print("Ok?");
    delay(3);
  }
  //Кидаем значения сервоприводов в EEPROM
  EEPROM.update(3000, ServoMemory[0]);
  EEPROM.update(3001, ServoMemory[1]);
  EEPROM.update(3002, ServoMemory[2]);
  EEPROM.update(3003, ServoMemory[3]);
  EEPROM.update(3004, ServoMemory[4]);
  lcd.clear();
  lcd.home();
  lcd.print("Szhatie = Ok");

}

void TuningServo2() {
  x = 0;
  i = 750;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Razoshmite hand");
  delay(3000);
  Pisk();
  lcd.setCursor(1, 2);
  lcd.print("Write to memory...");
  while ((i < 1500) && (SignalSDatchika >= korector)) {
TuningServo2:
    delay(2);
    chistka();
    Usrednenie();
    delay(3);
    EEPROM.update(i, Srednie_1);
    i = i + 1;
    SummaDo2 = SummaDo2 + Srednie_1;
    if (x >= 10) goto TuningServo2; //Счётчик до 10
    x = x++;
  }
  Pisk();
  lcd.clear();
  lcd.home();
  lcd.print("Zadaite znachenie Fingers");
  delay(1500);
  Pisk();
  while (KeyA < 1) {
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
    lcd.setCursor(0, 2);
    lcd.print("Ok?");
    delay(3);
  }
  //Кидаем значения сервоприводов в EEPROM
  EEPROM.update(3005, ServoMemory[5]);
  EEPROM.update(3006, ServoMemory[6]);
  EEPROM.update(3007, ServoMemory[7]);
  EEPROM.update(3008, ServoMemory[8]);
  EEPROM.update(3009, ServoMemory[9]);
  lcd.clear();
  lcd.home();
  lcd.print("Razhatie = Ok");

}

void TuningServo3() {
  x = 0;
  i = 1500;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Povernite hand");
  lcd.setCursor(3, 1);
  lcd.print("V Levo");
  delay(3000);
  Pisk();
  lcd.setCursor(1, 2);
  lcd.print("Write to memory...");
  while ((i < 2250) && (SignalSDatchika >= korector)) {
TuningServo3:
    delay(2);
    chistka();
    Usrednenie();
    delay(3);
    EEPROM.update(i, Srednie_1);
    i = i + 1;
    SummaDo3 = SummaDo3 + Srednie_1;
    if (x >= 10) goto TuningServo3; //Счётчик до 10
    x = x++;
  }
  Pisk();
  lcd.clear();
  lcd.home();
  lcd.print("Zadaite znachenie Fingers");
  delay(1500);
  Pisk();
  while (KeyA < 1) {
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
    lcd.setCursor(0, 2);
    lcd.print("Ok?");
    delay(3);
  }
  //Кидаем значения сервоприводов в EEPROM
  EEPROM.update(3010, ServoMemory[10]);
  EEPROM.update(3011, ServoMemory[11]);
  EEPROM.update(3012, ServoMemory[12]);
  EEPROM.update(3013, ServoMemory[13]);
  EEPROM.update(3014, ServoMemory[14]);
  lcd.clear();
  lcd.home();
  lcd.print("Povorot V Levo = Ok");

}

void TuningServo4() {

  x = 0;
  i = 2250;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Povernite hand");
  lcd.setCursor(3, 1);
  lcd.print("V Pravo");
  delay(3000);
  Pisk();
  lcd.setCursor(1, 2);
  lcd.print("Write to memory...");
  while ((i < 3000) && (SignalSDatchika >= korector)) {
TuningServo4:
    delay(2);
    chistka();
    Usrednenie();
    delay(3);
    EEPROM.update(i, Srednie_1);
    i = i + 1;
    SummaDo4 = SummaDo4 + Srednie_1;
    if (x >= 10) goto TuningServo4; //Счётчик до 10
    x = x++;
  }
  Pisk();
  lcd.clear();
  lcd.home();
  lcd.print("Zadaite znachenie Fingers");
  delay(1500);
  Pisk();
  while (KeyA < 1) {
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
    lcd.setCursor(0, 2);
    lcd.print("Ok?");
    delay(3);
  }
  //Кидаем значения сервоприводов в EEPROM
  EEPROM.update(3015, ServoMemory[15]);
  EEPROM.update(3016, ServoMemory[16]);
  EEPROM.update(3017, ServoMemory[17]);
  EEPROM.update(3018, ServoMemory[18]);
  EEPROM.update(3019, ServoMemory[19]);
  lcd.clear();
  lcd.home();
  lcd.print("Povorot V Pravo = Ok");

}

void SearchValues() {

  i = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Search values");
  lcd.setCursor(1, 1);
  lcd.print("in EEPROM");
  while (i < 2999) {
    lcd.setCursor(0, 2);
    lcd.print(i);
    flash[i] = EEPROM.read(i);
    i = i + 1;
  }
  lcd.setCursor(10, 2);
  lcd.print("Ok");
  Pisk();
  delay(250);
  Pisk();

}

void proverka() {

  while (digitalRead(ElectrodeR) == 1 || digitalRead(ElectrodeL) == 1) {
    lcd.clear();
    lcd.home();
    lcd.print("No electrodes connected");
    lcd.setCursor(0, 1);
    lcd.print("Waiting for connection");
  }
  lcd.clear();
  lcd.home();
  lcd.print("Electrodes found!");
  delay(3000);

}

void chistka() {

  SignalSDatchika = analogRead(A0);

  if (index > 2) index = 0; // переключаем индекс с 0 до 2 (0, 1, 2, 0, 1, 2…)
  val_0[index] = SignalSDatchika; // записываем значение с датчика в массив
  val_1[index] = Srednie_0;
  index = index + 1;

}

void Usrednenie() {

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
