#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"
#include "TOVC.h"
#include "student.h"
#include "archive.h"
#include "functions.h"


/**
 * @brief initialiser les variables globales
 *
*/
void init()
{
    // initializer le seed de generation des nombres aleatoires
    srand(time(NULL));

    /* debut initialisation de la variable globale MATIERES */
    char temp_str[1024];

    // ouverture de fichier des matieres
    FILE *matieres_file = fopen("files/static/matieres.txt", "r");
    if (matieres_file == NULL)
    {
        printf("[ERROR] erreur d'ouverture du fichier des matieres de chaque annee !\n");
        exit(EXIT_FAILURE);
    }

    int line = 0, annee, matiere;
    while (fgets(temp_str, sizeof(temp_str), matieres_file)) // lire une ligne
    {
        // zapper la ligne qui ne contient pas les noms des matieres
        if (!(line % 2))
        {
            line++;
            continue;
        }

        annee = line / 2;
        matiere = 0;

        // recuperer les noms des matieres separees par ','
        char * strToken = strtok(temp_str, ",");
        while (strToken != NULL)
        {
            // nettoyage du caractere '\n' (dans le cas de la derniere note de chaque annee)
            if (strToken[strlen(strToken) - 1] == '\n')
                strToken[strlen(strToken) - 1] = '\0';

            // allocation + affectation du nom de la matieres a la matiere correspondante dans le tableau MATIERES
            MATIERES[annee][matiere] = calloc(strlen(strToken) + 1, sizeof(char));
            strcpy(MATIERES[annee][matiere], strToken);

            // aller a la matiere suivante
            strToken = strtok ( NULL, "," );
            matiere++;
        }

        // mettre le reste des matieres s'ils existent a NULL (dans le cas d'une annee qui contient des matieres < MAX_NOTES)
        for (int i = matiere; i < MAX_NOTES; i++)
            MATIERES[annee][matiere] = NULL;

        line++;
    }

    fclose(matieres_file);

    /* fin initialisation de la variable globale MATIERES */
}

/**
 * @brief libérer la variable globale MATIERES
 */
void clean()
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            free(MATIERES[i][j]);
            MATIERES[i][j] = NULL;
        }
    }
}


/**
 * @brief charger initialement tous les élèves d'une année scolaire
 *
 * @param nomf nom du fichier du chargement initial des élèves
 */
void chargement_initial_students(char nomf[])
{
    // declarations des variables
    FILE *noms_file, *prenoms_file;
    TOVC file, *f = &file;
    Buffer buf;
    Student e;
    StudentStr s;
    int taille;
    int i, j, k;
    int max_note_index;
    int min_index_nom, max_index_nom, index_nom;
    int min_index_prenom, max_index_prenom, index_prenom;
    char noms_array[NBR_NOMS][1024];
    char prenoms_array[NBR_PRENOMS][1024];
    char temp_str[1024];

    // chargement fichier des noms
    noms_file = fopen("files/static/noms.txt" , "r");
    if (noms_file == NULL)
    {
        printf("[ERROR] erreur d'ouverture de fichier des noms !\n");
        exit(EXIT_FAILURE);
    }

    // chargement fichier des prenoms et genres
    prenoms_file = fopen("files/static/prenoms.txt", "r");
    if (prenoms_file == NULL)
    {
        printf("[ERROR] erreur d'ouverture de fichier des prenoms !\n");
        fclose(noms_file);
        exit(EXIT_FAILURE);
    }

    /* debut creation des tableaux de manipulation */

    // noms_array
    for (k = 0; k < NBR_NOMS; k++)
    {
        fgets(temp_str, sizeof(temp_str), noms_file);
        temp_str[strlen(temp_str) - 1] = '\0';
        strcpy(noms_array[k], temp_str);
    }
    fclose(noms_file);

    // prenoms_array
    for (k = 0; k < NBR_PRENOMS; k++)
    {
        fgets(temp_str, sizeof(temp_str), prenoms_file);
        temp_str[strlen(temp_str) - 1] = '\0';
        strcpy(prenoms_array[k], temp_str);
    }
    fclose(prenoms_file);

    /* fin creation des tableaux de manipulation */

    // ouverture du fichier de l'annee courante (TOVC)
    ouvrir(f, nomf, 'N');

    // remplissage du fichier tovc
    i = 1; // initialisation de l'ecriture dans le premier bloc
    j = 0; // initialisation de l'ecriture dans la position 0

    e.eff = 'F'; // mettre le champ efface a 'F' <=> 'Faux'
    for (int classe = 0; classe <= 5; classe++) // parcourir les classes de 'P' jusqu'a '5'
    {
        e.classId.annee = (classe == 0) ? 'P' : '0' + classe; // generer la partie annee dans l'identifiant la classe
        for (int numSalle = 1; numSalle <= NBR_CLASSES_PAR_ANNEE; numSalle++) // parcourir toutes les salle de l'annee correspondante
        {
            e.classId.numSalle = numSalle; // generer la partie numero du salle (sequentiel par annee) dans l'identifiant la classe

            // initialisation des bornes des nombre aleatoires generer pour selectionner le nom et le prenom du l'eleve
            min_index_nom = 0;
            min_index_prenom = 0;
            max_index_nom = NBR_NOMS / NBR_ELEVES_MAX_PAR_CLASSE;
            max_index_prenom = NBR_PRENOMS / NBR_ELEVES_MAX_PAR_CLASSE;

            for (k = 0; k < NBR_ELEVES_MAX_PAR_CLASSE; k++) // parcourir les eleves de la classe courante
            {
                strcpy(e.id, IDS_ARRAY[ID_INDEX]); // prendre un id aleatoire depuis le tableau des ids aleatoires
                ID_INDEX++; // mise a jour la position du suivant id aleatoire a prendre

                // calculer le nombre des matieres etudiees dans l'annes courante
                max_note_index = nombre_matieres_annee(e.classId.annee);

                // affecter des notes aleatoires a l'eleve entre 00.00 et 20.00 selon les matieres etudiees (les autres sont a -1)
                for(int note_index = 0; note_index < MAX_NOTES; note_index ++)
                    e.notes[note_index] = (note_index < max_note_index) ? generate_note(e.classId.annee) : (-1.0);

                // generer le nom et le prenom du l'eleve aleatoirement d'une maniere ordonnee

                // choisir le nom depuis le tableau des noms
                index_nom = random_int(min_index_nom, max_index_nom);
                min_index_nom = index_nom;
                max_index_nom += NBR_NOMS / NBR_ELEVES_MAX_PAR_CLASSE;

                // allocation et affectation de e.nom
                e.nom = calloc(strlen(noms_array[index_nom]) + 1, sizeof(char));
                strcpy(e.nom, noms_array[index_nom]);

                // choisir le prenom et le genre depuis le tableau des prenoms et genres
                index_prenom = random_int(min_index_prenom, max_index_prenom);
                min_index_prenom = index_prenom;
                max_index_prenom += NBR_PRENOMS / NBR_ELEVES_MAX_PAR_CLASSE;

                // allocation et affectation de e.prenom
                e.prenom = calloc(strlen(prenoms_array[index_prenom]) + 1, sizeof(char));
                subStr(prenoms_array[index_prenom], e.prenom, 0, strlen(prenoms_array[index_prenom]) - 2);

                // affectation de e.genre
                e.genre = prenoms_array[index_prenom][strlen(prenoms_array[index_prenom]) - 1];

                // creation de la chaine (s) correspondante a l'etudiant (e)
                Student_to_StudentStr(&e, &s);
                taille = TAILLE_CHAMPS_STUDENT_FIXES + taille_champ_notes_student(&e) + taille_var_student(&e);

                // ecriture de la chaine dans le fichier
                ecrire_chaine(f, &buf, &i, &j, taille, s);

                // mise a jour de l'entete du fichier
                aff_entete(f, ENTETE_NBR_ENREG, entete(f, ENTETE_NBR_ENREG) + 1);
                aff_entete(f, ENTETE_NBR_CHAR_INS, entete(f, ENTETE_NBR_CHAR_INS) + taille);

                // liberer l'espace memoire alloue, pour ne pas le perdre en passant a l'eleve suivant
                free(s);
                free_student_fields(&e);
            }
        }
    }

    // mise a jour de l'entete
    aff_entete(f, ENTETE_NUMERO_DERNIER_BLOC, i);
    aff_entete(f, ENTETE_POS_LIBRE, j);

    // ecriture du caractere de la fin de la chaine (\0) dans le dernier bloc
    buf.tab[j] = '\0';
    ecrire_dir(f, i, &buf);

    // fermer le fichier et sauvegarder les modifications
    fermer(f);
}

