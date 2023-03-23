#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <math.h>
#include "TOVC.h"
#include "guiTools.h"
#include "utils.h"


/**
 * @brief calculer le nombre de positions dans un entier
 *
 * @param n
 * @return int
 */
int nbPos(int n)
{
    int pos = 1;

    while (n / 10 != 0)
    {
        pos++;
        n /= 10;
    }

    return pos;
}

/**
 * @brief convertir un entier en une chaine de caracteres
 *
 * @param n
 * @param chaine
 * @param taille
 */
void intToStr(int n, char chaine[], int taille)
{
    const int ZEROS = taille - nbPos(n);
    if (ZEROS < 0)
    {
        printf("[ERROR] la taille de l'entier a convertir vers une chaine de caractere est plus grande que la taille de la chaine !!\n");
        strcpy(chaine, "");
        return;
    }

    char *temp_str = calloc(taille + 1, sizeof(char));
    sprintf(temp_str, "%d", n);

    for (int i = 0; i < ZEROS; i++)
        chaine[i] = '0';
    chaine[ZEROS] = '\0';

    strcat(chaine, temp_str);
    free(temp_str);
}

/**
 * @brief convertir une chaine de caracteres en un entier
 *
 * @param chaine
 * @param n
 */
void strToInt(const char chaine[], int *n)
{
    *n = atoi(chaine);
}

/**
 * @brief extraire une sous-chaine d'une chaine de caracteres
 *
 * @param src
 * @param dist
 * @param startIndex
 * @param charNum
 */
void subStr(const char src[], char *dist, int startIndex, int charNum)
{
    for (int i = startIndex; i < startIndex + charNum; i++)
        *(dist + i-startIndex) = src[i];
}

/**
 * @brief generer un nombre aleatoire entre min_val et max_val (inclus)
 *
 * @param min_val
 * @param max_val
 * @return int
 */
int random_int(int min_val, int max_val)
{
    return (rand() % (max_val - min_val + 1)) + min_val;
}

/**
 * @brief calculer la puissances
 *
 * @param n
 * @param p
 * @return int
 */
int power(int n, int p)
{
    return pow(n, p);
}

/**
 * @brief supprimer tous les fichiers dans un repertoire
 *
 * @param path_to_folder
 */
void deleteFolderFiles(char path_to_folder[])
{
    DIR *folder = opendir(path_to_folder);
    struct dirent *next_file;
    char file_path[512];

    // parcourir tous les fichier qui existent dans le repertoire
    while ((next_file = readdir(folder)) != NULL)
    {
        sprintf(file_path, "%s/%s", path_to_folder, next_file->d_name); // creer le chemin de fichier
        remove(file_path); // supprimer le fichier
    }

    closedir(folder);
}

/**
 * @brief verifier si un fichier existe ou non
 *
 * @param path
 * @return true
 * @return false
 */
bool isFileExiste(char path[])
{
    if (access(path, F_OK) != -1)
        return true;

    return false;
}

/**
 * @brief mise a jour de la variable globale annee_courante
 * 
 * @param value 
 * @param isIncrement indique si on incremente l'anne courante ou la ecraser avec la nouvelle valeur
 */
void update_annee_courante(int value, bool isIncrement)
{
    int year1, year2;
    char part1[5], part2[5];

    if (isIncrement)
    {
        // lire l'annee courante
        sscanf(annee_courante, "%d_%d", &year1, &year2);
        // incrementer
        year1 += value;
        year2 += value;
    }
    else
    {
        // ecraser les valeur de l'annee courante
        year1 = value;
        year2 = value + 1;
    }

    // modifier la variable globale annee_courante

    intToStr(year1, part1, 4);
    part1[4] = '\0';
    intToStr(year2, part2, 4);
    part2[4] = '\0';

    sprintf(annee_courante, "%s_%s", part1, part2);
}

/**
 * @brief generer le chemin vers un fichier
 * 
 * @param annee_scolaire l'annee du fichier
 * @param flag indique le type du fichier
 * @param path le chemin vers le fichier
 */
void fileFullPath(char annee_scolaire[], int flag, char path[])
{
    switch (flag)
    {
        case FILE_ANNEE_COURANTE:
            sprintf(path, "files/years/annee_%s.tovc", annee_scolaire);
            break;
        case FILE_ARCHIVE:
            sprintf(path, "files/archives/archive_%s.tovc", annee_scolaire);
            break;
        case FILE_INDEX:
            sprintf(path, "files/indexes/index_%s.tof", annee_scolaire);
            break;
        case FILE_IDS:
            sprintf(path, "files/ids/ids_%s.bin", annee_scolaire);
            break;
        default:
            strcpy(path, "");
            break;
    }
}

/**
 * @brief remplir la table des IDs aleatoirement
 * 
 */
