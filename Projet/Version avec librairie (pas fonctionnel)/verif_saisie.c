#include "lib.h"

char* verif_saisie(char tab[27], char c, char* mot, char* cache, int etat){
	bool ltr_valide = false;
	if(lettre_utilisee(tab, c))		printf("Lettre déjà utilisée ! Essayez autre chose.\n\n");
	else{
		for(int i = 0; i < strlen(mot); i++){
			if(c == mot[i]){
				cache[i] = c;
				ltr_valide = true;
			}
		}
		if(ltr_valide)		printf("Nouvelle lettre trouvée ! Continuez.\n\n");
		else{
			printf("Faute ! Cette lettre ne fait pas partie du mot cherché.\n\n");
			etat++;//le but est de faire avancer l'état du pendu ici mais on a pas réussi,
				   //d'où la présence de l'algo "faute"
		}
	}
	return cache;
}
