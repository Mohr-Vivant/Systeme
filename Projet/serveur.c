#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SIZE 15

#define PORT 6000
#define MAX_BUFFER 1000
#define MAX_CLIENTS 3

const char *EXIT = "exit";


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
	
    int fdSocketAttente;
    int fdSocketCommunication;
    struct sockaddr_in coordonneesServeur;
    struct sockaddr_in coordonneesAppelant;
    char tampon[MAX_BUFFER];
    int nbRecu;
    int longueurAdresse;
    int pid;

    fdSocketAttente = socket(PF_INET, SOCK_STREAM, 0);

    if (fdSocketAttente < 0) {
        printf("socket incorrecte\n");
        exit(EXIT_FAILURE);
    }

    // On prépare l’adresse d’attachement locale
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coordonneesServeur, 0x00, longueurAdresse);

    coordonneesServeur.sin_family = PF_INET;
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_addr.s_addr = htonl(INADDR_ANY);
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_port = htons(PORT);

    if (bind(fdSocketAttente, (struct sockaddr *) &coordonneesServeur, sizeof(coordonneesServeur)) == -1) {
        printf("erreur de bind\n");
        exit(EXIT_FAILURE);
    }

    if (listen(fdSocketAttente, 5) == -1) {
        printf("erreur de listen\n");
        exit(EXIT_FAILURE);
    }

    socklen_t tailleCoord = sizeof(coordonneesAppelant);

    int nbClients = 0;

    while (nbClients < MAX_CLIENTS) {
        if ((fdSocketCommunication = accept(fdSocketAttente, (struct sockaddr *) &coordonneesAppelant,
                                            &tailleCoord)) == -1) {
            printf("erreur de accept\n");
            exit(EXIT_FAILURE);
        }

        printf("Client connecté - %s:%d\n",
               inet_ntoa(coordonneesAppelant.sin_addr),
               ntohs(coordonneesAppelant.sin_port));

        do{
			//initialisation
			char* mot = mot_alea();//le mot à trouver
			int taille = strlen(mot);//nb de lettres du mot
			char* cache = malloc((taille+1) * sizeof(char));//on reserve la place pour le mot caché + une fin de chaine
			int etat = 0;//état du pendu, fait perdre la partie après 10 erreurs
			char tab[27] = {0};//tableau contenant les lettres déjà utilisées + 1 pour l'erreur de saisie
			cache = mot_cache(cache, taille);
			
			//on affiche la situation actuelle du jeu
			if ((pid = fork()) == 0) {//--------------------------------partie msg 1--------
				close(fdSocketAttente);

				while (1) {
					// on attend le message du client
					// la fonction recv est bloquante
					nbRecu = recv(fdSocketCommunication, tampon, MAX_BUFFER, 0);

					if (nbRecu > 0) {
						tampon[nbRecu] = 0;
						printf("Recu de %s:%d : %s\n",
							   inet_ntoa(coordonneesAppelant.sin_addr),
							   ntohs(coordonneesAppelant.sin_port),
							   tampon);

						if (testQuitter(tampon)) {
							break; // on quitte la boucle
						}
					}

					lireMessage(tampon);

					if (testQuitter(tampon)) {
						send(fdSocketCommunication, tampon, strlen(tampon), 0);
						break; // on quitte la boucle
					}

					// on envoie le message au client
					send(fdSocketCommunication, tampon, strlen(tampon), 0);
				}

				exit(EXIT_SUCCESS);
			}//valeur de "cache" envoyée au client------------------------------------------
			
			
			//on demande au joueur de jouer, on récupère une valeur tampon
				
			//déroulement de la partie
			do{
				c = tampon[0];			//permet d'éviter les erreurs de saisie
				c = normaliser_saisie(c);//on normalise c pour simplifier son utilisation
				
				//on utilise la saisie du joueur
				cache = verif_saisie(tab, c, mot, cache, etat);//mets le mot caché à jour
				etat = faute(tab, c, mot, etat);//mets l'état du pendu à jour
				*tab = maj_tab(tab, c);//devrait mettre la liste des lettres déjà utilisée pour
				//ne pas faire perdre de points en double mais ne fonctionne pas on l'as laissé
				//en dehors des commentaires car le programme fonctionne avec (ça n'as juste pas l'effet voulu)
				
				//on envoieles valeurs mises à jour de "c", "etat" et "cache"
				if ((pid = fork()) == 0) {
					close(fdSocketAttente);

					while (1) {//---------------------------------------partie msg 2--------
						// on attend le message du client
						// la fonction recv est bloquante
						nbRecu = recv(fdSocketCommunication, tampon, MAX_BUFFER, 0);

						if (nbRecu > 0) {
							tampon[nbRecu] = 0;
							printf("Recu de %s:%d : %s\n",
								   inet_ntoa(coordonneesAppelant.sin_addr),
								   ntohs(coordonneesAppelant.sin_port),
								   tampon);

							if (testQuitter(tampon)) {
								break; // on quitte la boucle
							}
						}

						lireMessage(tampon);

						if (testQuitter(tampon)) {
							send(fdSocketCommunication, tampon, strlen(tampon), 0);
							break; // on quitte la boucle
						}

						// on envoie le message au client
						send(fdSocketCommunication, tampon, strlen(tampon), 0);
					}

					exit(EXIT_SUCCESS);
				}//-------------------------------------------------------------------------
			
			}while(!victoire(cache) && !defaite(etat));	
        

        nbClients++;
    }

    close(fdSocketCommunication);
    close(fdSocketAttente);
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        wait(NULL);
    }

    printf("Fin du programme.\n");
    return EXIT_SUCCESS;
}