/**
 * @brief chercher un élève (enregistrement Student) dans un fcihier
 *
 * @param nomf le nom du fichier où se fera la recherche
 * @param e l'enregistrement Student de l'élève à chercher
 * @param trouv booléen qui indique si l'enregistrement recherché est trouvé ou non
 * @param eff un booléen qui indique si l'enregistrement recherché est effacé (logiquement) ou non
 * @param i le numéro du bloc où se trouve l'enregistrement recherché
 * @param j la position dans le bloc d'où commence l'enregistrement recherché
 */
void recherche_student(char nomf[], Student e, bool *trouv, bool *eff, int *i, int *j)
{
    TOVC f;
    Buffer buf;
    Student temp_student;
    bool stop = false;
    int cmp;

    *trouv = false;
    *eff = false;
    *i = 1;
    *j = 0;

    ouvrir(&f, nomf, 'A');

    if (entete(&f, ENTETE_NUMERO_DERNIER_BLOC) <= 0) //le cas où le fichier est vide
    {
        fermer(&f);
        return;
    }

    int temp_i = 1, temp_j = 0;
    lire_dir(&f, temp_i, &buf); // lecture du premier bloc
    while ((!(*trouv)) && (!stop))
    {
        lire_student(&f, &buf, &temp_i, &temp_j, &temp_student); //lire (séquentiellement) un enregistrement à partir du buffer

        cmp = comparer_student(&temp_student, &e); //comparer l'enregistrement lu avec celui recherché (sans comparer leurs identifiants)

        if (cmp == 0) // temp_student == e
        {
            if (strlen(e.id) > 0)
            {
                if (strcmp(temp_student.id, e.id) == 0) //comparer les identifiants des deux enregistrements
                {
                    *trouv = true;
                    *eff = (toupper(temp_student.eff) == 'F') ? false : true;  //vérifier si l'enregistrement n'est pas éffacé
                }
                else
                {
                    *i = temp_i;
                    *j = temp_j;
                }
            }
            else // ignorer le id
            {
                *trouv = true;
                *eff = (toupper(temp_student.eff) == 'F') ? false : true;
            }
        }
        else if (cmp == -1) // temp_student < e
        {
            *i = temp_i;
            *j = temp_j;
        }
        else if (cmp == 1) // temp_student > e
            stop = true;

        if ((temp_i >= entete(&f, ENTETE_NUMERO_DERNIER_BLOC)) && (temp_j >= entete(&f, ENTETE_POS_LIBRE))) // fin du fichier
            stop = true;

        free_student_fields(&temp_student); //libérer les allocations faites pour temp_student
    }

    fermer(&f);
}

/**
 * @brief chercher un enregistrement Student par son identifiant (le champ id)
 *
 * @param nomf le nom du fichier où se fera la recherche
 * @param id l'identifiant de l'enregistrement recherché
 * @param trouv booléen qui indique si l'enregistrement recherché est trouvé ou non
 * @param eff un booléen qui indique si l'enregistrement recherché est effacé (logiquement) ou non
 * @param i le numéro du bloc où se trouve l'enregistrement recherché
 * @param j la position dans le bloc d'où commence l'enregistrement recherché
 */
void recherche_student_by_id(char nomf[], char id[], bool *trouv, bool *eff, int *i, int *j)
{
    TOVC f;
    Buffer buf;
    Student temp_student;
    bool stop = false;

    *trouv = false;
    *eff = false;
    *i = 1;
    *j = 0;

    ouvrir(&f, nomf, 'A');

    if (entete(&f, ENTETE_NUMERO_DERNIER_BLOC) <= 0) // fichier vide
    {
        fermer(&f);
        return;
    }

    int temp_i = 1, temp_j = 0;
    lire_dir(&f, temp_i, &buf); // lecture du premier bloc
    while ((!(*trouv)) && (!stop))
    {
        lire_student(&f, &buf, &temp_i, &temp_j, &temp_student); //lire (séquentiellement) un enregistrement à partir du buffer

        if (strcmp(temp_student.id, id) == 0) //comparer les identifiants
        {
            *trouv = true;
            *eff = (toupper(temp_student.eff) == 'F') ? false : true; //vérifier si l'enregistrement n'est pas éffacé
        }
        else
        {
            *i = temp_i;
            *j = temp_j;
        }

        if ((temp_i >= entete(&f, ENTETE_NUMERO_DERNIER_BLOC)) && (temp_j >= entete(&f, ENTETE_POS_LIBRE))) //fin du fichier
            stop = true;

        free_student_fields(&temp_student); //libérer les allocations faites pour temp_student
    }

    fermer(&f);
}

/**
 * @brief insérer un enregistrement Student dans un fichier
 *
 * @param nomf le nom du fichier où insérer le fichier
 * @param e l'enregistrement à insérer dans le fichier
 */
void insertion_student(char nomf[], Student e)
{
    int i, j;
    bool trouv, eff;
    StudentStr s;

    recherche_student(nomf, e, &trouv, &eff, &i, &j);
    if (trouv && (!eff)) //l'enregistrement est déja présent sans etre effacé
        return;

    Student_to_StudentStr(&e, &s);
    inserer_chaine(nomf, i, j, TAILLE_CHAMPS_STUDENT_FIXES + taille_champ_notes_student(&e) + taille_var_student(&e), s);
    free(s);
}

/**
 * @brief supprimer logiquement un enregistrement Student d'un fichier
 *
 * @param nomf le nom du fichier où se fera la suppression
 * @param e l'enregistrement à supprimer logiquement du fichier
 */
void suppression_logique_student(char nomf[], Student e)
{
    TOVC f;
    Buffer buf;
    bool trouv, eff;
    int i, j;

    recherche_student(nomf, e, &trouv, &eff, &i, &j);
    if ((!trouv) || eff) //s'il n'existe pas ou il est déja effacé
        return;

    j += (TAILLE_CHAMP_TAILLE + TAILLE_CHAMP_CLE); //aller vers la position du champs eff
    if (j >= MAX_CHARS)
    {
        i += (j / MAX_CHARS);
        j %= MAX_CHARS;
    }

    ouvrir(&f, nomf, 'A');

    lire_dir(&f, i, &buf);
    buf.tab[j] = 'V'; //changer la valeur du champs eff
    ecrire_dir(&f, i, &buf); //écrire le buffer avec l'enregistrement effacé

    aff_entete(&f, ENTETE_NBR_CHAR_SUP, entete(&f, ENTETE_NBR_CHAR_SUP) + TAILLE_CHAMPS_STUDENT_FIXES + taille_champ_notes_student(&e) + taille_var_student(&e));
    fermer(&f);
}