void randomize_ids()
{
    char temp_str[TAILLE_CHAMP_CLE + 1];

    // generer des ids sequentiels unique de 0 jusqu'a NBR_IDS - 1
    for(int k = 0; k < NBR_IDS; k++)
    {
        intToStr(k, temp_str, TAILLE_CHAMP_CLE);
        temp_str[TAILLE_CHAMP_CLE] = '\0';
        strcpy(IDS_ARRAY[k], temp_str);
    }

    // shuffle IDS_ARRAY
    for (int k = 0; k < NBR_IDS; k++)
    {
        int l = k + rand() / (RAND_MAX / (NBR_IDS - k) + 1);
        strcpy(temp_str, IDS_ARRAY[l]);
        strcpy(IDS_ARRAY[l], IDS_ARRAY[k]);
        strcpy(IDS_ARRAY[k], temp_str);
    }
}

/**
 * @brief lire une annee scolaire depuis l'utilisateur
 * 
 * @param year l'annee lue
 */
void get_annee_courante(char year[])
{
    char annee[256];
    int scanf_code;
    int year1, year2;
    bool valide;

get_annee:

    system("color 07");
    printf("\n\tVeuillez entrer l'annee courante s'il vous plait: (ex: 2022/2023) ");
    fflush(stdin);
    scanf_code = scanf("%s", annee);

    valide = true;
    for (int i = 0; i < 4; i++)
    {
        if (!valide)
            break;

        if ((annee[i] < '0') || (annee[i] > '9'))
            valide = false;
    }
    for (int i = 5; i < 9; i++)
    {
        if (!valide)
            break;

        if ((annee[i] < '0') || (annee[i] > '9'))
            valide = false;
    }

    if ((scanf_code != 1) || (strlen(annee) != 9) || (annee[4] != '/') || (!valide))
    {
        system("color 47");
        deleteConsoleLines(-3);
        goto get_annee;
    }

    sscanf(annee, "%d/%d", &year1, &year2);
    if (year2 != (year1 + 1))
    {
        system("color 47");
        deleteConsoleLines(-3);
        goto get_annee;
    }

    fflush(stdin);
    annee[4] = '_';
    strcpy(year, annee);
}

/**
 * @brief afficher une annee scolaire a l'ecran
 * 
 */
void print_annee_courante()
{
    char annee[10];
    strcpy(annee, annee_courante);
    annee[4] = '/';
    printf("%s", annee);
}

/**
 * @brief supprimer les fichiers qui ne sont pas necessaire au debut du programme
 * 
 * @param year 
 */
void delete_useless_files(char year[])
{
    DIR *folder;
    struct dirent *next_file;
    char file_path[512];
    char file_name[256];
    char part1[256], part2[256], part3[256], part4[256];
    int year1, year2, temp_year1, temp_year2;
    char *folders[] = {
        "files/years",
        "files/archives",
        "files/indexes",
        "files/ids",
    };

    sscanf(year, "%[0-9]_%[0-9]", part1, part2);
    strToInt(part1, &year1);
    strToInt(part2, &year2);

    // parcourir tous les repertoires
    for (int i = 0; i < 4; i++)
    {
        folder = opendir(folders[i]);

        // parcourir tous les fichiers
        while ((next_file = readdir(folder)) != NULL)
        {
            strcpy(file_name, next_file->d_name);
            if ((strcmp(file_name, ".") == 0) || (strcmp(file_name, "..") == 0))
                continue;

            // extraire l'annee depuis le nom du fichier
            sscanf(file_name, "%[a-zA-Z0-9 ]_%[0-9]_%[0-9].%[a-zA-Z0-9 ]", part1, part2, part3, part4);
            strToInt(part2, &temp_year1);
            strToInt(part3, &temp_year2);

            // si le fichier inutile
            if ((temp_year1 > year1) || (temp_year2 > year2))
            {
                sprintf(file_path, "%s/%s", folders[i], file_name);
                remove(file_path); // supprimer le fichier
            }
        }

        closedir(folder);
    }
}

/**
 * @brief permuter entre 2 IDs dans la table des IDs globale
 * 
 * @param id1 
 * @param id2 
 */
void permute_ids_in_array(char id1[], char id2[])
{
    int i1 = -1, i2 = -1;
    bool trouv1 = false, trouv2 = false;
    char temp[TAILLE_CHAMP_CLE + 1];

    // rechercher les indices i1, i2 dans la table des IDs globale
    for (int i = 0; i < NBR_IDS; i++)
    {
        if (trouv1 && trouv2)
            break;

        if (!trouv1)
        {
            if (strcmp(IDS_ARRAY[i], id1) == 0)
            {
                i1 = i;
                trouv1 = true;
            }
        }

        if (!trouv2)
        {
            if (strcmp(IDS_ARRAY[i], id2) == 0)
            {
                i2 = i;
                trouv2 = true;
            }
        }
    }

    // effectuer la permutation
    if (i1 != i2)
    {
        strcpy(temp, IDS_ARRAY[i1]);
        strcpy(IDS_ARRAY[i1], IDS_ARRAY[i2]);
        strcpy(IDS_ARRAY[i2], temp);
    }
}
