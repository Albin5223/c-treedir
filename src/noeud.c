#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<assert.h>
#include "noeud.h"

/**
 * Cette fonction permet d'initialiser une liste à partir d'un noeud
 * @param tete La tete de la liste
 * @return La liste
*/
liste_noeud *initList(noeud *tete){
    liste_noeud *list = malloc(sizeof(liste_noeud));
    assert(list != NULL);
    list->no = tete;
    list->succ = NULL;
    return list;
}

/**
 * Fonction qui permet de donner la taille d'une liste
 * @param list La liste
 * @return la taille de la liste
*/
unsigned int size(liste_noeud *list){
    unsigned int size = 1;
    liste_noeud *tmp = list;
    while (tmp->succ != NULL){
        ++size;
        tmp = tmp->succ;
    }
    return size;
}

void renameNode(noeud *courant, char *nouveau){
    int i = 0;
    while(i<strlen(nouveau)){
        *(courant->nom+i) = *(nouveau+i);
        i++;
    }
    *(courant->nom+i) = '\0';

}

/**
 * Cette fonction permet d'ajouter dans une liste un noeud, dans l'ordre alphabétique + trier par type (dossier/fichier)
 * PS : Les dossiers seront donc toujours avant les fichiers, et dans chacun des deux blocs, il y aura un tri alphabetique
 * @param list La liste dans laquelle on veut ajouter le noeud
 * @param noeud Le noeud à ajouter
 * @return La nouvelle tete de la liste (au cas ou elle a été changée)
 * 
 * @warning ! Ne jamais utiliser cette fonction directement parce que c'est une fonction auxiliaire ! Il faut passer par addNodeToFilsOfNode qui s'en sert correctement
*/
liste_noeud *addToListAlpha(liste_noeud *list, noeud *node){
    assert(list != NULL || node != NULL);

    if(verifierNomDejaExistant(list->no->pere,node->nom)){
        puts("Message d'erreur : Vous essayez d'ajouter un noeud avec un nom déjà existant dans une liste");
        return list;
    }

    liste_noeud *newList = initList(node);
    liste_noeud *prev = NULL;
    liste_noeud *tmp = list;

    if(node->est_dossier){                 // CAS 1 : Si le noeud est un dossier, on veut l'ajouter dans la première partie avec les dossiers      
        while(tmp != NULL && tmp->no->est_dossier){
            if(strcmp(node->nom,tmp->no->nom) < 0){       // On trouve l'emplacement
                if(prev == NULL){           // CAS 1.1 : Le dossier doit etre placé au tout début
                    newList->succ = list;
                    return newList;
                }
                else{                       // CAS 1.2 : Le dossier doit être placé entre d'autres dossiers
                    prev->succ = newList;
                    newList->succ = tmp;
                    return list;
                }
            }
            prev = tmp;
            tmp = tmp->succ;
        }
        // CAS 1.3 : Le dossier doit être placé à la fin des dossiers
        if(prev == NULL){   // CAS 1.3.1 : Il n'y avait en faite pas de dossier
            if(tmp == NULL){    // CAS 1.3.1.1 : Il n'y avait pas de fichier non plus
                return newList;
            }
            else{               // CAS 1.2.1.2 : Il y avait au moins 1 fichier
                newList->succ = tmp;
                return newList;
            }
        }
        // CAS 1.3.2 : Il y avait au moins un dossier derrière
        prev->succ = newList;
        newList->succ = tmp;
        return list;
    }
    else{                               // CAS 2 : Si le noeud qu'on veut ajouter n'est pas un dossier, on veut donc l'ajouter dans la seconde partie avec les fichier
        while (tmp != NULL && tmp->no->est_dossier){    // On traverse tous les dossiers
            prev = tmp;
            tmp = tmp->succ;
        }

        if(tmp == NULL){        // CAS 2.1 : Il n'y a pas de fichiers
            if(prev == NULL){       // CAS 2.1.1 : Il n'y a pas de dossier non plus
                return newList;
            }
            else{                   // CAS 2.1.2 : Il y a des dossiers
                prev->succ = newList;
                return list;
            }
        }
        else{                   // CAS 2.2 : Il y a des fichiers
            while (tmp != NULL){
                if(strcmp(node->nom,tmp->no->nom) < 0){   // On trouve l'emplacement
                    if(prev == NULL){           // CAS 2.2.1 : Il n'y a pas de dossier derriere et le noeud doit être placé au début
                        newList->succ = list;
                        return newList;
                    }
                    else{               // CAS 2.2.2 : Il y a des dossiers ou fichiers derriere
                        prev->succ = newList;
                        newList->succ = tmp;
                        return list;
                    }
                }
                prev = tmp;
                tmp = tmp->succ;
            }
            // CAS 2.3 : Le fichier doit être placé à la fin
            prev->succ = newList;
            return list;
        }
    }
}

