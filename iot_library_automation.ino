//Khai thu vien va bien cho thoi gian
#include "RTClib.h"
#include <Adafruit_I2CDevice.h>


RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };


#define uS_TO_S_FACTOR 1000000ULL     /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP_BREAK 0.5       /* Time ESP32 will go to sleep (in hours) */
#define TIME_TO_SLEEP_AFTERSCHOOL 14 /* Time ESP32 will go to sleep (in hours) */
#define TIME_TO_SLEEP_DAY 24         /* Time ESP32 will go to sleep (in hours) */


RTC_DATA_ATTR int bootCount = 0;





//Pin connected to ST_CP of 74HC595
#define latchPin 27
//Pin connected to SH_CP of 74HC595
#define clockPin 19
////Pin connected to DS of 74HC595
#define dataPin 18
#define loa 4
#define SAMPLES 250
#define SENSOR_PIN 15
float value;
float percent;
byte a[8];
// float b[16] = { 1883.7, 1893.9, 1904.1, 1914.4, 1924.6, 1934.8, 1945.1, 1955.3, 1965.5, 1975.81986, 1996.3, 2006.5, 3000,4000 };  //46, 0,25
float b[16] = { 1883.7, 1893.9, 1904.1, 1914.4, 1924.6, 1934.8, 1945.1, 1955.3, 1965.5, 1975.81986, 1996.3, 2006.5, 2016.7, 3000, 40000 };  //46, 0,25

//rickroll
#define a3f 208  // 208 Hz
#define b3f 233  // 233 Hz
#define b3 247   // 247 Hz
#define c4 261   // 261 Hz MIDDLE C
#define c4s 277  // 277 Hz
#define e4f 311  // 311 Hz
#define f4 349   // 349 Hz
#define a4f 415  // 415 Hz
#define b4f 466  // 466 Hz
#define b4 493   //  493 Hz
#define c5 523   // 523 Hz
#define c5s 554  // 554  Hz
#define e5f 622  // 622 Hz
#define f5 698   // 698 Hz
#define f5s 740  // 740 Hz
#define a5f 831  // 831 Hz
#define rest -1
volatile int beatlength = 100;  // determines tempo
float beatseparationconstant = 0.3;
int threshold;
int p;  // part index
int s;  // song index
boolean flag;
int song1_chorus_melody[] = { b4f, b4f, a4f, a4f,
                              f5, f5, e5f, b4f, b4f, a4f, a4f, e5f, e5f, c5s, c5, b4f,
                              c5s, c5s, c5s, c5s,
                              c5s, e5f, c5, b4f, a4f, a4f, a4f, e5f, c5s,
                              b4f, b4f, a4f, a4f,
                              f5, f5, e5f, b4f, b4f, a4f, a4f, a5f, c5, c5s, c5, b4f,
                              c5s, c5s, c5s, c5s,
                              c5s, e5f, c5, b4f, a4f, rest, a4f, e5f, c5s, rest };

int song1_chorus_rhythmn[] = { 1, 1, 1, 1,
                               3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
                               1, 1, 1, 1,
                               3, 3, 3, 1, 2, 2, 2, 4, 8,
                               1, 1, 1, 1,
                               3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
                               1, 1, 1, 1,
                               3, 3, 3, 1, 2, 2, 2, 4, 8, 4 };
void setup() {
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(loa, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
  flag = true;
  p = 1;
  s = 0;
  a[0] = 1;
  a[1] = 3;
  a[2] = 7;
  a[3] = 15;
  a[4] = 31;
  a[5] = 63;
  a[6] = 127;
  a[7] = 255;

  //Setup cho thoi gian
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }


  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));
  }
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
}

