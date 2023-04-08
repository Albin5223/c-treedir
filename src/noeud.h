#include <stdbool.h>

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

liste_noeud *initList(noeud *tete);
unsigned int size(liste_noeud *list);
liste_noeud *addToListAlpha(liste_noeud *list, noeud *node);


noeud *initNode(char *c);
unsigned int sizeOfFils(noeud *node);
bool addNodeToFilsOfNode(noeud *pere, noeud *fils);
void freeRecInNode(noeud *node);
void removeNode(noeud *node);
bool moveNode(noeud *nomade, noeud *newPere);
noeud *copyNode(noeud *node);
void print(noeud *node);
void printNameWithType(noeud *node);
void afficheFils(noeud *p);
void afficheNom(noeud *p);
noeud *retourRacine(noeud *p);
noeud *retourPere(noeud *p);
bool verifierNomDejaExistant(noeud *p, char *nom);
noeud *initFichier(noeud *pere, char *nom);
noeud *initDossierSimple(char *nom);
noeud *initDossier(noeud *pere,char *nom);
noeud *allerVers(noeud *courant,char *chem);
void afficheCheminVersRacine(noeud *courant);
noeud *getFils(noeud *courant, char *nom);
bool estParent(noeud *courant, noeud *pere);