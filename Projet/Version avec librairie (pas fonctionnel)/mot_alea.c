#include "lib.h"

char* mot_alea(void){
	//on ouvre le fichier txt qui contient notre liste de mots
	FILE* fichier;
	fichier = fopen("BDD.txt","r");
	
	/*cette partie devrait compter le nb de mots dans la bdd mais ne fonctionne pas
	//on compte le nombre max de mots
	int c=fgetc(fichier);
	int nbLignes = 0;
	while(c != EOF){
		if(c=='\n')		nbLignes++;
	}
	nbLignes --;
	if (nbLignes = -1){
		printf("Erreur : BDD vide, pas de mots en stock");
		return NULL;
	}
	*/
	int nbLignes = 25;//on donne une valeur fixe à la place (qu'il faut donc maj manuellement)
	
	//on défini un nombre aléatoire compris en 0 et max
	srand(time(NULL));
	int n = rand() % (nbLignes);
	
	//on récupère le mot correspondant au nb aléatoire généré	
	char* mot = malloc(MAX_SIZE * sizeof(char));
	for(int i = 0; i <= n; i++){
        fgets(mot, MAX_SIZE, fichier);
    }
    mot[strlen(mot)-1] = '\0';//on enleve le caractere de retour a la ligne et on ferme la chaine
	
	return mot;
}
