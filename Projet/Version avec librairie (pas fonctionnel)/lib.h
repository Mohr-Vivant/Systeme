#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 15

char* mot_alea(void);

void affichage_pendu(int etat);

char* mot_cache(char* cache, int taille);

char normaliser_saisie(char c);

bool defaite(int etat);

bool victoire(char* cache);

//ne fonctionne pas
bool lettre_utilisee(char tab[27], char c);

//ne fonctionne pas
char maj_tab(char tab[27], char c);

char* verif_saisie(char tab[27], char c, char* mot, char* cache, int etat);

int faute(char tab[27], char c, char* mot, int etat);
