#include "lib.h"

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
		
		printf("Voulez-vous rejouer (Yes/No) ? ");
		scanf(" %s", s);
		c = s[0];
		c = normaliser_saisie(c);
	}while(c == 'Y');
}
