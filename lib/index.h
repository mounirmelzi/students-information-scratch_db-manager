#ifndef INDEX_H_INCLUDED
#define INDEX_H_INCLUDED

    #include <stdbool.h>
    #include "student.h"


    /* --[ DECLARATIONS DES CONSTANTES ]-- */

    #define MAX_INDEX NBR_IDS // la taille de la table d'index
    #define MAX_INDEX_TOF 4 // la taille du buffer de fichier TOF qui contient la table d'index


    /* --[ DECLARATIONS DES STRUCTURES ]-- */

    typedef struct
    {
        int numBloc; // i
        int pos; // j
        ClassId classId; // cle
        float moyenne_min;
        float moyenne_max;
    } IndexData;

    typedef struct
    {
        IndexData tab[MAX_INDEX];
        int taille;
    } IndexTable;

    typedef struct
    {
        IndexData tab[MAX_INDEX_TOF];
        int nb;
    } Tbloc_TOF, Buffer_TOF;

    typedef struct
    {
        int NumeroDernierBloc;
        int NombreEnreg;
    } Entete_TOF;

    typedef struct
    {
        Entete_TOF entete;
        FILE *fichier;
    } TOF;


    IndexTable GIndex; // table d'index globale
    bool GIndexExist; // bool qui indique si la table d'index globale existe


    /* --[ DECLARATIONS DES FONCTIONS ]-- */

    // les fonctions de la machine abstraites pour un fichier TOF: [entete, aff_entete, lire_dir, ecrire_dir, ouvrir, fermer, alloc_bloc]
    int entete_TOF(TOF *f, int caracteristiqueNum);
    void aff_entete_TOF(TOF *f, int caracteristiqueNum, int valeur);
    void lire_dir_TOF(TOF *f, int blocNum, Buffer_TOF *buf);
    void ecrire_dir_TOF(TOF *f, int blocNum, Buffer_TOF *buf);
    void ouvrir_TOF(TOF *f, char nomf[], char mode);
    void fermer_TOF(TOF *f);
    int alloc_bloc_TOF(TOF *f);

    IndexTable *allouer_index(); // allouer un espace memoire pour contenir la table d'index
    void free_index(IndexTable **tabInd); // liberer l'espace memoire allouer pour la table d'index

    void afficher_index(IndexTable *tabInd); // afficher une table d'index

    void sauvegarder_index(char nomf[], IndexTable *tabInd); // sauvegarder la table d'index dans un fichier TOF
    void chargement_index(char nomf[], IndexTable *tabInd); // charger la table d'index depuis un fichier TOF

#endif // INDEX_H_INCLUDED
