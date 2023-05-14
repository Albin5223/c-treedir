#include "noeud.h"
#include "chemin.h"
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>

noeud *courant;

noeud *recupererNodeWithPath(noeud *courant,char *chem);

noeud *initRacine(){
    noeud *p=initNode("");
    p->est_dossier = true;
    p->pere=p;
    p->racine=p;
    return p;
}


void ls(){
    afficheFils(courant);
}

noeud *cd(char *chem){
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

void pwd(){
    afficheCheminVersRacine(courant);
    puts("");
}

/**
 * Petite fonction permettant de savoir si une chaine de caractères est composée uniquement de caracteres alphanumerique ou non
 * @param nom La chaine à verifier
 * @return true si c'est une chaine alphanumerique, false sinon
*/
bool isStringAlNum(char *nom){
    for (unsigned int i = 0; i < strlen(nom); i++){
        if(!isalnum(*(nom+i))){
            return false;
        }
    }
    return true;
}

bool mkdir(char *nom){
    if(isStringAlNum(nom)){
        return initDossier(courant, nom) != NULL; 
    }
    else{
        puts("Nom du fichier non-alphanumerique...");
        return false;
    }
}

bool touch(char *nom){
    if(isStringAlNum(nom)){
        return initFichier(courant,nom) != NULL;
    }
    else{
        puts("Nom du fichier non-alphanumerique...");
        return false;
    }
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
        free(sub);
        if(tmp == NULL){
            puts("chemin Inexistant");
            return NULL;
        }
        
        chem = chem+l+1;
    }
    noeud *cible = getFils(tmp,chem);

    return cible;
}

