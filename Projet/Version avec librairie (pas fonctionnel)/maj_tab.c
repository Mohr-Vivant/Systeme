#include "lib.h"

//ne fonctionne pas
char maj_tab(char tab[27], char c){//c'est une copie d'une partie du code "lettre_utilisee"
	int i = 0;//parce qu'on n'arrive pas à récuperer un tableau en meme temps qu'un booléen
	do{
		i++;
	}while(i < 26 || tab[i] != 0);
	if(i < 26)		tab[i] = c;
	return *tab;
}
