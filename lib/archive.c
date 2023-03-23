#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "TOVC.h"
#include "student.h"
#include "archive.h"
#include "guiTools.h"
#include "conio2.h"


ArchivedStudent create_archived_student(char id[], char nom[], char prenom[], char genre, char annee, int numSalle, float moyenne)
{
    ArchivedStudent e;

    e.eff = 'F';
    e.genre = toupper(genre);
    e.classId.annee = toupper(annee);
    e.classId.numSalle = numSalle;
    strcpy(e.id, id);
    e.moyenne = moyenne;

    e.nom = calloc(strlen(nom) + 1, sizeof(char));
    strcpy(e.nom, nom);

    e.prenom = calloc(strlen(prenom) + 1, sizeof(char));
    strcpy(e.prenom, prenom);

    return e;
}

/**
 * @brief liberer les champs nom et prenom d'un etudiant
 *
 * @param e
 */
void free_archived_student_fields(ArchivedStudent *e)
{
    free(e->nom);
    e->nom = NULL;
    free(e->prenom);
    e->prenom = NULL;
}

void afficher_archived_student(ArchivedStudent *e)
{
    printf("\t+-------------------------+\n");
    printf("\t| ID                      : %s\n", e->id);
    printf("\t| Nom                     : %s\n", e->nom);
    printf("\t| Prenom                  : %s\n", e->prenom);
    printf("\t| Genre                   : %s\n", ((e->genre == 'M') || (e->genre == 'm')) ? "Masculin" : "Feminin");
    printf("\t| Efface?                 : %s\n", ((e->eff == 'F') || (e->eff == 'f')) ? "NON" : "OUI");
    printf("\t| Niveau                  : %c\n", e->classId.annee);
    printf("\t| Numero du salle         : Salle %d\n", e->classId.numSalle);
    printf("\t| Moyenne                 : %.2f\n", e->moyenne);
    printf("\t+-------------------------+\n");
}


/**
 * @brief convertir un etudiant en une chaine de caracteres
 *
 * @param e
 * @param e_str
 */
void ArchivedStudent_to_ArchivedStudentStr(ArchivedStudent *e, ArchivedStudentStr *e_str)
{
    int pos = 0;
    int taille_var = taille_var_archived_student(e);
    *e_str = calloc(TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES + taille_var + 1, sizeof(char));
    ArchivedStudentStr temp_str = calloc(TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES + taille_var + 1, sizeof(char));

    // ecrire le champ taille dans la chaine dist
    intToStr(taille_var, temp_str, TAILLE_CHAMP_TAILLE);
    subStr(temp_str, (*e_str) + pos, 0, TAILLE_CHAMP_TAILLE);
    pos += TAILLE_CHAMP_TAILLE;

    // ecrire le champ cle dans la chaine dist
    subStr(e->id, (*e_str) + pos, 0, TAILLE_CHAMP_CLE);
    pos += TAILLE_CHAMP_CLE;

    // ecrire le champ eff dans la chaine dist
    (*e_str)[pos] = e->eff;
    pos += TAILLE_CHAMP_EFF;

    // ecrire le champ classId dans la chaine dist
    temp_str[0] = e->classId.annee;
    intToStr(e->classId.numSalle, temp_str + 1, TAILLE_CHAMP_CLASSID - 1);
    subStr(temp_str, (*e_str) + pos, 0, TAILLE_CHAMP_CLASSID);
    pos += TAILLE_CHAMP_CLASSID;

    // ecrire le champ moyenne dans la chaine dist
    int partieEntiere = e->moyenne / 1;
    int partieFractionnaire = (e->moyenne - (float)partieEntiere) * 100;
    intToStr(partieEntiere, temp_str, TAILLE_CHAMP_MOYENNE/2);
    intToStr(partieFractionnaire, temp_str + TAILLE_CHAMP_MOYENNE/2, TAILLE_CHAMP_MOYENNE/2);
    subStr(temp_str, (*e_str) + pos, 0, TAILLE_CHAMP_MOYENNE);
    pos += TAILLE_CHAMP_MOYENNE;

    // ecrire le champ genre dans la chaine dist
    (*e_str)[pos] = e->genre;
    pos += TAILLE_CHAMP_GENRE;

    // ecrire le champ nom#prenom dans la chaine dist
    char *temp = calloc((taille_var + 1), sizeof(char));
    sprintf(temp, "%s#%s", e->nom, e->prenom);
    subStr(temp, (*e_str) + pos, 0, taille_var);
    pos += taille_var;
    free(temp);

    // ajouter le caractere de la fin de la chaine
    (*e_str)[pos] = '\0';

    free(temp_str);
}

