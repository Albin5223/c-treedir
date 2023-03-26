#include "noeud.h"
#include "chemin.h"
#include<stdlib.h>
#include<stdio.h>

noeud *initRacine(){
    noeud *p=initNode("");
    p->est_dossier = true;
    p->pere=p;
    p->racine=p;
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
            noeud *p = courant;
            if(*(chem) == '/'){
                p=retourRacine(courant);
                chem = chem+1;
            }
            size_t somme = 0;
            size_t l = strlen(chem);
            size_t nb = nbSlash(chem);
    

            while(somme != l-nb){
                size_t res = longueur(chem);
                char *sub = recuperer(chem,res);
                
                p=allerVers(p,sub);
                if(p==NULL){
                    puts("Dossier inexistant");
                    return courant;
                }

                free(sub);

                chem = chem+res+1;
                somme +=res;
            }
            
            return p;
        }
    }
}

void pwd(noeud *courant){
    afficheCheminVersRacine(courant);
    puts("");
}

void mkdir(noeud *courant,char *nom){
    initDossier(courant, nom);
}

void touch(noeud *courant,char *nom){
    initFichier(courant,nom);
}






int main(){
    noeud *courant = initRacine();


    puts("--------------------------TEST 1--------------");
    mkdir(courant,"Cours");
    touch(courant,"Travail");
    
    touch(courant,"test");

    mkdir(courant,"TD");
    ls(courant);
    puts("--------------------------TEST 2--------------");
    courant = cd(courant,"Cours");
    
    touch(courant,"Cours1");
    touch(courant,"cours2");

    

    mkdir(courant,"ExempleCours");
    ls(courant);

    puts("--------------------------TEST 3--------------");
    courant = cd(courant,"Cours");


    puts("--------------------------TEST 4--------------");
    courant = cd(courant,"ExempleCours");
    touch(courant,"Exemple1");
    touch(courant,"Exemple2");
    ls(courant);
    pwd(courant);

    puts("--------------------------TEST 5--------------");
    courant=cd(courant,"");
    ls(courant);


    puts("--------------------------TEST 6--------------");
    print(courant);

    puts("--------------------------TEST 7--------------");
    pwd(courant);
    courant =cd(courant,"Cours/ExempleCours");
    ls(courant);



    

    return EXIT_SUCCESS;
}