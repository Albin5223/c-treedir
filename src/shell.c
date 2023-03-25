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
            noeud *p=allerVers(courant,chem);
            if(p==NULL){
                puts("Dossier inexistant");
                return EXIT_FAILURE;
            }
            return p;
        }
    }
}

void pwd(noeud *courant){
    afficheCheminVersRacine(courant);
}

void mkdir(noeud *courant,char *nom){
    noeud *nouveauDossier = initDossier(courant, nom);
}

void touch(noeud *courant,char *nom){
    noeud *nouveauFichier = initFichier(courant,nom);
}






int main(){
    noeud *racine = initRacine();

    return EXIT_SUCCESS;
}