/**
 * @brief convertir une chaine de caracteres en un etudiant
 *
 * @param e_str
 * @param e
 */
void ArchivedStudentStr_to_ArchivedStudent(ArchivedStudentStr e_str, ArchivedStudent *e)
{
    int pos = 0;
    char temp_str[TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES + 1];
    char str_note[TAILLE_CHAMP_MOYENNE + 1];
    char taille_var_str[TAILLE_CHAMP_TAILLE + 1];
    int taille_var_int = 0;

    subStr(e_str, taille_var_str, 0, TAILLE_CHAMP_TAILLE);
    taille_var_str[TAILLE_CHAMP_TAILLE] = '\0';
    strToInt(taille_var_str, &taille_var_int);
    pos += TAILLE_CHAMP_TAILLE;

    // ecrire le champ id de l'enregistrement e a partir de e_str
    subStr(e_str, e->id, pos, TAILLE_CHAMP_CLE);
    e->id[TAILLE_CHAMP_CLE] = '\0';
    pos += TAILLE_CHAMP_CLE;

    // ecrire le champ eff de l'enregistrement e a partir de e_str
    e->eff = e_str[pos];
    pos += TAILLE_CHAMP_EFF;

    // ecrire le champ classId de l'enregistrement e a partir de e_str
    e->classId.annee = e_str[pos] ;
    subStr(e_str, temp_str, pos + 1, TAILLE_CHAMP_CLASSID - 1) ;
    temp_str[TAILLE_CHAMP_CLASSID - 1] = '\0';
    strToInt(temp_str, &(e->classId.numSalle));
    pos += TAILLE_CHAMP_CLASSID;

    // ecrire le champ moyenne de l'enregistrement e a partir de e_str
    int partieEnt, partieFrac;

    subStr(e_str, temp_str, pos, TAILLE_CHAMP_MOYENNE);
    temp_str[TAILLE_CHAMP_MOYENNE] = '\0';

    subStr(temp_str, str_note, 0, TAILLE_CHAMP_MOYENNE/2);
    str_note[TAILLE_CHAMP_MOYENNE/2] = '\0';
    strToInt(str_note, &partieEnt);
    e->moyenne = partieEnt;

    subStr(temp_str, str_note, TAILLE_CHAMP_MOYENNE/2, TAILLE_CHAMP_MOYENNE/2);
    str_note[TAILLE_CHAMP_MOYENNE/2] = '\0';
    strToInt(str_note, &partieFrac);
    e->moyenne += partieFrac * 0.01;

    pos += TAILLE_CHAMP_MOYENNE;

    // ecrire le champ genre de l'enregistrement e a partir de e_str
    e->genre = e_str[pos];
    pos += TAILLE_CHAMP_GENRE;

    // ecrire le champ nom et prenom de l'enregistrement e a partir de e_str
    char *nom = calloc(taille_var_int + 1, sizeof(char));
    char *prenom = calloc(taille_var_int + 1, sizeof(char));
    sscanf(e_str + pos, "%[0-9a-zA-Z ]#%[0-9a-zA-Z ]", nom, prenom);
    e->nom = calloc(strlen(nom) + 1, sizeof(char));
    e->prenom = calloc(strlen(prenom) + 1, sizeof(char));
    strcpy(e->nom, nom);
    strcpy(e->prenom, prenom);
    free(nom);
    free(prenom);
}

/**
 * @brief convertir un Student en un ArchivedStudent en calculant sa moyenne
 *
 * @param e1
 * @param e2
 */
