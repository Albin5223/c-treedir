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
                free(sub);
                if(p==NULL){
                    puts("Dossier inexistant");
                    return NULL;
                }
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

bool mkdir(noeud *courant,char *nom){
    return initDossier(courant, nom) != NULL;
}

bool touch(noeud *courant,char *nom){
    return initFichier(courant,nom) != NULL;
}

noeud *recupererNodeWithPath(noeud *courant,char *chem){
    noeud *tmp = courant;
    if(*(chem) == '/'){
        tmp=retourRacine(courant);
        if(strlen(chem)==1){
            return retourRacine(courant);
        }
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

bool rm(noeud *courant, char *chem){
    noeud *cible = recupererNodeWithPath(courant,chem);
    if(cible == NULL){
        puts("Fichier ou dossier inexistant");
        return false;
    }
    if(cible->est_dossier){
        if(estParent(courant,cible)){
            puts("Le dossier cible est le pere du dossier courant. Opération impossible");
            return false;
        }
        else{
            removeNode(cible);
        }
    }
    else{
        removeNode(cible);
    }
    return true;
}


bool mv(noeud *courant,char *chem1,char *chem2){
    noeud *cible = recupererNodeWithPath(courant,chem1);
    if(cible == NULL){
        puts("Fichier ou dossier inexistant");
        return false;
    }

    noeud *arrive = recupererNodeWithPath(courant,chem2);
    if(arrive == NULL){
        puts("Dossier inexistant");
        return false;
    }
    if(estParent(arrive,cible)){
        puts("Console invalide, vous essayez de déplacer un dossier dans un de ses fils");
        return false;
    }
    return moveNode(cible,arrive);


}


bool cp(noeud *courant,char *chem1,char *chem2){
    noeud *cible = recupererNodeWithPath(courant,chem1);
    if(cible == NULL){
        puts("Fichier ou dossier inexistant");
        return false;
    }

    noeud *arrive = recupererNodeWithPath(courant,chem2);
    if(arrive == NULL){
        puts("Dossier inexistant");
        return false;
    }
    if(!arrive->est_dossier){
        puts("Le chemin d'arrive n'est pas un dossier");
        return false;
    }
    
    noeud *copie = copyNode(cible);
    if (addNodeToFilsOfNode(arrive,copie)){
        return true;
    }
    removeNode(cible);

    return false;
}

/**
 * Cette méthode contient une série d'instructions de test, qui modifie par la même occasion le noeud en argument
 * @param courant L'adresse du noeud qui représentera la racine des tests
 * @warning Il faut bel et bien donner l'adresse du noeud afin de pouvoir le modifier lors de l'appel à la fonction (exemple : arboDefaut(&racine))
*/
void arboDefaut(noeud **courant){
    mkdir(*courant,"Cours");
    touch(*courant,"Travail");
    
    touch(*courant,"test");

    mkdir(*courant,"TD");

    *courant = cd(*courant,"Cours");
    
    touch(*courant,"Cours1");
    touch(*courant,"cours2");

    

    mkdir(*courant,"ExempleCours");

    *courant = cd(*courant,"ExempleCours");
    touch(*courant,"Exemple1");
    touch(*courant,"Exemple2");

    *courant=cd(*courant,"");



    *courant = cd(*courant,"Cours/ExempleCours");


    rm(*courant,"Exemple2");


    *courant = cd(*courant,"");
    rm(*courant,"Cours/Cours1");

    
    *courant =cd(*courant,"Cours/ExempleCours");

    *courant =cd(*courant,"");
    mkdir(*courant,"Test");

    mv(*courant,"Cours/ExempleCours","Test");
    *courant = cd (*courant,"Test");

    *courant = cd(*courant,"/Cours");

    
    *courant = cd(*courant,"..");
    *courant = cd(*courant,"Test");
    
    mkdir(*courant,"Prenom");
    *courant = cd(*courant,"Prenom");
    touch(*courant,"Servan");
    touch(*courant,"Albin");
    touch(*courant,"Margaux");
    touch(*courant,"Perseverance");
    mkdir(*courant,"Ville");
    *courant = cd(*courant,"Ville");
    touch(*courant,"Paris");
    touch(*courant,"Berlin");
    touch(*courant,"Rome");
    *courant = cd(*courant,"/");
    
    cp(*courant,"Test/Prenom","Cours");

    *courant =cd(*courant,"");
}

/**
 * Méthode qui permet d'executer une ligne de commande directement sur le noeud courant en argument
 * @param courant L'adresse du noeud en question
 * @param commande Le contenu de la commande (exemple : ls, cd, pd, etc...)
 * @param arg1 Le contenu du premier argument
 * @param arg2 Le contenu du deuxieme argument
 * @param nb_args Le nombre d'arguments, en comptant la commande egalement, exemple : "mv chem1 chem2" comptera comme 3 arguments
*/
bool executeCommande(noeud **courant, char *commande, char *arg1, char *arg2, int nb_args){
    if(strcmp(commande,"ls") == 0){
            if(nb_args == 1){
                ls(*courant);
                return true;
            }
            else{
                puts("Nombre d'arguments incorrect...");
                return false;
            }
        }
        
    else if(strcmp(commande,"cd") == 0){
        if(nb_args == 1){
            *courant = cd(*courant,"");
            return *courant != NULL;
        }
        else if(nb_args == 2){
            *courant = cd(*courant,arg1);
            return *courant != NULL;
        }
        else{
            puts("Nombre d'arguments incorrect...");
            return false;
        }
    }

    else if(strcmp(commande,"pwd") == 0){
        if(nb_args == 1){
            pwd(*courant);
            return true;
        }
        else{
            puts("Nombre d'arguments incorrect...");
            return false;
        }
    }

    else if(strcmp(commande,"mkdir") == 0){
        if(nb_args == 1){
            puts("Il manque un argument...");
            return false;
        }
        else if(nb_args == 2){
            return mkdir(*courant,arg1);
        }
        else{
            puts("Nombre d'arguments incorrect...");
            return false;
        }
    }

    else if(strcmp(commande,"touch") == 0){
        if(nb_args == 1){
            puts("Il manque un argument...");
            return false;
        }
        else if(nb_args == 2){
            return touch(*courant,arg1);
        }
        else{
            puts("Nombre d'arguments incorrect...");
            return false;
        }
    }

    else if(strcmp(commande,"rm") == 0){
        if(nb_args == 1){
            puts("Il manque un argument...");
            return false;
        }
        else if(nb_args == 2){
            return rm(*courant,arg1);
        }
        else{
            puts("Nombre d'arguments incorrect...");
            return false;
        }
    }

    else if(strcmp(commande,"cp") == 0){
        if(nb_args == 3){
            return cp(*courant,arg1,arg2);
        }
        else{
            puts("Nombre d'arguments incorrect...");
            return false;
        }
    }

    else if(strcmp(commande,"mv") == 0){
        if(nb_args == 3){
            return mv(*courant,arg1,arg2);
        }
        else{
            puts("Nombre d'arguments incorrect...");
            return false;
        }
    }

    else if(strcmp(commande,"print") == 0){
        if(nb_args == 1){
            print((*courant)->racine);
            return true;
        }
        else{
            puts("Nombre d'arguments incorrect...");
            return false;
        }
    }

    else if(strcmp(commande,"quit") == 0){
        if(nb_args != 1){
            puts("Nombre d'arguments incorrect...");
        }
        return false;
    }
    else{
        puts("Commande introuvable...");
        return false;
    }
}

/**
 * Méthode qui permet de lancer un shell ou l'utilisateur rentre ses commandes à la main
 * @param courant L'adresse du noeud représentant la racine
*/
void shellManuel(noeud **courant){
    puts("--------------------------TEST DES COMMANDES DU SHELL A LA MAIN---------------------");

    puts("Bienvenue sur le shell C. Les commandes disponibles sont :\n- ls\n- cd\n- pwd\n- mkdir\n- touch\n- rm\n- cp\n- mv\n- print\n- quit\n");

    // Bout de code permettant de donner le choix à l'utilisateur s'il veut une arborescence par défaut ou non
    /*
    char *reponse = malloc(sizeof(char)*4);
    *reponse = '\0';

    printf("Voulez-vous une arborescence de test ? (oui/non) : ");
    while (true){
        fgets(reponse,4,stdin);
        if(strcmp(reponse,"oui") == 0){
            arboDefaut(courant);
            break;
        }
        else if(strcmp(reponse,"non") == 0){
            break;
        }
        else{
            printf("\nRepondez par 'oui' ou 'non' : ");
        }
    }
    free(reponse);

    */

    int MAX_L_COMMANDE = 8;
    int MAX_L_ARGS = 100;
    int MAX_L_BUFFER = 2*MAX_L_ARGS + MAX_L_COMMANDE;

    char *commande = malloc(sizeof(char)*(MAX_L_COMMANDE+1));
    char *arg1 = malloc(sizeof(char)*(MAX_L_ARGS+1));
    char *arg2 = malloc(sizeof(char)*(MAX_L_ARGS+1));
    char *buffer = malloc(sizeof(char)*(MAX_L_BUFFER+1));

    int nb_args = 0;

    while (strcmp(commande,"quit") != 0 || nb_args != 1){    

        *commande = '\0';
        *arg1 = '\0';
        *arg2 = '\0';

        nb_args = 0;
        afficheCheminVersRacine(*courant);printf("> ");
        fgets(buffer,MAX_L_BUFFER,stdin);
        int n = sscanf(buffer," %s %s %s",commande,arg1,arg2);
        nb_args += (n>0) ? n : 0;
        // Pour savoir le nombre d'arguments donnés :
        //printf("%d\n",nb_args);
        executeCommande(courant,commande,arg1,arg2,nb_args);
    }
    // Free de tous les mallocs

    free(commande);
    free(arg1);
    free(arg2);
    free(buffer);
}

/**
 * Méthode qui permet de lancer un shell, ou les commandes sont executés à partir d'un fichier
 * @param courant L'adresse du noeud représentant la racine
 * @param chemin Le chemin du fichier contenant les commandes
*/
void shellAuto(noeud **courant, char *chemin){
    
    FILE *flux = fopen(chemin,"r");
    if(flux == NULL){perror("Probleme ouverture de fichier...");}
    else{

        int MAX_L_COMMANDE = 8;
        int MAX_L_ARGS = 100;
        int MAX_L_BUFFER = 2*MAX_L_ARGS + MAX_L_COMMANDE;

        char *commande = malloc(sizeof(char)*(MAX_L_COMMANDE+1));
        char *arg1 = malloc(sizeof(char)*(MAX_L_ARGS+1));
        char *arg2 = malloc(sizeof(char)*(MAX_L_ARGS+1));
        char *buffer = malloc(sizeof(char)*(MAX_L_BUFFER+1));

        int nb_args = 0;
        int n_line = 1;

        while (fgets(buffer,MAX_L_BUFFER,flux) != NULL){

            *commande = '\0';
            *arg1 = '\0';
            *arg2 = '\0';

            nb_args = 0;

            int n = sscanf(buffer," %s %s %s",commande,arg1,arg2);
            nb_args += (n>0) ? n : 0;

            if(executeCommande(courant,commande,arg1,arg2,nb_args) == false){
                printf("Il y a un soucis à la ligne %u : %s \n",n_line,buffer);
                break;
            }

            n_line++;
        }

        free(commande);
        free(arg1);
        free(arg2);
        free(buffer);

        if(fclose(flux) != 0){perror("Probleme fermeture de fichier...");}
    }
}

int main(int argc, char *argv[]){
    noeud *courant = initRacine();

    if(argc <= 1){              // CAS 1 : Pas d'argument donc shell manuel
        shellManuel(&courant);
    }
    else if(argc == 2){         // CAS 2 : Un argument -> on tente le lancement du shell auto
        printf("Fichier pointe : %s \n",*(argv+1));
        shellAuto(&courant,*(argv+1));
    }
    else{                       // CAS 3 : Trop d'arguments
        puts("Trop d'arguments...");
    }

    free(courant);

    return EXIT_SUCCESS;
}