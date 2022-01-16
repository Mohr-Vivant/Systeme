#include "lib.h"

char normaliser_saisie(char c){
    if(c >= 'a' && c <= 'z')
        c = c + 'A' - 'a';//on change les minuscules en majuscules

    if(c < 'A' || c > 'Z'){//on vérifie si le caractere est bien une lettre majuscule
        c = (char)-1;
        printf("Erreur : faute de saisie d'une lettre valide\n");
    }
    return c;
}
