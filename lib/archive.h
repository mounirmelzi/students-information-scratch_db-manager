#ifndef ARCHIVE_H_INCLUDED
#define ARCHIVE_H_INCLUDED

    #include <stdbool.h>
    #include "TOVC.h"
    #include "index.h"


    /* --[ DECLARATIONS DES CONSTANTES ]-- */

    #define TAILLE_CHAMP_MOYENNE 4 // la taille de champ moyenne (ex: 14,87 => 1487)
    #define TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES (TAILLE_CHAMPS_STUDENT_FIXES+TAILLE_CHAMP_MOYENNE) // la taille des champs fixes


    /* --[ DECLARATIONS DES STRUCTURES ]-- */

    typedef struct
    {
        char eff;
        char id[TAILLE_CHAMP_CLE + 1];
        ClassId classId;
        char *nom;
        char *prenom;
        char genre;
        float moyenne;
    } ArchivedStudent;

    // taille, cle, eff, classId, moyenne, genre, nom, prenom
    typedef char *ArchivedStudentStr;


    /* --[ DECLARATIONS DES FONCTIONS ]-- */

    ArchivedStudent create_archived_student(char id[], char nom[], char prenom[], char genre, char annee, int numSalle, float moyenne);
    void free_archived_student_fields(ArchivedStudent *e); // liberer les champs nom et prenom d'un etudiant
    void afficher_archived_student(ArchivedStudent *e);

    void ArchivedStudent_to_ArchivedStudentStr(ArchivedStudent *e, ArchivedStudentStr *e_str); // convertir un etudiant en une chaine de caracteres
    void ArchivedStudentStr_to_ArchivedStudent(ArchivedStudentStr e_str, ArchivedStudent *e); // convertir une chaine de caracteres en un etudiant
    void Student_to_ArchivedStudent(Student *e1, ArchivedStudent *e2); // convertir un Student en un ArchivedStudent en calculant sa moyenne

    int comparer_archived_student(ArchivedStudent *e1, ArchivedStudent *e2); // comparer entre 2 etudiants selon la classe et la moyenne
    int taille_var_archived_student(ArchivedStudent *e); // calculer la taille variable d'un etudiant

    void lire_archived_student(TOVC *f, Buffer *buf, int *i, int *j, ArchivedStudent *e); // lire un etudiant depuis un fichier TOVC
    void ecrire_archived_student(TOVC *f, Buffer *buf, int *i, int *j, ArchivedStudent *e); // ecrire un etudiant dans un fichier TOVC

    ArchivedStudent copy_archived_student(ArchivedStudent e); // creer une copie d'un etudiant
    ArchivedStudent get_archived_student(int scan_id); // lire un etudiant depuis l'utilisateur

#endif // ARCHIVE_H_INCLUDED
