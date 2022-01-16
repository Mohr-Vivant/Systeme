#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 15

char* mot_alea(){
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

char* mot_cache(char* cache, int taille){
	for(int i = 0; i < taille; i++){
        cache[i] = '#';
    }
    cache[taille] = '\0';
    return cache;//crée un mot de la meme taille que celui recherché mais composé de #
}

char normaliser_saisie(char c){
    if(c >= 'a' && c <= 'z')
        c = c + 'A' - 'a';//on change les minuscules en majuscules

    if(c < 'A' || c > 'Z'){//on vérifie si le caractere est bien une lettre majuscule
        c = (char)-1;
        printf("Erreur : faute de saisie d'une lettre valide\n");
    }
    return c;
}

bool defaite(int etat){
	return etat==10;//renvoie true si 10 erreurs ont été commises
}

bool victoire(char* cache){
	bool res = true;
	for(int i = 0; i < strlen(cache); i++){
		if(cache[i] == '#')		res = false;
	}
	return res;//renvoie true si le mot est complet
}

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

//ne fonctionne pas
char maj_tab(char tab[27], char c){//c'est une copie d'une partie du code "lettre_utilisee"
	int i = 0;//parce qu'on n'arrive pas à récuperer un tableau en meme temps qu'un booléen
	do{
		i++;
	}while(i < 26 || tab[i] != 0);
	if(i < 26)		tab[i] = c;
	return *tab;
}

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

int main(){
	char c;//lettre donnée par le joueur
	do{
		//initialisation
		char* mot = mot_alea();//le mot à trouver
		int taille = strlen(mot);//nb de lettres du mot
		char* cache = malloc((taille+1) * sizeof(char));//on reserve la place pour le mot caché + une fin de chaine
		int etat = 0;//état du pendu, fait perdre la partie après 10 erreurs
		char tab[27] = {0};//tableau contenant les lettres déjà utilisées + 1 pour l'erreur de saisie
		char s[taille];//nécessaire pour la saisie de c
		cache = mot_cache(cache, taille);
		
		printf("Bienvenue dans le jeu du Pendu !\n");
		printf("Un mot aléatoire a été généré\n\n");
		
		//déroulement de la partie
		do{
			//on affiche la situation actuelle du jeu
			printf("Mot à trouver : %s\n", cache);
			affichage_pendu(etat);
			
			//on demande au joueur de jouer
			printf("Saisissez une lettre à proposer :");
			scanf(" %s", s);	//on demande à l'utilisateur d'écrire une lettre
			c = s[0];			//ces 2 étapes permettent d'éviter les erreurs de saisie
			c = normaliser_saisie(c);//on normalise c pour simplifier son utilisation
			printf("Lettre prise en compte : %c\n", c);
			
			//on utilise la saisie du joueur
			cache = verif_saisie(tab, c, mot, cache, etat);//mets le mot caché à jour
			etat = faute(tab, c, mot, etat);//mets l'état du pendu à jour
			*tab = maj_tab(tab, c);//devrait mettre la liste des lettres déjà utilisée pour
			//ne pas faire perdre de points en double mais ne fonctionne pas on l'as laissé
			//en dehors des commentaires car le programme fonctionne avec (ça n'as juste pas l'effet voulu)
		}while(!victoire(cache) && !defaite(etat));
		
		//conclution de la partie
		if(victoire(cache))		printf("Bien joué ! Vous avez trouvé le mot %s en seulement %d fautes !\n",cache, etat);
		else if(defaite(etat)){
			affichage_pendu(etat);
			printf("Pendu ! Dommage, le mot était %s mais vous y étiez presque !\n", mot);
		}else 	printf("Erreur : pb avec la vérification des conditions de victoire/défaite.");
		
		printf("\nVoulez-vous rejouer (Yes/No) ? ");
		scanf(" %s", s);
		c = s[0];
		c = normaliser_saisie(c);
	}while(c == 'Y');
	printf("\nAu revoir !\n");
}
