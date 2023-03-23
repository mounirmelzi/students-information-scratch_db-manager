#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

    #include <stdbool.h>
    #include "student.h"


    /* --[ DECLARATIONS DES CONSTANTES ]-- */

    // des constante pour les flags de la fonction "fileFullPath"
    #define FILE_ANNEE_COURANTE 1
    #define FILE_ARCHIVE 2
    #define FILE_INDEX 3
    #define FILE_IDS 4


    /* --[ DECLARATIONS DES FONCTIONS ]-- */

    int nbPos(int n); // calculer le nombre de positions (digits) dans un entier
    void intToStr(int n, char chaine[], int taille); // convertir un entier vers une chaine de caracteres
    void strToInt(const char chaine[], int *n); // convertir une chaine de caracteres vers un entier
    void subStr(const char src[], char *dist, int startIndex, int charNum); // extraire une sous-chaine d'une chaine de caracteres
    int random_int(int min_val, int max_val); // generer un nombre aleatoire entre min_val et max_val (inclus)
    int power(int n, int p); // calculer la puissance
    void deleteFolderFiles(char path_to_folder[]); // supprimer tous les fichiers dans un repertoire
    bool isFileExiste(char path[]); // verifier si un fichier existe ou non
    void update_annee_courante(int value, bool isIncrement); // mise a jour de la variable globale annee_courante
    void fileFullPath(char annee_scolaire[], int flag, char path[]); // generer le chemin vers un fichier
    void randomize_ids(); // remplir la table des IDs aleatoirement
    void get_annee_courante(char year[]); // lire une annee scolaire depuis l'utilisateur
    void print_annee_courante(); // afficher une annee scolaire a l'ecran
    void delete_useless_files(char year[]); // supprimer les fichiers qui ne sont pas necessaire au debut du programme
    void permute_ids_in_array(char id1[], char id2[]); // permuter entre 2 IDs dans la table des IDs globale

#endif // UTILS_H_INCLUDED
