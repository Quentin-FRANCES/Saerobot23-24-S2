#include "mbed.h"

#define REG_WHO_AM_I      0x0D //Numéro du registre à consulter pour connaître l'identifiant du composant
#define REG_CTRL_REG_1    0x2A //Numéro du registre à remplir avec 0x01 pour activer le composant
#define REG_OUT_X_MSB     0x01 //Numéro du registre à consulter pour acquérir l'accélération en X sur 2 octets
#define REG_OUT_Y_MSB     0x03 //Numéro du registre à consulter pour acquérir l'accélération en Y sur 2 octets
#define REG_OUT_Z_MSB     0x05 //Numéro du registre à consulter pour acquérir l'accélération en Z sur 2 octets
#define UINT14_MAX        16384 //correspond au nombre décimal max codé sur 14 bits
#define adresse_I2C       0x3A     //Adresse 8 bits du composant MMA8451 sur le bus I2C

I2C composant( PTE25 , PTE24 );      //Création de l'objet I2C appelé "composant" connecté aux broches PTE25 (SDA) et PTE24 (SCL)
InterruptIn am1(D2);  // encodeur mot_G
InterruptIn am2(D11); // encodeur mot_D
DigitalIn bm1(D3);  // encodeur mot_G
DigitalIn bm2(D10); // encodeur mot_D
PwmOut MOT_D(D8); // puissance mot_D
PwmOut MOT_G(D6); // puissance mot_G
DigitalOut SMOT_D(D9); // sens mot_D
DigitalOut SMOT_G(D7); // sens mot_G
int16_t cpt1 = 0, cpt2 = 0;

void init();
void tourne(int16_t);
void avance(int16_t);
void incrementation1(void);
void incrementation2(void);
void vitesse(float,float);                 //Fonction d'initialisation
//char getWhoAmI() ;             //Fonction permettant de récupérer l'identifiant du composant (ID)
int getAccAxis(char AXE) ;     //Fonction permettant de récupérer la valeur de l'accélération suivant tel ou tel AXE

int main() {                                                    //Programme principal
    int val_z, etat = 0; float val_z_g;                                   //variable qui va stocker la mesure de l'accélération                                        
    init();                                                     //Mise en marche du circuit
    //printf("je suis = %d\n\r",getWhoAmI());                     //Affichage de l'ID
    while(1) {                                                  //Boucle infinie
        val_z=getAccAxis(REG_OUT_Z_MSB);                        // Lecture de l'acceleration en Z - valeur brute
        val_z_g = (float)val_z / 4000;
        printf("etat : %d, val z : %lg\n\r", etat, val_z_g);
        switch(etat){
            case 0 :
                vitesse(0.0,0.0);
                if(val_z_g > 1.0){
                    etat = 1;
                }
            break;
            case 1 :
                avance(400);
                etat=2;
                break;
            case 2:
                vitesse(0.0,0.0);
                wait(5);
                etat = 0;
                break;
        }
        wait(0.1);
    }
}

void init() {                               //Fonction d'initialisation
    char data[2] = {REG_CTRL_REG_1,0x01};   //On remplit un tableau de char avec 2 valeurs : numéro de registre et valeur
    composant.write(adresse_I2C,data,2);    //Activation du composant MMA8451
    MOT_G.period_us(100);
    MOT_D.period_us(100);
    am1.rise(&incrementation1);
    am2.rise(&incrementation2);
}

/*char getWhoAmI() {                      //Fonction permettant de récupérer l'identifiant du composant (ID)
    char registre[1]={REG_WHO_AM_I};    //Création et initialisation d'un tableau de char appelé "registre"
    char qui_je_suis[1];                //Création d'un tableau de 1 char qui permettra de stocker l'identifiant (ID)     
    composant.write(adresse_I2C,registre, 1, true); //Ecriture sur le bus = "Esclave n°0x3A, donne moi ton identifiant" - true = "j'attends ta réponse"
    composant.read(adresse_I2C, qui_je_suis, 1);    //Attente de réception de 1 octet : l'ID
    return qui_je_suis[0];                          //La fonction renvoie l'ID au programme principal
}*/

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

void incrementation1(void)
{
    if(bm1.read() == 0)
        cpt1++;
    else
        cpt1--;
}

void incrementation2(void)
{
    if(bm2.read() == 1)
        cpt2++;
    else
        cpt2--;
}

void vitesse(float a, float b) {
    //batterie = ((bat.read() * 3.3) * (59.0 / 12.0)) + 0.6;
    if(a >= 0)
    {
        SMOT_G.write(0);
        MOT_G.pulsewidth_us(a);
    }
    else {
        SMOT_G.write(1);
        MOT_G.pulsewidth_us(100-(-1 * a));
    }
    if(b >= 0)
    {
        SMOT_D.write(0);
        MOT_D.pulsewidth_us(b);
    }
    else {
        SMOT_D.write(1);
        MOT_D.pulsewidth_us(100-(-1 * b));
    }
}

void tourne(int16_t angle)
{
    cpt1 = 0;
    cpt2 = 0;
    angle = (angle*465/360)/2;
   
    do{
        if(cpt1 > (-1 * cpt2))    
            vitesse(35.0,-40.0);
        else
            vitesse(35.0,-30.0);
        wait_us(150.0);  
    }while(cpt1 < angle);
}

void avance(int16_t dist)
{
    cpt1 = 0;
    cpt2 = 0;
    do{
        if(cpt1>cpt2)
        {
            vitesse(35.0,40.0);
        }
        else if(cpt1<=cpt2)
        {
            vitesse(35.0,30.0);
        }
        wait_us(150);
    }while(cpt1 < dist);
}

