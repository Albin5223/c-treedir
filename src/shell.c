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


void arboDefaut(noeud *courant){
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
}


void shellManuel(noeud *courant){
    puts("--------------------------TEST DES COMMANDES DU SHELL A LA MAIN---------------------");

    puts("Bienvenue sur le shell C. Les commandes disponibles sont :\n- ls\n- cd\n- pwd\n- mkdir\n- touch\n- rm\n- cp\n- mv\n- print\n- quit\n");

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
                puts("Nombre d'arguments incorrect...");
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
                puts("Nombre d'arguments incorrect...");
            }
        }

        else if(strcmp(commande,"pwd") == 0){
            if(nb_args == 1){
                pwd(courant);
            }
            else{
                puts("Nombre d'arguments incorrect...");
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
                puts("Nombre d'arguments incorrect...");
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
                puts("Nombre d'arguments incorrect...");
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
                puts("Nombre d'arguments incorrect...");
            }
        }

        else if(strcmp(commande,"cp") == 0){
            if(nb_args == 3){
                cp(courant,arg1,arg2);
            }
            else{
                puts("Nombre d'arguments incorrect...");
            }
        }

        else if(strcmp(commande,"mv") == 0){
            if(nb_args == 3){
                mv(courant,arg1,arg2);
            }
            else{
                puts("Nombre d'arguments incorrect...");
            }
        }

        else if(strcmp(commande,"print") == 0){
            if(nb_args == 1){
                print(courant->racine);
            }
            else{
                puts("Nombre d'arguments incorrect...");
            }
        }

        else if(strcmp(commande,"quit") == 0){
            if(nb_args != 1){puts("Nombre d'arguments incorrect...");}
        }

        else{
            puts("Commande introuvable...");
        }

    }
    //Free de tous les mallocs

    free(commande);
    free(arg1);
    free(arg2);
    free(buffer);
}

#define LINE_LENGTH 100

void shellAuto(noeud *courant, char *chemin){
    
    FILE *flux = fopen(chemin,"r");
    if(flux == NULL){perror("Probleme ouverture de fichier...");}
    else{
        char *line = malloc(sizeof(char) * LINE_LENGTH);

        while (fgets(line,LINE_LENGTH,flux) != NULL){
            printf("%s",line);
        }


        if(fclose(flux) != 0){perror("Probleme fermeture de fichier...");}
    }
}

int main(int argc, char *argv[]){
    noeud *courant = initRacine();
    //arboDefaut(courant);

    // On revient bien à la racine au cas ou
    courant = cd(courant,"");

    if(argc <= 1){              // CAS 1 : Pas d'argument donc shell manuel
        shellManuel(courant);
    }
    else if(argc == 2){         // CAS 2 : Un argument -> on tente le lancement du shell auto
        printf("Fichier pointe : %s \n",*(argv+1));
        shellAuto(courant,*(argv+1));
    }
    else{                       // CAS 3 : Trop d'arguments
        puts("Trop d'arguments...");
    }


    return EXIT_SUCCESS;
}