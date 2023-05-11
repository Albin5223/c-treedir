#include "chemin.h"
#include <stdlib.h>

/**
 * Cette fonction renvoie le nombre de / dans s1
 * @param s1 est le mot dont on veut le début
 * @return size_t le nombre de / dans s1
*/

size_t nbSlash(char *s1){
    size_t compt = 0;
    size_t i = 0;
    while(*(s1+i) != '\0'){
        if(*(s1+i)=='/'){
            compt++;
        }
        i++;
    }
    return compt;
}

/**
 * Cette fonction renvoie la longueur du mot qui se trouve avant le prochain /
 * @param s1 est le chemin qu'on veut decomposer
 * @return La longueur du mot avant /
*/
size_t longueur(char *s1){
    size_t i = 0;
    while(*(s1+i) != '/' && *(s1+i) != '\0'){
        i++;
    }
    return i;
}


/**
 * Cette fonction renvoie le mot de 0 a lon
 * @param s1 est le mot dont on veut le début
 * @param lon est la longueur du sous-mot
 * @return L'emplacement du sous mot !!! 
 * @warning Il faut free la valeur de retour
*/
char *recuperer(char *s1,size_t lon){
    char *tab = malloc((lon+1)*sizeof(char));

    for(size_t i = 0;i<lon;i++){
        *(tab+i) = *(s1+i);
    }
    *(tab+lon)='\0';

    return tab;
}
