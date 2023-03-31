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
 * Description: Verifica se string só contem números
 *
 * Parameter: string a confirmar se tem apenas digitos
 * Return: 1 caso a string apenas contenha digitos, e 0 em caso contrário
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int all_digits(char* str);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: verify ip
 * Description: Verifica se é ip
 *
 * Parameter: string com valor do IP
 * Return: 1 caso o valor  do IP esteja correto, e 0 caso contrário
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int verify_ip(char* str);

#endif