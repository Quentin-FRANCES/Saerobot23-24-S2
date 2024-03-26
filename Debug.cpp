//a mettre dans le main

    #ifdef DEBUG
        printf("Bonjour et bienvenue sur le programme de debug de notre Robot - Le Vomonet \n\r");
        printf("Choisissez ce que vous voulez afficher : \n\r");
        printf("1 - Affichage des valeurs des capteurs\n\r");
        printf("2 - Affichage de l'etat du jack et FDC\n\r");
        printf("3 - Mesure de la tension\n\r");
        printf("4 - Tout ci-dessus\n\r");
        printf("5 - Test moteurs\n\r");
        printf("Que voulez vous afficher ? : ");
        scanf("%d", &ask);
        printf("\n\r");
    #endif


// A mettre dans le .h
void debug_batt(void){
   float u_batt, u_batt_mc, R4 = 4700, R3 = 1400;
    u_batt_mc = V_batt.read() * 3.3;
    u_batt = u_batt_mc * ((R3+R4)/R4) + 0.7; 
    printf("Tension Batterie : %lg V\n\r", u_batt * 3.3);
}

void debug_capt(void){
    printf("Capteur 1 (tout a gauche) : %.2lf\r\n", capt_1.read());
    printf("Capteur 2 : %.2lf\r\n", capt_g.read());
    printf("Capteur 3 : %.2lf\r\n", capt_d.read());
    printf("Capteur 4 (tout a droite) : %.2lf\r\n", capt_4.read());
}

void debug_FDC_JACK(void){
    printf("JACK : %d ||| FDC : %d\n\r", JACK.read(), FDC.read());
}

void debug_test_mot(int etat){
    int thD_debug, thG_debug;
    if(etat == 0){
        printf("tests moteurs en cours...\n\r");
        thD_debug = 0.90 *T, thG_debug = 0.88 * T;
        MOT_D.pulsewidth(thD_debug);
        MOT_G.pulsewidth(thG_debug);
        wait(5);
        MOT_D.pulsewidth(1);
        MOT_G.pulsewidth(1);
        wait(3);
        thD_debug = 0.50 *T, thG_debug = 0.48 * T;
        MOT_D.pulsewidth(thD_debug);
        MOT_G.pulsewidth(thG_debug);
        wait(5);
        MOT_D.pulsewidth(1);
        MOT_G.pulsewidth(1);
        wait(8);
    }
    if(etat == 1){
        printf("ESSAI MOTEUR IMPOSSIBLE, ROULAGE EN COURS !!!!\n\r");
    }
    if(etat == 2){
        printf("REMETTRE LE JACK POUR LANCER LE TEST MOTEUR\n\r");
    }

}

void debug(int etat, int ask){
    printf("etat : %d\n\r", etat);
    if(ask == 1) debug_capt();
    if(ask == 2) debug_FDC_JACK();
    if(ask == 3) debug_batt();
    if(ask == 4){debug_capt(); debug_FDC_JACK(); debug_batt();}
    if(ask == 5) debug_test_mot(etat);
    wait(0.7);
    printf("\n\r");
}
