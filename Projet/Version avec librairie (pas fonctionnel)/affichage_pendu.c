#include "lib.h"

void affichage_pendu(int etat){
    switch (etat){//l'etat défini l'affichage du pendu
		case 0 : printf("\n\n\n\n\n"); break;
		case 1 : printf("\n\n\n\n\n___ ___"); break;
		case 2 : printf("\n   |\n   |\n   |\n   |\n___|___"); break;
		case 3 : printf("   ______\n   |/\n   |\n   |\n   |\n___|___"); break;
		case 4 : printf("   ______\n   |/   |\n   |\n   |\n   |\n___|___"); break;
		case 5 : printf("   ______\n   |/   |\n   |    O\n   |\n   |\n___|___"); break;
		case 6 : printf("   ______\n   |/   |\n   |    O\n   |    |\n   |\n___|___"); break;
		case 7 : printf("   ______\n   |/   |\n   |    O\n   |   /|\n   |\n___|___"); break;
		case 8 : printf("   ______\n   |/   |\n   |    O\n   |   /|\\\n   |\n___|___"); break;
		case 9 : printf("   ______\n   |/   |\n   |    O\n   |   /|\\\n   |   /\n___|___"); break;
		case 10 : printf("   ______\n   |/   |\n   |    O\n   |   /|\\\n   |   / \\\n___|___"); break;
        default : printf("Erreur : problème avec l affichage du pendu.\n");
    }
    printf("\n\n");
}
