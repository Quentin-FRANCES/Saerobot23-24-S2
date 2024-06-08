#include "Grove_LCD_RGB_Backlight.h"
#include "SRF05.h"
#include "mbed.h"
#define VMOY 30
#define K 30

Grove_LCD_RGB_Backlight LCD(D14, D15);
AnalogIn capt_1(A1);
AnalogIn capt_2(A2);
AnalogIn capt_3(A3);
AnalogIn capt_4(A4);
SRF05 Capt_avant(A5, D4);
SRF05 Capt_gauche(PTE30, D5);
DigitalIn Capteur_IR(D13);
InterruptIn am1(D2);   // encodeur mot_G
InterruptIn am2(D11);  // encodeur mot_D
DigitalIn bm1(D3);     // encodeur mot_G
DigitalIn bm2(D10);    // encodeur mot_D
PwmOut MOT_D(D8);      // puissance mot_D
PwmOut MOT_G(D6);      // puissance mot_G
DigitalOut SMOT_D(D9); // sens mot_D
DigitalOut SMOT_G(D7); // sens mot_G
// DigitalIn JACK(PTE20); // enfoncé à 0;
DigitalIn FDC(PTE21);  // enfoncé à 0;
DigitalOut myled(LED1);

Timer t;

int16_t cpt1 = 0, cpt2 = 0, IR, cpt = 0, cp = 0, jack,
        fdc;

char tab[20];

void init();
void tourne(int16_t);
void avance(int16_t);
void incrementation1(void);
void incrementation2(void);
void vitesse(float, float);
void toutdroit();

int main() {
  int8_t etat = 0;
  int16_t cpt1av;
  float captav, captg, captavav, captgav;
  float capt1, capt2, capt3, capt4,err,cmdG,cmdD,delta;

  //JACK.mode(PullUp);
  FDC.mode(PullUp);
  init();

  while (1) {
    fdc = FDC.read();
    captav = Capt_avant.read();
    captg = Capt_gauche.read();
    capt2 = capt_2.read();
    capt3 = capt_3.read();
    delta = capt2 - capt3;

          
    if(delta<=0) {
        SMOT_G.write(0);
        SMOT_D.write(0);
        MOT_G.pulsewidth_us(50.0);
        MOT_D.pulsewidth_us(30.0*delta);
    }
    if(delta>=0) {
        SMOT_G.write(0);
        SMOT_D.write(0);
        MOT_G.pulsewidth_us(30.0*delta);
        MOT_D.pulsewidth_us(50.0);
    }
    else if(fdc == 0) {
    vitesse (0.0, 0.0);
    LCD.setRGB(255,255,255);
    LCD.locate(6,0);
    sprintf(tab,"WIN");
    LCD.print(tab);
    }
    //printf("capt1 = %.2f  capt2 = %.2f   capt3 = %.2f  capt4 = %.2f \n\r", capt1, capt2,capt3, capt4);
    captavav = captav;
    captgav = captg;
    wait_us(50);
  }
}

void init() {
  MOT_G.period_us(100);
  MOT_D.period_us(100);
  am1.rise(&incrementation1);
  am2.rise(&incrementation2);
  LCD.clear();
}

void incrementation1(void) {
  if (bm1.read() == 0)
    cpt1++;
  else
    cpt1--;
}

void incrementation2(void) {
  if (bm2.read() == 1)
    cpt2++;
  else
    cpt2--;
}

void vitesse(float a, float b) {
  // batterie = ((bat.read() * 3.3) * (59.0 / 12.0)) + 0.6;
  if (a >= 0) {
    SMOT_G.write(0);
    MOT_G.pulsewidth_us(a);
  } else {
    SMOT_G.write(1);
    MOT_G.pulsewidth_us(100 - (-1 * a));
  }
  if (b >= 0) {
    SMOT_D.write(0);
    MOT_D.pulsewidth_us(b);
  } else {
    SMOT_D.write(1);
    MOT_D.pulsewidth_us(100 - (-1 * b));
  }
}

void tourne(int16_t angle) {
  cpt1 = 0;
  cpt2 = 0;
  angle = (angle * 424 / 360) / 2;

  do {
    if (cpt1 > (-1 * cpt2))
      vitesse(35.0, -40.0);
    else
      vitesse(35.0, -30.0);
    wait_us(150.0);
  } while (cpt1 < angle);
  vitesse(0.0, 0.0);
}

void avance(int16_t dist) {
  cpt1 = 0;
  cpt2 = 0;
  do {
    if (cpt1 > cpt2) {
      vitesse(35.0, 40.0);
    } else if (cpt1 <= cpt2) {
      vitesse(35.0, 30.0);
    }
    wait_us(150);
  } while (cpt1 < dist);
  vitesse(0.0, 0.0);
}

void toutdroit() {
  if (cpt1 > cpt2) {
    vitesse(35.0, 40.0);
  } else if (cpt1 <= cpt2) {
    vitesse(35.0, 30.0);
  }
  wait_us(50);
}
