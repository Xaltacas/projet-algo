#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tas.h"


Tas initialiser(int taillemax){
	Tas nouveau;
	
	nouveau.pile = (Arete*)malloc((taillemax+1) * sizeof(Arete));
	nouveau.pile[0].l = 0;
	nouveau.indices = (int*)malloc((taillemax+1) * sizeof(int));
	
	for(int i = 0 ; i < taillemax + 1; i++)
		nouveau.indices[i] = 0;
	return nouveau;
}

double log2(double x){
	return log(x) / log(2);
}

Arete creerA(int s1, int s2, int l){
	Arete nouveau;
	nouveau.s1 = s1;
	nouveau.s2 = s2;
	nouveau.l = l;

	return nouveau;
}

void inverser(Tas* K,int i,int j){

	Arete* tab = K -> pile;
	Arete temp = tab[i];

	K -> indices[tab[i].s2] = j;
	K -> indices[tab[j].s2] = i;

	tab[i] = tab[j];
	
	tab[j] = temp;

}

int est_vide(Tas K){
	return K.pile[0].l == 0 ;
}

int est_feuille(Tas K, int i){
	return K.pile[0].l < (i*2);
}

int a_un_fils_droit(Tas K, int i){
	return K.pile[0].l >= (i*2) + 1 ;
}

int fgauche(Tas K, int i){
	return K.pile[0].l >= (i*2)? i*2 : 0;
}

int fdroite(Tas K, int i){
	return K.pile[0].l >= (i*2) + 1 ? (i*2) + 1 : 0;
}

int pere(Tas K, int i){
	return i/2;
}

void ajouter(Tas* K, int s1, int s2, int l){

	Arete* pilt = K -> pile;
	
	pilt[0].l++;
	int indice = pilt[0].l;

	pilt[indice] = creerA(s1,s2,l);

	int demisI = indice/2;

	while((pilt[indice].l < pilt[demisI].l) && indice != 1){
		inverser(K,indice,demisI);
		indice = demisI;
		demisI = indice/2;
	}
	K -> indices[s2] = indice;
}

void supprime(Tas* K, int s2){

	Arete* pilt = K -> pile;
	
	int indice = K -> indices[s2];
	
	//K -> indices[s2] = -1;

	inverser(K,pilt[0].l,indice);
	
	pilt[0].l--;

	int finis = 0;
	int gauche, droite;
	
	while( !finis){
		gauche = fgauche(*K,indice);
		droite = fdroite(*K,indice);

		if(!gauche){
			finis = 1;
		}
		else if(!droite){
			if(pilt[indice].l <= pilt[gauche].l){
				finis =1;	
			}
			else{
				inverser(K,indice,gauche);
				finis = 1;
			}
		}
		else{
			int min = pilt[gauche].l > pilt[droite].l ? droite : gauche;
			
			if(pilt[indice].l <= pilt[min].l)
				finis =1;
			else{
				inverser(K,indice,min);
				indice = min;
			}

		}

	}
}

int recupMin(Tas* K){
	if(est_vide(*K))
		return 0;
	else{
		
		int res = K -> pile[1].s2;
		supprime(K,res);
		return res;
	}
}


void interclassement(Arete* Tab,int start,int mid,int end){
	int x = start;
	int y = mid;
	
	int taille = end - start +1;

	Arete* res;

	//printf("%ld\t",sizeof(Arete));
	
	
	res = (Arete*)malloc(taille * sizeof(Arete));
	if(res == NULL)
		printf("fail\n");
	


	for(int i = 0; i < taille; i++){
		if(((Tab[x].l < Tab[y].l) && x < mid) || (y > end)){
			res[i] = Tab[x];
			x++;
		}
		else{
			res[i] = Tab[y];
			y++;
		}
	}

	for(int i = 0; i < taille; i++){
		Tab[start + i] = res[i];
	}
	free(res);
}	

void fusion_para(Arete* Tab,int start, int end){
	int n = end - start +1;

	if( n > 1){
		fusion_para(Tab, start, start + (n/2) -1);
		fusion_para(Tab, start + (n/2),end);
		interclassement(Tab, start, start + (n/2), end);
	}
}

void tri_selec(Arete* tab, int taille){

	Arete c;
	for(int i = 0 ;i < taille-1; i++){
		for(int j = i+1 ; j < taille; j++){
			if ( tab[i].l >= tab[j].l ){
				c = tab[i];
				tab[i] = tab[j];
				tab[j] = c;
			}
		}
	}
}
