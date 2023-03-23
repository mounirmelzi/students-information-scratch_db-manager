#ifndef TOVC_H_INCLUDED
#define TOVC_H_INCLUDED


    /* --[ DECLARATIONS DES CONSTANTES ]-- */

    #define MAX_CHARS 512 // le nombre des caracteres dans un bloc
    #define MAX_TAUX_CHARGEMENT 0.7 // le taux de chargement qui provoque le lancement de la reorganization du fichier TOVC

    // des constantes pour les numeros des entetes du fichier TOVC
    #define ENTETE_NUMERO_DERNIER_BLOC 1
    #define ENTETE_POS_LIBRE 2
    #define ENTETE_NBR_ENREG 3
    #define ENTETE_NBR_CHAR_INS 4
    #define ENTETE_NBR_CHAR_SUP 5


    /* --[ DECLARATIONS DES STRUCTURES ]-- */

    typedef struct
    {
        char tab[MAX_CHARS + 1];
    } Tbloc, Buffer;

    typedef struct
    {
        int numeroDernierBloc;
        int posLibre;
        int nbrEnreg;
        int nbrCharIns;
        int nbrCharSupp;
    } Entete;

    typedef struct
    {
        Entete entete;
        FILE *fichier;
    } TOVC;


    char annee_courante[10]; // chaine de caracteres globale pour garder l'annee courante


    /* --[ DECLARATIONS DES FONCTIONS ]-- */

    // les fonctions de la machine abstraites pour un fichier TOVC: [entete, aff_entete, lire_dir, ecrire_dir, ouvrir, fermer, alloc_bloc]
    int entete(TOVC *f, int caracteristiqueNum);
    void aff_entete(TOVC *f, int caracteristiqueNum, int valeur);
    void lire_dir(TOVC *f, int blocNum, Buffer *buf);
    void ecrire_dir(TOVC *f, int blocNum, Buffer *buf);
    void ouvrir(TOVC *f, char nomf[], char mode);
    void fermer(TOVC *f);
    int alloc_bloc(TOVC *f);

    void lire_chaine(TOVC *f, Buffer *buf, int *i, int *j, int taille, char *chaine[]); // lire une chaine de caracteres a partir du fichier
    void ecrire_chaine(TOVC *f, Buffer *buf, int *i, int *j, int taille, char chaine[]); // ecrire une chaine de caracteres dans du fichier
    double taux_chargement(TOVC *f); // calculer le taux de chargement d'un fichier

    void inserer_chaine(char nomf[], int i, int j, int taille, char chaine[]); // inserer une chaine dans un fichier (avec decalages jusqu'a la fin du fichier)

    void afficher_entete(TOVC *f); // afficher l'entete d'un fichier
    void afficher_bloc(TOVC *f, int i); // afficher le contenu d'un bloc dans le fichier
    void afficher_fichier(TOVC *f); // afficher le contenu de tous le fichier


#endif // TOVC_H_INCLUDED

