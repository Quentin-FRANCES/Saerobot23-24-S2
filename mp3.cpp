#include "mbed.h"

Serial pc(USBTX, USBRX);
Serial device(A2, A3);
void playpause();
void volume(int);
void previous();
void next();
void stop();
void fich();
char val;
int v = 1;
int main() {
  volume(v);
    device.baud(9600);
  while (1) {
    if (pc.readable()) {
      val = pc.getc();
      if (val == 'a') {
        if (v < 31) {
          v++;
        }
        volume(v);
        printf("volume= %d \n\r", v);
      }
      if (val == 'z') {
        if (v > 0) {
          v--;
        }
        volume(v);
        printf("volume= %d \n\r", v);
      }
      if (val == 'p') {
        playpause();
        printf("playPause \n\r");
      }
      if (val == 'l') {
        previous();
        printf("Previous \n\r");
      }
      if (val == 'm') {
        next();
        printf("next \n\r");
      }
      if (val == 's') {
        stop();
        printf("stop \n\r");
      }
      if (val == 't') {
        fich();
        printf("changement de piste \n\r");
        
      }
    }
  }
}

void playpause() {
  char tab[5] = {0x7E, 0x03, 0xAA, 0, 0xEF};
  tab[3] = tab[1] + tab[2];
  for (int i = 0; i < 5; i++) {
    device.putc(tab[i]);
  }
}
void volume(int parametre) {
  char tab[6] = {0x7E, 0x04, 0xAE, parametre, 0, 0xEF};
  tab[4] = tab[1] + tab[2] + tab[3];
  for (int i = 0; i < 6; i++) {
    device.putc(tab[i]);
  }
}

void next() {
  char tab[5] = {0x7E, 0x03, 0xAC, 0, 0xEF};
  tab[3] = tab[1] + tab[2];
  for (int i = 0; i < 5; i++) {
    device.putc(tab[i]);
  }
}

void previous() {
  char tab[5] = {0x7E, 0x03, 0xAD, 0, 0xEF};
  tab[3] = tab[1] + tab[2];
  for (int i = 0; i < 5; i++) {
    device.putc(tab[i]);
  }
}
void stop() {
  char tab[5] = {0x7E, 0x03, 0xAB, 0, 0xEF};
  tab[3] = tab[1] + tab[2];
  for (int i = 0; i < 5; i++) {
    device.putc(tab[i]);
  }
}
void fich() {
  char tab[9] = {0x7E, 0x07, 0xA3, 'C', 'A', 'R', 'S', 0, 0xEF};
  tab[7] = tab[1] + tab[2] + tab[3] + tab[4] + tab[5] + tab[6];
  for (int i = 0; i < 9; i++) {
    device.putc(tab[i]);
  }
}
