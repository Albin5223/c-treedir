#include "noeud.h"
#include "chemin.h"
#include<stdlib.h>
#include<stdio.h>


noeud *recupererNodeWithPath(noeud *courant,char *chem);

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

noeud *recupererNodeWithPath(noeud *courant,char *chem){
    noeud *tmp = courant;
    if(*(chem) == '/'){
        tmp=retourRacine(courant);
        chem = chem+1;
    }
    while(nbSlash(chem)!=0){
        size_t l = longueur(chem);
        char *sub = recuperer(chem,l);
        tmp = allerVers(tmp,sub);
        if(tmp == NULL){
            puts("chemin Inexistant");
            return NULL;
        }
        free(sub);
        chem = chem+l+1;
    }
    noeud *cible = getFils(tmp,chem);

    return cible;
}

void rm(noeud *courant, char *chem){
    noeud *cible = recupererNodeWithPath(courant,chem);
    if(cible == NULL){
        puts("Fichier ou dossier inexistant");
        return;
    }
    if(cible->est_dossier){
        if(estParent(courant,cible)){
            puts("Le dossier cible est le pere du dossier courant. Opération impossible");
            return;
        }
        else{
            removeNode(cible);
        }
    }
    else{
        removeNode(cible);
    }
}


void mv(noeud *courant,char *chem1,char *chem2){
    noeud *cible = recupererNodeWithPath(courant,chem1);
    if(cible == NULL){
        puts("Fichier ou dossier inexistant");
        return;
    }

    noeud *arrive = recupererNodeWithPath(courant,chem2);
    if(arrive == NULL){
        puts("Dossier inexistant");
        return;
    }
    moveNode(cible,arrive);
}


void cp(noeud *courant,char *chem1,char *chem2){
    noeud *cible = recupererNodeWithPath(courant,chem1);
    if(cible == NULL){
        puts("Fichier ou dossier inexistant");
        return;
    }

    noeud *arrive = recupererNodeWithPath(courant,chem2);
    if(arrive == NULL){
        puts("Dossier inexistant");
        return;
    }
    if(!arrive->est_dossier){
        puts("Le chemin d'arrive n'est pas un dossier");
        return;
    }
    
    noeud *copie = copyNode(cible);
    addNodeToFilsOfNode(arrive,copie);
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

    puts("--------------------------TEST 8--------------");

    rm(courant,"Exemple2");
    ls(courant);

    puts("--------------------------TEST 9--------------");

    courant = cd(courant,"");
    print(courant);
    rm(courant,"Cours/Cours1");
    puts("--------------------------TEST 9- Apres suppression-------------");
    print(courant);

    
    puts("--------------------------TEST 10---------------------");
    courant =cd(courant,"Cours/ExempleCours");
    ls(courant);
    print(courant);

    puts("--------------------------TEST 11---------------------");
    courant =cd(courant,"");
    mkdir(courant,"Test");
    print(courant);
    puts("--------------------------TEST 12---Apres mv----------------");

    mv(courant,"Cours/ExempleCours","Test");
    print(courant);
    courant = cd (courant,"Test");
    ls(courant);

    courant = cd(courant,"/Cours");
    ls(courant);

    puts("--------------------------TEST 13-------------------");
    courant = cd(courant,"..");
    courant = cd(courant,"Test");
    
    mkdir(courant,"Prenom");
    courant = cd(courant,"Prenom");
    touch(courant,"Servan");
    touch(courant,"Albin");
    touch(courant,"Margaux");
    touch(courant,"Perseverance");
    mkdir(courant,"Ville");
    courant = cd(courant,"Ville");
    touch(courant,"Paris");
    touch(courant,"Berlin");
    touch(courant,"Rome");
    courant = cd(courant,"/");
    print(courant);
    
    puts("--------------------------TEST 14--Apres cp--------------");
    cp(courant,"Test/Prenom","Cours");
    print(courant);
    





    

    return EXIT_SUCCESS;
}