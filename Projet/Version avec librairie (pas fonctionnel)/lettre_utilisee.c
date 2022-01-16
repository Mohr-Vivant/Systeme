#include "lib.h"

//ne fonctionne pas
bool lettre_utilisee(char tab[27], char c){//vérifie si la lettre choisie a déjà été utilisée
	bool utilisee = false;
	int i = 0;
	do{
		if(c == tab[i])		utilisee = true;//on se sert d'un tableau qui se rempli au fil de la partie
		i++;
	}while(i < 26 || tab[i] != 0);
	if(i < 26)		tab[i] = c;//aujoute c au tableau
	return utilisee;
}