/**
 * @brief mettre à jour les informations d'un enregistrement Student
 *
 * @param nomf nom du fichier où se trouve l'enregistrement à mettre à jour
 * @param eAc l'ancienne version de l'enregistrement
 * @param eNv les nouveau enregistrement contenant les nouvelles informations
 */
bool mise_a_jour_student(char nomf[], Student eAc , Student eNv)
{
    TOVC f;
    Buffer buf;
    int i, j;
    bool trouv, eff;

    // l'unicite des id !
    if (strcmp(eNv.id, eAc.id) != 0)
    {
        recherche_student_by_id(nomf, eNv.id, &trouv, &eff, &i, &j);
        if (trouv)
            return false;

        recherche_student(nomf, eAc, &trouv, &eff, &i, &j);

        // sauvegarder les modifications dans le fichier
        ouvrir(&f, nomf, 'A');
        lire_dir(&f, i, &buf);
        ecrire_student(&f, &buf, &i, &j, &eNv);
        ecrire_dir(&f, i, &buf);
        fermer(&f);

        // permutation des ids dans la table des ids pour garder l'unicite des IDs prochainement
        permute_ids_in_array(eNv.id, eAc.id);

        return true;
    }

    // ordre + taille variable
    if ((strcmp(eAc.nom, eNv.nom) != 0) || (strcmp(eAc.prenom, eNv.prenom) != 0) || (eAc.classId.annee != eNv.classId.annee) || (eAc.classId.numSalle != eNv.classId.numSalle))
    {
        recherche_student(nomf, eAc, &trouv, &eff, &i, &j);
        Student temp_e = copy_student(eAc);

        for (int k = 0; k < TAILLE_CHAMP_CLE; k++)
            temp_e.id[k] = '-'; // modification de l'id pour garder l'unicite
        temp_e.id[TAILLE_CHAMP_CLE] = '\0';

        temp_e.eff = 'V'; // suppression logique

        // sauvegarder les modifications dans le fichier
        ouvrir(&f, nomf, 'A');
        lire_dir(&f, i, &buf);
        ecrire_student(&f, &buf, &i, &j, &temp_e);
        ecrire_dir(&f, i, &buf);
        fermer(&f);
        free_student_fields(&temp_e);

        insertion_student(nomf, eNv);
        return true;
    }

    // modification de genre ou notes(dans ce cas le nombre des notes ne change pas car classId n'a pas change)
    recherche_student(nomf, eAc, &trouv, &eff, &i, &j);
    ouvrir(&f, nomf, 'A');
    lire_dir(&f, i, &buf);
    ecrire_student(&f, &buf, &i, &j, &eNv);
    ecrire_dir(&f, i, &buf);
    fermer(&f);
    return true;
}

/**
 * @brief reorganiser un fichier dans un nouveau selon les changements faits dans l'ancien (supprimer physiquement les enregistrements effacés logiquement)
 *
 * @param nomf le nom du fichier à reorganiser
 * @param new_nomf le nom du nouveau fichier organisé à créer à partir d'un ancien
 */
void reorganisation_students(char nomf[], char new_nomf[])
{
    TOVC f1, f2;
    Buffer buf1, buf2;
    int i1 = 1, j1 = 0, i2 = 1, j2 = 0, taille;
    Student e;
    StudentStr s;

    // ouvrir le fichier original
    ouvrir(&f1, nomf, 'A');
    lire_dir(&f1, i1, &buf1); // lecture du premier bloc

    // ouvrir le nouveau fichier
    ouvrir(&f2, new_nomf, 'N');

    // parcourir le fichier original
    for (int k = 0; k < entete(&f1, ENTETE_NBR_ENREG); k++)
    {
        lire_student(&f1, &buf1, &i1, &j1, &e);

        if (e.eff == 'F')
        {
            Student_to_StudentStr(&e, &s);
            taille = strlen(s);

            // ajouter l'eleve au nouveau fichier
            ecrire_chaine(&f2, &buf2, &i2, &j2, taille, s);

            // mise a jour de l'entete de nouveau fichier
            aff_entete(&f2, ENTETE_NBR_ENREG, entete(&f2, ENTETE_NBR_ENREG) + 1);
            aff_entete(&f2, ENTETE_NBR_CHAR_INS, entete(&f2, ENTETE_NBR_CHAR_INS) + taille);

            free(s);
        }

        free_student_fields(&e);
    }

    // mise a jour de l'entete de nouveau fichier
    aff_entete(&f2, ENTETE_NUMERO_DERNIER_BLOC, i2);
    aff_entete(&f2, ENTETE_POS_LIBRE, j2);

    // ecriture du caractere de la fin de la chaine (\0) dans le dernier bloc du nouveau fichier
    buf2.tab[j2] = '\0';
    ecrire_dir(&f2, i2, &buf2); // derniere ecriture

    fermer(&f1);
    fermer(&f2);
}


/**
 * @brief créer le fichier archive à partir d'un fichier d'une année courante
 *
 * @param nomf_annee_courante le nom du fichier pour lequel créer l'archive associé
 * @param nomf_archive le nom du fichier de l'archive à créer
 */