void loop() {
  time_watcher();
  value = 0;
  for (int i = 0; i < SAMPLES; i++) {
    value += analogRead(SENSOR_PIN);
    delay(1);
  }
  value = value / SAMPLES;
  percent = value;  //map(value, 0, 4095, 0.00, 100.00);
  Serial.print("Noise Detector: ");
  Serial.println(percent);
  if (percent >= 0 and percent < b[0]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[0]);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
  } else if (percent >= b[0] and percent < b[1]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[1]);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
  } else if (percent >= b[1] and percent < b[2]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[2]);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
  } else if (percent >= b[2] and percent < b[3]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[3]);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
  } else if (percent >= b[3] and percent < b[4]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[4]);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
  } else if (percent >= b[4] and percent < b[5]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[5]);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
  } else if (percent >= b[5] and percent < b[6]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[6]);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
  } else if (percent >= b[6] and percent < b[7]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[7]);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
  } else if (percent >= b[7] and percent < b[8]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[7]);
    shiftOut(dataPin, clockPin, MSBFIRST, a[0]);
    digitalWrite(latchPin, HIGH);
  } else if (percent >= b[8] and percent < b[9]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[7]);
    shiftOut(dataPin, clockPin, MSBFIRST, a[1]);
    digitalWrite(latchPin, HIGH);
  } else if (percent >= b[9] and percent < b[10]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[7]);
    shiftOut(dataPin, clockPin, MSBFIRST, a[2]);
    digitalWrite(latchPin, HIGH);
  } else if (percent >= b[10] and percent < b[11]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[7]);
    shiftOut(dataPin, clockPin, MSBFIRST, a[3]);
    digitalWrite(latchPin, HIGH);
  } else if (percent >= b[11] and percent < b[12]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[7]);
    shiftOut(dataPin, clockPin, MSBFIRST, a[4]);
    digitalWrite(latchPin, HIGH);
    tone(loa, 220);
    delay(150);
    tone(loa, 2093);
    delay(150);
    tone(loa, 82);
    delay(150);
    noTone(loa);
    delay(150);
    noTone(loa);
    delay(150);
  } else if (percent >= b[12] and percent < b[13]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[7]);
    shiftOut(dataPin, clockPin, MSBFIRST, a[5]);
    digitalWrite(latchPin, HIGH);
    tone(loa, 220);
    delay(150);
    tone(loa, 2093);
    delay(150);
    tone(loa, 82);
    delay(150);
    noTone(loa);
    delay(150);
    tone(loa, 220);
    delay(150);
    tone(loa, 2093);
    delay(150);
    tone(loa, 82);
    delay(150);
    noTone(loa);
    delay(150);
    noTone(loa);
    delay(150);
  } else if (percent >= b[13] and percent < b[14]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[7]);
    shiftOut(dataPin, clockPin, MSBFIRST, a[6]);
    digitalWrite(latchPin, HIGH);
    tone(loa, 220);
    delay(150);
    tone(loa, 2093);
    delay(150);
    tone(loa, 82);
    delay(150);
    noTone(loa);
    delay(150);
    tone(loa, 220);
    delay(150);
    tone(loa, 2093);
    delay(150);
    tone(loa, 82);
    delay(150);
    noTone(loa);
    delay(150);
    tone(loa, 220);
    delay(150);
    tone(loa, 2093);
    delay(150);
    tone(loa, 82);
    delay(150);
    noTone(loa);
    delay(150);
    noTone(loa);
    delay(150);
  } else if (percent >= b[14]) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a[7]);
    shiftOut(dataPin, clockPin, MSBFIRST, a[7]);
    digitalWrite(latchPin, HIGH);
    flag = true;
    while (flag == true) { play(); }
  }
  delay(150);
}
void play() {
  int notelength;
  if (p == 1) {
    notelength = beatlength * song1_chorus_rhythmn[s];
    if (song1_chorus_melody[s] > 0) {
      tone(loa, song1_chorus_melody[s], notelength);
    }
    s++;
    if (s >= sizeof(song1_chorus_melody) / sizeof(int)) {
      p++;
      s = 0;
    }
  }
  delay(notelength);
  noTone(loa);
  delay(notelength * beatseparationconstant);
  if (p == 2) {
    p = 1;
    flag = false;
  }
}



//Ham cho THOI GIAN

void start_deep_sleep_break() {
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_BREAK * 60* 60 * uS_TO_S_FACTOR);
  Serial.println("Ra choi sang/chieu: deep sleep");
  Serial.flush();
  esp_deep_sleep_start();
}

void start_deep_sleep_afterschool() {
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_AFTERSCHOOL *60* 60 * uS_TO_S_FACTOR);
  Serial.println("Ra ve: deep sleep");
  Serial.flush();
  esp_deep_sleep_start();
}
void start_deep_sleep_day() {
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_DAY*60* 60 * uS_TO_S_FACTOR);
  Serial.println("Nguyen ngay: deep sleep");
  Serial.flush();
  esp_deep_sleep_start();
}

void time_watcher() {
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  if (now.hour() == 8 && now.minute() == 45 && now.second() == 0) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
    start_deep_sleep_break();
  }

  if (now.hour() == 14 && now.minute() == 30 && now.second() == 0) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
    start_deep_sleep_break();
  }
  if (now.hour() == 17 && now.minute() == 25 && now.second() == 0) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
    start_deep_sleep_afterschool();
  }
  if (now.dayOfTheWeek() == 0) {
    start_deep_sleep_day();
  }
  Serial.println();
}