/**
 * Cette fonction permet d'initialiser un noeud
 * @warning Ses attributs sont initialisés à false et NULL partout (à part le nom), il faut donc les changer en fonction du contexte
 * @return Un noeud par défaut
*/
noeud *initNode(char *c){
    noeud *node = malloc(sizeof(noeud));
    assert(node != NULL);
    node->est_dossier = false;
    for(size_t i = 0;i<strlen(c)+1;i++){
        *(node->nom+i)=*(c+i);
    }
    node->pere = NULL;
    node->racine = NULL;
    node->fils = NULL;

    return node;
}

/**
 * Fonction qui donne le nombre de fils que possède un noeud
 * @param node Le noeud
 * @return Le nombre de fils de ce noeud
*/
unsigned int sizeOfFils(noeud *node){
    if(node->fils == NULL){
        return 0;
    }
    return size(node->fils);
}

/**
 * Cette fonction ajoute un noeud dans la liste de fils d'un autre noeud
 * @param pere Ce noeud est le pere qui se voit rajouter un fils
 * @param fils Le noeud qui devient le fils
*/
bool addNodeToFilsOfNode(noeud *pere, noeud *fils){
    if(verifierNomDejaExistant(pere,fils->nom)){
        puts("Message d'erreur : Vous essayez d'ajouter un noeud avec un nom déjà existant dans une liste");
        return false;
    }
    if(!pere->est_dossier){
        puts("Message d'erreur : Vous essayez d'ajouter un noeud dans un autre noeud qui n'est PAS un dossier");
        return false;
    }
    fils->pere = pere;
    fils->racine = pere->racine;
    if(pere->fils == NULL){
        pere->fils = initList(fils);
    }
    else{
        pere->fils = addToListAlpha(pere->fils,fils);
    }
    return true;
}

/**
 * Fonction qui permet de free récursivement un noeud ainsi que ses fils
 * @param node Le noeud à free
*/
void freeRecInNode(noeud *node){
    liste_noeud *tmp = node->fils;
    while (tmp != NULL){
        liste_noeud *tmp1 = tmp->succ;
        removeNode(tmp->no);
        tmp = tmp1;
    }
    free(node->fils);
    free(node);
}

/**
 * Fonction qui permet de supprimer complètement un noeud de l'arborescence
 * @param node Le noeud à supprimer
*/
void removeNode(noeud *node){
    noeud *pere = node->pere;

    liste_noeud *prev = NULL;
    liste_noeud *tmp = pere->fils;

    liste_noeud *trouve = NULL;

    // ETAPE 1 : Supprimer ce noeud de la liste de fils de son père
    while(tmp != NULL){
        if(tmp->no == node){   // On trouve ou est le noeud
            trouve = tmp;
            if(prev == NULL){   // Le noeud est au début
                pere->fils = tmp->succ;
            }
            else{                   // Le noeud est plus loin
                prev->succ = tmp->succ;
            }
            break;
        }
        prev = tmp;
        tmp = tmp->succ;
    }
    if(trouve != NULL){
        free(trouve);
    }
    
    // ETAPE 2 : "free" tous les noeuds
    freeRecInNode(node);
}

/**
 * Cette fonction déplace un noeud vers un nouveau père
 * @param nomade Le noeud qui va se déplacer
 * @param newPere Le nouveau père du noeud nomade
*/
bool moveNode(noeud *nomade, noeud *newPere){
    if(nomade->pere == NULL){
        puts("Message d'erreur : Le noeud que vous voulez DEPLACER n'a pas de pere (situation anormale)");
        return false;
    }
    if(!newPere->est_dossier){
        puts("Message d'erreur : Le noeud d'arrive n'est pas un dossier");
        return false;
    }

    noeud *pere = nomade->pere;

    liste_noeud *prev = NULL;
    liste_noeud *tmp = pere->fils;

    liste_noeud * trouve = NULL;

    // ETAPE 1 : Supprimer ce noeud de la liste de fils de son père
    while(tmp != NULL){
        if(tmp->no == nomade){  // On trouve ou est le noeud
            trouve = tmp;
            if(prev == NULL){       // Le noeud est au début
                pere->fils = tmp->succ;
            }
            else{                   // Le noeud est plus loin
                prev->succ = tmp->succ;
            }
            break;
        }
        prev = tmp;
        tmp = tmp->succ;
    }
    free(trouve);
    // ETAPE 2 : Ajouter le nomade comme fils du nouveau pere
    if(addNodeToFilsOfNode(newPere,nomade)){
        return true;
    }
    else{
        addNodeToFilsOfNode(pere,nomade);
        return false;
    }
}