void create_archive(char nomf_annee_courante[], char nomf_archive[])
{
    TOVC f, temp_f;
    Buffer buf, temp_buf;
    Student e1;
    ArchivedStudent e2;
    int i, j, k, prec_k;
    int temp_i, temp_j;
    char *temp_files[NBR_CLASSES_PAR_ANNEE * 5];
    IndexTable index;

    k = 0;
    for (i = 0; i < 5; i++)
    {
        for (j = 1; j <= NBR_CLASSES_PAR_ANNEE; j++)
        {
            temp_files[k] = calloc(strlen("files/temp/temp_archive_.tovc") + TAILLE_CHAMP_CLASSID + 1, sizeof(char));
            sprintf(temp_files[k], "files/temp/temp_archive_%c%d.tovc", '1' + i, j);

            ouvrir(&temp_f, temp_files[k], 'N');
            fermer(&temp_f);

            k++;
        }
    }

    // fragmentation
    ouvrir(&f, nomf_annee_courante, 'A');
    i = 1;
    j = 0;
    lire_dir(&f, i, &buf); // lecture du premier bloc
    prec_k = -1;
    while (i <= entete(&f, ENTETE_NUMERO_DERNIER_BLOC)) //parcourir tous les blocs du fichier
    {
        if ((i == entete(&f, ENTETE_NUMERO_DERNIER_BLOC)) && (j >= entete(&f, ENTETE_POS_LIBRE)))
            break;

        lire_student(&f, &buf, &i, &j, &e1); //lire sequentiellement les etudiant

        if ((e1.classId.annee == 'P') || (e1.classId.annee == 'p') || (e1.eff == 'V') || (e1.eff == 'v')) //ignorer l'annee P et les etudiants efface
        {
            free_student_fields(&e1);
            continue;
        }

        Student_to_ArchivedStudent(&e1, &e2);
        free_student_fields(&e1);

        k = (e2.classId.annee - '1')*NBR_CLASSES_PAR_ANNEE + (e2.classId.numSalle - 1);

        if (k != prec_k)
            index.taille = 0;

        prec_k = k;

        // inserer chaque etudiant dans le bon fichier
        insertion_archived_student_index(temp_files[k], e2, &index);
        free_archived_student_fields(&e2);
    }
    fermer(&f);

    // fusion
    ouvrir(&f, nomf_archive, 'N');
    i = 1;
    j = 0;
    for (k = 0; k < NBR_CLASSES_PAR_ANNEE * 5; k++) // parcourir tous les fichiers temp
    {
        ouvrir(&temp_f, temp_files[k], 'A');
        temp_i = 1;
        temp_j = 0;
        lire_dir(&temp_f, temp_i, &temp_buf);
        while (temp_i <= entete(&temp_f, ENTETE_NUMERO_DERNIER_BLOC))
        {
            if ((temp_i == entete(&temp_f, ENTETE_NUMERO_DERNIER_BLOC)) && (temp_j >= entete(&temp_f, ENTETE_POS_LIBRE)))
                break;

            lire_archived_student(&temp_f, &temp_buf, &temp_i, &temp_j, &e2);
            ecrire_archived_student(&f, &buf, &i, &j, &e2); // fusion dans le fichier archive finale

            aff_entete(&f, ENTETE_NBR_ENREG, entete(&f, ENTETE_NBR_ENREG) + 1);
            aff_entete(&f, ENTETE_NBR_CHAR_INS, entete(&f, ENTETE_NBR_CHAR_INS) + TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES + taille_var_archived_student(&e2));

            free_archived_student_fields(&e2);
        }

        fermer(&temp_f);
        remove(temp_files[k]);
        free(temp_files[k]);
    }

    aff_entete(&f, ENTETE_NUMERO_DERNIER_BLOC, i);
    aff_entete(&f, ENTETE_POS_LIBRE, j);

    buf.tab[j] = '\0';
    ecrire_dir(&f, i, &buf); // derniere ecriture

    fermer(&f);
}

/**
 * @brief créer la table index d'un fichier archive
 *
 * @param nomf_archive le nom du fichier de l'archive pour qui l'index sera créé
 * @param tabInd la table index créée associée à un fichier archive
 */
void create_index(char nomf_archive[], IndexTable *tabInd)
{
    TOVC f;
    Buffer buf;
    int i, j, prec_i, prec_j;
    ArchivedStudent e, prec_e;
    IndexData index_data;

    // initialisation de l'index (index vide <=> taille = 0)
    tabInd->taille = 0;

    ouvrir(&f, nomf_archive, 'A');

    // fichier vide
    if (entete(&f, ENTETE_NUMERO_DERNIER_BLOC) <= 0)
    {
        fermer(&f);
        return;
    }

    i = 1; prec_i = 1;
    j = 0; prec_j = 0;

    lire_dir(&f, i, &buf);

    // lire le premier enregistrement dans le fichier
    lire_archived_student(&f, &buf, &i, &j, &e);
    free_archived_student_fields(&e);
    prec_e = e;

    // preparation de IndexData
    index_data.classId = prec_e.classId;
    index_data.moyenne_min = prec_e.moyenne;
    index_data.numBloc = prec_i;
    index_data.pos = prec_j;

    // parcourir le fichier d'archivage
    while ((i < entete(&f, ENTETE_NUMERO_DERNIER_BLOC)) || (j < entete(&f, ENTETE_POS_LIBRE)))
    {
        // sauvegarder l'etudiant courant et son emplacement avant de lire le suivant
        prec_i = i;
        prec_j = j;
        prec_e = e;

        // lire l'etudiant suivant depuis le fichier darchive
        lire_archived_student(&f, &buf, &i, &j, &e);
        free_archived_student_fields(&e);

        // comparer avec les donner sauvgarder precedament si la class ou le bloc change
        // si c'est le cas => insertion dans l'index + preparation de la case suivante
        if ((toupper(prec_e.classId.annee) != toupper(e.classId.annee)) || (prec_e.classId.numSalle != e.classId.numSalle))
        {
            // insertion
            index_data.moyenne_max = prec_e.moyenne;
            tabInd->tab[tabInd->taille] = index_data;
            tabInd->taille++;

            // preparation de la case suivante
            index_data.classId = e.classId;
            index_data.moyenne_min = e.moyenne;
            index_data.numBloc = prec_i;
            index_data.pos = prec_j;
        }
        else if (prec_i != i) // i > prec_i
        {
            if ((i == entete(&f, ENTETE_NUMERO_DERNIER_BLOC)) && (j >= entete(&f, ENTETE_POS_LIBRE)))
                break;

            // insertion
            index_data.moyenne_max = e.moyenne;
            tabInd->tab[tabInd->taille] = index_data;
            tabInd->taille++;

            // preparation de la case suivante
            prec_i = i;
            prec_j = j;
            prec_e = e;

            lire_archived_student(&f, &buf, &i, &j, &e);
            free_archived_student_fields(&e);

            index_data.classId = e.classId;
            index_data.moyenne_min = e.moyenne;
            index_data.numBloc = prec_i;
            index_data.pos = prec_j;
        }
    }

    // derniere ecriture dans l'index
    index_data.moyenne_max = e.moyenne;
    tabInd->tab[tabInd->taille] = index_data;
    tabInd->taille++;

    fermer(&f);
}

/**
 * @brief chercher la position dans l'index où pourrait se trouver un enregistrement ArchivedStudent
 *
 * @param tabInd l'index du fichier archive où se fera la recherche
 * @param e l'enregistrement recherché
 * @return int l'indice du tableau de l'indexe où pourrait se trouver l'enregistrement
 */
int recherche_index(IndexTable *tabInd, ArchivedStudent e)
{
    int k;
    int binf = 0;
    int bsup = tabInd->taille - 1;
    bool trouv = false;
    IndexData data;

    // recherche dichotomique de la classe dans la table d'index
    while ((binf <= bsup) && (!trouv))
    {
        k = (binf + bsup) / 2;
        data = tabInd->tab[k];

        if ((e.classId.annee == data.classId.annee) && (e.classId.numSalle == data.classId.numSalle))
            trouv = true;
        else if ((e.classId.annee < data.classId.annee) || ((e.classId.annee == data.classId.annee) && (e.classId.numSalle < data.classId.numSalle)))
            bsup = k - 1;
        else
            binf = k + 1;
    }

    if (!trouv)
    {
        k = bsup;
        if (k < 0)
            k = 0;
    }

    // recherche de l'intervalle des moyennes dans l'index !!
    if (trouv)
    {
        while (e.moyenne <= data.moyenne_min)
        {
            if ((k-1 < 0) || (e.classId.annee != tabInd->tab[k-1].classId.annee) || (e.classId.numSalle != tabInd->tab[k-1].classId.numSalle))
                break;

            k--;
            data = tabInd->tab[k];
        }

        while (e.moyenne > data.moyenne_max)
        {
            if ((k+1 > tabInd->taille - 1) || (e.classId.annee != tabInd->tab[k+1].classId.annee) || (e.classId.numSalle != tabInd->tab[k+1].classId.numSalle))
                break;

            k++;
            data = tabInd->tab[k];
        }
    }

    return k;
}

