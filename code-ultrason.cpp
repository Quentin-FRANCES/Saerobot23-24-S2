#include "mbed.h"


DigitalOut trig(D4);
DigitalIn echo(D5);
Timer timer;
int main() {
  int etat = 0;
  while (1) {

    switch (etat) {
    case 0:
      trig.write(1);
      wait_us(10);
      trig.write(0);
      etat = 1;
      break;
    case 1:
      if (echo.read() == 1) {
        timer.start();
        etat = 2;
      }
      break;
    case 2:
      if (echo.read() == 0) {
        timer.stop();
        float mesure = timer.read();
        printf("mesure = %.7f \n\r", mesure);
        wait(0.1);
        etat = 0;
      }
    }
  }
}uligggyr
