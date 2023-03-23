#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conio2.h"
#include "guiTools.h"
#include "index.h"


int entete_TOF(TOF *f, int caracteristiqueNum)
{
    switch(caracteristiqueNum)
    {
        case 1:
            return f->entete.NumeroDernierBloc;
            break;
        case 2:
            return f->entete.NombreEnreg;
            break;
        default:
            printf("[ERROR] caracteristique n'existe pas !");
            return -999;
            break;
    }
}

void aff_entete_TOF(TOF *f, int caracteristiqueNum, int valeur)
{
    switch(caracteristiqueNum)
    {
        case 1:
            f->entete.NumeroDernierBloc = valeur;
            break;
        case 2:
            f->entete.NombreEnreg = valeur;
            break;
        default:
            printf("[ERROR] caracteristique n'existe pas !");
            break;
    }
}

void lire_dir_TOF(TOF *f, int blocNum, Buffer_TOF *buf)
{
    fseek(f->fichier, sizeof(Entete_TOF) + (blocNum-1)*sizeof(Tbloc_TOF), SEEK_SET);
    fread(buf, sizeof(Tbloc_TOF), 1, f->fichier);
}
void ecrire_dir_TOF(TOF *f, int blocNum, Buffer_TOF *buf)
{
    fseek(f->fichier, sizeof(Entete_TOF) + (blocNum-1)*sizeof(Tbloc_TOF), SEEK_SET);
    fwrite(buf, sizeof(Tbloc_TOF), 1, f->fichier);
}

void ouvrir_TOF(TOF *f, char nomf[], char mode)
{
    if ((mode == 'N') || (mode == 'n'))
    {
        f->fichier = fopen(nomf, "wb+");
        if (f->fichier == NULL)
        {
            printf("[ERROR] erreur d'ouverture de fichier !");
            return;
        }

        aff_entete_TOF(f, 1, 0); // le numero du dernier bloc = 0
        aff_entete_TOF(f, 2, 0); // le nombre des enregistrements dans le fichier = 0
        fwrite(&(f->entete), sizeof(Entete_TOF), 1, f->fichier);
        rewind(f->fichier);
    }
    else if ((mode == 'A') || (mode == 'a'))
    {
        f->fichier = fopen(nomf, "rb+");
        if (f->fichier == NULL)
        {
            printf("[ERROR] erreur d'ouverture de fichier !");
            return;
        }

        fread(&(f->entete), sizeof(Entete_TOF), 1, f->fichier);
        rewind(f->fichier);
    }
    else
    {
        printf("[ERROR] mode n'existe pas !");
        f->fichier = NULL;
        return;
    }
}

void fermer_TOF(TOF *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(Entete_TOF), 1, f->fichier);
    fclose(f->fichier);
}

int alloc_bloc_TOF(TOF *f)
{
    int numeroDernierBloc = entete_TOF(f, 1) + 1;
    aff_entete_TOF(f, 1, numeroDernierBloc);
    return numeroDernierBloc;
}


/**
 * @brief allouer un espace memoire pour contenir la table d'index
 * 
 * @return IndexTable* pointeur vers l'espace memoire alloue
 */
IndexTable *allouer_index()
{
    return (IndexTable *)calloc(1, sizeof(IndexTable));
}

/**
 * @brief liberer l'espace memoire allouer pour la table d'index
 * 
 * @param tabInd 
 */
void free_index(IndexTable **tabInd)
{
    free(*tabInd);
    *tabInd = NULL;
}


/**
 * @brief afficher une table d'index
 * 
 * @param tabInd 
 */
void afficher_index(IndexTable *tabInd)
{
    int h, w;
    getConsoleSize(&w, &h);
    char *title = "AFFICHAGE DE L'INDEX";
    gotoxy((w-strlen(title))/2, 2);
    printf("%s\n\n", title);

    for (int i = 0; i < tabInd->taille; i++)
    {
        if (i % 2)
        {
            gotoxy(w/2, wherey()-7);
            printf("\t+------- INDEX [%3d ] -------+\n", i);
            gotoxy(w/2, wherey());
            printf("\t| class ID                   : %c%d\n", tabInd->tab[i].classId.annee, tabInd->tab[i].classId.numSalle);
            gotoxy(w/2, wherey());
            printf("\t| moyenne min                : %.2f\n", tabInd->tab[i].moyenne_min);
            gotoxy(w/2, wherey());
            printf("\t| moyenne max                : %.2f\n", tabInd->tab[i].moyenne_max);
            gotoxy(w/2, wherey());
            printf("\t| numero du bloc (i)         : %d\n", tabInd->tab[i].numBloc);
            gotoxy(w/2, wherey());
            printf("\t| position dans le bloc (j)  : %d\n", tabInd->tab[i].pos);
            gotoxy(w/2, wherey());
            printf("\t+----------------------------+\n");
        }
        else
        {
            printf("\t+------- INDEX [%3d ] -------+\n", i);
            printf("\t| class ID                   : %c%d\n", tabInd->tab[i].classId.annee, tabInd->tab[i].classId.numSalle);
            printf("\t| moyenne min                : %.2f\n", tabInd->tab[i].moyenne_min);
            printf("\t| moyenne max                : %.2f\n", tabInd->tab[i].moyenne_max);
            printf("\t| numero du bloc (i)         : %d\n", tabInd->tab[i].numBloc);
            printf("\t| position dans le bloc (j)  : %d\n", tabInd->tab[i].pos);
            printf("\t+----------------------------+\n");
        }
    }
}


/**
 * @brief sauvegarder la table d'index dans un fichier TOF
 * 
 * @param nomf 
 * @param tabInd 
 */
void sauvegarder_index(char nomf[], IndexTable *tabInd)
{
    TOF f;
    Buffer_TOF buf;
    int i = 1;
    int j = 0;

    ouvrir_TOF(&f, nomf, 'N');

    for (int k = 0; k < tabInd->taille; k++)
    {
        if (j < MAX_INDEX_TOF)
        {
            buf.tab[j] = tabInd->tab[k];
            j++;
        }
        else
        {
            buf.nb = MAX_INDEX_TOF;
            ecrire_dir_TOF(&f, i, &buf);
            i++;
            buf.tab[0] = tabInd->tab[k];
            j = 1;
        }
    }

    buf.nb = j;
    ecrire_dir_TOF(&f, i, &buf);
    aff_entete_TOF(&f, 1, i);
    aff_entete_TOF(&f, 2, tabInd->taille);

    fermer_TOF(&f);
}

/**
 * @brief charger la table d'index depuis un fichier TOF
 * 
 * @param nomf 
 * @param tabInd 
 */
void chargement_index(char nomf[], IndexTable *tabInd)
{
    TOF f;
    Buffer_TOF buf;
    tabInd->taille = 0;

    ouvrir_TOF(&f, nomf, 'A');

    for (int i = 1; i <= entete_TOF(&f, 1); i++)
    {
        lire_dir_TOF(&f, i, &buf);

        for (int j = 0; j < buf.nb; j++)
        {
            tabInd->tab[tabInd->taille] = buf.tab[j];
            tabInd->taille++;
        }
    }

    fermer_TOF(&f);
}
