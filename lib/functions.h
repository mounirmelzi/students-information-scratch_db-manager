#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

    #include <stdbool.h>
    #include "TOVC.h"


    /* --[ DECLARATIONS DES FONCTIONS ]-- */

    void init(); // initialiser les variable necessaire pour le bon deroulement du programme
    void clean(); // liberer l'espace memoire allouer pour les variables globales

    void chargement_initial_students(char nomf[]); // chargement initiale aleatoire du fichier de l'annee courante
    void recherche_student(char nomf[], Student e, bool *trouv, bool *eff, int *i, int *j); // recherche sequentielle optimisée d'un etudiant dans le fichier de l'annee courante
    void recherche_student_by_id(char nomf[], char id[], bool *trouv, bool *eff, int *i, int *j); // recherche d'un etudiant dans le fichier de l'annee courante par son ID
    void insertion_student(char nomf[], Student e); // insertion d'un etudiant dans le fichier de l'annee courante
    void suppression_logique_student(char nomf[], Student e); // suppression logique d'un etudiant depuis le fichier de l'annee courante
    bool mise_a_jour_student(char nomf[], Student eAc , Student eNv); // mise a jour des informations d'un etudiant
    void reorganisation_students(char nomf[], char new_nomf[]); // effectuer la re-organisation du fichier de l'annee courante

    /*
        La cle de l'archive est: ClassId + la moyenne
        Le type du fichier archive: TOVC

        Le cout de l’archivage est: le cout de fragmentation + le cout de la fusion: "N est le nombre total des bloc dans le fichier"
            - le cout de fragmentation:
                N lectures [pour lire les donner du fichier de l'annee courante]
                + N * NOMBRE_ENREGISTREMENT_MOYEN_PAR_BLOC * (N / (5*NBR_CLASSES_PAR_ANNEE)) lectures [pour rechercher l'emplacement dans un fichier temp]
                + N * NOMBRE_ENREGISTREMENT_MOYEN_PAR_BLOC * (N / (5*NBR_CLASSES_PAR_ANNEE)) ecritures [pour inserer dans un fichier temp]

                = ((N^2  * (NOMBRE_ENREGISTREMENT_MOYEN_PAR_BLOC)/(5*NBR_CLASSES_PAR_ANNEE)) + N) lectures + (N^2  * (NOMBRE_ENREGISTREMENT_MOYEN_PAR_BLOC)/(5*NBR_CLASSES_PAR_ANNEE)) ecritures
            - le cout de la fusion:
                N ecritures + N lectures (divise entre 5*NBR_CLASSES_PAR_ANNEE fichiers)

            --> Le cout de l'archivage d'une seul annee est:
                Lectures: ((N^2  * (NOMBRE_ENREGISTREMENT_MOYEN_PAR_BLOC)/(5*NBR_CLASSES_PAR_ANNEE)) + 2*N)
                Ecritures: ((N^2  * (NOMBRE_ENREGISTREMENT_MOYEN_PAR_BLOC)/(5*NBR_CLASSES_PAR_ANNEE)) + N)

        Complexity: N^2 (Lectures) + N^2 (Ecritures)
    */
    void create_archive(char nomf_annee_courante[], char nomf_archive[]); // creer le fichier archive apartir du fichier de l'annee courante
    /*
        Le type d'index:
            index secondaire car les cle ne sont pas uniques
            il contient le ClassId et les intervalles des moyennes
            il est dense par rapport au ClassId mais no-dense par rapport au moyennes car chaque classe est ordonnee selon les moyennes
    */
    void create_index(char nomf_archive[], IndexTable *tabInd); // creer la table d'index apartir du fichier archive
    int recherche_index(IndexTable *tabInd, ArchivedStudent e); // rechercher la case dans la table d'index correspodante a un etuduant (recherche dichotomique dans l'index seulement)
    void recherche_archived_student_index(char nomf[], ArchivedStudent e, IndexTable *tabInd, bool *trouv, bool *eff, int *i, int *j, int *k); // recherche d'un etudiant dans le fichier archive en utilisant l'index
    void recherche_archived_student_by_id(char nomf[], char id[], bool *trouv, bool *eff, int *i, int *j); // recherche d'un etudiant dans le fichier archive par son ID (sequentiellement sans l'index)
    void insertion_archived_student_index(char nomf[], ArchivedStudent e, IndexTable *tabInd); // insertion d'un etudiant dans le fichier archive
    void suppression_logique_archived_student(char nomf[], IndexTable *tabInd, ArchivedStudent e); // suppression logique d'un etudiant depuis le fichier archive
    void reorganisation_archived_students(char nomf[], char new_nomf[]); // effectuer la re-organisation du fichier archive

    int requete_intervalle(char nomfArchive[], IndexTable *tabInd, ClassId classId, float moyMin, float moyMax); // requete a intervalle selon les moyennes dans le fichier archive
    void generate_next_year(char nomf_annee_courante[], char nomf_next_annee_courante[]); // generer le fichier de l'annee suivante apartir du fichier de l'annee courante


#endif // FUNCTIONS_H_INCLUDED