/**
 * @brief chercher un élève (enregistrement ArchivedStudent) dans un fcihier archive
 *
 * @param nomf le nom du fichier où se fera la recherche
 * @param e l'enregistrement ArchivedStudent de l'élève à chercher
 * @param tabInd l'index du fichier archive
 * @param trouv booléen qui indique si l'enregistrement recherché est trouvé ou non
 * @param eff un booléen qui indique si l'enregistrement recherché est effacé (logiquement) ou non
 * @param i le numéro du bloc où se trouve l'enregistrement recherché
 * @param j la position dans le bloc d'où commence l'enregistrement recherché
 * @param k l'indice dans le tableau index ou se trouve l'enregistrement (sa moyenne appartient à l'intervalle present dans la case k de l'index)
 */
void recherche_archived_student_index(char nomf[], ArchivedStudent e, IndexTable *tabInd, bool *trouv, bool *eff, int *i, int *j, int *k)
{
    TOVC f;
    Buffer buf;
    ArchivedStudent temp_student;
    bool stop = false;
    int cmp;

    *k = 0;
    *i = 1;
    *j = 0;
    *trouv = false;
    *eff = false;

    // recherche dans l'index
    if (tabInd->taille <= 0) // fichier vide
        return;

    *k = recherche_index(tabInd, e);
    *i = tabInd->tab[*k].numBloc;
    *j = tabInd->tab[*k].pos;


    // recherche dans le fichier
    ouvrir(&f, nomf, 'A');

    int temp_i = *i, temp_j = *j;
    lire_dir(&f, temp_i, &buf);
    while ((!(*trouv)) && (!stop))
    {
        lire_archived_student(&f, &buf, &temp_i, &temp_j, &temp_student);

        cmp = comparer_archived_student(&temp_student, &e);

        if (cmp == 0) // temp_student == e
        {
            if (strlen(e.id) > 0)
            {
                if (strcmp(temp_student.id, e.id) == 0)
                {
                    *trouv = true;
                    *eff = (toupper(temp_student.eff) == 'F') ? false : true;
                }
                else
                {
                    *i = temp_i;
                    *j = temp_j;
                }
            }
            else // ignorer le id (pour la requete a intervalle)
            {
                *trouv = true;
                *eff = (toupper(temp_student.eff) == 'F') ? false : true;
            }
        }
        else if (cmp == -1) // temp_student < e
        {
            *i = temp_i;
            *j = temp_j;
        }
        else if (cmp == 1) // temp_student > e
            stop = true;

        if ((temp_i >= entete(&f, ENTETE_NUMERO_DERNIER_BLOC)) && (temp_j >= entete(&f, ENTETE_POS_LIBRE)))
            stop = true;

        free_archived_student_fields(&temp_student);
    }

    fermer(&f);
}

/**
 * @brief chercher un élève (enregistrement ArchivedStudent) dans un fcihier archive selon le ID
 *
 * @param nomf le nom du fichier où se fera la recherche
 * @param id le ID de l'enregistrement ArchivedStudent (l'élève) à chercher
 * @param trouv booléen qui indique si l'enregistrement recherché est trouvé ou non
 * @param eff un booléen qui indique si l'enregistrement recherché est effacé (logiquement) ou non
 * @param i le numéro du bloc où se trouve l'enregistrement recherché
 * @param j la position dans le bloc d'où commence l'enregistrement recherché
 */
void recherche_archived_student_by_id(char nomf[], char id[], bool *trouv, bool *eff, int *i, int *j)
{
    TOVC f;
    Buffer buf;
    ArchivedStudent temp_student;
    bool stop = false;

    *trouv = false;
    *eff = false;
    *i = 1;
    *j = 0;

    ouvrir(&f, nomf, 'A');

    if (entete(&f, ENTETE_NUMERO_DERNIER_BLOC) <= 0) //fichier vide
    {
        fermer(&f);
        return;
    }

    int temp_i = 1, temp_j = 0;
    lire_dir(&f, temp_i, &buf); // lecture du premier bloc
    while ((!(*trouv)) && (!stop))
    {
        lire_archived_student(&f, &buf, &temp_i, &temp_j, &temp_student); //lire (séquentiellement) un enregistrement à partir du buffer

        if (strcmp(temp_student.id, id) == 0) //comparer les identifiants
        {
            *trouv = true;
            *eff = (toupper(temp_student.eff) == 'F') ? false : true;
        }
        else
        {
            *i = temp_i;
            *j = temp_j;
        }

        if ((temp_i >= entete(&f, ENTETE_NUMERO_DERNIER_BLOC)) && (temp_j >= entete(&f, ENTETE_POS_LIBRE)))
            stop = true;

        free_archived_student_fields(&temp_student); //libérer les allocations faites pour temp_student
    }

    fermer(&f);
}

/**
 * @brief insérer un enregistrement ArchivedStudent dans un fichier archive
 *
 * @param nomf le nom du fichier où insérer le fichier
 * @param e l'enregistrement à insérer dans le fichier
 * @param tabInd l'index du fichier archive
 */
