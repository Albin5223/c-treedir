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

extern liste_noeud *initList(noeud *tete);
extern unsigned int size(liste_noeud *list);
extern liste_noeud *addToListAlpha(liste_noeud *list, noeud *node);


extern noeud *initNode(char *c);
extern unsigned int sizeOfFils(noeud *node);
extern bool addNodeToFilsOfNode(noeud *pere, noeud *fils);
extern void freeRecInNode(noeud *node);
extern void removeNode(noeud *node);
extern bool moveNode(noeud *nomade, noeud *newPere);
extern noeud *copyNode(noeud *node);
extern void print(noeud *node);
extern void printNameWithType(noeud *node);
extern void afficheFils(noeud *p);
extern void afficheNom(noeud *p);
extern noeud *retourRacine(noeud *p);
extern noeud *retourPere(noeud *p);
extern bool verifierNomDejaExistant(noeud *p, char *nom);
extern noeud *initFichier(noeud *pere, char *nom);
extern noeud *initDossierSimple(char *nom);
extern noeud *initDossier(noeud *pere,char *nom);
extern noeud *allerVers(noeud *courant,char *chem);
extern void afficheCheminVersRacine(noeud *courant);
extern noeud *getFils(noeud *courant, char *nom);
extern bool estParent(noeud *courant, noeud *pere);
extern void renameNode(noeud *courant, char* nouveau);
extern void tree(noeud *node);