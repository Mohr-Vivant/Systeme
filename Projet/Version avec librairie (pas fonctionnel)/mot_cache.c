#include "lib.h"

char* mot_cache(char* cache, int taille){
	for(int i = 0; i < taille; i++){
        cache[i] = '#';
    }
    cache[taille] = '\0';
    return cache;//crée un mot de la meme taille que celui recherché mais composé de #
}