void insertion_archived_student_index(char nomf[], ArchivedStudent e, IndexTable *tabInd)
{
    int i, j, k;
    int taille;
    bool trouv, eff;
    ArchivedStudentStr s;
    int pos, ind;

    recherche_archived_student_index(nomf, e, tabInd, &trouv, &eff, &i, &j, &k);

    // insertion dans le fichier
    if (trouv && (!eff))
        return;

    ArchivedStudent_to_ArchivedStudentStr(&e, &s);
    taille = TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES + taille_var_archived_student(&e);
    inserer_chaine(nomf, i, j, taille, s);
    free(s);

    // mise a jour de l'index

    // si la table d'index est vide ==> insertion dans la premiere case
    if (tabInd->taille == 0)
    {
        tabInd->tab[0].classId = e.classId;
        tabInd->tab[0].moyenne_min = ((e.moyenne - 0.5) >= 0) ? e.moyenne - 0.5 : e.moyenne;
        tabInd->tab[0].moyenne_max = ((e.moyenne + 0.5) <= 20) ? e.moyenne + 0.5 : e.moyenne;
        tabInd->tab[0].numBloc = 1;
        tabInd->tab[0].pos = 0;

        tabInd->taille++;
        return;
    }

    // si la classe n'existe pas ==> insertion de la classe
    if ((e.classId.annee != tabInd->tab[k].classId.annee) || (e.classId.numSalle != tabInd->tab[k].classId.numSalle))
    {
        if ((e.classId.annee < tabInd->tab[k].classId.annee) || (((e.classId.annee == tabInd->tab[k].classId.annee)) && (e.classId.numSalle < tabInd->tab[k].classId.numSalle))) // insertion dans la case k
        {
            pos = k + 1;

            for (ind = tabInd->taille; ind >= pos; ind--)
                tabInd->tab[ind] = tabInd->tab[ind-1];

            tabInd->tab[k].classId = e.classId;
            tabInd->tab[k].moyenne_min = ((e.moyenne - 0.5) >= 0) ? e.moyenne - 0.5 : e.moyenne;
            tabInd->tab[k].moyenne_max = ((e.moyenne + 0.5) <= 20) ? e.moyenne + 0.5 : e.moyenne;
            tabInd->tab[k].numBloc = i;
            tabInd->tab[k].pos = j;
        }
        else // (e.classId > tabInd->tab[k].classId) ==> insertion dans la case k+1
        {
            pos = k + 2;

            for (ind = tabInd->taille; ind >= pos; ind--)
                tabInd->tab[ind] = tabInd->tab[ind-1];

            tabInd->tab[k+1].classId = e.classId;
            tabInd->tab[k+1].moyenne_min = ((e.moyenne - 0.5) >= 0) ? e.moyenne - 0.5 : e.moyenne;
            tabInd->tab[k+1].moyenne_max = ((e.moyenne + 0.5) <= 20) ? e.moyenne + 0.5 : e.moyenne;
            tabInd->tab[k+1].numBloc = i;
            tabInd->tab[k+1].pos = j;
        }

        tabInd->taille++;

        // maj(i, j)
        for (ind = pos; ind < tabInd->taille; ind++)
        {
            tabInd->tab[ind].pos += taille;
            if (tabInd->tab[ind].pos >= MAX_CHARS)
            {
                tabInd->tab[ind].numBloc += (tabInd->tab[ind].pos / MAX_CHARS);
                tabInd->tab[ind].pos %= MAX_CHARS;
            }
        }

        return;
    }

    // la classe existe dans l'index

    if ((e.moyenne >= tabInd->tab[k].moyenne_min) && (e.moyenne <= tabInd->tab[k].moyenne_max)) // dans le bon interval
    {
        if (tabInd->tab[k].numBloc == i) // si le bloc ne change pas
        {
            // maj(i,j)
            for (ind = k + 1; ind < tabInd->taille; ind++)
            {
                tabInd->tab[ind].pos += taille;
                if (tabInd->tab[ind].pos >= MAX_CHARS)
                {
                    tabInd->tab[ind].numBloc += (tabInd->tab[ind].pos / MAX_CHARS);
                    tabInd->tab[ind].pos %= MAX_CHARS;
                }
            }

            return;
        }

        // le bloc change ==> eclatement --> insertion d'une nouvelle case dans la position k / k+1

        // voir si il peut etre le min de son suivant
        if ((i == tabInd->tab[k+1].numBloc) && (e.classId.annee == tabInd->tab[k+1].classId.annee) && (e.classId.numSalle == tabInd->tab[k+1].classId.numSalle))
        {
            tabInd->tab[k+1].numBloc = i;
            tabInd->tab[k+1].pos = j;
            tabInd->tab[k+1].moyenne_min = e.moyenne;

            tabInd->tab[k].moyenne_max = e.moyenne;

            // maj(i,j)
            for (ind = k + 2; ind < tabInd->taille; ind++)
            {
                tabInd->tab[ind].pos += taille;
                if (tabInd->tab[ind].pos >= MAX_CHARS)
                {
                    tabInd->tab[ind].numBloc += (tabInd->tab[ind].pos / MAX_CHARS);
                    tabInd->tab[ind].pos %= MAX_CHARS;
                }
            }

            return;
        }

        // insertion d'une nouvelle case dans la position k+1
        for (ind = tabInd->taille; ind >= k + 2; ind--)
            tabInd->tab[ind] = tabInd->tab[ind-1];

        tabInd->tab[k+1].classId = e.classId;
        tabInd->tab[k+1].numBloc = i;
        tabInd->tab[k+1].pos = j;
        tabInd->tab[k+1].moyenne_min = e.moyenne;
        tabInd->tab[k+1].moyenne_max = tabInd->tab[k].moyenne_max;

        tabInd->tab[k].moyenne_max = e.moyenne;

        tabInd->taille++;

        // maj(i,j)
        for (ind = k + 2; ind < tabInd->taille; ind++)
        {
            tabInd->tab[ind].pos += taille;
            if (tabInd->tab[ind].pos >= MAX_CHARS)
            {
                tabInd->tab[ind].numBloc += (tabInd->tab[ind].pos / MAX_CHARS);
                tabInd->tab[ind].pos %= MAX_CHARS;
            }
        }
    }
    else if (e.moyenne < tabInd->tab[k].moyenne_min) // pas dans le bon interval ==> changement des bornes
    {
        tabInd->tab[k].numBloc = i;
        tabInd->tab[k].pos = j;
        tabInd->tab[k].moyenne_min = e.moyenne;

        // maj(i,j)
        for (ind = k + 1; ind < tabInd->taille; ind++)
        {
            tabInd->tab[ind].pos += taille;
            if (tabInd->tab[ind].pos >= MAX_CHARS)
            {
                tabInd->tab[ind].numBloc += (tabInd->tab[ind].pos / MAX_CHARS);
                tabInd->tab[ind].pos %= MAX_CHARS;
            }
        }
    }
    else // e.moyenne > tabInd->tab[k].moyenne_max : pas dans le bon interval ==> changement des bornes
    {
        tabInd->tab[k].moyenne_max = e.moyenne;

        // maj(i,j)
        for (ind = k + 1; ind < tabInd->taille; ind++)
        {
            tabInd->tab[ind].pos += taille;
            if (tabInd->tab[ind].pos >= MAX_CHARS)
            {
                tabInd->tab[ind].numBloc += (tabInd->tab[ind].pos / MAX_CHARS);
                tabInd->tab[ind].pos %= MAX_CHARS;
            }
        }
    }
}

/**
 * @brief supprimer logiquement un enregistrement ArchivedStudent d'un fichier archive
 *
 * @param nomf le nom du fichier où se fera la suppression
 * @param tabInd l'index du fichier archive
 * @param e l'enregistrement à supprimer logiquement du fichier
 */
void suppression_logique_archived_student(char nomf[], IndexTable *tabInd, ArchivedStudent e)
{
    TOVC f;
    Buffer buf;
    bool trouv, eff;
    int i, j, k;

    recherche_archived_student_index(nomf, e, tabInd, &trouv, &eff, &i, &j, &k);
    if ((!trouv) || eff) //le cas ou l'enregistrement n'existe pas ou il est déja effacé
        return;

    j += (TAILLE_CHAMP_TAILLE + TAILLE_CHAMP_CLE); //aller vers la position du champ eff
    if (j >= MAX_CHARS)
    {
        i += (j / MAX_CHARS);
        j %= MAX_CHARS;
    }

    ouvrir(&f, nomf, 'A');

    lire_dir(&f, i, &buf);
    buf.tab[j] = 'V';  //changer le champ eff
    ecrire_dir(&f, i, &buf); //ecrire le buffer avec l'enregistrement effacé

    aff_entete(&f, ENTETE_NBR_CHAR_SUP, entete(&f, ENTETE_NBR_CHAR_SUP) + TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES + taille_var_archived_student(&e));
    fermer(&f);
}

/**
 * @brief reorganiser un fichier dans un nouveau selon les changements faits dans l'ancien (supprimer physiquement les enregistrements effacés logiquement)
 *
 * @param nomf le nom du fichier à reorganiser
 * @param new_nomf le nom du nouveau fichier organisé à créer à partir d'un ancien
 */
