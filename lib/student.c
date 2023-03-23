#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "student.h"
#include "guiTools.h"
#include "conio2.h"


/**
 * @brief créer un élève (enregistrement de structure Student) à partir de ses informations: identifiant, nom, prénom, genre, année de scolarisation et le numéro de salle
 *
 * @param id l'identifiant de l'élève
 * @param nom le nom de l'élève
 * @param prenom le prénom de l'élève
 * @param genre le genre de l'élève
 * @param annee l'année de scolarisation de l'élève
 * @param numSalle le numéro de la salle de l'élève
 * @return Student l'enregistrement contenant les informations de l'élève créé
 */
Student create_student(char id[], char nom[], char prenom[], char genre, char annee, int numSalle)
{
    Student e;
    e.eff = 'F';
    e.genre = toupper(genre);
    e.classId.annee = toupper(annee);
    e.classId.numSalle = numSalle;
    strcpy(e.id, id);

    e.nom = calloc(strlen(nom) + 1, sizeof(char));
    strcpy(e.nom, nom);

    e.prenom = calloc(strlen(prenom) + 1, sizeof(char));
    strcpy(e.prenom, prenom);

    int max_note_index = nombre_matieres_annee(annee);

    for(int i = 0; i < MAX_NOTES; i ++)
        e.notes[i] = (i < max_note_index) ?  generate_note(annee) : (-1.0);

    return e;
}

 /**
 * @brief libérer les allocations faites pour les champs de taille variable (nom et prénom) d'un enregistrement de structure Student
 *
 * @param e l'enregistrement de structure Student à lui libérer les champs variable
 */
void free_student_fields(Student *e)
{
    free(e->nom);
    e->nom = NULL;
    free(e->prenom);
    e->prenom = NULL;
}

/**
 * @brief afficher les informations relatives à un élève (enregistrement de structure Student)
 *
 * @param e l'enregistrement de structure Student à afficher
 */
void afficher_student(Student *e)
{
    const int max_note_index = nombre_matieres_annee(e->classId.annee);

    printf("\t+-----------------------------------+\n");
    printf("\t| ID                                : %s\n", e->id);
    printf("\t| Nom                               : %s\n", e->nom);
    printf("\t| Prenom                            : %s\n", e->prenom);
    printf("\t| Genre                             : %s\n", ((e->genre == 'M') || (e->genre == 'm')) ? "Masculin" : "Feminin");
    printf("\t| Efface?                           : %s\n", ((e->eff == 'F') || (e->eff == 'f')) ? "NON" : "OUI");
    printf("\t| Niveau                            : %c\n", e->classId.annee);
    printf("\t| Numero du salle                   : Salle %d\n", e->classId.numSalle);

    printf("\t| Notes                             :\n");
    for (int i = 0; i < max_note_index; i++)
    {
        printf("\t|      %2d) %s", i + 1, MATIERES[(toupper(e->classId.annee) == 'P') ? 0 : (e->classId.annee - '0')][i]);
        gotoxy(45, wherey());
        printf(":> %.2f\n", e->notes[i]);
    }

    printf("\t+-----------------------------------+\n");
}


/**
 * @brief convertir un enregistrement de structure Student en une chaine de caractères
 *
 * @param e l'enregistrement de structure Student à convertir
 * @param e_str la chaine de caractères contenant le résultat de la conversion
 */
