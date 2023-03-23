#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "utils.h"
#include "TOVC.h"
#include "student.h"
#include "index.h"
#include "archive.h"
#include "functions.h"
#include "conio2.h"
#include "guiTools.h"
#include "gui.h"


int menu_debut()
{
    int h, w;
    getConsoleSize(&w, &h);

    const int nbr_choix = 5;
    char *menu_principal_choix[] = {
        " Creer des nouveau fichiers ",
        " Chargement des fichiers existentes ",
        " Help ",
        " About ",
        " Quiter le programme ",
    };

    int max_len = strlen(menu_principal_choix[0]);
    int len;
    for (int i = 1; i < nbr_choix; i++)
    {
        len = strlen(menu_principal_choix[i]);
        if (len > max_len)
            max_len = len;
    }

    textcolor(YELLOW);
    char *title = "  TP SFSD - Gestion des informations scolaires -  ";
    gotoxy((w-strlen(title))/2, -2 + (h-2*nbr_choix)/2);
    printf("%s", title);

    return menu(menu_principal_choix, nbr_choix, true, 0, (w-max_len)/2, 3+(h-2*nbr_choix)/2, 1, LIGHTCYAN, BLACK, LIGHTMAGENTA, BLACK, true, false);
}

int menu_principal()
{
    int h, w;
    getConsoleSize(&w, &h);

    const int nbr_choix = 16;
    char *menu_principal_choix[] = {
        " Affichage des donnees ",
        " Inserer un eleve (ficher : annee courante) ",
        " Mise a jour des informations d'un eleve (ficher : annee courante) ",
        " Supprimer un eleve (ficher : annee courante) ",
        " Archiver l'annee courante ",
        " Construire l'index de l'archive de l'annee courante ",
        " Sauvegarder la table d'index dans un ficher TOF ",
        " Charger la table d'index depuis un fichier TOF ",
        " Inserer un eleve (ficher : archive) ",
        " Supprimer un eleve (ficher : archive) ",
        " Requete a intervalle selon les moyennes des eleves ",
        " Generer l'annee suivante ",
        " Simulation de 5 annees successives ",
        " Effectuer un transfert ",
        " Retour vers la page precedente ",
        " Quiter le programme ",
    };

    int max_len = strlen(menu_principal_choix[0]);
    int len;
    for (int i = 1; i < nbr_choix; i++)
    {
        len = strlen(menu_principal_choix[i]);
        if (len > max_len)
            max_len = len;
    }

    gotoxy((w-strlen("Annee Courante: xxxx/xxxx"))/2, -3 + (h-nbr_choix)/2);
    textcolor(LIGHTCYAN);
    printf("Annee Courante: ");
    print_annee_courante();

    gotoxy((w-strlen("MENU PRINCIPAL"))/2, (h-nbr_choix)/2);
    textcolor(LIGHTMAGENTA);
    printf("%s", "MENU PRINCIPAL");

    return menu(menu_principal_choix, nbr_choix, true, 0, (w-max_len)/2, 2+(h-nbr_choix)/2, 0, WHITE, BLACK, BLACK, LIGHTMAGENTA, true, false);
}

int menu_affichage()
{
    int h, w;
    getConsoleSize(&w, &h);

    const int nbr_choix = 8;
    char *menu_principal_choix[] = {
        " 1 - Afficher l'entete d'un fichier ",
        " 2 - Afficher un bloc dans un fichier ",
        " 3 - Afficher tous le contenu d'un fichier ",
        " 4 - Afficher la table d'index ",
        " 5 - Afficher les informations d'un etudiant ",
        " 6 - Afficher tous les etudiants dans une classe ",
        " <-- Retour vers la page precedente ",
        " <!> Quiter le programme ",
    };

    int max_len = strlen(menu_principal_choix[0]);
    int len;
    for (int i = 1; i < nbr_choix; i++)
    {
        len = strlen(menu_principal_choix[i]);
        if (len > max_len)
            max_len = len;
    }

    textcolor(LIGHTMAGENTA);
    char *title = "Affichage des donnees";
    gotoxy((w-strlen(title))/2, -2+(h-2*nbr_choix)/2);
    printf("%s", title);

    return menu(menu_principal_choix, nbr_choix, false, 0, (w-max_len)/2, 3 + (h-2*nbr_choix)/2, 1, WHITE, BLACK, LIGHTMAGENTA, BLACK, true, false);
}