void reorganisation_archived_students(char nomf[], char new_nomf[])
{
    TOVC f1, f2;
    Buffer buf1, buf2;
    int i1 = 1, j1 = 0, i2 = 1, j2 = 0, taille;
    ArchivedStudent e;
    ArchivedStudentStr s;

    // ouvrir le fichier original
    ouvrir(&f1, nomf, 'A');
    lire_dir(&f1, i1, &buf1);

    // ouvrir le nouveau fichier
    ouvrir(&f2, new_nomf, 'N');

    // parcourir le fichier original
    for (int k = 0; k < entete(&f1, ENTETE_NBR_ENREG); k++)
    {
        lire_archived_student(&f1, &buf1, &i1, &j1, &e);

        if (e.eff == 'F')
        {
            ArchivedStudent_to_ArchivedStudentStr(&e, &s);
            taille = strlen(s);

            // ajouter l'eleve au nouveau fichier
            ecrire_chaine(&f2, &buf2, &i2, &j2, taille, s);

            // mise a jour de l'entete de nouveau fichier
            aff_entete(&f2, ENTETE_NBR_ENREG, entete(&f2, ENTETE_NBR_ENREG) + 1);
            aff_entete(&f2, ENTETE_NBR_CHAR_INS, entete(&f2, ENTETE_NBR_CHAR_INS) + taille);

            free(s);
        }

        free_archived_student_fields(&e);
    }

    // mise a jour de l'entete de nouveau fichier
    aff_entete(&f2, ENTETE_NUMERO_DERNIER_BLOC, i2);
    aff_entete(&f2, ENTETE_POS_LIBRE, j2);

    // ecriture du caractere de la fin de la chaine (\0) dans le dernier bloc du nouveau fichier
    buf2.tab[j2] = '\0';
    ecrire_dir(&f2, i2, &buf2); // derniere ecriture

    fermer(&f1);
    fermer(&f2);
}


/**
 * @brief faire la recherche des enregistrement dont la moyenne est dans un intervalle donné (requete à intervalle) et les afficher
 *
 * @param nomfArchive le nom du fichier où se fera la requete
 * @param tabInd l'indexe du fichier archive où se fera la requete
 * @param classId la classe où se fera la requete
 * @param moyMin la borne inferieure de l'intervalle
 * @param moyMax la birne supérieure de l'intervalle
 * @return int le nombre d'enregistrement dont la moyenne appartient à l'intervalle donné
 */
int requete_intervalle(char nomfArchive[], IndexTable *tabInd, ClassId classId, float moyMin, float moyMax)
{
    TOVC f;
    Buffer buf;
    ArchivedStudent eMin, e;
    int k, i, j, taille;
    bool trouv, eff;
    char tailleStr[TAILLE_CHAMP_TAILLE + 1];
    char *studentTmp;
    char *studentStr;

    eMin.moyenne = moyMin;
    eMin.classId = classId;
    strcpy(eMin.id, ""); // pour ignorer le id pendant la recherche

    recherche_archived_student_index(nomfArchive, eMin, tabInd, &trouv, &eff, &i, &j, &k);

    ouvrir (&f, nomfArchive, 'A');
    lire_dir(&f, i, &buf);
    int cpt = 0;
    while (i <= entete(&f, ENTETE_NUMERO_DERNIER_BLOC))
    {
        if ((i == entete(&f, ENTETE_NUMERO_DERNIER_BLOC)) && (j >= entete(&f, ENTETE_POS_LIBRE))) //fin du fichier
            break;

        //recupere le champs taille
        for (k = 0; k < TAILLE_CHAMP_TAILLE; k++)
        {
            if (j >= MAX_CHARS)
            {
                j = 0;
                i++;
                lire_dir(&f, i, &buf);
            }

            tailleStr[k] = buf.tab[j];
            j++;
        }
        tailleStr[TAILLE_CHAMP_TAILLE] = '\0';
        strToInt(tailleStr, &taille);

        // recupere les caracteres de l'enregistrement
        studentTmp = calloc(TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES + taille - TAILLE_CHAMP_TAILLE + 1, sizeof(char));
        for (k = 0; k < TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES + taille - TAILLE_CHAMP_TAILLE; k++)
        {
            if (j >= MAX_CHARS)
            {
                j = 0;
                i++;
                lire_dir(&f, i, &buf);
            }

            studentTmp[k] = buf.tab[j];
            j++;
        }
        studentTmp[TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES + taille - TAILLE_CHAMP_TAILLE] = 0;

        // regrouper les 2 parties
        studentStr = calloc(TAILLE_CHAMPS_ARCHIVED_STUDENT_FIXES + taille + 1, sizeof(char));
        strcpy(studentStr, tailleStr);
        strcat(studentStr, studentTmp);
        free(studentTmp);
        ArchivedStudentStr_to_ArchivedStudent(studentStr, &e);
        free(studentStr);

        if((e.moyenne > moyMax) || (e.moyenne < moyMin) || (e.classId.annee != classId.annee) || (e.classId.numSalle != classId.numSalle))
        {
            free_archived_student_fields(&e);
            break;
        }

        // Traitement des resultat de la requete
        afficher_archived_student(&e);
        cpt++;
        //

        free_archived_student_fields(&e);
    }

    fermer(&f);
    return cpt;
}

/**
 * @brief génere le fichier de l'année suivante à partir d'un fichier d'une année courante
 *
 * @param nomf_annee_courante le nom du fichier de l'annee courante à partir duquel la generation se fer
 * @param nomf_next_annee_courante le nom du fichier de la nouvelle année générée
 */