void Student_to_StudentStr(Student *e, StudentStr *e_str)
{
    int pos = 0;
    int taille_var = taille_var_student(e);
    int taille_note = taille_champ_notes_student(e);
    *e_str = calloc(TAILLE_CHAMPS_STUDENT_FIXES + taille_note + taille_var + 1, sizeof(char));
    StudentStr temp_str = calloc(TAILLE_CHAMPS_STUDENT_FIXES + taille_note + taille_var + 1, sizeof(char));

    // écrire le champ taille dans la chaine de destination
    intToStr(taille_var, temp_str, TAILLE_CHAMP_TAILLE);
    subStr(temp_str, (*e_str) + pos, 0, TAILLE_CHAMP_TAILLE);
    pos += TAILLE_CHAMP_TAILLE;

    // écrire le champ clé dans la chaine de destination
    subStr(e->id, (*e_str) + pos, 0, TAILLE_CHAMP_CLE);
    pos += TAILLE_CHAMP_CLE;

    // écrire le champ eff dans la chaine de destination
    (*e_str)[pos] = e->eff;
    pos += TAILLE_CHAMP_EFF;

    // écrire le champ classId dans la chaine de destination
    temp_str[0] = e->classId.annee;
    intToStr(e->classId.numSalle, temp_str + 1, TAILLE_CHAMP_CLASSID - 1);
    subStr(temp_str, (*e_str) + pos, 0, TAILLE_CHAMP_CLASSID);
    pos += TAILLE_CHAMP_CLASSID;

    // écrire le champ notes dans la chaine de destination
    float note;
    int partieEntiere, partieFractionnaire;
    for (int i = 0; i < nombre_matieres_annee(e->classId.annee); i++)
    {
        note = e->notes[i];
        partieEntiere = note / 1;
        partieFractionnaire = (note - (float)partieEntiere) * 100;

        intToStr(partieEntiere, temp_str, TAILLE_NOTE/2);
        intToStr(partieFractionnaire, temp_str + TAILLE_NOTE/2, TAILLE_NOTE/2);

        subStr(temp_str, (*e_str) + (pos + i*TAILLE_NOTE), 0, TAILLE_NOTE);
    }
    pos += taille_note;

    // écrire le champ genre dans la chaine de destination
    (*e_str)[pos] = e->genre;
    pos += TAILLE_CHAMP_GENRE;

    // écrire le champ nom#prenom dans la chaine de destination
    char *temp = calloc((taille_var + 1), sizeof(char));
    sprintf(temp, "%s#%s", e->nom, e->prenom);
    subStr(temp, (*e_str) + pos, 0, taille_var);
    pos += taille_var;
    free(temp);

    // ajouter le caractère de la fin de la chaine de destination
    (*e_str)[pos] = '\0';

    free(temp_str);
}

/**
 * @brief convertir une chaine de caractères en un enregistrement de structure Student
 *
 * @param e_str la chaine de caractères à convertir
 * @param e l'enregistrement de structure Student contenant les informations de la chaine de caractères
 */
