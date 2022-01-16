#include "lib.h"

int faute(char tab[27], char c, char* mot, int etat){
	//cet algo est une répétition d'une partie de l'algo "verif_saisie" car on a pas réussi
	//à récupérer 2 infos dans le meme algo (et donc à faire avancer le pendu en plus du mot)
	if(!lettre_utilisee(tab,c)){				// si la lettre saisie n'as pas déjà été utilisée...
		bool ltr_valide = false;
		for(int i = 0; i < strlen(mot); i++){	//...et si elle ne fait pas partie du mot...
			if(c == mot[i]){
				ltr_valide = true;
			}
		}
		if(!ltr_valide)		etat++;				//... le joueur prends une faute
	}
	return etat;
}
