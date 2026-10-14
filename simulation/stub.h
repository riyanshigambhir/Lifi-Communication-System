// Minimal software model of the Arduino functions used by the sketches.
#pragma once
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
void setup();
void loop();
unsigned long micros();
unsigned long millis();
void delay(unsigned long ms);
void pinMode(int, int);
void digitalWrite(int, int);
int digitalRead(int);
struct SerialC {
  void begin(int) {}
  void print(const char* s) { printf("%s", s); }
  void println(const char* s) { printf("%s\n", s); }
  void print(char c) { printf("%c", c); }
  void println(char c) { printf("%c\n", c); }
};
extern SerialC Serial;
