#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>


struct noeud ;
struct liste_noeud ;


struct noeud {
    bool est_dossier ;
    char nom [100];
    struct noeud * pere ;
    struct noeud * racine ;
    struct liste_noeud * fils ;
};
typedef struct noeud noeud ;

struct liste_noeud {
    struct noeud * no ;
    struct liste_noeud * succ ;
};
typedef struct liste_noeud liste_noeud ;



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
    noeud *tmp = p;
    while(tmp->racine!=tmp){
        tmp=tmp->pere;
    }

    return tmp;
}

//Cett fonction verifie si le nom existe deja dans les fils du noeud courant
bool verifierNomDejaExistant(noeud *p, char *nom){
    int trouve = strcmp(nom,p->nom);
    if(trouve == 0){
        return false;
    }
    liste_noeud *tmp = p->fils;
    while (tmp!=NULL){
        char *s1 = tmp->no->nom;
        trouve = strcmp(nom,s1);
        if(trouve == 0){
            return false;
        }
    }
    return true;
}


//Cette fonction crée un nouveau dossier à partir du noeud mis en argument, il s'agira par la suite du noeud courant
//Elle renvoie le noeud du nouveau dossier
noeud *creerDossier(noeud *p,char *nom){
    //On créer une case pour un nouveau noeud
    noeud *nouveau = malloc(sizeof(noeud));
    if (nouveau ==NULL || !verifierNomDejaExistant(p,nom)){
        return NULL;
    }
    nouveau->est_dossier=true;
    nouveau->fils=NULL;
    nouveau->racine=p->racine;
    nouveau->pere=p;

    //On ajoute ce noeud dans les fils du noeud courant
    liste_noeud *m = malloc(sizeof(liste_noeud));
    m->no=nouveau;
    if(p->fils==NULL){
        p->fils=m;
    }
    else{
        liste_noeud *tmp = p->fils;
        while (tmp->succ!=NULL){  
            tmp = tmp->succ;
        }

        tmp->succ=m;
    }
    
    return nouveau;



}