void Student_to_ArchivedStudent(Student *e1, ArchivedStudent *e2)
{
    e2->classId = e1->classId;
    e2->eff = e1->eff;
    e2->genre = e1->genre;
    strcpy(e2->id, e1->id);
    e2->moyenne = moyenne_student(e1);
    e2->nom = calloc(strlen(e1->nom) + 1, sizeof(char));
    e2->prenom = calloc(strlen(e1->prenom) + 1, sizeof(char));
    strcpy(e2->nom, e1->nom);
    strcpy(e2->prenom, e1->prenom);
}


/**
 * @brief comparer entre 2 etudiants selon la classe et la moyenne
 *
 * @param e1
 * @param e2
 * @return int
 */
int comparer_archived_student(ArchivedStudent *e1, ArchivedStudent *e2)
{
    // e1 > e2: 1
    // e1 < e2: -1
    // e1 = e2: 0

    if (toupper(e1->classId.annee) == 'P' && toupper(e2->classId.annee) != 'P')
        return -1;
    if (toupper(e1->classId.annee) != 'P' && toupper(e2->classId.annee) == 'P')
        return 1;
    if (e1->classId.annee != e2->classId.annee)
        return (((e1->classId.annee - e2->classId.annee) < 0) ? -1: 1);

    if (e1->classId.numSalle != e2->classId.numSalle)
        return (((e1->classId.numSalle - e2->classId.numSalle) < 0) ? -1: 1);

    if (e1->moyenne < e2->moyenne)
        return -1;
    else if (e1->moyenne > e2->moyenne)
        return 1;

    return 0;
}

/**
 * @brief calculer la taille variable d'un etudiant
 *
 * @param e
 * @return int
 */
int taille_var_archived_student(ArchivedStudent *e)
{
    return strlen(e->nom) + strlen(e->prenom) + 1;
}


/**
 * @brief lire un etudiant depuis un fichier TOVC
 *
 * @param f
 * @param buf
 * @param i
 * @param j
 * @param e
 */
void lire_archived_student(TOVC *f, Buffer *buf, int *i, int *j, ArchivedStudent *e)
{
    // declaration des variable
    char *taille_var_str;
    int taille_var_int;
    char *semi_ArchivedStudentStr;
    ArchivedStudentStr s;

    // lire le champ taille variable
    lire_chaine(f, buf, i, j, TAILLE_CHAMP_TAILLE, &taille_var_str);

    // convertir le champ taille variable en entier
    strToInt(taille_var_str, &taille_var_int);

    // lire le reste de la chaine StudentStr
    lire_chaine(f, buf, i, j, TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES - TAILLE_CHAMP_TAILLE + taille_var_int, &semi_ArchivedStudentStr);

    // concatener la taille avec le reste
    s = calloc(TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES + taille_var_int + 1, sizeof(char));
    subStr(taille_var_str, s, 0, TAILLE_CHAMP_TAILLE);
    subStr(semi_ArchivedStudentStr, s + TAILLE_CHAMP_TAILLE, 0, TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES - TAILLE_CHAMP_TAILLE + taille_var_int);
    s[TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES + taille_var_int] = '\0';

    // construire l'enregistrement student e
    ArchivedStudentStr_to_ArchivedStudent(s, e);

    // clear les allocations dynamiques
    free(taille_var_str);
    free(semi_ArchivedStudentStr);
    free(s);
}

/**
 * @brief ecrire un etudiant dans un fichier TOVC
 *
 * @param f
 * @param buf
 * @param i
 * @param j
 * @param e
 */
void ecrire_archived_student(TOVC *f, Buffer *buf, int *i, int *j, ArchivedStudent *e)
{
    ArchivedStudentStr s;
    ArchivedStudent_to_ArchivedStudentStr(e, &s);
    ecrire_chaine(f, buf, i, j, TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES + taille_var_archived_student(e), s);
    free(s);
}

/**
 * @brief creer une copie d'un etudiant
 *
 * @param e
 * @return ArchivedStudent
 */