void StudentStr_to_Student(StudentStr e_str, Student *e)
{
    int pos = 0;
    char temp_str[TAILLE_CHAMPS_STUDENT_FIXES + TAILLE_NOTE*MAX_NOTES + 1];
    char str_note[TAILLE_NOTE + 1];
    char taille_var_str[TAILLE_CHAMP_TAILLE + 1];
    int taille_var_int = 0;

    subStr(e_str, taille_var_str, 0, TAILLE_CHAMP_TAILLE);
    taille_var_str[TAILLE_CHAMP_TAILLE] = '\0';
    strToInt(taille_var_str, &taille_var_int);
    pos += TAILLE_CHAMP_TAILLE;

    // écrire le champ id de l'enregistrement e à partir de e_str
    subStr(e_str, e->id, pos, TAILLE_CHAMP_CLE);
    e->id[TAILLE_CHAMP_CLE] = '\0';
    pos += TAILLE_CHAMP_CLE;

    // écrire le champ eff de l'enregistrement e à partir de e_str
    e->eff = e_str[pos];
    pos += TAILLE_CHAMP_EFF;

    // écrire le champ classId de l'enregistrement e à partir de e_str
    e->classId.annee = e_str[pos] ;
    subStr(e_str, temp_str, pos + 1, TAILLE_CHAMP_CLASSID - 1) ;
    temp_str[TAILLE_CHAMP_CLASSID - 1] = '\0';
    strToInt(temp_str, &(e->classId.numSalle));
    pos += TAILLE_CHAMP_CLASSID;

    // écrire le champ notes de l'enregistrement e à partir de e_str
    int partieEnt, partieFrac;
    for(int i = 0; i < MAX_NOTES; i++)
    {
        if (i >= nombre_matieres_annee(e->classId.annee))
        {
            e->notes[i] = -1.0;
            continue;
        }

        subStr(e_str, temp_str, pos + i*TAILLE_NOTE, TAILLE_NOTE);
        temp_str[TAILLE_NOTE] = '\0';

        subStr(temp_str, str_note, 0, TAILLE_NOTE/2);
        str_note[TAILLE_NOTE/2] = '\0';
        strToInt(str_note, &partieEnt);
        e->notes[i] = partieEnt;

        subStr(temp_str, str_note, TAILLE_NOTE/2, TAILLE_NOTE/2);
        str_note[TAILLE_NOTE/2] = '\0';
        strToInt(str_note, &partieFrac);
        e->notes[i] += partieFrac * 0.01;
    }
    pos += TAILLE_NOTE * nombre_matieres_annee(e->classId.annee);

    // écrire le champ genre de l'enregistrement e à partir de e_str
    e->genre = e_str[pos];
    pos += TAILLE_CHAMP_GENRE;

    // écrire le champ nom et prenom de l'enregistrement e à partir de e_str
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
 * @brief comparer deux élèves (enregistrements de structure Student) selon leurs classes, noms et prénoms
 *
 * @param e1 l'enregistrement de structure Student relatif au premier élève
 * @param e2 l'enregistrement de structure Student relatif au deuxième élève
 * @return int le resultat de comparaison (e1 > e2 -> 1) (e1 < e2 -> -1) (e1 = e2 -> 0)
 */
int comparer_student(Student *e1, Student *e2)
{
    // e1 > e2: 1
    // e1 < e2: -1
    // e1 = e2: 0

    int temp;

    if (toupper(e1->classId.annee) == 'P' && toupper(e2->classId.annee) != 'P')
        return -1;
    if (toupper(e1->classId.annee) != 'P' && toupper(e2->classId.annee) == 'P')
        return 1;
    if (e1->classId.annee != e2->classId.annee)
        return (((e1->classId.annee - e2->classId.annee) < 0) ? -1: 1);

    if (e1->classId.numSalle != e2->classId.numSalle)
        return (((e1->classId.numSalle - e2->classId.numSalle) < 0) ? -1: 1);

    temp = strcmp(e1->nom, e2->nom);
    if (temp != 0)
        return ((temp < 0) ? -1: 1);

    temp = strcmp(e1->prenom, e2->prenom);
    if (temp != 0)
        return ((temp < 0) ? -1: 1);

    return 0;
}

/**
 * @brief calculer la taille des champs variables (nom et prenom) d'un enregistrement de structure Student
 *
 * @param e l'enregistrement à calculer la taille de ses champs variables
 * @return int la taille des champs variables de l'enregistrement de structure Student
 */
int taille_var_student(Student *e)
{
    return strlen(e->nom) + strlen(e->prenom) + 1;
}

/**
 * @brief donner le nombre de matières relatif à une année de scolarisation
 *
 * @param annee le caractère représentant l'année de scolarisation pour laquelle on cherche le nombre de matières
 * @return int le nombre de matières de l'année de scolarisation
 */
int nombre_matieres_annee(char annee)
{
    switch (toupper(annee))
    {
        case 'P':
            return NBR_MATIERES_ANNEE_P;
            break;
        case '1':
            return NBR_MATIERES_ANNEE_1;
            break;
        case '2':
            return NBR_MATIERES_ANNEE_2;
            break;
        case '3':
            return NBR_MATIERES_ANNEE_3;
            break;
        case '4':
            return NBR_MATIERES_ANNEE_4;
            break;
        case '5':
            return NBR_MATIERES_ANNEE_5;
            break;
        default:
            printf("[ERROR] annee non valide !");
            return -1;
    }
}

/**
 * @brief calculer la taille du champ des notes d'un enregistrement de structure Student
 *
 * @param e l'enregistrement de structure Student à calculer la taille de son champ de notes
 * @return int la taille du champ de notes relatif à un enregistrement
 */
int taille_champ_notes_student(Student *e)
{
    return nombre_matieres_annee(e->classId.annee) * TAILLE_NOTE;
}

/**
 * @brief calculer la moyenne d'un élève (enregistrement de structure Student) à partir de ses notes
 *
 * @param e l'enregistrement de l'élève qu'on souhaite lui calculer la moyenne
 * @return float la moyenne calculé d'un élève
 */
float moyenne_student(Student *e)
{
    float somme = 0.0;

    for (int i = 0; i < nombre_matieres_annee(e->classId.annee); i++)
        somme += e->notes[i];

    return somme / nombre_matieres_annee(e->classId.annee);
}

/**
 * @brief générer une note aléatoire
 *
 * @param annee l'année de scolarisation
 * @return float la noté générée
 */
float generate_note(char annee)
{
    const int NOTES_PARTIES_FRAC[] = {0, 25, 50, 75, 100};
    const int LIMITE = ((((annee == 'P') || (annee == 'p')) ? 0 : (annee - '0')) < 3) ? 8 : 4;

    if (!random_int(0, 4))
        return random_int(8, 19) + 0.01 * NOTES_PARTIES_FRAC[random_int(0, 4)];

    if (random_int(0, LIMITE))
        return random_int(LIMITE, 19) + 0.01 * NOTES_PARTIES_FRAC[random_int(0, 4)];

    return random_int(0, LIMITE) + 0.01 * NOTES_PARTIES_FRAC[random_int(0, 4)];
}


/**
 * @brief lire un enregistrement de structure Student à partir d'un fichier TOVC
 *
 * @param f un pointeur vers le fichier TOVC
 * @param buf le buffer où se trouve l'enregistrement à lire
 * @param i le numéro du bloc où se trouve l'enregistrement à lire
 * @param j la position dans le bloc d'où commence l'enregistrement à lire
 * @param e l'enregistrement lu
 */
void lire_student(TOVC *f, Buffer *buf, int *i, int *j, Student *e)
{
    // déclaration des variables
    char *StudentStr_partie1, *StudentStr_partie2;
    StudentStr s = "";
    char taille_var_str[TAILLE_CHAMP_CLE + 1] = "";
    int taille_var_int = 0, taille_champ_note = 0;

    // lire les champs: taille, cle, eff, classId
    lire_chaine(f, buf, i, j, TAILLE_CHAMPS_STUDENT_FIXES - TAILLE_CHAMP_GENRE, &StudentStr_partie1);

    // convertir le champ taille en un entier
    subStr(StudentStr_partie1, taille_var_str, 0, TAILLE_CHAMP_TAILLE);
    taille_var_str[TAILLE_CHAMP_CLE] = '\0';
    strToInt(taille_var_str, &taille_var_int);

    // calculer la taille du champs des notes (elle varie selon l'année)
    taille_champ_note = TAILLE_NOTE * nombre_matieres_annee(StudentStr_partie1[TAILLE_CHAMP_TAILLE + TAILLE_CHAMP_CLE + TAILLE_CHAMP_EFF]);

    // lire le reste de la chaine StudentStr
    lire_chaine(f, buf, i, j, taille_champ_note + TAILLE_CHAMP_GENRE + taille_var_int, &StudentStr_partie2);

    // concaténer la taille avec le reste
    s = calloc(TAILLE_CHAMPS_STUDENT_FIXES + taille_champ_note + taille_var_int + 1, sizeof(char));
    subStr(StudentStr_partie1, s, 0, TAILLE_CHAMPS_STUDENT_FIXES - TAILLE_CHAMP_GENRE);
    subStr(StudentStr_partie2, s + TAILLE_CHAMPS_STUDENT_FIXES - TAILLE_CHAMP_GENRE, 0, taille_champ_note + TAILLE_CHAMP_GENRE + taille_var_int);
    s[TAILLE_CHAMPS_STUDENT_FIXES + taille_champ_note + taille_var_int] = '\0';

    // construire l'enregistrement Student e
    StudentStr_to_Student(s, e);

    // libérer les allocations dynamiques
    free(StudentStr_partie1);
    free(StudentStr_partie2);
    free(s);
}


/**
 * @brief écrire un enregistrement de structure Student dans un fichier TOVC
 *
 * @param f un pointeur vers un fichier TOVC
 * @param buf le buffer où insérer l'enregistrement
 * @param i le numéro du bloc où insérer l'enregistrement
 * @param j la position dans le bloc d'où commencer l'écriture
 * @param e l'enregistrement à écrire
 */
void ecrire_student(TOVC *f, Buffer *buf, int *i, int *j, Student *e)
{
    StudentStr s;
    Student_to_StudentStr(e, &s);
    ecrire_chaine(f, buf, i, j, TAILLE_CHAMPS_STUDENT_FIXES + taille_champ_notes_student(e) + taille_var_student(e), s);
    free(s);
}


/**
 * @brief sauvegarder la table des identifiants dans un fichier
 *
 * @param nomf le nom du fichier où sauvegarder la table des identifiants
 */
void save_ids_array(char nomf[])
{
    FILE *f = fopen(nomf, "w");

    char index_str[TAILLE_CHAMP_CLE + 1];
    intToStr(ID_INDEX, index_str, TAILLE_CHAMP_CLE);
    index_str[TAILLE_CHAMP_CLE] = '\0';

    fwrite(index_str, sizeof(index_str), 1, f);
    fwrite(IDS_ARRAY, sizeof(IDS_ARRAY), 1, f);

    fclose(f);
}

/**
 * @brief charger la table des identifiants à partir d'un fichier
 *
 * @param nomf le nom du fichier duquel charger la table des identifiants
 */
void load_ids_array(char nomf[])
{
    FILE *f = fopen(nomf, "r");

    char index_str[TAILLE_CHAMP_CLE + 1];
    fread(index_str, sizeof(index_str), 1, f);
    strToInt(index_str, &ID_INDEX);

    fread(IDS_ARRAY, sizeof(IDS_ARRAY), 1, f);

    fclose(f);
}

/**
 * @brief copier les informations relatives à un enregistrement de structure Student dans un autre
 *
 * @param e l'enregistrement à copier
 * @return Student la copie éffectué de l'enregistrement original
 */
Student copy_student(Student e)
{
    Student e2;

    e2.classId = e.classId;
    e2.eff = e.eff;
    e2.genre = e.genre;
    strcpy(e2.id, e.id);

    e2.nom = calloc(strlen(e.nom) + 1, sizeof(char));
    strcpy(e2.nom, e.nom);

    e2.prenom = calloc(strlen(e.prenom) + 1, sizeof(char));
    strcpy(e2.prenom, e.prenom);

    for (int i = 0; i < MAX_NOTES; i++)
        e2.notes[i] = e.notes[i];

    return e2;
}

/**
 * @brief lire un etudiant depuis l'utilisateur
 *
 * @param flag qui commande la lecture de l'ID de l'eleve
 * @return Student l'etudiant lue apartir de l'utilisateur
 */
Student get_student(int scan_id)
{
    Student e;
    const int TAILLE_CHAINE_MAX = 1024;
    int scanf_code;
    char edit_notes;
    char nom[TAILLE_CHAINE_MAX];
    char prenom[TAILLE_CHAINE_MAX];
    char id[TAILLE_CHAMP_CLE + 1];
    ClassId class_id;
    char genre;

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
    printf("\n\tEntrer le niveau du l'eleve: (entre P et 5) ");
    fflush(stdin);
    scanf_code = scanf("%c", &class_id.annee);
    if ((scanf_code != 1) || ((toupper(class_id.annee) != 'P') && ((class_id.annee < '1') || (class_id.annee > '5'))))
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

    e = create_student(id, nom, prenom, genre, class_id.annee, class_id.numSalle);

get_confirmation:
    system("color 07");
    printf("\n\tVoullez vous entrer les notes ou les generer aleatoirement? (E: entrer / A: aleatoire) ");
    fflush(stdin);
    scanf_code = scanf("%c", &edit_notes);
    edit_notes = toupper(edit_notes);
    if ((scanf_code != 1) || ((edit_notes != 'E') && (edit_notes != 'A')))
    {
        system("color 47");
        deleteConsoleLines(-3);
        goto get_confirmation;
    }

    if (edit_notes == 'E')
    {
        int max_note_index = nombre_matieres_annee(class_id.annee);

        for(int i = 0; i < max_note_index; i++)
        {
get_note:
            system("color 07");
            printf("\t\tVeuillez entrer la note de la matiere \"%s\": (entre 0 et 20) ", MATIERES[(toupper(class_id.annee) == 'P') ? 0 : (class_id.annee - '0')][i]) ;
            fflush(stdin);
            scanf_code = scanf("%f", &e.notes[i]);
            if ((scanf_code != 1) || (e.notes[i] < 0) || (e.notes[i] > 20))
            {
                system("color 47");
                deleteConsoleLines(-2);
                goto get_note;
            }
        }

        for (int i = max_note_index; i < MAX_NOTES; i++)
            e.notes[i] = -1.0;
    }

    fflush(stdin);
    system("cls");

    return e;
}
