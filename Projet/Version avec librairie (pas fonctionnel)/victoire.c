#include "lib.h"

bool victoire(char* cache){
	bool res = true;
	for(int i = 0; i < strlen(cache); i++){
		if(cache[i] == '#')		res = false;
	}
	return res;//renvoie true si le mot est complet
}