ArchivedStudent copy_archived_student(ArchivedStudent e)
{
    ArchivedStudent e2;

    e2.classId = e.classId;
    e2.eff = e.eff;
    e2.genre = e.genre;
    strcpy(e2.id, e.id);
    e2.moyenne = e.moyenne;

    e2.nom = calloc(strlen(e.nom) + 1, sizeof(char));
    strcpy(e2.nom, e.nom);

    e2.prenom = calloc(strlen(e.prenom) + 1, sizeof(char));
    strcpy(e2.prenom, e.prenom);

    return e2;
}

/**
 * @brief lire un etudiant depuis l'utilisateur
 *
 * @param scan_id
 * @return ArchivedStudent
 */
ArchivedStudent get_archived_student(int scan_id)
{
    const int TAILLE_CHAINE_MAX = 1024;
    int scanf_code;
    char nom[TAILLE_CHAINE_MAX];
    char prenom[TAILLE_CHAINE_MAX];
    char id[TAILLE_CHAMP_CLE + 1];
    ClassId class_id;
    char genre;
    float moyenne;

    system("cls");

    int h, w;
    getConsoleSize(&w, &h);
    char *title = "ENTRER LES INFORMATIONS DE L'ELEVE ...";
    gotoxy((w-strlen(title))/2, 2);
    printf("%s", title);

    printf("\n\n\n\tEntrer le nom du l'eleve: ");
    fflush(stdin);
    fgets(nom, TAILLE_CHAINE_MAX, stdin);
    nom[strlen(nom) - 1] = '\0';

    printf("\n\tEntrer le prenom du l'eleve: ");
    fflush(stdin);
    fgets(prenom, TAILLE_CHAINE_MAX, stdin);
    prenom[strlen(prenom) - 1] = '\0';

get_genre:
    system("color 07");
    printf("\n\tEntrer le genre du l'eleve: (M / F) ");
    fflush(stdin);
    scanf_code = scanf("%c", &genre);
    if ((scanf_code != 1) || ((toupper(genre) != 'M') && (toupper(genre) != 'F')))
    {
        system("color 47");
        deleteConsoleLines(-3);
        goto get_genre;
    }

    if (scan_id == 1)
    {
get_id:
        system("color 07");
        printf("\n\tEntrer le ID du l'eleve: (sur %d characteres numeriques) ", TAILLE_CHAMP_CLE);
        fflush(stdin);
        scanf_code = scanf("%s", id);
        if ((scanf_code != 1) || (strlen(id) != TAILLE_CHAMP_CLE) || (strspn(id, "0123456789") != TAILLE_CHAMP_CLE))
        {
            system("color 47");
            deleteConsoleLines(-3);
            goto get_id;
        }
    }
    else if (scan_id == 2)
    {
        strcpy(id, IDS_ARRAY[ID_INDEX]);
        ID_INDEX++;
    }
    else
    {
        strcpy(id, "");
    }

get_niveau:
    system("color 07");
    printf("\n\tEntrer le niveau du l'eleve: (entre 1 et 5) ");
    fflush(stdin);
    scanf_code = scanf("%c", &class_id.annee);
    if ((scanf_code != 1) || (class_id.annee < '1') || (class_id.annee > '5'))
    {
        system("color 47");
        deleteConsoleLines(-3);
        goto get_niveau;
    }

get_salle:
    system("color 07");
    printf("\n\tEntrer le numero de salle du l'eleve: (entre 1 et %d) ", NBR_CLASSES_PAR_ANNEE);
    fflush(stdin);
    scanf_code = scanf("%d", &class_id.numSalle);
    if ((scanf_code != 1) || (class_id.numSalle < 1) || (class_id.numSalle > NBR_CLASSES_PAR_ANNEE))
    {
        system("color 47");
        deleteConsoleLines(-3);
        goto get_salle;
    }

get_moyenne:
    system("color 07");
    printf("\n\tEntrer la moyenne de l'eleve: (entre 0 et 20) ");
    fflush(stdin);
    scanf_code = scanf("%f", &moyenne);
    if ((scanf_code != 1) || (moyenne < 0) || (moyenne > 20))
    {
        system("color 47");
        deleteConsoleLines(-3);
        goto get_moyenne;
    }

    fflush(stdin);
    system("cls");

    return create_archived_student(id, nom, prenom, genre, class_id.annee, class_id.numSalle, moyenne);
}

