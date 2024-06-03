#include "mbed.h"

Serial mon_module_bluetooth(PTE22,PTE23);

int main(){
  char Caractere_lu;
  while(true){
    if(mon_module_bluetooth.readable()){
      Caractere_lu = mon_module_bluetooth.getc();
      printf("%c\n\r", Caractere_lu);
    }
  }
}