/**
 * @warning Cette fonction renvoie un noeud, qui est belle et bien une copie de "node", MAIS cette copie n'a pas de pere, ni de racine, il faut donc les mettre à jour par la suite
 * @param node Le noeud à copier
 * @return Une copie de node
*/
noeud *copyNode(noeud *node){
    noeud *copy = initNode(node->nom);
    copy->est_dossier = node->est_dossier;
    copy->racine = node->racine;
    if(node->est_dossier){
        liste_noeud *tmp = node->fils;
        while(tmp!=NULL){
            noeud *copieFils = copyNode(tmp->no);
            addNodeToFilsOfNode(copy,copieFils);
            tmp=tmp->succ;
        }
    }
    
    return copy;
}

/**
 * Permet d'afficher tout le contenu du noeud
 * @param node noeud à afficher
*/
void print(noeud *node){
    printf("Noeud ");printNameWithType(node);printf(", ");

    if(node->pere != node){
        printf("pere : %s, ",strcmp(node->pere->nom,"")==0 ? "/" : node->pere->nom);
    }

    if(node->est_dossier){
        unsigned int tfils = sizeOfFils(node);
        printf("%u fils : ",tfils);
    }

    liste_noeud *tmp = node->fils;
    while (tmp != NULL){
        printNameWithType(tmp->no);printf(", ");
        tmp = tmp->succ;
    }
    puts("");

    tmp = node->fils;
    while (tmp != NULL){
        print(tmp->no);
        tmp = tmp->succ;
    }
}

/**
 * Petite méthode auxiliaire pour afficher le nom d'un noeud ainsi que son type
 * @param node Le noeud en question
*/
void printNameWithType(noeud *node){
    strcmp(node->nom,"") == 0 ? printf("/ ") : printf("%s ",node->nom);
    node->est_dossier ? printf("(D)") : printf("(F)");
}

/**
 * Fonction qui affiche ligne par ligne, le nom de chaque fils d'un noeud
 * @param p le noeud en question
*/
void afficheFils(noeud *p){
    liste_noeud *tmp = p->fils;

    while(tmp !=NULL){
        afficheNom(tmp->no);
        tmp = tmp->succ;
    }
}

/**
 * Fonction qui permet simplement d'afficher le nom d'un noeud, suivi d'un retour à la ligne
 * @param p Le noeud dont on veut récupérer le nom
*/
void afficheNom(noeud *p){
    printf("%s",p->nom);
    p->est_dossier ? printf("/\n") : printf("\n");
}

/**
 * Fonction qui permet de retourner à la racine d'un noeud
 * @param p Le noeud en question
 * @return La racine de ce noeud
*/
noeud *retourRacine(noeud *p){
    return p->racine;
}

/**
 * Fonction qui permet de retourner au pere d'un noeud
 * @param p Le noeud en question
 * @return Le pere de ce noeud
*/
noeud *retourPere(noeud *p){
    return p->pere;
}

/**
 * Cette fonction verifie si un nom existe déja dans les fils du noeud courant
 * @param p noeud courant
 * @param nom nom à vérifier s'il existe déjà
 * @return true si ce nom existe déjà, false sinon
*/
bool verifierNomDejaExistant(noeud *p, char *nom){
    int trouve = 1;
    liste_noeud *tmp = p->fils;
    while (tmp!=NULL){
        char *s1 = tmp->no->nom;
        trouve = strcmp(nom,s1);
        if(trouve == 0){
            return true;
        }
        tmp=tmp->succ;
    }
    return false;
}

/**
 * Fonction qui permet d'initialiser un fichier dans un fossier
*/
noeud *initFichier(noeud *pere, char *nom){
    if(verifierNomDejaExistant(pere,nom)){
        puts("Nom de Fichier deja Existant");
        return NULL;
    }
    noeud *n = initNode(nom);
    n->est_dossier=false;
    n->pere=pere;
    n->racine=pere->racine;

    if(addNodeToFilsOfNode(pere,n)){
        return n;
    }
    removeNode(n);
    return NULL;
}


/**
 * Cette fonction permet de créer un dossier vierge, sans père ni fils
 * @param nom Le nom du dossier
 * @return Le noeud du dossier
*/
noeud *initDossierSimple(char *nom){
    noeud *dossier = initNode(nom);
    dossier->est_dossier = true;
    return dossier;
}