bool rm(char *chem){
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


bool mv(char *chem1,char *chem2){
    noeud *cible = recupererNodeWithPath(courant,chem1);
    if(cible == NULL){
        puts("Fichier ou dossier inexistant");
        return false;
    }

    
    noeud *arrive = courant;
    if(*(chem2) == '/'){
        arrive=retourRacine(courant);
        chem2 = chem2+1;
    }
    while(nbSlash(chem2)!=0){
        size_t l = longueur(chem2);
        char *sub = recuperer(chem2,l);
        arrive = allerVers(arrive,sub);
        if(arrive == NULL){
            puts("chemin d'arrive inexistant");
            free(sub);
            return false;
        }
        free(sub);
        chem2 = chem2+l+1;
    }

    // Cela veut dire qu'on garde le même nom d'origine
    if(*(chem2) == '\0'){
        chem2 = cible->nom;
    }

    if(strlen(chem2) > NAME_LEN){
        puts("Le nom du fichier depasse 99 caracteres");
        return false;
    }

    if(!isStringAlNum(chem2)){
        puts("Nom du fichier non-alphanumerique...");
        return false;
    }

    if(verifierNomDejaExistant(arrive,chem2)){
        puts("Nom deja existant dans ce dossier");
        return false;
    }

    if(estParent(arrive,cible)){
        puts("Console invalide, vous essayez de déplacer un dossier dans un de ses fils");
        return false;
    }
    renameNode(cible,chem2);
    if(arrive == NULL){
        puts("Dossier inexistant");
        return false;
    }
    return moveNode(cible,arrive);
}


bool cp(char *chem1,char *chem2){
    noeud *cible = recupererNodeWithPath(courant,chem1);
    if(cible == NULL){
        puts("Fichier ou dossier inexistant");
        return false;
    }

    noeud *arrive = courant;
    if(*(chem2) == '/'){
        arrive=retourRacine(courant);
        chem2 = chem2+1;
    }
    while(nbSlash(chem2)!=0){
        size_t l = longueur(chem2);
        char *sub = recuperer(chem2,l);
        arrive = allerVers(arrive,sub);
        if(arrive == NULL){
            puts("chemin d'arrive inexistant");
            free(sub);
            return false;
        }
        free(sub);
        chem2 = chem2+l+1;
    }

    // Cela veut dire qu'on garde le même nom d'origine
    if(*(chem2) == '\0'){
        chem2 = cible->nom;
    }
    if(strlen(chem2) > NAME_LEN){
        puts("Le nom du fichier depasse 99 caracteres");
        return false;
    }
    if(arrive == NULL){
        puts("Dossier inexistant");
        return false;
    }
    if(estParent(arrive,cible)){
        puts("Console invalide, vous essayez de déplacer un dossier dans un de ses fils");
        return false;
    }
    if(!arrive->est_dossier){
        puts("Le chemin d'arrive n'est pas un dossier");
        return false;
    }
    if(verifierNomDejaExistant(arrive,chem2)){
        puts("Nom deja existant dans ce dossier");
        return false;
    }
    if(!isStringAlNum(chem2)){
        puts("Nom du fichier non-alphanumerique...");
        return false;
    }
    
    noeud *copie = copyNode(cible);
    
    renameNode(copie,chem2);
    if (addNodeToFilsOfNode(arrive,copie)){
        return true;
    }
    removeNode(cible);

    return false;
}

/**
 * Méthode qui permet d'executer une ligne de commande directement sur le noeud courant en argument
 * @param courant L'adresse du noeud en question
 * @param commande Le contenu de la commande (exemple : ls, cd, pd, etc...)
 * @param arg1 Le contenu du premier argument
 * @param arg2 Le contenu du deuxieme argument
 * @param nb_args Le nombre d'arguments, en comptant la commande egalement, exemple : "mv chem1 chem2" comptera comme 3 arguments
*/
bool executeCommande(char *commande, char *arg1, char *arg2, int nb_args){
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
            courant = cd("");
            return courant != NULL;
        }
        else if(nb_args == 2){
            noeud *tmp = cd(arg1);
            if(tmp!=NULL){
                courant = tmp;
                return true;
            }
            return false;
        }
        else{
            puts("Nombre d'arguments incorrect...");
            return false;
        }
    }

    else if(strcmp(commande,"pwd") == 0){
        if(nb_args == 1){
            pwd();
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
            return mkdir(arg1);
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
            return touch(arg1);
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
            return rm(arg1);
        }
        else{
            puts("Nombre d'arguments incorrect...");
            return false;
        }
    }

    else if(strcmp(commande,"cp") == 0){
        if(nb_args == 3){
            return cp(arg1,arg2);
        }
        else{
            puts("Nombre d'arguments incorrect...");
            return false;
        }
    }

    else if(strcmp(commande,"mv") == 0){
        if(nb_args == 3){
            return mv(arg1,arg2);
        }
        else{
            puts("Nombre d'arguments incorrect...");
            return false;
        }
    }

    else if(strcmp(commande,"print") == 0){
        if(nb_args == 1){
            print(courant->racine);
            return true;
        }
        else{
            puts("Nombre d'arguments incorrect...");
            return false;
        }
    }

    else if(strcmp(commande,"tree") == 0){
        if(nb_args == 1){
            tree(courant->racine);
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
void shellManuel(){
    puts("--------------------------TEST DES COMMANDES DU SHELL A LA MAIN---------------------");

    puts("Bienvenue sur le shell C. Les commandes disponibles sont :\n- ls\n- cd\n- pwd\n- mkdir\n- touch\n- rm\n- cp\n- mv\n- print\n- tree\n- quit\n");

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
        afficheCheminVersRacine(courant);printf("> ");
        fgets(buffer,MAX_L_BUFFER,stdin);
        int n = sscanf(buffer," %s %s %s",commande,arg1,arg2);
        nb_args += (n>0) ? n : 0;
        // Pour savoir le nombre d'arguments donnés :
        //printf("%d\n",nb_args);
        executeCommande(commande,arg1,arg2,nb_args);
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
void shellAuto(char *chemin){

    bool erreur = false;
    
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
            if(strlen(commande) != 0){
                if(executeCommande(commande,arg1,arg2,nb_args) == false){
                    printf("Il y a un soucis à la ligne %u : %s \n",n_line,buffer);
                    erreur = true;
                    break;
                }
            }
            
            n_line++;
        }


        free(commande);
        free(arg1);
        free(arg2);
        free(buffer);

        if(fclose(flux) != 0){perror("Probleme fermeture de fichier...");}

        if(erreur){
            return;
        }

        puts("\nVoulez-vous passer à un shell manuel ? (oui/non) : ");

        char *reponse = malloc(sizeof(char)*80);
        while (true){
            fgets(reponse,80,stdin);
            *(reponse+strlen(reponse)-1)='\0';
            if(strcmp(reponse,"oui") == 0){
                shellManuel();
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
    }
}

int main(int argc, char *argv[]){
    courant = initRacine();

    if(argc <= 1){              // CAS 1 : Pas d'argument donc shell manuel
        shellManuel();
    }
    else if(argc == 2){         // CAS 2 : Un argument -> on tente le lancement du shell auto
        printf("Fichier pointe : %s \n",*(argv+1));
        shellAuto(*(argv+1));
    }
    else{                       // CAS 3 : Trop d'arguments
        puts("Trop d'arguments...");
    }

    // On retourne à la racine pour tout free à la fin
    courant = cd("");
    freeRecInNode(courant);

    return EXIT_SUCCESS;
}