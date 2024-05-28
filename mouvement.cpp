#include "mbed.h"


InterruptIn am1(D2);  // encodeur mot_G
InterruptIn am2(D11); // encodeur mot_D
DigitalIn bm1(D3);  // encodeur mot_G
DigitalIn bm2(D10); // encodeur mot_D
PwmOut MOT_D(D8); // puissance mot_D
PwmOut MOT_G(D6); // puissance mot_G
DigitalOut SMOT_D(D9); // sens mot_D
DigitalOut SMOT_G(D7); // sens mot_G
DigitalIn JACK(PTE20); // enfoncé à 0;
DigitalIn FDC(PTE21);  // enfoncé à 0;

int16_t cpt1 = 0, cpt2 = 0;

void init();
void tourne(int16_t);
void avance(int16_t);
void incrementation1(void);
void incrementation2(void);
void vitesse(float,float);
int main() {

    JACK.mode(PullUp);
    FDC.mode(PullUp);
    init();
    // avance(100);
    // wait(1);
    /*tourne(90);
    wait(1);
    avance(100);
    wait(1);
    tourne(90);*/
    while(1) 
    {
        //float jack = JACK.read();
        //float fdc = FDC.read();
        //if (jack == 1 && fdc == 1) {
            /*if(cpt1>cpt2)
            vitesse(37.0,43.0);
            else if(cpt1<=cpt2)
            vitesse(43.0,37.0);*/
        //}
        avance(400);
        vitesse(0.0,0.0);
        wait(3);
        tourne(90);
        vitesse(0.0,0.0);
        wait(3);
    }
}

void init()
{
    MOT_G.period_us(100);
    MOT_D.period_us(100);
    am1.rise(&incrementation1);
    am2.rise(&incrementation2);
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


