#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SIZE 15

#define PORT 6000
#define MAX_BUFFER 1000

const char *EXIT = "exit";

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

void lireMessage(char tampon[]) {
    printf("Saisir un message à envoyer :\n");
    fgets(tampon, MAX_BUFFER, stdin);
    strtok(tampon, "\n");
}

int testQuitter(char tampon[]) {
    return strcmp(tampon, EXIT) == 0;
}


int main(int argc, char const *argv[]) {
	char c;//lettre donnée par le joueur
	
    int fdSocket;
    int nbRecu;
    struct sockaddr_in coordonneesServeur;
    int longueurAdresse;
    char tampon[MAX_BUFFER];

    fdSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (fdSocket < 0) {
        printf("socket incorrecte\n");
        exit(EXIT_FAILURE);
    }

    // On prépare les coordonnées du serveur
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coordonneesServeur, 0x00, longueurAdresse);

    coordonneesServeur.sin_family = PF_INET;
    // adresse du serveur
    inet_aton("127.0.0.1", &coordonneesServeur.sin_addr);
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_port = htons(PORT);

    if (connect(fdSocket, (struct sockaddr *) &coordonneesServeur, sizeof(coordonneesServeur)) == -1) {
        printf("connexion impossible\n");
        exit(EXIT_FAILURE);
    }

    printf("connexion ok\n");

    while (1) {
		printf("Bienvenue dans le jeu du Pendu !\nAppuyez sur entrer...");
		char* cache = malloc((taille+1) * sizeof(char));//on reserve la place pour le mot caché + une fin de chaine
		int etat = 0;//état du pendu, fait perdre la partie après 10 erreurs
        
        lireMessage(tampon);//------------------------------------------partie rep 1

        if (testQuitter(tampon)) {
            send(fdSocket, tampon, strlen(tampon), 0);
            break; // on quitte la boucle
        }

        // on envoie le message au serveur
        send(fdSocket, tampon, strlen(tampon), 0);

        // on attend la réponse du serveur
        nbRecu = recv(fdSocket, tampon, MAX_BUFFER, 0);

        if (nbRecu > 0) {
            tampon[nbRecu] = 0;
            printf("Recu : %s\n", tampon);

            if (testQuitter(tampon)) {
                break; // on quitte la boucle
            }
        }//on récupère la valeur de "cache"------------------------------------------
        
        printf("Un mot aléatoire a été généré\n\n");
        
        do{
			printf("Mot à trouver : %s\n", cache);
			affichage_pendu(etat);
			printf("Saisissez une lettre à proposer :");
			
			lireMessage(tampon);//------------------------------------------partie rep 2

			if (testQuitter(tampon)) {
				send(fdSocket, tampon, strlen(tampon), 0);
				break; // on quitte la boucle
			}

			// on envoie le message au serveur
			send(fdSocket, tampon, strlen(tampon), 0);

			// on attend la réponse du serveur
			nbRecu = recv(fdSocket, tampon, MAX_BUFFER, 0);

			if (nbRecu > 0) {
				tampon[nbRecu] = 0;
				printf("Recu : %s\n", tampon);

				if (testQuitter(tampon)) {
					break; // on quitte la boucle
				}
			}//on récupère la version normalisée de c et les valeurs de "etat" et de "cache"----
			
			printf("Lettre prise en compte : %c\n", c);
        }while(!victoire(cache) && !defaite(etat));
        
        //conclution de la partie
		if(victoire(cache))		printf("Bien joué ! Vous avez trouvé le mot %s en seulement %d fautes !\n",cache, etat);
		else if(defaite(etat)){
			affichage_pendu(etat);
			printf("Pendu ! Dommage, le mot était %s mais vous y étiez presque !\n", mot);
		}else 	printf("Erreur : pb avec la vérification des conditions de victoire/défaite.");
		
    }

    close(fdSocket);

    return EXIT_SUCCESS;
}
