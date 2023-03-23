#ifndef STUDENT_H_INCLUDED
#define STUDENT_H_INCLUDED

    #include "TOVC.h"


    /* --[ DECLARATIONS DES CONSTANTES ]-- */

    #define MAX_NOTES 9 // le nombre max des matieres etudiees dans toutes les annees
    #define TAILLE_NOTE 4 // la taille d'une seul note (ex: 12,75 => 1275)

    #define TAILLE_CHAMP_TAILLE 3 // seulement la taille des champs variables (taille du nom + 1[#] + taille du prenom)
    #define TAILLE_CHAMP_CLE 4 // la taille du cle (id)
    #define TAILLE_CHAMP_EFF 1 // un caractere ('F': faux, 'V': vrai)
    #define TAILLE_CHAMP_CLASSID 3 // un caractere pour l'annee (le niveau) et 2 caracteres pour le numero du salle
    #define TAILLE_CHAMP_GENRE 1 // un caractere ('M': masculin/male, 'F': feminin/female)
    #define TAILLE_CHAMPS_STUDENT_FIXES 12 // la somme des tailles des champs precedentes

    #define NBR_IDS 10000 // nombre des ids (ils doivent tous etre representer sur 'TAILLE_CHAMP_CLE' caracteres)
    #define NBR_NOMS 703 // nombre des noms dans le fichier des noms
    #define NBR_PRENOMS 365 // nombre des prenoms dans le fichier des prenoms et genres

    #define NBR_CLASSES_PAR_ANNEE 4 // nombre de classe dans l'ecole pour chaque annee
    #define NBR_ELEVES_MAX_PAR_CLASSE 25 // nombre moyen des etudiants par classe

    #define NBR_MATIERES_ANNEE_P 4 // le nombre des matieres etudiees dans l'annee preparative
    #define NBR_MATIERES_ANNEE_1 6 // le nombre des matieres etudiees dans l'annee 1
    #define NBR_MATIERES_ANNEE_2 6 // le nombre des matieres etudiees dans l'annee 2
    #define NBR_MATIERES_ANNEE_3 9 // le nombre des matieres etudiees dans l'annee 3
    #define NBR_MATIERES_ANNEE_4 9 // le nombre des matieres etudiees dans l'annee 4
    #define NBR_MATIERES_ANNEE_5 9 // le nombre des matieres etudiees dans l'annee 5

    // variable globale qui contient les noms des matieres etudiees dans chaque annee selon l'ordre alphabetique
    char *MATIERES[6/*numero de l'annee ('P'<=>0)*/][MAX_NOTES/*indice de matiere*/];

    // variables globale pour garder tous les IDs possibles et separer les IDs utilises
    char IDS_ARRAY[NBR_IDS][TAILLE_CHAMP_CLE + 1];
    int ID_INDEX;


    /* --[ DECLARATIONS DES STRUCTURES ]-- */

    typedef struct
    {
        char annee;
        int numSalle;
    } ClassId;

    typedef struct
    {
        char eff;
        char id[TAILLE_CHAMP_CLE + 1];
        ClassId classId;
        char *nom;
        char *prenom;
        char genre;
        float notes[MAX_NOTES];
    } Student;

    // taille, cle, eff, classId, notes, genre, nom, prenom
    // exemple: "0120001f4030800090007250005167520001975100014001550mmelzi#mounir" + '\0'
    typedef char *StudentStr;


    /* --[ DECLARATIONS DES FONCTIONS ]-- */

    Student create_student(char id[], char nom[], char prenom[], char genre, char annee, int numSalle);
    void free_student_fields(Student *e); // liberer les champs nom et prenom d'un etudiant
    void afficher_student(Student *e);

    void Student_to_StudentStr(Student *e, StudentStr *e_str); // convertir un etudiant en une chaine de caracteres
    void StudentStr_to_Student(StudentStr e_str, Student *e); // convertir une chaine de caracteres en un etudiant

    int comparer_student(Student *e1, Student *e2); // comparer entre 2 etudiants selon la classe, le nom et le prenom
    int taille_var_student(Student *e); // calculer la taille variable d'un etudiant
    int nombre_matieres_annee(char annee); // calculer le nombre des matieres etudiees dans une annee
    int taille_champ_notes_student(Student *e); // calculer la taille du champ notes (tableau des notes)
    float moyenne_student(Student *e); // calculer la moyenne d'un etudiant
    float generate_note(char annee); // generer une note aleatoirement selon le niveau

    void lire_student(TOVC *f, Buffer *buf, int *i, int *j, Student *e); // lire un etudiant depuis un fichier TOVC
    void ecrire_student(TOVC *f, Buffer *buf, int *i, int *j, Student *e); // ecrire un etudiant dans un fichier TOVC

    void save_ids_array(char nomf[]); // sauvegarder la table des IDs dans un fichier
    void load_ids_array(char nomf[]); // charger la table des IDs depuis un fichier

    Student copy_student(Student e); // creer une copie d'un etudiant
    Student get_student(int scan_id); // lire un etudiant depuis l'utilisateur

#endif // STUDENT_H_INCLUDED