void choix_0()
{
    TOVC f;
    Buffer buf;
    int i, j;
    bool trouv, eff;
    char file_name[256];
    char id[TAILLE_CHAMP_CLE + 1];
    int scanf_code;
    Student e;
    ClassId class_id;
    char annee[10];
    int choix;
    char *choices[] = {
        "Annee Scolaire",
        "Archive",
    };


    while (true)
    {
        system("cls");
        clrscr();

        int w, h;
        getConsoleSize(&w, &h);

        switch (menu_affichage())
        {
            case 0: // Afficher l'entete d'un fichier
            {
                gotoxy((w-strlen("Voulez-vous consulter les donnees d'un fichier d'une annee scolaire ou d'un fichier d'archive ?"))/2, 2);
                textcolor(YELLOW);
                printf("Voulez-vous consulter les donnees d'un fichier d'une annee scolaire ou d'un fichier d'archive ?");
                choix = menu(choices, 2, true, 0, (w-strlen("Annee Scolaire"))/2, 4, 0, LIGHTRED, BLACK, LIGHTGREEN, BLACK, true, false);

                get_annee_courante(annee);

                if (choix == 0)
                    fileFullPath(annee, FILE_ANNEE_COURANTE, file_name);
                else if (choix == 1)
                    fileFullPath(annee, FILE_ARCHIVE, file_name);

                if (!isFileExiste(file_name))
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] Le fichier correspondant a l'annee que vous avez entre n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

                system("cls");
                clrscr();

                ouvrir(&f, file_name, 'A');
                textcolor(WHITE);
                afficher_entete(&f);
                fermer(&f);

                textcolor(YELLOW);
                printf("\n");
                system("pause");
                textcolor(LIGHTGRAY);
                break;
            }

            case 1: // Afficher un bloc dans un fichier
            {
                gotoxy((w-strlen("Voulez-vous consulter les donnees d'un fichier d'une annee scolaire ou d'un fichier d'archive ?"))/2, 2);
                textcolor(YELLOW);
                printf("Voulez-vous consulter les donnees d'un fichier d'une annee scolaire ou d'un fichier d'archive ?");
                choix = menu(choices, 2, true, 0, (w-strlen("Annee Scolaire"))/2, 4, 0, LIGHTRED, BLACK, LIGHTGREEN, BLACK, true, false);

                get_annee_courante(annee);

                if (choix == 0)
                    fileFullPath(annee, FILE_ANNEE_COURANTE, file_name);
                else if (choix == 1)
                    fileFullPath(annee, FILE_ARCHIVE, file_name);

                if (!isFileExiste(file_name))
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] Le fichier correspondant a l'annee que vous avez entre n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

get_numBloc:
                ouvrir(&f, file_name, 'A');
                system("color 07");
                printf("\n\tEntrer le numero du bloc a affiche: (entre 1 et %d) ", entete(&f, ENTETE_NUMERO_DERNIER_BLOC));
                fflush(stdin);
                scanf_code = scanf("%d", &i);
                if ((scanf_code != 1) || (i < 1) || (i > entete(&f, ENTETE_NUMERO_DERNIER_BLOC)))
                {
                    system("color 47");
                    deleteConsoleLines(-3);
                    goto get_numBloc;
                }

                system("cls");
                clrscr();

                textcolor(WHITE);
                afficher_bloc(&f, i);
                fermer(&f);

                textcolor(YELLOW);
                printf("\n");
                system("pause");
                textcolor(LIGHTGRAY);
                break;
            }

            case 2: // Afficher tous le contenu d'un fichier
            {
                gotoxy((w-strlen("Voulez-vous consulter les donnees d'un fichier d'une annee scolaire ou d'un fichier d'archive ?"))/2, 2);
                textcolor(YELLOW);
                printf("Voulez-vous consulter les donnees d'un fichier d'une annee scolaire ou d'un fichier d'archive ?");
                choix = menu(choices, 2, true, 0, (w-strlen("Annee Scolaire"))/2, 4, 0, LIGHTRED, BLACK, LIGHTGREEN, BLACK, true, false);

                get_annee_courante(annee);

                if (choix == 0)
                    fileFullPath(annee, FILE_ANNEE_COURANTE, file_name);
                else if (choix == 1)
                    fileFullPath(annee, FILE_ARCHIVE, file_name);

                if (!isFileExiste(file_name))
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] Le fichier correspondant a l'annee que vous avez entre n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

                system("cls");
                clrscr();

                ouvrir(&f, file_name, 'A');
                afficher_fichier(&f);
                fermer(&f);

                textcolor(YELLOW);
                printf("\n");
                system("pause");
                textcolor(LIGHTGRAY);
                break;
            }

            case 3: // Afficher la table d'index
            {
                if (GIndexExist)
                {
                    textcolor(WHITE);
                    afficher_index(&GIndex);
                    printf("\n\n\t");
                    textcolor(LIGHTGRAY);
                    system("pause");
                }
                else
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] La table d'index de fichier archive pour l'annee courante n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

                break;
            }

            case 4: // Afficher les informations d'un etudiant
            {
                fileFullPath(annee_courante, FILE_ANNEE_COURANTE, file_name);
                if (!isFileExiste(file_name))
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] Le fichier de l'annee courante n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

get_id:
                system("color 07");
                printf("\n\tVeuillez entrer le ID du l'eleve: (sur %d characteres numeriques) ", TAILLE_CHAMP_CLE);
                fflush(stdin);
                fgets(id, TAILLE_CHAMP_CLE + 1, stdin);
                fflush(stdin);
                id[TAILLE_CHAMP_CLE] = '\0';

                if ((strlen(id) != TAILLE_CHAMP_CLE) || (strspn(id, "0123456789") != TAILLE_CHAMP_CLE))
                {
                    system("color 47");
                    deleteConsoleLines(-3);
                    goto get_id;
                }

                recherche_student_by_id(file_name, id, &trouv, &eff, &i, &j);
                if (!trouv)
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\tLe id %s n'existe pas dans le fichier de l'annee courante\n\n", id);
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

                ouvrir(&f, file_name, 'A');
                lire_dir(&f, i, &buf);
                printf("\n\n");
                textcolor(LIGHTCYAN);
                printf("\t[ Bloc     : %d ]\n", i);
                textcolor(LIGHTMAGENTA);
                printf("\t[ Position : %d ]\n\n", j);
                lire_student(&f, &buf, &i, &j, &e);
                textcolor(LIGHTGREEN);
                afficher_student(&e);
                textcolor(LIGHTGRAY);
                printf("\n\n");
                free_student_fields(&e);
                fermer(&f);

                printf("\t");
                system("pause");

                break;
            }

            case 5: // Afficher tous les etudiants dans une classe
            {
                fileFullPath(annee_courante, FILE_ANNEE_COURANTE, file_name);
                if (!isFileExiste(file_name))
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] Le fichier de l'annee courante n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

get_niveau:
                system("color 07");
                printf("\n\tVeuillez entrer le niveau: (entre P et 5) ");
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
                printf("\n\tVeuillez entrer le numero de salle: (entre 1 et %d) ", NBR_CLASSES_PAR_ANNEE);
                fflush(stdin);
                scanf_code = scanf("%d", &class_id.numSalle);
                if ((scanf_code != 1) || (class_id.numSalle < 1) || (class_id.numSalle > NBR_CLASSES_PAR_ANNEE))
                {
                    system("color 47");
                    deleteConsoleLines(-3);
                    goto get_salle;
                }

                e = create_student("", "", "", 'x', class_id.annee, class_id.numSalle);
                recherche_student(file_name, e, &trouv, &eff, &i, &j);
                free_student_fields(&e);

                int count = 0;
                system("cls");
                clrscr();

                textcolor(YELLOW);
                printf("\n\tLes etudians dans la classe \"%c - %d\" sont: \n\n", toupper(class_id.annee), class_id.numSalle);

                ouvrir(&f, file_name, 'A');
                lire_dir(&f, i, &buf);
                while (i <= entete(&f, ENTETE_NUMERO_DERNIER_BLOC))
                {
                    if ((i == entete(&f, ENTETE_NUMERO_DERNIER_BLOC)) && (j >= entete(&f, ENTETE_POS_LIBRE)))
                        break;

                    lire_student(&f, &buf, &i, &j, &e);

                    if((toupper(e.classId.annee) != toupper(class_id.annee)) || (e.classId.numSalle != class_id.numSalle))
                    {
                        free_student_fields(&e);
                        break;
                    }
                    (count % 2) ? textcolor(LIGHTMAGENTA): textcolor(LIGHTCYAN);
                    afficher_student(&e);
                    free_student_fields(&e);
                    count++;
                }
                fermer(&f);

                textcolor(YELLOW);
                printf("\n\n\t%d etudiants trouves dans la classe \"%c - %d\"", count, toupper(class_id.annee), class_id.numSalle);

                textcolor(LIGHTGRAY);
                printf("\n\n\t");
                system("pause");

                break;
            }

            case 6: // Retour vers la page precedente
            {
                return;
                break;
            }

            case 7: // Quiter le programme
            {
                fin_programme();
                break;
            }
        }
    }

    system("cls");
    clrscr();
    system("color 07");
}

