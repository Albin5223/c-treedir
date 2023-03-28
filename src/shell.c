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

void rm(noeud *courant, char *chem){
    noeud *tmp = courant;
    if(*(chem) == '/'){
        tmp=retourRacine(courant);
        chem = chem+1;
    }
    while(nbSlash(chem)!=0){
        size_t l = longueur(chem);
        tmp = allerVers(tmp,recuperer(chem,l));
        if(tmp == NULL){
            puts("chemin Inexistant");
            return;
        }

        chem = chem+l+1;
    }
    noeud *cible = getFils(tmp,chem);
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

    puts("--------------------------TEST 8--------------");

    courant = cd(courant,"");
    print(courant);
    rm(courant,"Cours/Cours1");
    puts("--------------------------TEST 8 Apres Suppresion--------------");
    puts("--------------------------TEST 9---------------------");
    courant =cd(courant,"Cours/ExempleCours");
    ls(courant);
    rm(courant,"/Cours");



    print(courant);
    

    puts("--------------------------TEST DES COMMANDES DU SHELL A LA MAIN---------------------");

    puts("Bienvenue sur le shell C. Les commandes disponibles sont :\n- ls\n- cd\n- pwd\n- mkdir\n- touch\n- rm\n- cp\n- quit\n");

    courant = cd(courant,"");

    int MAX_L_COMMANDE = 8;
    int MAX_L_ARGS = 100;
    int MAX_L_BUFFER = MAX_L_ARGS + MAX_L_COMMANDE;

    char *commande = malloc(sizeof(char)*(MAX_L_COMMANDE+1));
    char *arg1 = malloc(sizeof(char)*(MAX_L_ARGS+1));
    char *arg2 = malloc(sizeof(char)*(MAX_L_ARGS+1));
    char *buffer = malloc(sizeof(char)*(MAX_L_BUFFER+1));

    int nb_args = 0;

    while (strcmp(commande,"quit") != 0 || nb_args != 1){    

        nb_args = 0;
        while (nb_args <= 0){
            afficheCheminVersRacine(courant);printf("> ");
            fgets(buffer,MAX_L_BUFFER,stdin);
            int n = sscanf(buffer," %s %s %s",commande,arg1,arg2);
            nb_args += (n>0) ? n : 0;
            // Pour savoir le nombre d'arguments donnés :
            //printf("%d\n",nb_args);
        }

        if(strcmp(commande,"ls") == 0){
            if(nb_args == 1){
                ls(courant);
            }
            else{
                puts("Trop d'arguments...");
            }
        }
        
        else if(strcmp(commande,"cd") == 0){
            if(nb_args == 1){
                courant = cd(courant,"");
            }
            else if(nb_args == 2){
                courant = cd(courant,arg1);
            }
            else{
                puts("Trop d'arguments...");
            }
        }

        else if(strcmp(commande,"pwd") == 0){
            if(nb_args == 1){
                pwd(courant);
            }
            else{
                puts("Trop d'arguments...");
            }
        }

        else if(strcmp(commande,"mkdir") == 0){
            if(nb_args == 1){
                puts("Il manque un argument...");
            }
            else if(nb_args == 2){
                mkdir(courant,arg1);
            }
            else{
                puts("Trop d'arguments...");
            }
        }

        else if(strcmp(commande,"touch") == 0){
            if(nb_args == 1){
                puts("Il manque un argument...");
            }
            else if(nb_args == 2){
                touch(courant,arg1);
            }
            else{
                puts("Trop d'arguments...");
            }
        }

        else if(strcmp(commande,"rm") == 0){
            if(nb_args == 1){
                puts("Il manque un argument...");
            }
            else if(nb_args == 2){
                rm(courant,arg1);
            }
            else{
                puts("Trop d'arguments...");
            }
        }

        else if(strcmp(commande,"quit") == 0){
            if(nb_args != 1){puts("Trop d'arguments...");}
        }

        else{
            puts("Commande introuvable...");
        }

    }
    

    return EXIT_SUCCESS;
}