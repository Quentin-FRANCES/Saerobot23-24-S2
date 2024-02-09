void debug(int etat){
    float u_batt, u_batt_mc, R4 = 4700, R3 = 1400;
    u_batt_mc = V_batt.read() * 3.3;
    u_batt = u_batt_mc * ((R3+R4)/R4) + 0.7;
    printf("etat : %d et jack.read %d\n\r", etat, JACK.read());
    printf("Capteur 1 (tout a gauche) : %.2lf\r\n", capt_1.read());
    printf("Capteur 2 : %.2lf\r\n", capt_g.read());
    printf("Capteur 3 : %.2lf\r\n", capt_d.read());
    printf("Capteur 4 (tout a droite) : %.2lf\r\n", capt_4.read());
    printf("Tension Batterie : %lg V\n\r", u_batt * 3.3);
    printf("\n\r");

}
