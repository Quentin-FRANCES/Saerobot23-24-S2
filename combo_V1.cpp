#include "Grove_LCD_RGB_Backlight.h"
#include "SRF05.h"
#include "mbed.h"
#define VMOY 30
#define K 30
#define REG_WHO_AM_I      0x0D //Numéro du registre à consulter pour connaître l'identifiant du composant
#define REG_CTRL_REG_1    0x2A //Numéro du registre à remplir avec 0x01 pour activer le composant
#define REG_OUT_X_MSB     0x01 //Numéro du registre à consulter pour acquérir l'accélération en X sur 2 octets
#define REG_OUT_Y_MSB     0x03 //Numéro du registre à consulter pour acquérir l'accélération en Y sur 2 octets
#define REG_OUT_Z_MSB     0x05 //Numéro du registre à consulter pour acquérir l'accélération en Z sur 2 octets
#define UINT14_MAX        16384 //correspond au nombre décimal max codé sur 14 bits
#define adresse_I2C       0x3A     //Adresse 8 bits du composant MMA8451 sur le bus I2C
I2C composant( PTE25 , PTE24 );
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
int getAccAxis(char AXE) ;

int main() {
  int8_t etat = 0;
  int16_t cpt1av;
  float captav, captg, captavav, captgav;
  float capt1, capt2, capt3, capt4,err,cmdG,cmdD,delta;
 int val_z;float val_z_g;     
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
    val_z = getAccAxis(REG_OUT_Z_MSB);                        // Lecture de l'acceleration en Z - valeur brute
    val_z_g = (float)val_z / 4000;
    switch(etat){
        case 0 :
            vitesse(0.0,0.0);
            if(val_z_g > 1.0){
                etat = 1;
            }break;
        case 1: 
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
    break;
    }
    //printf("capt1 = %.2f  capt2 = %.2f   capt3 = %.2f  capt4 = %.2f \n\r", capt1, capt2,capt3, capt4);
    captavav = captav;
    captgav = captg;
    wait_us(50);
  }
}}

void init() {
  MOT_G.period_us(100);
  MOT_D.period_us(100);
  am1.rise(&incrementation1);
  am2.rise(&incrementation2);
  LCD.clear();
  char data[2] = {REG_CTRL_REG_1,0x01};   //On remplit un tableau de char avec 2 valeurs : numéro de registre et valeur
    composant.write(adresse_I2C,data,2);
}

int getAccAxis(char AXE) {  //Fonction permettant de récupérer la valeur de l'accélération suivant tel ou tel AXE
    int acceleration;       //variable qui va stocker la mesure de l'accélération
    char registre[1]={AXE};     //Création d'un tableau de 1 char stockant le n° du registre à interroger (AXE passé en paramètre)
    char data[2];               //Création d'un tableau de 2 char qui stockera la réponse du composant
    composant.write(adresse_I2C,registre,1, true);  //Ecriture sur le bus = "Esclave n°0x3A, donne moi l'acceleration suivant AXE" - true = "j'attends ta réponse"
    composant.read(adresse_I2C, data, 2);           //Attente de réception de 2 octets : données de l'accélération 
    acceleration = (data[0] << 6) | (data[1] >> 2); //Traitement des données pour reconstituer la valeur de l'acceleration
    if (acceleration > UINT14_MAX/2)                //Si la valeur dépasse la moitié de 2^14 - 1
        acceleration = acceleration - UINT14_MAX;   //Elle est en réalité négative, on lui soustrait 2^14 - 1
    return acceleration;                            //La fonction renvoie la valeur de l'accélération - 4096 <=> acceleration de +1 g [m.s^(-2)]
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
