#include "mbed.h"

DigitalIn infrarouge(D13);

int main() {
  while(1) {
    int inf = infrarouge.read();
    printf("%d \n\r", inf);
    wait(0.5);
  }
}
