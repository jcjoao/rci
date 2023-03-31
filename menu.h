#ifndef __MENU__
#define __MENU__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: menu
 * Description: Imprime Menu
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void menu();

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: helpp
 * Description: Imprime Help
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void helpp();

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: all_digits
 * Description: Verifica se String só contem numeros
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int all_digits(char* str);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: verify ip
 * Description: Verifica se é ip
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int verify_ip(char* str);

#endif