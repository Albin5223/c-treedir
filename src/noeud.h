#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>


struct noeud;
struct liste_noeud;


struct noeud {
    bool est_dossier;
    char nom [100];
    struct noeud *pere;
    struct noeud *racine;
    struct liste_noeud *fils;
};
typedef struct noeud noeud;

struct liste_noeud {
    struct noeud *no;
    struct liste_noeud *succ;
};
typedef struct liste_noeud liste_noeud;

/**
 * Cette fonction permet d'initialiser un noeud
 * @warning Ses attributs sont initialisés à false et NULL partout (à part le nom), il faut donc les changer en fonction du contexte
 * @return Un noeud par défaut
*/
noeud *initNode(char *c){
    noeud *node = malloc(sizeof(noeud));
    assert(node != NULL);
    node->est_dossier = false;
    node->nom[0] = c;
    node->pere = NULL;
    node->racine = NULL;
    node->fils = NULL;  
}

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
 * Cette fonction permet d'ajouter dans une liste un noeud, dans l'ordre alphabétique + trier par type (dossier/non-dossier)
 * PS : Les dossiers seront donc toujours avant les non-dossiers, et dans chacun des deux blocs, il y aura un tri alphabetique
 * @param list La liste dans laquelle on veut ajouter le noeud
 * @param noeud Le noeud à ajouter
 * @return La nouvelle tete de la liste (au cas ou elle a été changée)
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

    if(!node->est_dossier){                 // CAS 1 : Si le noeud est un dossier, on veut l'ajouter dans la première partie avec les dossiers      
        while(tmp != NULL && tmp->no->est_dossier){
            if(node->nom < tmp->no->nom){       // On trouve l'emplacement
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
            if(tmp == NULL){    // CAS 1.3.1.1 : Il n'y avait pas de non-dossier non plus
                return newList;
            }
            else{               // CAS 1.2.1.2 : Il y avait au moins 1 non-dossier
                newList->succ = tmp;
                return newList;
            }
        }
        // CAS 1.3.2 : Il y avait au moins un dossier derrière
        prev->succ = newList;
        newList->succ = tmp;
        return list;
    }
    else{                               // CAS 2 : Si le noeud qu'on veut ajouter n'est pas un dossier, on veut donc l'ajouter dans la seconde partie avec les non-dossier
        while (tmp != NULL && tmp->no->est_dossier){    // On traverse tous les dossiers
            prev = tmp;
            tmp = tmp->succ;
        }

        if(tmp == NULL){        // CAS 2.1 : Il n'y a pas de non-dossiers
            if(prev == NULL){       // CAS 2.1.1 : Il n'y a pas de dossier non plus
                return newList;
            }
            else{                   // CAS 2.1.2 : Il y a des dossiers
                prev->succ = newList;
                return list;
            }
        }
        else{                   // CAS 2.2 : Il y a des non-dossiers
            while (tmp != NULL){
                if(node->nom < tmp->no->nom){   // On trouve l'emplacement
                    if(prev == NULL){           // CAS 2.2.1 : Il n'y a pas de dossier derriere et le noeud doit être placé au début
                        newList->succ = list;
                        return newList;
                    }
                    else{               // CAS 2.2.2 : Il y a des dossiers ou non-dossiers derriere
                        prev->succ = newList;
                        newList->succ = tmp;
                        return list;
                    }
                }
                prev = tmp;
                tmp = tmp->succ;
            }
            // CAS 2.3 : Le non-dossier doit être placé à la fin
            prev->succ = newList;
            return list;
        }
    }
}

/**
 * Permet d'afficher tout le contenu du noeud
 * @param node noeud à afficher
*/
void print(noeud *node){
    printf("Noeud ");printNameWithType(node);printf(", ");

    printf("pere : %s, ",node->pere->nom);

    unsigned int tfils = sizeOfFils(node);
    printf("%u fils : ",tfils);

    liste_noeud *tmp = node->fils;
    while (tmp != NULL){
        printNameWithType(tmp->no);printf(", ");
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
    printf("%s ",node->nom);
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

void afficheNom(noeud *p){
    printf("%s \n",p->nom);
}

noeud *retourRacine(noeud *p){
    return p->racine;
}

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
    int trouve = strcmp(nom,p->nom);
    if(trouve == 0){
        return true;
    }
    liste_noeud *tmp = p->fils;
    while (tmp!=NULL){
        char *s1 = tmp->no->nom;
        trouve = strcmp(nom,s1);
        if(trouve == 0){
            return true;
        }
    }
    return false;
}

noeud *initFichier(noeud *pere, char *nom){
    if(!verifierNomDejaExistant(pere,nom)){
        puts("Nom de Fichier deja Existant");
        return NULL;
    }
    noeud *n = initNode(nom);
    n->est_dossier=false;
    n->pere=pere;
    n->racine=pere->racine;
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
    if(!verifierNomDejaExistant(pere,nom)){
        puts("Nom de Dossier deja Existant");
        return NULL;
    }
    noeud *dossier = initDossierSimple(nom);
    
    dossier->racine=pere->racine;
    dossier->pere=pere;


    //On ajoute ce noeud dans les fils du noeud courant
    liste_noeud *m = malloc(sizeof(liste_noeud));
    m->no=dossier;
    if(pere->fils==NULL){
        pere->fils=m;
    }
    else{
        liste_noeud *tmp = pere->fils;
        while (tmp->succ!=NULL){  
            tmp = tmp->succ;
        }

        tmp->succ=m;
    }

    pere->fils=addToListAlpha(pere->fils,dossier);
    
    return dossier;
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
        if(p->est_dossier && strcmp(chem,p->nom)){
            return p;
        }
        tmp=tmp->succ;
    }
    return NULL;
}


void afficheCheminVersRacine(noeud *courant){
    if(courant->pere==courant){
        printf("%s/",courant->nom);
    }
    else{
        afficheCheminVersRacine(courant->pere);
        printf("%s/",courant->nom);
    }
}