void generate_next_year(char nomf_annee_courante[], char nomf_next_annee_courante[])
{
    int i, j, k, temp_i, temp_j;
    char *temp_files[NBR_CLASSES_PAR_ANNEE * 6];
    TOVC f, temp_f;
    Buffer buf, temp_buf;
    Student e;
    StudentStr s;
    int taille;
    float moyenne;
    int max_note_index;
    FILE *noms_file, *prenoms_file;
    char noms_array[NBR_NOMS][1024];
    char prenoms_array[NBR_PRENOMS][1024];
    char temp_str[1024];
    int min_index_nom, max_index_nom, index_nom;
    int min_index_prenom, max_index_prenom, index_prenom;

    // creation des fichier de la fragmetation
    k = 0;
    for (i = 0; i < 6; i++)
    {
        for (j = 1; j <= NBR_CLASSES_PAR_ANNEE; j++)
        {
            temp_files[k] = calloc(strlen("files/temp/temp_next_year_.tovc") + TAILLE_CHAMP_CLASSID + 1, sizeof(char));
            sprintf(temp_files[k], "files/temp/temp_next_year_%c%d.tovc", '0' + i, j);

            ouvrir(&temp_f, temp_files[k], 'N');
            fermer(&temp_f);

            k++;
        }
    }

    // generation aleatoire de la nouvelle promotion (annee P)

    // chargement fichier des noms
    noms_file = fopen("files/static/noms.txt" , "r");
    if (noms_file == NULL)
    {
        printf("[ERROR] erreur d'ouverture de fichier des noms !\n");
        exit(EXIT_FAILURE);
    }
    // chargement fichier des prenoms et genres
    prenoms_file = fopen("files/static/prenoms.txt", "r");
    if (prenoms_file == NULL)
    {
        printf("[ERROR] erreur d'ouverture de fichier des prenoms !\n");
        fclose(noms_file);
        exit(EXIT_FAILURE);
    }
    /* debut creation des tableaux de manipulation */
    // noms_array
    for (k = 0; k < NBR_NOMS; k++)
    {
        fgets(temp_str, sizeof(temp_str), noms_file);
        temp_str[strlen(temp_str) - 1] = '\0';
        strcpy(noms_array[k], temp_str);
    }
    fclose(noms_file);
    // prenoms_array
    for (k = 0; k < NBR_PRENOMS; k++)
    {
        fgets(temp_str, sizeof(temp_str), prenoms_file);
        temp_str[strlen(temp_str) - 1] = '\0';
        strcpy(prenoms_array[k], temp_str);
    }
    fclose(prenoms_file);

    e.classId.annee = 'P';
    e.eff = 'F';
    for (int numSalle = 1; numSalle <= NBR_CLASSES_PAR_ANNEE; numSalle++) // parcourir toutes les salle de l'annee correspondante
    {
        ouvrir(&temp_f, temp_files[numSalle - 1], 'A');
        i = 1;
        j = 0;

        e.classId.numSalle = numSalle; // generer la partie numero du salle (sequentiel par annee) dans l'identifiant la classe

        // initialisation des bornes des nombre aleatoires generer pour selectionner le nom et le prenom du l'eleve
        min_index_nom = 0;
        min_index_prenom = 0;
        max_index_nom = NBR_NOMS / NBR_ELEVES_MAX_PAR_CLASSE;
        max_index_prenom = NBR_PRENOMS / NBR_ELEVES_MAX_PAR_CLASSE;

        for (k = 0; k < NBR_ELEVES_MAX_PAR_CLASSE; k++) // parcourir les eleves de la classe courante
        {
            strcpy(e.id, IDS_ARRAY[ID_INDEX]); // prendre un id aleatoire depuis le tableau des ids aleatoires
            ID_INDEX++; // mise a jour la position du suivant id aleatoire a prendre

            // affecter des notes aleatoires a l'eleve entre 00.00 et 20.00 selon les matieres etudiees (les autres sont a -1)
            for(int note_index = 0; note_index < MAX_NOTES; note_index ++)
                e.notes[note_index] = (note_index < NBR_MATIERES_ANNEE_P) ? generate_note('P') : (-1.0);

            // generer le nom et le prenom du l'eleve aleatoirement d'une maniere ordonnee

            // choisir le nom depuis le tableau des noms
            index_nom = random_int(min_index_nom, max_index_nom);
            min_index_nom = index_nom;
            max_index_nom += NBR_NOMS / NBR_ELEVES_MAX_PAR_CLASSE;

            // allocation et affectation de e.nom
            e.nom = calloc(strlen(noms_array[index_nom]) + 1, sizeof(char));
            strcpy(e.nom, noms_array[index_nom]);

            // choisir le prenom et le genre depuis le tableau des prenoms et genres
            index_prenom = random_int(min_index_prenom, max_index_prenom);
            min_index_prenom = index_prenom;
            max_index_prenom += NBR_PRENOMS / NBR_ELEVES_MAX_PAR_CLASSE;

            // allocation et affectation de e.prenom
            e.prenom = calloc(strlen(prenoms_array[index_prenom]) + 1, sizeof(char));
            subStr(prenoms_array[index_prenom], e.prenom, 0, strlen(prenoms_array[index_prenom]) - 2);

            // affectation de e.genre
            e.genre = prenoms_array[index_prenom][strlen(prenoms_array[index_prenom]) - 1];

            // creation de la chaine (s) correspondante a l'etudiant (e)
            Student_to_StudentStr(&e, &s);
            //taille = strlen(s);
            taille = TAILLE_CHAMPS_STUDENT_FIXES + taille_champ_notes_student(&e) + taille_var_student(&e);

            // ecriture de la chaine dans le fichier
            ecrire_chaine(&temp_f, &temp_buf, &i, &j, taille, s);

            // mise a jour de l'entete du fichier
            aff_entete(&temp_f, ENTETE_NBR_ENREG, entete(&temp_f, ENTETE_NBR_ENREG) + 1);
            aff_entete(&temp_f, ENTETE_NBR_CHAR_INS, entete(&temp_f, ENTETE_NBR_CHAR_INS) + taille);

            // liberer l'espace memoire alloue, pour ne pas le perdre en passant a l'eleve suivant
            free(s);
            free_student_fields(&e);
        }

        // mise a jour de l'entete
        aff_entete(&temp_f, ENTETE_NUMERO_DERNIER_BLOC, i);
        aff_entete(&temp_f, ENTETE_POS_LIBRE, j);

        temp_buf.tab[j] = '\0';
        ecrire_dir(&temp_f, i, &temp_buf);

        fermer(&temp_f);
    }

    // parcourir le fichier de l'anne courante et inserer chaque eleve selon sa moyenne dans son propre fichier (fragmentation)
    ouvrir(&f, nomf_annee_courante, 'A');
    i = 1;
    j = 0;
    lire_dir(&f, i, &buf);
    while (i <= entete(&f, ENTETE_NUMERO_DERNIER_BLOC))
    {
        if ((i == entete(&f, ENTETE_NUMERO_DERNIER_BLOC)) && (j >= entete(&f, ENTETE_POS_LIBRE)))
            break;

        lire_student(&f, &buf, &i, &j, &e);
        moyenne = moyenne_student(&e);

        if ((e.eff == 'V') || (e.eff == 'v') || ((e.classId.annee == '5') && (moyenne >= 10)))
        {
            free_student_fields(&e);
            continue;
        }

        // si moyenne >= 10 ==> l'etudiant passe a l'annee suivante
        if (moyenne >= 10)
            e.classId.annee = ((e.classId.annee == 'P') || (e.classId.annee == 'p')) ? '1' : (e.classId.annee + 1);

        // regenaration aleatoire des note de la nouvelle annee
        max_note_index = nombre_matieres_annee(e.classId.annee);
        for(int note_index = 0; note_index < MAX_NOTES; note_index ++)
            e.notes[note_index] = (note_index < max_note_index) ? generate_note(e.classId.annee) : (-1.0);

        insertion_student(temp_files[(((e.classId.annee == 'P') || (e.classId.annee == 'p')) ? 0 : (e.classId.annee - '0')) * NBR_CLASSES_PAR_ANNEE + (e.classId.numSalle - 1)], e);
        free_student_fields(&e);
    }
    fermer(&f);

    // fusion des fichier temp precedemment crees
    ouvrir(&f, nomf_next_annee_courante, 'N');
    i = 1;
    j = 0;
    for (k = 0; k < NBR_CLASSES_PAR_ANNEE * 6; k++)
    {
        ouvrir(&temp_f, temp_files[k], 'A');
        temp_i = 1;
        temp_j = 0;
        lire_dir(&temp_f, temp_i, &temp_buf);
        while (temp_i <= entete(&temp_f, ENTETE_NUMERO_DERNIER_BLOC))
        {
            if ((temp_i == entete(&temp_f, ENTETE_NUMERO_DERNIER_BLOC)) && (temp_j >= entete(&temp_f, ENTETE_POS_LIBRE)))
                break;

            lire_student(&temp_f, &temp_buf, &temp_i, &temp_j, &e);
            ecrire_student(&f, &buf, &i, &j, &e);

            aff_entete(&f, ENTETE_NBR_ENREG, entete(&f, ENTETE_NBR_ENREG) + 1);
            aff_entete(&f, ENTETE_NBR_CHAR_INS, entete(&f, ENTETE_NBR_CHAR_INS) + TAILLE_CHAMPS_STUDENT_FIXES + taille_champ_notes_student(&e) + taille_var_student(&e));

            free_student_fields(&e);
        }

        fermer(&temp_f);
        remove(temp_files[k]);
        free(temp_files[k]);
    }

    aff_entete(&f, ENTETE_NUMERO_DERNIER_BLOC, i);
    aff_entete(&f, ENTETE_POS_LIBRE, j);

    buf.tab[j] = '\0';
    ecrire_dir(&f, i, &buf); // derniere ecriture

    fermer(&f);
}