/**
 * Cette fonction crée un nouveau dossier dans le noeud "p"
 * @param pere Le dossier dans lequel on veut créer ce dossier (le père)
 * @param nom Le nom du dossier que l'on veut créer
 * @return Le noeud du nouveau dossier
*/
noeud *initDossier(noeud *pere,char *nom){
    if(verifierNomDejaExistant(pere,nom)){
        puts("Nom de Dossier deja Existant");
        return NULL;
    }
    noeud *dossier = initDossierSimple(nom);
    
    dossier->racine=pere->racine;
    dossier->pere=pere;


    //On ajoute ce noeud dans les fils du noeud courant
    if (addNodeToFilsOfNode(pere,dossier)){
        return dossier;
    }

    removeNode(dossier);
    return NULL;
}

/**
 * Cette fonction renvoie le dossier qui porte le nom chem dans le noeud "p"
 * @param courant Le dossier dans lequel on veut chercher le chemin
 * @param nom Le nom du dossier dans lequel on veut y aller
 * @return Le noeud du dossier portant le nom nom
*/

noeud *allerVers(noeud *courant,char *chem){

    liste_noeud *tmp = courant->fils;
    while(tmp !=NULL){
        noeud *p = tmp->no;
        if(p->est_dossier && strcmp(chem,p->nom)==0){
            return p;
        }
        tmp=tmp->succ;
    }
    return NULL;
}

/**
 * Fonction qui affiche tout le chemin depuis la racine jusqu'au noeud (utile pour "pwd")
 * @param courant Le noeud en question
*/
void afficheCheminVersRacine(noeud *courant){
    if(strcmp(courant->nom,courant->racine->nom)==0){
        printf("%s/",courant->nom);
    }
    else{
        afficheCheminVersRacine(courant->pere);
        printf("%s/",courant->nom);
    }
}

/**
 * Cette fonction renvoie le noeud fil du noeud courant pourtant le nom nom;
 * @param courant Le dossier dans lequel on veut chercher le fils
 * @param nom le nom du noeud qu'on cherche
 * @return le dossier portant le nom, sinon NULL si il existe pas;
*/
noeud *getFils(noeud *courant, char *nom){
    liste_noeud *fils = courant->fils;

    while(fils != NULL){
        if(strcmp(fils->no->nom,nom)==0){
            return fils->no;
        }
        fils = fils->succ;
    }

    return NULL;
}

/**
 * Cette fonction vérifier si le noeud pere est dans les parents de courant
 * @warning pere doit être un dossier
 * @param courant est le noeud courant
 * @param pere est le noeud dont on veut vérifier qu'il est dans les pere de courant
 * @return un boolean 
*/
bool estParent(noeud *courant,noeud *pere){
    if (courant->racine==courant && courant != pere){
        return false;
    }
    if(courant == pere){
        return true;
    }
    else{
        return estParent(courant->pere,pere);
    }
}

/**
 * Méthode auxiliaire privée, pour afficher l'arborescence
 * @param node L'arborescence à afficher
 * @param profondeur La profondeur actuelle du noeud
 * @param endOfList Bool qui permet de savoir si on est dans une fin de liste ou pas (utile pour un bon printage)
*/
void tree_rec(noeud *node, unsigned int profondeur, bool endOfList){
    
    // │   └   ├   ─

    puts("");

    for (size_t i = 0; i < profondeur; i++){
        printf("│");
    }

    if(node->est_dossier){
        if(strcmp(node->nom,"") == 0){
            if(endOfList){
                printf("└─ /");
            }
            else{
                printf("├─ /");
            }
        }
        else{
            if(endOfList && node->fils == NULL){
                printf("└─ %s/",node->nom);
            }
            else{
                printf("├─ %s/",node->nom);
            }
        }
    }
    else{
        if(endOfList){
            printf("└─ %s",node->nom);
        }
        else{
            printf("├─ %s",node->nom);
        }
    }

    liste_noeud *tmp = node->fils;
    while (tmp != NULL){
        if(tmp->succ != NULL){
            tree_rec(tmp->no,profondeur+1,false);   
        }
        else{
            tree_rec(tmp->no,profondeur+1,true);
        }
        tmp = tmp->succ;
    }
    
}

/**
 * Méthode pour afficher l'arborescence
 * @param node L'arborescence à afficher
*/
void tree(noeud *node){
    if(node->fils == NULL){
        tree_rec(node,0,true);
    }
    else{
        tree_rec(node,0,false);
    }
    puts("");
}