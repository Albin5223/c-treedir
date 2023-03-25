#include<noeud.h>
#include<stdlib.h>
#include<stdio.h>

noeud *initRacine(){
    noeud *p=initNode("Racine");

    return p;
}

void ls(noeud *courant){
    afficheFils(courant);
}

noeud *cd(noeud *courant,char *chem){
    if(strcmp(chem,"")==0){
        return retourRacine(courant);
    }
    else{
        if(strcmp(chem,"..")==0){
            return retourPere(courant);
        }
        else{
            
        }
    }
}





int main(){
    noeud *racine = initRacine();

    return EXIT_SUCCESS;
}