void choix_1()
{
    Student e;
    char nomf[256];
    char *choices[] = {
        "Oui, inserer",
        "Non, annuler",
    };
    int w, h;
    getConsoleSize(&w, &h);

    fileFullPath(annee_courante, FILE_ANNEE_COURANTE, nomf);
    if (!isFileExiste(nomf))
    {
        textcolor(LIGHTRED);
        printf("\n\n\t[ERROR] Le fichier de l'annee courante n'existe pas !\n\n");
        textcolor(LIGHTGRAY);
        printf("\t");
        system("pause");
        return;
    }

    e = get_student(2);

    textcolor(YELLOW);
    printf("\n\tVeuillez confirmer l'insertion de l'etudiant suivant dans le fichier de l'annee courante\n\n");
    textcolor(WHITE);
    afficher_student(&e);

    int choix = menu(choices, 2, true, 0, (w-strlen("Oui, inserer"))/2, wherey() + 2, 0, LIGHTRED, BLACK, LIGHTGREEN, BLACK, true, false);
    if (choix == 0)
    {
        clock_t start_time = clock();
        insertion_student(nomf, e);
        clock_t end_time = clock();

        textcolor(LIGHTGREEN);
        printf("\n\tL'operation est terminee avec succes dans %.2f seconde\n\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);

        textcolor(LIGHTGRAY);
        printf("\t");
        system("pause");
    }

    system("cls");
    clrscr();
    textcolor(LIGHTGRAY);
    free_student_fields(&e);
}

void choix_2()
{
    char file_name[256];
    char id[TAILLE_CHAMP_CLE + 1];
    int i, j;
    bool trouv, eff;
    TOVC f;
    Buffer buf;
    int w, h;
    Student e, e2;
    int choix1, choix2;
    char *choices_champs[] = {
        "ID",
        "Nom",
        "Prenom",
        "Genre",
        "Niveau",
        "Numero de salle",
        "Notes",
    };
    char *choices_confirm[] = {
        "Oui, modifier",
        "Non, annuler",
    };

    fileFullPath(annee_courante, FILE_ANNEE_COURANTE, file_name);
    if (!isFileExiste(file_name))
    {
        textcolor(LIGHTRED);
        printf("\n\n\t[ERROR] Le fichier de l'annee courante a archive n'existe pas !");
        textcolor(LIGHTGRAY);
        printf("\n\n\t");
        system("pause");
        return;
    }

    getConsoleSize(&w, &h);
    gotoxy((w-strlen("ENTRER LE ID DE L'ELEVE A MODIFIER ..."))/2, 2);
    printf("ENTRER LE ID DE L'ELEVE A MODIFIER ...\n\n");

get_id1:
    system("color 07");
    printf("\n\tEntrer le ID du l'eleve: (sur %d characteres numeriques) ", TAILLE_CHAMP_CLE);
    fflush(stdin);
    fgets(id, TAILLE_CHAMP_CLE + 1, stdin);
    id[TAILLE_CHAMP_CLE] = '\0';
    if ((strlen(id) != TAILLE_CHAMP_CLE) || (strspn(id, "0123456789") != TAILLE_CHAMP_CLE))
    {
        system("color 47");
        deleteConsoleLines(-3);
        goto get_id1;
    }

    fflush(stdin);
    system("cls");
    clrscr();

    recherche_student_by_id(file_name, id, &trouv, &eff, &i, &j);
    if (trouv)
    {
        ouvrir(&f, file_name, 'A');
        lire_dir(&f, i, &buf);
        lire_student(&f, &buf, &i, &j, &e);
        fermer(&f);

        if (eff)
        {
            textcolor(YELLOW);
            printf("\n\tL'etudiant suivant est efface, vous ne pouvez pas le modifier\n\n");
            textcolor(LIGHTRED);
            afficher_student(&e);
            free_student_fields(&e);
            textcolor(LIGHTGRAY);
            printf("\n\n\t");
            system("pause");
            system("cls");
            clrscr();
            return;
        }
        else // effectuer la modification d'un seul champ
        {
            textcolor(YELLOW);
            gotoxy((w-strlen("Voulez vous modifier les informations de l'eleve suivant ?"))/2, 2);
            printf("Voulez vous modifier les informations de l'eleve suivant ?\n\n");

            textcolor(WHITE);
            afficher_student(&e);

            choix2 = menu(choices_confirm, 2, true, 0, (w-strlen("Oui, modifier"))/2, wherey()+1, 0, LIGHTRED, BLACK, LIGHTGREEN, BLACK, true, false);
            if (choix2 == 1)
            {
                free_student_fields(&e);
                return;
            }

            e2 = copy_student(e);
            const int TAILLE_CHAINE_MAX = 1024;
            int scanf_code;
            char nom[TAILLE_CHAINE_MAX];
            char prenom[TAILLE_CHAINE_MAX];

            textcolor(YELLOW);
            gotoxy((w-strlen("Veuillez selectionner le champ a modifier"))/2, 2);
            printf("Veuillez selectionner le champ a modifier\n\n");
            choix1 = menu(choices_champs, 7, true, 0, (w-strlen("Numero de salle"))/2, wherey(), 0, LIGHTCYAN, BLACK, LIGHTMAGENTA, BLACK, true, false);
            if (choix1 == 0) // id
            {
get_id2:
                system("color 07");
                printf("\n\tEntrer le nouveau ID du l'eleve: (sur %d characteres numeriques) ", TAILLE_CHAMP_CLE);
                fflush(stdin);
                fgets(e2.id, TAILLE_CHAMP_CLE + 1, stdin);
                e2.id[TAILLE_CHAMP_CLE] = '\0';
                if ((strlen(e2.id) != TAILLE_CHAMP_CLE) || (strspn(e2.id, "0123456789") != TAILLE_CHAMP_CLE))
                {
                    system("color 47");
                    deleteConsoleLines(-3);
                    goto get_id2;
                }

                printf("\n");
            }
            else if (choix1 == 1) // nom
            {
                printf("\n\tEntrer le nouveau nom du l'eleve: ");
                fflush(stdin);
                fgets(nom, TAILLE_CHAINE_MAX, stdin);
                fflush(stdin);
                nom[strlen(nom) - 1] = '\0';
                free(e2.nom);
                e2.nom = calloc(strlen(nom) + 1, sizeof(char));
                strcpy(e2.nom, nom);
                printf("\n");
            }
            else if (choix1 == 2) // prenom
            {
                printf("\n\tEntrer le nouveau prenom du l'eleve: ");
                fflush(stdin);
                fgets(prenom, TAILLE_CHAINE_MAX, stdin);
                fflush(stdin);
                prenom[strlen(prenom) - 1] = '\0';
                free(e2.prenom);
                e2.prenom = calloc(strlen(prenom) + 1, sizeof(char));
                strcpy(e2.prenom, prenom);
                printf("\n");
            }
            else if (choix1 == 3) // genre
            {
                if (toupper(e2.genre) == 'F')
                    e2.genre = 'M';
                else if (toupper(e2.genre) == 'M')
                    e2.genre = 'F';

                printf("\n");
            }
            else if (choix1 == 4) // niveau
            {
get_niveau:
                system("color 07");
                printf("\n\tEntrer le nouveau niveau du l'eleve: (entre P et 5) ");
                fflush(stdin);
                scanf_code = scanf("%c", &e2.classId.annee);
                if ((scanf_code != 1) || ((toupper(e2.classId.annee) != 'P') && ((e2.classId.annee < '1') || (e2.classId.annee > '5'))))
                {
                    system("color 47");
                    deleteConsoleLines(-3);
                    goto get_niveau;
                }

                if (e.classId.annee != e2.classId.annee)
                {
                    int max_note_index = nombre_matieres_annee(e2.classId.annee);

                    for(int i = 0; i < MAX_NOTES; i ++)
                        e2.notes[i] = (i < max_note_index) ? generate_note(e2.classId.annee) : (-1.0);

                    gotoxy((w-strlen("Les notes de l'eleve dans le nouveau niveau ont ete genere aleatoirement"))/2, wherey() + 1);
                    textcolor(YELLOW);
                    printf("Les notes de l'eleve dans le nouveau niveau ont ete genere aleatoirement\n");
                    textcolor(LIGHTGRAY);
                    gotoxy((w-strlen("Pour les changer veuillez lancer la mise a jour a nouveau"))/2, wherey());
                    textcolor(YELLOW);
                    printf("Pour les changer veuillez lancer la mise a jour a nouveau\n\n");
                    textcolor(LIGHTGRAY);
                }
            }
            else if (choix1 == 5) // numero de salle
            {
get_salle:
                system("color 07");
                printf("\n\tEntrer le nouveau numero de salle du l'eleve: (entre 1 et %d) ", NBR_CLASSES_PAR_ANNEE);
                fflush(stdin);
                scanf_code = scanf("%d", &e2.classId.numSalle);
                if ((scanf_code != 1) || (e2.classId.numSalle < 1) || (e2.classId.numSalle > NBR_CLASSES_PAR_ANNEE))
                {
                    system("color 47");
                    deleteConsoleLines(-3);
                    goto get_salle;
                }

                printf("\n");
            }
            else if (choix1 == 6) // notes
            {
                int max_note_index = nombre_matieres_annee(e2.classId.annee);
                printf("\n");
                for(int i = 0; i < max_note_index; i++)
                {
get_note:
                    system("color 07");
                    printf("\tVeuillez entrer la nouvelle note de la matiere \"%s\": (entre 00.00 et 20.00) ", MATIERES[(toupper(e2.classId.annee) == 'P') ? 0 : (e2.classId.annee - '0')][i]) ;
                    fflush(stdin);
                    scanf_code = scanf("%f", &e2.notes[i]);
                    if ((scanf_code != 1) || (e2.notes[i] < 0) || (e2.notes[i] > 20))
                    {
                        system("color 47");
                        deleteConsoleLines(-2);
                        goto get_note;
                    }
                }

                for (int i = max_note_index; i < MAX_NOTES; i++)
                    e2.notes[i] = -1.0;

                printf("\n");
            }

            textcolor(YELLOW);
            gotoxy((w-strlen("Voulez vous sauvegarder ces modifications dans le fichier ?"))/2, wherey());
            printf("Voulez vous sauvegarder ces modifications dans le fichier ?\n\n");

            textcolor(LIGHTGREEN);
            afficher_student(&e2);

            choix2 = menu(choices_confirm, 2, true, 0, (w-strlen("Oui, modifier"))/2, wherey()+1, 0, LIGHTRED, BLACK, LIGHTGREEN, BLACK, true, false);
            if (choix2 == 0)
            {
                fflush(stdin);
                system("cls");
                clrscr();

                clock_t start_time = clock();
                bool isModified = mise_a_jour_student(file_name, e, e2);
                clock_t end_time = clock();

                if (isModified)
                {
                    textcolor(LIGHTGREEN);
                    printf("\n\tLa modification est terminee avec succes dans %.2f secondes !\n\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);
                    free_student_fields(&e2);
                    free_student_fields(&e);
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                }
                else
                {
                    textcolor(LIGHTRED);
                    printf("\n\t[WARNING] Vos modifications ne sont pas autorise !\n\n");
                    free_student_fields(&e2);
                    free_student_fields(&e);
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                }
            }
            else if (choix2 == 1)
            {
                free_student_fields(&e);
                free_student_fields(&e2);
                return;
            }
        }
    }
    else
    {
        textcolor(LIGHTRED);
        printf("\n\n\tL'etudiant que vous voulez modifier n'existe pas dans le fichier de l'annee courante !");
        textcolor(LIGHTGRAY);
        printf("\n\n\t");
        system("pause");
        system("cls");
        clrscr();
        return;
    }
}

void choix_3()
{
    char file_name[256];
    ClassId class_id;
    char id[TAILLE_CHAMP_CLE + 1];
    char nom[1024];
    char prenom[1024];
    Student e;
    int i, j;
    bool trouv, eff;
    int h, w;
    int scanf_code;
    TOVC f;
    Buffer buf;
    clock_t start_time, end_time;
    char *choices[] = {
        "Oui, supprimer",
        "Non, annuler",
    };

    fileFullPath(annee_courante, FILE_ANNEE_COURANTE, file_name);
    if (!isFileExiste(file_name))
    {
        textcolor(LIGHTRED);
        printf("\n\n\t[ERROR] Le fichier de l'annee courante a archive n'existe pas !\n\n");
        textcolor(LIGHTGRAY);
        printf("\t");
        system("pause");
        return;
    }

    getConsoleSize(&w, &h);
    gotoxy((w-strlen("ENTRER LES INFORMATIONS DE L'ELEVE A EFFACE ..."))/2, 2);
    printf("ENTRER LES INFORMATIONS DE L'ELEVE A EFFACE ...");

    printf("\n\n\n\tEntrer le nom du l'eleve: ");
    fflush(stdin);
    fgets(nom, 1024, stdin);
    nom[strlen(nom) - 1] = '\0';

    printf("\n\tEntrer le prenom du l'eleve: ");
    fflush(stdin);
    fgets(prenom, 1024, stdin);
    prenom[strlen(prenom) - 1] = '\0';

get_id:
    system("color 07");
    printf("\n\tEntrer le ID du l'eleve: (sur %d characteres numeriques) ", TAILLE_CHAMP_CLE);
    fflush(stdin);
    fgets(id, TAILLE_CHAMP_CLE + 1, stdin);
    id[TAILLE_CHAMP_CLE] = '\0';
    if ((strlen(id) != TAILLE_CHAMP_CLE) || (strspn(id, "0123456789") != TAILLE_CHAMP_CLE))
    {
        system("color 47");
        deleteConsoleLines(-3);
        goto get_id;
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

    fflush(stdin);
    system("cls");
    clrscr();

    e = create_student(id, nom, prenom, 'x', class_id.annee, class_id.numSalle);
    recherche_student(file_name, e, &trouv, &eff, &i, &j);
    free_student_fields(&e);

    if (trouv)
    {
        ouvrir(&f, file_name, 'A');
        lire_dir(&f, i, &buf);
        lire_student(&f, &buf, &i, &j, &e);
        fermer(&f);

        if (eff)
        {
            textcolor(YELLOW);
            printf("\n\tL'etudiant suivant est deja efface\n\n");
            textcolor(LIGHTRED);
            afficher_student(&e);
            textcolor(LIGHTGRAY);
            printf("\n\n\t");
            system("pause");
            system("cls");
            clrscr();
            free_student_fields(&e);
            return;
        }
        else
        {
            textcolor(YELLOW);
            printf("\n\tVeuillez confirmer la suppression de l'etudiant suivant depuis le fichier de l'annee courante\n\n");
            textcolor(WHITE);
            afficher_student(&e);

            int choix = menu(choices, 2, true, 0, (w-strlen("Oui, supprimer"))/2, wherey() + 2, 0, LIGHTRED, BLACK, LIGHTGREEN, BLACK, true, false);
            if (choix == 0)
            {
                fflush(stdin);
                system("cls");
                clrscr();

                start_time = clock();
                suppression_logique_student(file_name, e);
                end_time = clock();

                textcolor(LIGHTGREEN);
                printf("\n\tLa suppression du l'eleve suivant est terminee avec succes dans %.2f secondes !\n\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);
                afficher_student(&e);
                free_student_fields(&e);
            }
            else if (choix == 1)
            {
                textcolor(LIGHTGRAY);
                free_student_fields(&e);
                return;
            }
        }
    }
    else
    {
        textcolor(LIGHTRED);
        printf("\n\n\tL'etudiant que vous voulez supprimer n'existe pas dans le fichier de l'annee courante !");
        textcolor(LIGHTGRAY);
        printf("\n\n\t");
        system("pause");
        system("cls");
        clrscr();
        return;
    }

    ouvrir(&f, file_name, 'A');
    if (taux_chargement(&f) > MAX_TAUX_CHARGEMENT)
    {
        textcolor(YELLOW);
        printf("\n\t[WARNING]Le taux de chargement (%.2f) du ficher de l'annee courante est tres eleve!\n", taux_chargement(&f));
        printf("\t[WARNING]Une re-organisation va se commencer dans un instant... ");

        fermer(&f);

        char temp_nomf[256];
        fileFullPath("temp", FILE_ANNEE_COURANTE, temp_nomf);
        start_time = clock();
        reorganisation_students(file_name, temp_nomf);
        end_time = clock();
        remove(file_name);
        rename(temp_nomf, file_name);

        textcolor(LIGHTGREEN);
        printf("\n\n\tLa re-organisation du fichier de l'annee courante est terminee avec succes dans %.2f secondes !\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);
    }
    else
    {
        fermer(&f);
    }

    textcolor(LIGHTGRAY);
    printf("\n\t");
    system("pause");
}

void choix_8()
{
    ArchivedStudent e;
    char nomf[256];
    char *choices[] = {
        "Oui, inserer",
        "Non, annuler",
    };
    int w, h;
    getConsoleSize(&w, &h);

    fileFullPath(annee_courante, FILE_ARCHIVE, nomf);
    if (!isFileExiste(nomf))
    {
        textcolor(LIGHTRED);
        printf("\n\n\t[ERROR] Le fichier archive de l'annee courante n'existe pas !\n\n");
        textcolor(LIGHTGRAY);
        printf("\t");
        system("pause");
        return;
    }

    if (!GIndexExist)
    {
        textcolor(LIGHTRED);
        printf("\n\n\t[ERROR] La table d'index du l'archive de l'annee courante n'existe pas !\n\n");
        textcolor(LIGHTGRAY);
        printf("\t");
        system("pause");
        return;
    }

    e = get_archived_student(1);

    int i, j;
    bool trouv, eff;
    recherche_archived_student_by_id(nomf, e.id, &trouv, &eff, &i, &j);
    if (trouv)
    {
        textcolor(LIGHTRED);
        printf("\n\n\t[WARNING] Le ID que vous avez entre existe deja dans le fichier !\n");
        printf("\t[WARNING] Vous ne pouvez pas inserer un eleve avec ce ID %s dans l'archive !\n\n", e.id);
        free_archived_student_fields(&e);
        textcolor(LIGHTGRAY);
        printf("\t");
        system("pause");
        return;
    }

    textcolor(YELLOW);
    printf("\n\tVeuillez confirmer l'insertion de l'eleve suivant dans le fichier archive de l'annee courante\n\n");
    textcolor(WHITE);
    afficher_archived_student(&e);

    int choix = menu(choices, 2, true, 0, (w-strlen("Oui, inserer"))/2, wherey() + 2, 0, LIGHTRED, BLACK, LIGHTGREEN, BLACK, true, false);
    if (choix == 0)
    {
        clock_t start_time = clock();
        insertion_archived_student_index(nomf, e, &GIndex);
        clock_t end_time = clock();

        textcolor(LIGHTGREEN);
        printf("\n\tL'operation est terminee avec succes dans %.2f seconde\n\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);

        textcolor(LIGHTGRAY);
        printf("\t");
        system("pause");
    }

    system("cls");
    clrscr();
    textcolor(LIGHTGRAY);
    free_archived_student_fields(&e);
}

void choix_9()
{
    ArchivedStudent e;
    int w, h;
    TOVC f;
    Buffer buf;
    int i, j;
    bool trouv, eff;
    char nomf[256];
    char id[TAILLE_CHAMP_CLE + 1];
    char *choices[] = {
        "Oui, supprimer",
        "Non, annuler",
    };

    fileFullPath(annee_courante, FILE_ARCHIVE, nomf);
    if (!isFileExiste(nomf))
    {
        textcolor(LIGHTRED);
        printf("\n\n\t[ERROR] Le fichier archive de l'annee courante n'existe pas !\n\n");
        textcolor(LIGHTGRAY);
        printf("\t");
        system("pause");
        return;
    }

    if (!GIndexExist)
    {
        textcolor(LIGHTRED);
        printf("\n\n\t[ERROR] La table d'index du l'archive de l'annee courante n'existe pas !\n\n");
        textcolor(LIGHTGRAY);
        printf("\t");
        system("pause");
        return;
    }

    getConsoleSize(&w, &h);
    gotoxy((w-strlen("ENTRER LE ID DE L'ELEVE A SUPPRIMER ..."))/2, 2);
    printf("ENTRER LE ID DE L'ELEVE A SUPPRIMER ...\n\n");

get_id:
    system("color 07");
    printf("\n\tEntrer le ID du l'eleve: (sur %d characteres numeriques) ", TAILLE_CHAMP_CLE);
    fflush(stdin);
    fgets(id, TAILLE_CHAMP_CLE + 1, stdin);
    id[TAILLE_CHAMP_CLE] = '\0';
    if ((strlen(id) != TAILLE_CHAMP_CLE) || (strspn(id, "0123456789") != TAILLE_CHAMP_CLE))
    {
        system("color 47");
        deleteConsoleLines(-3);
        goto get_id;
    }

    fflush(stdin);
    system("cls");
    clrscr();

    recherche_archived_student_by_id(nomf, id, &trouv, &eff, &i, &j);
    if (trouv)
    {
        ouvrir(&f, nomf, 'A');
        lire_dir(&f, i, &buf);
        lire_archived_student(&f, &buf, &i, &j, &e);
        fermer(&f);

        if (eff)
        {
            textcolor(YELLOW);
            printf("\n\tL'etudiant suivant est deja efface\n\n");
            textcolor(LIGHTRED);
            afficher_archived_student(&e);
            textcolor(LIGHTGRAY);
            printf("\n\n\t");
            system("pause");
            system("cls");
            clrscr();
            free_archived_student_fields(&e);
            return;
        }
        else
        {
            textcolor(YELLOW);
            printf("\n\tVeuillez confirmer la suppression de l'etudiant suivant depuis l'archive de l'annee courante\n\n");
            textcolor(WHITE);
            afficher_archived_student(&e);

            int choix = menu(choices, 2, true, 0, (w-strlen("Oui, supprimer"))/2, wherey() + 2, 0, LIGHTRED, BLACK, LIGHTGREEN, BLACK, true, false);
            if (choix == 0)
            {
                fflush(stdin);
                system("cls");
                clrscr();

                clock_t start_time = clock();
                suppression_logique_archived_student(nomf, &GIndex, e);
                clock_t end_time = clock();

                textcolor(LIGHTGREEN);
                printf("\n\tLa suppression du l'eleve suivant est terminee avec succes dans %.2f secondes !\n\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);
                afficher_archived_student(&e);
                free_archived_student_fields(&e);
            }
            else if (choix == 1)
            {
                textcolor(LIGHTGRAY);
                free_archived_student_fields(&e);
                return;
            }
        }
    }
    else
    {
        textcolor(LIGHTRED);
        printf("\n\n\tL'etudiant que vous voulez supprimer n'existe pas dans l'archive de l'annee courante !");
        textcolor(LIGHTGRAY);
        printf("\n\n\t");
        system("pause");
        system("cls");
        clrscr();
        return;
    }

    ouvrir(&f, nomf, 'A');
    if (taux_chargement(&f) > MAX_TAUX_CHARGEMENT)
    {
        textcolor(YELLOW);
        printf("\n\t[WARNING] Le taux de chargement (%.2f) du l'archive de l'annee courante est tres eleve!\n", taux_chargement(&f));
        printf("\t[WARNING] Une re-organisation va se commencer dans un instant... ");

        fermer(&f);

        char temp_nomf[256];
        fileFullPath("temp", FILE_ARCHIVE, temp_nomf);

        clock_t start_time = clock();

        reorganisation_archived_students(nomf, temp_nomf);
        create_index(temp_nomf, &GIndex);
        GIndexExist = true;

        char index_nomf[256];
        fileFullPath(annee_courante, FILE_INDEX, index_nomf);
        sauvegarder_index(index_nomf, &GIndex);

        clock_t end_time = clock();

        remove(nomf);
        rename(temp_nomf, nomf);

        textcolor(LIGHTGREEN);
        printf("\n\n\tLa re-organisation de l'archive de l'annee courante est terminee avec succes dans %.2f secondes !\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);
    }
    else
    {
        fermer(&f);
    }

    textcolor(LIGHTGRAY);
    printf("\n\t");
    system("pause");
}

void choix_13()
{
    Student e;
    ArchivedStudent e2;
    char nomf_annee[256];
    char nomf_archive[256];
    char *choices[] = {
        "Oui, inserer",
        "Non, annuler",
    };
    int w, h;

    fileFullPath(annee_courante, FILE_ANNEE_COURANTE, nomf_annee);
    if (!isFileExiste(nomf_annee))
    {
        textcolor(LIGHTRED);
        printf("\n\n\t[ERROR] Le fichier de l'annee courante n'existe pas !\n\n");
        textcolor(LIGHTGRAY);
        printf("\t");
        system("pause");
        return;
    }

    fileFullPath(annee_courante, FILE_ARCHIVE, nomf_archive);
    if (!isFileExiste(nomf_archive))
    {
        textcolor(LIGHTRED);
        printf("\n\n\t[ERROR] Le fichier archive de l'annee courante n'existe pas !\n\n");
        textcolor(LIGHTGRAY);
        printf("\t");
        system("pause");
        return;
    }

    if (!GIndexExist)
    {
        textcolor(LIGHTRED);
        printf("\n\n\t[ERROR] La table d'index du l'archive de l'annee courante n'existe pas !\n\n");
        textcolor(LIGHTGRAY);
        printf("\t");
        system("pause");
        return;
    }

    e = get_student(2);

    textcolor(YELLOW);
    printf("\n\tVeuillez confirmer l'insertion de l'etudiant suivant dans le fichier de l'annee courante et l'archive\n\n");
    textcolor(WHITE);
    afficher_student(&e);

    getConsoleSize(&w, &h);
    int choix = menu(choices, 2, true, 0, (w-strlen("Oui, inserer"))/2, wherey() + 2, 0, LIGHTRED, BLACK, LIGHTGREEN, BLACK, true, false);
    if (choix == 0)
    {
        clock_t start_time = clock();

        insertion_student(nomf_annee, e);
        if (toupper(e.classId.annee) != 'P')
        {
            Student_to_ArchivedStudent(&e, &e2);
            insertion_archived_student_index(nomf_archive, e2, &GIndex);
            free_archived_student_fields(&e2);
        }

        clock_t end_time = clock();

        textcolor(LIGHTGREEN);
        printf("\n\tL'operation est terminee avec succes dans %.2f seconde\n\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);

        textcolor(LIGHTGRAY);
        printf("\t");
        system("pause");
    }

    system("cls");
    clrscr();
    textcolor(LIGHTGRAY);
    free_student_fields(&e);
}



void help_page()
{
    system("cls");
    clrscr();
    system("color f0");
    textbackground(WHITE);
    showConsoleCursor(false);

    char *text1 = "Travail Pratique 2022/2023 - Structure de Fichiers et Structures de Donnees - ESI Alger\n\n";
    char *text2 = "Gestion des informations scolaires des eleves d'une ecole primaire \n\n";
    char *text3 = "Alerts Importantes a respecter\n";

    char *text4 = "\to) La structure de repertoire \"./files/\" doit etre comme ca:";
    char *text5 = "\t\t-> archives";
    char *text6 = "\t\t-> ids";
    char *text7 = "\t\t-> indexes";
    char *text8 = "\t\t-> static";
    char *text9 = "\t\t   -> matieres.txt";
    char *text10 = "\t\t   -> noms.txt";
    char *text11 = "\t\t   -> prenom.txt";
    char *text12 = "\t\t-> temp";
    char *text13 = "\t\t-> years\n";

    char *text14 = "\to) Le contenu des fichiers static doit etre coherent avec les constantes dans le code.\n";
    char *text15 = "\to) La modification d'une constante dans le code peut entrainer la modification des autres constante.\n";
    char *text16 = "\to) Apres chaque modification dans le code les fichiers doivent etre re-generer a nouveau.\n";

    int w, h;
    getConsoleSize(&w, &h);

    textcolor(BLUE);
    gotoxy((w-strlen(text1))/2, wherey()+2);
    printf("%s", text1);

    textcolor(BLUE);
    gotoxy((w-strlen(text2))/2, wherey()+1);
    printf("%s", text2);

    textcolor(BLACK);
    gotoxy((w-strlen(text3))/2, wherey()+1);
    printf("%s", text3);

    textcolor(LIGHTRED);
    gotoxy(1, wherey()+1);
    printf("%s", text4);

    textcolor(BLACK);
    gotoxy(1, wherey()+1);
    printf("%s", text5);

    textcolor(BLACK);
    gotoxy(1, wherey()+1);
    printf("%s", text6);

    textcolor(BLACK);
    gotoxy(1, wherey()+1);
    printf("%s", text7);

    textcolor(BLACK);
    gotoxy(1, wherey()+1);
    printf("%s", text8);

    textcolor(BLACK);
    gotoxy(1, wherey()+1);
    printf("%s", text9);

    textcolor(BLACK);
    gotoxy(1, wherey()+1);
    printf("%s", text10);

    textcolor(BLACK);
    gotoxy(1, wherey()+1);
    printf("%s", text11);

    textcolor(BLACK);
    gotoxy(1, wherey()+1);
    printf("%s", text12);

    textcolor(BLACK);
    gotoxy(1, wherey()+1);
    printf("%s", text13);


    textcolor(LIGHTRED);
    gotoxy(1, wherey()+1);
    printf("%s", text14);

    textcolor(LIGHTRED);
    gotoxy(1, wherey()+1);
    printf("%s", text15);

    textcolor(LIGHTRED);
    gotoxy(1, wherey()+1);
    printf("%s", text16);


    getch();
    showConsoleCursor(true);

    fflush(stdin);
    system("color 07");
    textcolor(LIGHTGRAY);
    textbackground(BLACK);
    system("cls");
    clrscr();
}

void about_page()
{
    system("cls");
    clrscr();
    system("color f0");
    textbackground(WHITE);

    char *text1 = "Travail Pratique 2022/2023 - Structure de Fichiers et Structures de Donnees - ESI Alger\n\n";
    char *text2 = "Gestion des informations scolaires des eleves d'une ecole primaire \n\n\n";
    char *text3 = "Ce TP est realise en binome (Section: C   Groupe: 8)\n";
    char *text4 = "--> Melzi Mounir <--";
    char *text5 = "--> Karou  Yasmine <--";

    int w, h;
    getConsoleSize(&w, &h);

    textcolor(BLACK);
    gotoxy((w-strlen(text1))/2, wherey()+5);
    printf("%s", text1);

    textcolor(BLACK);
    gotoxy((w-strlen(text2))/2, wherey()+1);
    printf("%s", text2);

    textcolor(BLACK);
    gotoxy((w-strlen(text3))/2, wherey()+1);
    printf("%s", text3);

    textcolor(LIGHTBLUE);
    gotoxy((w-strlen(text4))/2, wherey()+1);
    printf("%s", text4);

    textcolor(LIGHTBLUE);
    gotoxy((w-strlen(text5))/2, wherey()+1);
    printf("%s", text5);

    showConsoleCursor(false);
    getch();
    showConsoleCursor(true);

    fflush(stdin);
    system("color 07");
    textcolor(LIGHTGRAY);
    textbackground(BLACK);
    system("cls");
    clrscr();
}

void welcome_page()
{
    system("cls");
    clrscr();
    system("color f0");

    showConsoleCursor(false);
    textbackground(WHITE);
    int i = 0;
    int maxColors = 3;
    int colors[] = {LIGHTBLUE, BLUE, BLACK};
    int speed = 10;

    fflush(stdin);
    while(1)
    {
        if(kbhit())
        {
            getch();
            break;
        }

        gotoxy(1,1);
        textcolor(colors[i]);printf("\t\t @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                  @@@@@@@@@@      @@        @@@@@@@@@@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@             @@@@@@@@@@@@@@@@@@ @@@         @@@@@@@@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@@@@@@@@@@@@@@@@@@@@@@           @@@@@@@       @@@@@@@@@@@           @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@                 @@@@@         @@@@@              @@@@@@@            @@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@                    @@@@        @@@@                 @@@@@@           @@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@                    @@@       @@@@@                  @@@@@          @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@          @@@        @@       @@@@@@                   @@@          @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@          @@@                  @@@@@@@                  @@          @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@         @@@@                  @@@@@@@@@@@                          @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@        @@@@@                    @@@@@@@@@@@@@@@                    @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@@@@@@@@@@@@@@                      @@@@@@@@@@@@@@@@@                @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@@@@@@@@@@@@@@                         @@@@@@@@@@@@@@@@@@            @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@        @@@@@                               @@@@@@@@@@@@@@          @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@         @@@@                                     @@@@@@@@@         @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@          @@@                @@@@                    @@@@@@@        @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@          @@@         @@@    @@@@                     @@@@@@        @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@                     @@@@     @@@@                     @@@@@        @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@                    @@@@      @@@@@@                   @@@@@         @@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@                  @@@@@@       @@@@@@                 @@@@@           @@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@                @@@@@@@        @@@@@@@@@            @@@@@@          @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@         @@@@ @@@@@@@@@@@@@@@@@@@@@        @@@@@@@@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@         @@@    @@@@@@@@@@@@@@@@          @@@@@@@@@@@@@@@\n\n");

        textcolor(BLUE);
        int w, h;
        getConsoleSize(&w, &h);
        gotoxy((w-strlen("TP  SFSD"))/2, wherey() + 2);
        printf("TP  SFSD");

        gotoxy((w-strlen("Appuyez sur une touche pour continuer ..."))/2, wherey() + 2);
        printf("Appuyez sur une touche pour continuer ...");
    }

    fflush(stdin);
    system("color 07");
    textcolor(LIGHTGRAY);
    textbackground(BLACK);
    system("cls");
    clrscr();
}

void end_page()
{
    system("cls");
    clrscr();
    system("color f0");

    showConsoleCursor(false);
    textbackground(WHITE);
    int i = 0;
    int maxColors = 3;
    int colors[] = {LIGHTBLUE, BLUE, BLACK};
    int speed = 10;

    fflush(stdin);
    while(1)
    {
        if(kbhit())
        {
            getch();
            break;
        }

        gotoxy(1,1);
        textcolor(colors[i]);printf("\t\t @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                  @@@@@@@@@@      @@        @@@@@@@@@@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@             @@@@@@@@@@@@@@@@@@ @@@         @@@@@@@@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@@@@@@@@@@@@@@@@@@@@@@           @@@@@@@       @@@@@@@@@@@           @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@                 @@@@@         @@@@@              @@@@@@@            @@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@                    @@@@        @@@@                 @@@@@@           @@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@                    @@@       @@@@@                  @@@@@          @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@          @@@        @@       @@@@@@                   @@@          @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@          @@@                  @@@@@@@                  @@          @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@         @@@@                  @@@@@@@@@@@                          @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@        @@@@@                    @@@@@@@@@@@@@@@                    @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@@@@@@@@@@@@@@                      @@@@@@@@@@@@@@@@@                @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@@@@@@@@@@@@@@                         @@@@@@@@@@@@@@@@@@            @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@        @@@@@                               @@@@@@@@@@@@@@          @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@         @@@@                                     @@@@@@@@@         @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@          @@@                @@@@                    @@@@@@@        @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@          @@@         @@@    @@@@                     @@@@@@        @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@@                     @@@@     @@@@                     @@@@@        @@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@                    @@@@      @@@@@@                   @@@@@         @@@@@\n");
        textcolor(colors[i]);printf("\t\t     @@@@@                  @@@@@@       @@@@@@                 @@@@@           @@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t     @@@@@@                @@@@@@@        @@@@@@@@@            @@@@@@          @@@@@@@\n");
        textcolor(colors[i]);printf("\t\t @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@         @@@@ @@@@@@@@@@@@@@@@@@@@@        @@@@@@@@@@@@@\n");Sleep(speed);i++;if(i%maxColors == 0)i=0;
        textcolor(colors[i]);printf("\t\t @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@         @@@    @@@@@@@@@@@@@@@@          @@@@@@@@@@@@@@@\n\n");

        textcolor(BLUE);
        printf("\t\t\t\t\tMerci pour l'utilisation du notre application\n\n");

        textcolor(RED);
        printf("\tRealise par :");

        textcolor(LIGHTRED);
        printf(" MELZI MOUNIR | KAROU YASMINE      ");

        textcolor(RED);
        printf("Groupe :");

        textcolor(LIGHTRED);
        printf(" 08 | 08     ");

        textcolor(RED);
        printf("Section :");

        textcolor(LIGHTRED);
        printf(" C | C ");

        textcolor(RED);
        printf("\n\t\t\t\t      Sous la supervision de :");

        textcolor(LIGHTRED);
        printf(" Madame Artabaz Saliha\n\n");



        textcolor(BLUE);
        printf("\t\t\t\t\t\tAppuyez sur une touche pour sortir ... ");
    }

    fflush(stdin);
    system("color 07");
    textcolor(LIGHTGRAY);
    textbackground(BLACK);
    system("cls");
    clrscr();
}

void fin_programme()
{
    char file_name[256];
    fileFullPath(annee_courante, FILE_IDS, file_name);
    save_ids_array(file_name);

    clean();

    end_page();
    exit(EXIT_SUCCESS);
}

