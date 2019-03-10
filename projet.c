/* Mini-projet ROB3 Algorithmique 2012-13 */

/* pour compiler : gcc projet.c -o projet */
/* pour lancer le programme après compilation : ./projet nom_du_fichier_tsp_sans_extension */
/* exemple : ./projet berlin52 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "fonctions.h"
#include "tas.h"



/********/
/* main */
/********/

/* le contenu du fichier main est donne ici a titre de demonstration */
/* du fonctionnement des differentes fonctions et procedures */
int main(int argc, char *argv[])
{

	Graphe g;
	Coordonnees c;

	// lecture de l'instance
	c = lecture_instance(argv[1]);

	// affichage de l'instance
	//afficher_instance(argv[1]);


	// affichage de la tournee optimale
	char nom_tour[256];
	sprintf(nom_tour,"%s.opt",argv[1]);
/*
	afficher_tour(c,nom_tour);
	g = creer_graphe(c->n);


	// toutes les aretes partant du sommet 0 sont presentes
	int i,j;
	for(i = 0; i < c->n; i++) {
        	for(j = 0; j < c->n; j++) {
            		if((i == 0)&&(i!=j)) graphe_ajouter_arete(g, i, j);
        	}
    	}
	printf("le degre du sommet 0 est : %d\n",graphe_degre(g,0));
	afficher_graphe(c,g);

*/
	g = creer_graphe(c -> n);
	/*
	prim(c, g);

	graphe_retirer_arete(g,42,32);
	graphe_ajouter_arete(g,42,32);
	graphe_retirer_arete(g,42,32);
	graphe_ajouter_arete(g,42,32);
	graphe_retirer_arete(g,42,32);
	*/
	//arbre1(c,g);

	clock_t temps_initial = clock();

	printf("borne min = %d\n",borneMin(c,g,50778));

	clock_t temps_final = clock();

	printf("temps = %ld\n", (temps_final - temps_initial)/CLOCKS_PER_SEC);

	/*

	printf("longueur = %d\n",cycle(c,g));

	afficher_graphe(c,g);

	int compt =0;
	while(opt2bis(g,c) && compt < 100) compt++;

	printf("2-opt: %d\n",compt);
	printf("longueur = %d\n",longueur_cycle(g,c));

	afficher_graphe(c,g);


	compt =0;
	while(lissage(g,c) && compt < 100) compt++;

	printf("lissage : %d\n",compt);


	printf("longueur = %d\n",longueur_cycle(g,c));

	*/



	/*

	float poids[ c-> n];

	for(int i = 0; i < c ->n ; i++)
		poids[i] = 0;

	printf("dist min = %d\n",primPoids(c,g,poids));

	*/

	afficher_graphe(c,g);
	// conversion d'un fichier tour en un graphe


/*
	detruire_graphe(g);


	g = conversion_tour_graphe(nom_tour);
	afficher_graphe(c,g);
*/

	// liberation de la memoire
	detruire_graphe(g);
	detruire_coordonnees(c);



	return 0;
}
