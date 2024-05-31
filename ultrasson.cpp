#include "mbed.h"
#include "SRF05.h"

SRF05 Capt_avant(A5, D4);
SRF05 Capt_gauche(PTE30, D5);

int main() {
    while(true){
        printf("capt avant : %lg, capt gauche : %lg\n\r", Capt_avant.read(), Capt_gauche.read());
        wait(1);
    }
}
