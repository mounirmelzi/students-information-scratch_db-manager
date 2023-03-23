#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "lib/conio2.h"
#include "lib/utils.h"
#include "lib/TOVC.h"
#include "lib/student.h"
#include "lib/index.h"
#include "lib/archive.h"
#include "lib/functions.h"
#include "lib/guiTools.h"
#include "lib/gui.h"


int main(int argc, char **argv)
{
    welcome_page();

    char file_name[256];
    char file_name2[256];
    int w, h;
    clock_t start_time, end_time;
    ClassId class_id;
    float moyenne1, moyenne2;
    int scanf_code;

    init();

debut:

    GIndexExist = false;

    system("cls");
    clrscr();

    switch (menu_debut())
    {
        case 0:
        {
            get_annee_courante(annee_courante);
            fileFullPath(annee_courante, FILE_ANNEE_COURANTE, file_name);

            deleteFolderFiles("files/years");
            deleteFolderFiles("files/archives");
            deleteFolderFiles("files/indexes");
            deleteFolderFiles("files/temp");
            deleteFolderFiles("files/ids");

            randomize_ids();
            ID_INDEX = 0; // initialisation du compteur des ids uniques utilises

            chargement_initial_students(file_name);

            textcolor(LIGHTGREEN);
            printf("\n\n\tLe chargement initial du fichier de l'annee courante est termine avec succes\n\n");

            textcolor(LIGHTGRAY);
            printf("\t");
            system("pause");

            break;
        }

        case 1:
        {
            get_annee_courante(annee_courante);
            fileFullPath(annee_courante, FILE_ANNEE_COURANTE, file_name);

            if (isFileExiste(file_name))
            {
                deleteFolderFiles("files/temp");
                delete_useless_files(annee_courante);

                fileFullPath(annee_courante, FILE_IDS, file_name);
                if (!isFileExiste(file_name))
                {
                    system("cls");
                    clrscr();
                    system("color 47");
                    printf("\n\n\n\t\t[ERROR]");
                    printf("\n\n\t\tLe fichier des IDs n'existe pas! Le programme ne peut pas entre execute correctement !!\n");
                    printf("\t\t\tVous devez re-creer vos fichiers a nouveau\n");
                    printf("\t\t\tOu revenir sur une annee qui contienne le fichier des IDs\n\n\t\t");
                    system("pause");
                    exit(EXIT_FAILURE);
                }

                load_ids_array(file_name);

                textcolor(LIGHTGREEN);
                printf("\n\n\tLe fichier de l'annee courante est trouve avec success\n\n");

                textcolor(LIGHTGRAY);
                printf("\t");
                system("pause");
            }
            else
            {
                textcolor(LIGHTRED);
                printf("\n\n\t[ERROR] Le fichier de l'annee courante n'existe pas\n\n");

                textcolor(LIGHTGRAY);
                printf("\t");
                system("pause");

                goto debut;
            }

            break;
        }

        case 2:
        {
            help_page();
            goto debut;
            break;
        }

        case 3:
        {
            about_page();
            goto debut;
            break;
        }

        case 4:
        {
            clean();
            end_page();
            exit(EXIT_SUCCESS);
            break;
        }
    }

    system("color 07");

    GIndexExist = false;

    while (true)
    {
        system("cls");
        clrscr();
        getConsoleSize(&w, &h);

        switch (menu_principal())
        {
            case 0: // Affichage des donnees
            {
                choix_0();
                break;
            }

            case 1: // Inserer un eleve (ficher : annee courante)
            {
                choix_1();
                break;
            }

            case 2: // Mise a jour des informations d'un eleve (ficher : annee courante)
            {
                choix_2();
                break;
            }

            case 3: // Supprimer un eleve (ficher : annee courante)
            {
                choix_3();
                break;
            }

            case 4: // Archiver l'annee courante
            {
                fileFullPath(annee_courante, FILE_ANNEE_COURANTE, file_name);
                if (!isFileExiste(file_name))
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] Le fichier de l'annee courante a archive n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

                fileFullPath(annee_courante, FILE_ARCHIVE, file_name2);

                gotoxy((w-strlen("La creation du l'archive en cours... "))/2, h/2);
                textcolor(YELLOW);
                printf("La creation du l'archive en cours... ");

                start_time = clock();
                create_archive(file_name, file_name2);
                GIndexExist = false;
                GIndex.taille = 0;
                end_time = clock();

                textcolor(LIGHTGREEN);
                deleteConsoleLines(1);
                gotoxy((w-strlen("La creation de l'archive est terminee avec succes dans x.xx secondes"))/2, h/2);
                printf("La creation de l'archive est terminee avec succes dans %.2f secondes", (double)(end_time-start_time)/CLOCKS_PER_SEC);

                textcolor(LIGHTGRAY);
                printf("\n\n\n\n");
                gotoxy((w-strlen("Appuyez sur une touche pour continuer... "))/2, wherey());
                system("pause");
                break;
            }

            case 5: // Construire l'index du l'archive de l'annee courante
            {
                fileFullPath(annee_courante, FILE_ARCHIVE, file_name);
                if (!isFileExiste(file_name))
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] Le fichier archive a indexe n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

                gotoxy((w-strlen("La creation du l'index en cours... "))/2, h/2);
                textcolor(YELLOW);
                printf("La creation du l'index en cours... ");

                start_time = clock();
                create_index(file_name, &GIndex);
                GIndexExist = true;
                end_time = clock();

                textcolor(LIGHTGREEN);
                deleteConsoleLines(1);
                gotoxy((w-strlen("La creation de l'index est terminee avec succes dans x.xx secondes"))/2, h/2);
                printf("La creation de l'index est terminee avec succes dans %.2f secondes", (double)(end_time-start_time)/CLOCKS_PER_SEC);

                textcolor(LIGHTGRAY);
                printf("\n\n\n\n");
                gotoxy((w-strlen("Appuyez sur une touche pour continuer... "))/2, wherey());
                system("pause");
                break;
            }

            case 6: // Sauvegarder la table d'index dans un ficher TOF
            {
                if (!GIndexExist)
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] La table d'index du l'archive de l'annee courante n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

                fileFullPath(annee_courante, FILE_INDEX, file_name);

                gotoxy((w-strlen("La sauvegade du l'index en cours... "))/2, h/2);
                textcolor(YELLOW);
                printf("La sauvegade du l'index en cours... ");

                start_time = clock();
                sauvegarder_index(file_name, &GIndex);
                end_time = clock();

                textcolor(LIGHTGREEN);
                deleteConsoleLines(1);
                gotoxy((w-strlen("La sauvegade du l'index est terminee avec succes dans x.xx secondes"))/2, h/2);
                printf("La sauvegade du l'index est terminee avec succes dans %.2f secondes", (double)(end_time-start_time)/CLOCKS_PER_SEC);

                textcolor(LIGHTGRAY);
                printf("\n\n\n\n");
                gotoxy((w-strlen("Appuyez sur une touche pour continuer... "))/2, wherey());
                system("pause");

                break;
            }

            case 7: // Charger la table d'index depuis un fichier TOF
            {
                fileFullPath(annee_courante, FILE_INDEX, file_name);
                if (!isFileExiste(file_name))
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] Le fichier de l'index pour l'annee courante n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

                if (GIndexExist)
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] La table d'index du l'archive de l'annee courante existe deja dans la memoire centrale !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

                gotoxy((w-strlen("Le chargement du l'index en cours... "))/2, h/2);
                textcolor(YELLOW);
                printf("Le chargement du l'index en cours... ");

                start_time = clock();
                chargement_index(file_name, &GIndex);
                GIndexExist = true;
                end_time = clock();

                textcolor(LIGHTGREEN);
                deleteConsoleLines(1);
                gotoxy((w-strlen("Le chargement du l'index est terminee avec succes dans x.xx secondes"))/2, h/2);
                printf("Le chargement du l'index est terminee avec succes dans %.2f secondes", (double)(end_time-start_time)/CLOCKS_PER_SEC);

                textcolor(LIGHTGRAY);
                printf("\n\n\n\n");
                gotoxy((w-strlen("Appuyez sur une touche pour continuer... "))/2, wherey());
                system("pause");

                break;
            }

            case 8: // Inserer un eleve (ficher : archive)
            {
                choix_8();
                break;
            }

            case 9: // Supprimer un eleve (ficher : archive)
            {
                choix_9();
                break;
            }

            case 10: // Requete a interval selon les moyennes des eleves
            {
                fileFullPath(annee_courante, FILE_ARCHIVE, file_name);
                if (!isFileExiste(file_name))
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] Le fichier archive de l'annee courante n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

                if (!GIndexExist)
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] La table d'index du l'archive de l'annee courante n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

