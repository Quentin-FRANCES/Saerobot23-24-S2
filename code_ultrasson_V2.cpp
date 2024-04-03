#include "mbed.h"

DigitalOut trig(D4);
InterruptIn echo(D5);
Timer timer;
Ticker tic;
float mesure;
void echo_rise();
void echo_fall();
void tck();

int main(){
    echo.rise(&echo_rise);
    echo.fall(&echo_fall);
    tic.attach(&tck, 1);
    while(true){
        printf("val = %lg\n\r", mesure);
        printf("disance : %lg \n\r\n", (340*mesure)/2);
        wait(0.1);
    }
}

void tck(){
    trig.write(1);
    wait_us(20);
    trig.write(0);
}

void echo_rise(){
    timer.start();
}

void echo_fall(){
    timer.stop();
    mesure = timer.read();
    timer.reset();
}