get_niveau:
                system("color 07");
                printf("\n\tVeuillez entrer le niveau: (entre 1 et 5) ");
                fflush(stdin);
                scanf_code = scanf("%c", &class_id.annee);
                fflush(stdin);
                if ((scanf_code != 1) || (class_id.annee < '1') || (class_id.annee > '5'))
                {
                    system("color 47");
                    deleteConsoleLines(-3);
                    goto get_niveau;
                }

get_salle:
                system("color 07");
                printf("\n\tVeuillez entrer le numero de salle: (entre 1 et %d)  ", NBR_CLASSES_PAR_ANNEE);
                fflush(stdin);
                scanf_code = scanf("%d", &class_id.numSalle);
                fflush(stdin);
                if ((scanf_code != 1) || (class_id.numSalle < 1) || (class_id.numSalle > NBR_CLASSES_PAR_ANNEE))
                {
                    system("color 47");
                    deleteConsoleLines(-3);
                    goto get_salle;
                }

get_moyenne1:
                system("color 07");
                printf("\n\tVeullez entrer la premiere moyenne: (entre 0 et 20) ");
                fflush(stdin);
                scanf_code = scanf("%f", &moyenne1);
                if ((scanf_code != 1) || (moyenne1 < 0) || (moyenne1 > 20))
                {
                    system("color 47");
                    deleteConsoleLines(-3);
                    goto get_moyenne1;
                }

get_moyenne2:
                system("color 07");
                printf("\n\tVeullez entrer la deuxieme moyenne: (entre 0 et 20) ");
                fflush(stdin);
                scanf_code = scanf("%f", &moyenne2);
                if ((scanf_code != 1) || (moyenne2 < 0) || (moyenne2 > 20))
                {
                    system("color 47");
                    deleteConsoleLines(-3);
                    goto get_moyenne2;
                }

                if (moyenne1 > moyenne2)
                {
                    float _temp = moyenne1;
                    moyenne1 = moyenne2;
                    moyenne2 = _temp;
                }

                system("cls");
                clrscr();

                textcolor(YELLOW);
                printf("\n\tLes etudians dans la classe \"%c - %d\" ayant une moyenne entre %.2f et %.2f sont: \n\n", toupper(class_id.annee), class_id.numSalle, moyenne1, moyenne2);

                textcolor(WHITE);
                int count = requete_intervalle(file_name, &GIndex, class_id, moyenne1, moyenne2);

                textcolor(YELLOW);
                printf("\n\n\t%d Resultats trouves", count);

                textcolor(LIGHTGRAY);
                printf("\n\n\t");
                system("pause");
                break;
            }

            case 11: // Generer l'annee suivante
            {
                fileFullPath(annee_courante, FILE_ANNEE_COURANTE, file_name);
                if (!isFileExiste(file_name))
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] Le fichier de l'anne courante n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

                fileFullPath(annee_courante, FILE_IDS, file_name2);
                save_ids_array(file_name2);
                GIndexExist = false;
                GIndex.taille = 0;

                update_annee_courante(1, true);
                fileFullPath(annee_courante, FILE_ANNEE_COURANTE, file_name2);

                gotoxy((w-strlen("La generation de l'annee suivante en cours... "))/2, h/2);
                textcolor(YELLOW);
                printf("La generation de l'annee suivante en cours... ");

                start_time = clock();
                generate_next_year(file_name, file_name2);
                end_time = clock();

                textcolor(LIGHTGREEN);
                deleteConsoleLines(1);
                gotoxy((w-strlen("La generation de l'annee suivante est terminee avec succes dans x.xx secondes"))/2, h/2);
                printf("La generation de l'annee suivante est terminee avec succes dans %.2f secondes", (double)(end_time-start_time)/CLOCKS_PER_SEC);

                textcolor(LIGHTGRAY);
                printf("\n\n\n\n");
                gotoxy((w-strlen("Appuyez sur une touche pour continuer... "))/2, wherey());
                system("pause");
                break;
            }

            case 12: // Simulation de 5 annees successives
            {
                char nomf_annee1[256];
                char nomf_annee2[256];
                char nomf_archive[256];
                char nomf_index[256];
                char nomf_ids[256];
                clock_t start_time, end_time, time;

                fileFullPath(annee_courante, FILE_ANNEE_COURANTE, nomf_annee1);
                if (!isFileExiste(nomf_annee1))
                {
                    textcolor(LIGHTRED);
                    printf("\n\n\t[ERROR] Le fichier de l'annee courante n'existe pas !\n\n");
                    textcolor(LIGHTGRAY);
                    printf("\t");
                    system("pause");
                    continue;
                }

                fileFullPath(annee_courante, FILE_ARCHIVE, nomf_archive);
                fileFullPath(annee_courante, FILE_INDEX, nomf_index);
                fileFullPath(annee_courante, FILE_IDS, nomf_ids);

                time = clock();

                if(!isFileExiste(nomf_archive))
                {
                    textcolor(YELLOW);
                    printf("L'archivage en cours... ");
                    start_time = clock();
                    create_archive(nomf_annee1, nomf_archive);
                    end_time = clock();
                    textcolor(LIGHTGREEN);
                    deleteConsoleLines(1);
                    printf("L'archivage de l'annee (");
                    print_annee_courante();
                    printf(") est termine avec succes dans %.2f secondes\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);
                }
                else
                {
                    printf("Le fichier archive de l'annee (");
                    print_annee_courante();
                    printf(") existe deja\n");
                }

                textcolor(YELLOW);
                printf("Creation de l'index en cours... ");
                start_time = clock();
                create_index(nomf_archive, &GIndex);
                end_time = clock();
                textcolor(LIGHTGREEN);
                deleteConsoleLines(1);
                printf("La creation de l'index pour l'annee (");
                print_annee_courante();
                printf(") est terminee avec succes dans %.2f secondes\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);

                textcolor(YELLOW);
                printf("La sauvegarde de l'index en cours... ");
                start_time = clock();
                sauvegarder_index(nomf_index, &GIndex);
                end_time = clock();
                textcolor(LIGHTGREEN);
                deleteConsoleLines(1);
                printf("La sauvegarde de l'index pour l'annee (");
                print_annee_courante();
                printf(") est terminee avec succes dans %.2f secondes\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);

                textcolor(YELLOW);
                printf("La sauvegarde des IDs en cours... ");
                start_time = clock();
                save_ids_array(nomf_ids);
                end_time = clock();
                textcolor(LIGHTGREEN);
                deleteConsoleLines(1);
                printf("La sauvegarde des IDs de l'annee (");
                print_annee_courante();
                printf(") est terminee avec succes dans %.2f secondes\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);

                for (int i = 0; i < 4; i++)
                {
                    fileFullPath(annee_courante, FILE_ANNEE_COURANTE, nomf_annee1);
                    update_annee_courante(1, true);
                    fileFullPath(annee_courante, FILE_ANNEE_COURANTE, nomf_annee2);
                    fileFullPath(annee_courante, FILE_ARCHIVE, nomf_archive);
                    fileFullPath(annee_courante, FILE_INDEX, nomf_index);
                    fileFullPath(annee_courante, FILE_IDS, nomf_ids);

                    textcolor(YELLOW);
                    printf("\nLa generation de l'annee suivante en cours... ");
                    start_time = clock();
                    generate_next_year(nomf_annee1, nomf_annee2);
                    end_time = clock();
                    textcolor(LIGHTGREEN);
                    deleteConsoleLines(1);
                    printf("La generation de l'annee (");
                    print_annee_courante();
                    printf(") est terminee avec succes dans %.2f secondes\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);

                    textcolor(YELLOW);
                    printf("L'archivage en cours... ");
                    start_time = clock();
                    create_archive(nomf_annee2, nomf_archive);
                    end_time = clock();
                    textcolor(LIGHTGREEN);
                    deleteConsoleLines(1);
                    printf("L'archivage de l'annee (");
                    print_annee_courante();
                    printf(") est termine avec succes dans %.2f secondes\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);

                    textcolor(YELLOW);
                    printf("Creation de l'index en cours... ");
                    start_time = clock();
                    create_index(nomf_archive, &GIndex);
                    end_time = clock();
                    textcolor(LIGHTGREEN);
                    deleteConsoleLines(1);
                    printf("La creation de l'index pour l'annee (");
                    print_annee_courante();
                    printf(") est terminee avec succes dans %.2f secondes\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);

                    textcolor(YELLOW);
                    printf("La sauvegarde de l'index en cours... ");
                    start_time = clock();
                    sauvegarder_index(nomf_index, &GIndex);
                    end_time = clock();
                    textcolor(LIGHTGREEN);
                    deleteConsoleLines(1);
                    printf("La sauvegarde de l'index pour l'annee (");
                    print_annee_courante();
                    printf(") est terminee avec succes dans %.2f secondes\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);

                    if (i != 3)
                    {
                        textcolor(YELLOW);
                        printf("La sauvegarde des IDs en cours... ");
                        start_time = clock();
                        save_ids_array(nomf_ids);
                        end_time = clock();
                        textcolor(LIGHTGREEN);
                        deleteConsoleLines(1);
                        printf("La sauvegarde des IDs de l'annee (");
                        print_annee_courante();
                        printf(") est terminee avec succes dans %.2f secondes\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);
                    }
                }

                GIndexExist = true;
                textcolor(LIGHTMAGENTA);
                printf("\nL'operation est terminee avec succes dans %.2f secondes\n\n", (double)(clock()-time)/CLOCKS_PER_SEC);
                textcolor(LIGHTGRAY);
                fflush(stdin);
                system("pause");

                break;
            }

            case 13: // Effectuer un transfert
            {
                choix_13();
                break;
            }

            case 14: // Retour vers la page precedente
            {
                char file_name[256];
                fileFullPath(annee_courante, FILE_IDS, file_name);
                save_ids_array(file_name);
                goto debut;
                break;
            }

            case 15: // Quiter le programme
            {
                fin_programme();
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}
