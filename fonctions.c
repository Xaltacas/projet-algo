#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "fonctions.h"
#include "tas.h"


/***************************************************************************************/
/* les implementations des fonctions et procedures liees aux graphes et aux affichages */
/***************************************************************************************/

/* cree un nouveau graphe avec n sommets numerotes de 0 a n-1 et aucune arete */
Graphe creer_graphe(int n)
{
    Graphe g;
    int i;

    g = malloc(sizeof(struct graphe) + sizeof(struct voisins *) * (n-1));
    assert(g);

    g->n = n;
    g->m = 0;

    for(i = 0; i < n; i++) {
        g->alist[i] = malloc(sizeof(struct voisins));
        assert(g->alist[i]);

        g->alist[i]->d = 0;
        g->alist[i]->nbcell = 1;
        g->alist[i]->est_trie= 1;
    }

    return g;
}

/* libere la memoire utilisee par le graphe g */
void detruire_graphe(Graphe g)
{
    int i;

    for(i = 0; i < g->n; i++) free(g->alist[i]);
    free(g);
}

/* ajoute une arete a un graphe existant */
/* le faire plus d'une fois peut conduire a des resultats imprevisibles */
void graphe_ajouter_arete(Graphe g, int s1, int s2)
{
    assert(s1 >= 0);
    assert(s1 < g->n);
    assert(s2 >= 0);
    assert(s2 < g->n);

    /* est-il necessaire d'agrandir la liste de s1 ? */
    while(g->alist[s1]->d >= g->alist[s1]->nbcell) {
        g->alist[s1]->nbcell *= 2;
        g->alist[s1] =
            realloc(g->alist[s1],
                sizeof(struct voisins) + sizeof(int) * (g->alist[s1]->nbcell - 1));
    }

    /* est-il necessaire d'agrandir la liste de s2 ? */
    while(g->alist[s2]->d >= g->alist[s2]->nbcell) {
        g->alist[s2]->nbcell *= 2;
        g->alist[s2] =
            realloc(g->alist[s2],
                sizeof(struct voisins) + sizeof(int) * (g->alist[s2]->nbcell - 1));
    }

    /* ajoute le sommet s2 dans la liste d'adjacence de s1 */
    g->alist[s1]->list[g->alist[s1]->d++] = s2;
    g->alist[s1]->est_trie = 0;

    /* ajoute le sommet s1 dans la liste d'adjacence de s2 */
    g->alist[s2]->list[g->alist[s2]->d++] = s1;
    g->alist[s2]->est_trie = 0;

    /* incremente le compteur d'aretes */
    g->m++;
}

void graphe_retirer_arete(Graphe g, int s1,int s2){

	int i = 0;
	int finis = 0;
	while( !finis && i < g -> alist[s1] -> d){
		if( g -> alist[s1] -> list[i] == s2){
			 g -> alist[s1] -> list[i] = g -> alist[s1] -> list[g -> alist[s1] -> d-1];
			 g -> alist[s1] -> d--;
			 finis = 1;
		}
		i++;
	}

	i = 0;
	finis = 0;
	while( !finis && i < g -> alist[s2] -> d){
		if( g -> alist[s2] -> list[i] == s1){
			g -> alist[s2] -> list[i] = g -> alist[s2] -> list[g -> alist[s2] -> d-1];
			g -> alist[s2] -> d--;
			finis = 1;
		}
		i++;
	}
}


/* retourne le nombre de sommets d'un graphe */
int graphe_nb_sommets(Graphe g)
{
    return g->n;
}

/* retourne le nombre d'aretes d'un graphe */
int graphe_nb_aretes(Graphe g)
{
    return g->m;
}

/* retourne le degre d'un sommet s */
int graphe_degre(Graphe g, int s)
{
    assert(s >= 0);
    assert(s < g->n);

    return g->alist[s]->d;
}

/* a partir d'une liste de longueur 10 */
/* on appelle la recherche dichotomique */
#define BSEARCH_THRESHOLD (10)

static int intcmp(const void *a, const void *b)
{
    return *((const int *) a) - *((const int *) b);
}

/* retourne 1 si l'arete (s1,s2) existe, 0 sinon */
int graphe_arete_existe(Graphe g, int s1, int s2)
{
    int i;

    assert(s1 >= 0);
    assert(s1 < g->n);
    assert(s2 >= 0);
    assert(s2 < g->n);

    if(graphe_degre(g,s1) >= BSEARCH_THRESHOLD) {
        /* on s'assure que c'est trie */
        if(! g->alist[s1]->est_trie) {
            qsort(g->alist[s1]->list,g->alist[s1]->d,sizeof(int),intcmp);
        }

        /* on appelle la recherche dichotomique */
        return
            bsearch(&s2,g->alist[s1]->list,g->alist[s1]->d,sizeof(int),intcmp)!= 0;
    } else {
        /* on realise une simple recherche sequentielle */
        for(i = 0; i < g->alist[s1]->d; i++) {
            if(g->alist[s1]->list[i] == s2) return 1;
        }
        /* sinon */
        return 0;
    }
}

/* conversion d'un fichier tour en un graphe */
Graphe conversion_tour_graphe(char *nom_tour)
{
	FILE* fichier_tour = NULL;
	char temp[256];
	int i,j,dimension,sommet1,sommet2;
	Graphe g;

	char nom_complet_tour[256];

	/* ouverture en lecture du fichier nom_tour.tour */
	/* exemple : berlin52.opt.tour */
	sprintf(nom_complet_tour,"%s.tour",nom_tour);
	fichier_tour = fopen(nom_complet_tour, "r");

	/* on recupere la dimension dans nom_tour.tour */
	/* exemple : on recupere la dimension dans berlin52.opt.tour */
	do
	{
		fgets(temp,256,fichier_tour);
	} while (temp[0] != 'D');

	i=0;
	while (temp[i] != ':') i++;
	i=i+2;
	char dim[256];
	j=0;
	while (temp[i] != '\0')
	{
		dim[j]=temp[i];
		i++; j++;
	}
	dim[j]='\0';
	dimension=atoi(dim);

	/* creation d'un graphe vide */
	g = creer_graphe(dimension);

	/* on se place sur la ligne comportant le sommet 1 dans nom_tour.tour */
	/* exemple : ligne comportant le sommet 1 dans berlin52.opt.tour */
	fgets(temp,256,fichier_tour);
	while(temp[0] != '1')
	{
		fgets(temp,256,fichier_tour);
	}

	/* on parcourt le fichier nom_tour.tour pour generer le graphe g */
	/* exemple : on parcourt berlin52.opt.tour pour generer le graphe g */
	sommet1=1;
	fscanf(fichier_tour,"%d",&sommet2);
	while (sommet2 != -1)
	{
		graphe_ajouter_arete(g,sommet1-1,sommet2-1);
		sommet1=sommet2;
		fscanf(fichier_tour,"%d",&sommet2);
	}

	/* on genere la derniere arete de la tournee */
	graphe_ajouter_arete(g,sommet1-1,0);

	/* fermeture du fichier nom_tour.tour */
	fclose(fichier_tour);

	return g;
}

/* cree un tableau de n coordonnees numerotees de 0 a n-1 et toutes a (0,0) */
/* le parametre "nom" est le nom que l'on veut donner a l'instance */
Coordonnees creer_coordonnees(int n,char *nom)
{
    Coordonnees c;
    int i;

    c = malloc(sizeof(struct coordonnees) + sizeof(struct coord *) * (n-1));
    assert(c);

    c->n = n;
    sprintf(c->nom,"%s",nom);

    for(i = 0; i < n; i++) {
        c->clist[i] = malloc(sizeof(struct coord));
        assert(c->clist[i]);

        c->clist[i]->coordx = 0;
        c->clist[i]->coordy = 0;
    }

    return c;
}

/* libere la memoire utilisee par les coordonnees c */
void detruire_coordonnees(Coordonnees c)
{
    int i;

    for(i = 0; i < c->n; i++) free(c->clist[i]);
    free(c);
}

/* lit une instance dans le fichier nom.tsp, charge les coordonnees en memoire dans c */
/* et cree un fichier nom.gnu pour l'affichage */
Coordonnees lecture_instance(char* nom)
{
	int i,j,point,dimension;
	float coordx,coordy;
	char temp[256];
	char* plot = "plot";
 	FILE* fichier_tsp = NULL;
	FILE* fichier_dat = NULL;
	FILE* fichier_gnu = NULL;
	Coordonnees c;

	/* ouverture en lecture du fichier nom.tsp */
	/* exemple : berlin52.tsp */
	char nom_tsp[256];
       	sprintf(nom_tsp,"%s.tsp",nom);
	fichier_tsp = fopen(nom_tsp, "r");

	/* ouverture en ecriture du fichier nom.dat */
	/* le fichier stocke les differents points */
	/* sous un format interpretable par gnuplot */
	/* exemple : berlin52.dat */
	char nom_dat[256];
	sprintf(nom_dat,"%s.dat",nom);
	fichier_dat = fopen(nom_dat, "w");

	/* ouverture en ecriture du fichier nom.gnu */
	/* le fichier nom.gnu fait appel a nom.dat */
	/* exemple : berlin52.gnu */
	char nom_gnu[256];
	sprintf(nom_gnu,"%s.gnu",nom);
	fichier_gnu = fopen(nom_gnu, "w");

	if ((fichier_tsp != NULL)&&(fichier_dat != NULL)&&(fichier_gnu != NULL))
	{
        	/* on recupere la dimension dans nom.tsp */
		/* exemple : on recupere la dimension dans berlin52.tsp */
		for (i=0;i<3;i++) fgets(temp,256,fichier_tsp);
		fgets(temp,256,fichier_tsp);
		i=0;
		while (temp[i] != ':') i++;
		i=i+2;
		char dim[256];
		j=0;
		while (temp[i] != '\0')
		{
			dim[j]=temp[i];
			i++; j++;
		}
		dim[j]='\0';
		dimension=atoi(dim);

		/* creation du tableau de coordonnees */
		c = creer_coordonnees(dimension,nom);

		/* on charge les coordonnees en memoire a partir du fichier nom.tsp */
		/* exemple : a partir de berlin52.tsp */
		for (i=0;i<2;i++) fgets(temp,256,fichier_tsp);
		for (i=0;i<c->n;i++)
		{
			fscanf(fichier_tsp,"%d %f %f",&point,&coordx,&coordy);
			c->clist[point-1]->coordx = coordx;
			c->clist[point-1]->coordy = coordy;
			fprintf(fichier_dat,"%d %f %f \n",point,coordx,coordy);
		}
	}
    	else
	{
	        /* On affiche un message d'erreur */
	        printf("Impossible d'ouvrir le fichier");
	}

	/* on genere les dernieres lignes de berlin52.gnu */
	fprintf(fichier_gnu,"%s",plot);
	fprintf(fichier_gnu,"%c",' ');
        fprintf(fichier_gnu,"%c",putchar(39));
	fprintf(fichier_gnu,"%s",nom_dat);
 	fprintf(fichier_gnu,"%c",putchar(39));
	fprintf(fichier_gnu,"%s"," u 2:3 title ");
	fprintf(fichier_gnu,"%c",putchar(39));
	fprintf(fichier_gnu,"%s",nom);
 	fprintf(fichier_gnu,"%c",putchar(39));
	fprintf(fichier_gnu,"%c",'\n');
	fprintf(fichier_gnu,"%s","pause -1");
	fprintf(fichier_gnu,"%c",'\n');

	/* on ferme les differents fichiers */
	fclose(fichier_tsp);
	fclose(fichier_dat);
	fclose(fichier_gnu);

	/* retourne les coordonnees */
	return c;
}

/* affiche une instance apres lecture de l'instance nom.tsp */
/* a partir du fichier nom.gnu qui a ete genere */
void afficher_instance(char* nom)
{
	char commande[256];
       	sprintf(commande,"gnuplot %s.gnu",nom);
	system(commande);
}

/* affiche une tournee nom_tour.tour */
/* prerequis : avoir lance lecture_instance au prealable */
/* exemple : affiche berlin52.opt.tour apres avoir lu berlin52.tsp */
void afficher_tour(Coordonnees c, char* nom_tour)
{
	FILE* fichier_gnu = NULL;
	FILE* fichier_tour_gnu = NULL;
	FILE* fichier_tour = NULL;
	int sommet1,sommet2;

	char nom_gnu[256];
	char nom_tour_gnu[256];
	char nom_complet_tour[256];

	/* ouverture en lecture du fichier (c->nom).gnu */
	/* exemple : berlin52.gnu */
	sprintf(nom_gnu,"%s.gnu",c->nom);
	fichier_gnu = fopen(nom_gnu, "r");

	/* ouverture en ecriture du fichier nom_tour.gnu */
	/* exemple : berlin52.opt.gnu */
	sprintf(nom_tour_gnu,"%s.gnu",nom_tour);
	fichier_tour_gnu = fopen(nom_tour_gnu, "w");

	/* ouverture en lecture du fichier nom_tour.tour */
	/* exemple : berlin52.opt.tour */
	sprintf(nom_complet_tour,"%s.tour",nom_tour);
	fichier_tour = fopen(nom_complet_tour, "r");

	/* on se place sur la ligne comportant le sommet 1 dans nom_tour.tour */
	/* exemple : ligne comportant le sommet 1 dans berlin52.opt.tour */
	char temp[256];
	fgets(temp,256,fichier_tour);
	while(temp[0] != '1')
	{
		fgets(temp,256,fichier_tour);
	}

	/* on copie la premiere ligne de nom.gnu dans nom_tour.gnu */
	/* exemple : on copie de berlin52.gnu vers berlin52.opt.gnu */
	char ligne[256];
	fgets(ligne,sizeof(ligne),fichier_gnu);
	fprintf(fichier_tour_gnu,"%s",ligne);

	/* on parcourt le fichier nom_tour.tour pour generer nom_tour.gnu */
	/* exemple : on parcourt berlin52.opt.tour pour generer berlin52.opt.gnu */
	sommet1=1;
	fscanf(fichier_tour,"%d",&sommet2);
	while (sommet2 != -1)
	{
		fprintf(fichier_tour_gnu,"set arrow from %f,%f to %f,%f nohead\n",c->clist[sommet1-1]->coordx,c->clist[sommet1-1]->coordy,c->clist[sommet2-1]->coordx,c->clist[sommet2-1]->coordy);
		sommet1=sommet2;
		fscanf(fichier_tour,"%d",&sommet2);
	}

	/* on genere les dernieres lignes de nom_tour.gnu */
	/* exemple : on genere les dernieres lignes de berlin52.opt.gnu */
	fprintf(fichier_tour_gnu,"set arrow from %f,%f to %f,%f nohead\n",c->clist[sommet1-1]->coordx,c->clist[sommet1-1]->coordy,c->clist[0]->coordx,c->clist[0]->coordy);
	fprintf(fichier_tour_gnu,"%s","replot\n");
	fprintf(fichier_tour_gnu,"%s","pause -1\n");

	/* fermeture des differents fichiers */
	fclose(fichier_gnu);
	fclose(fichier_tour_gnu);
	fclose(fichier_tour);

	/* affichage obtenu par 'gnuplot nom_tour.gnu' */
	/* exemple : affichage obtenu par 'gnuplot berlin52.opt.gnu' */
	char commande[256];
	sprintf(commande,"gnuplot %s.gnu",nom_tour);
	system(commande);
}

/* affiche le graphe g */
/* prerequis : avoir lance lecture_instance au prealable */
void afficher_graphe(Coordonnees c, Graphe g)
{
	FILE* fichier_gnu = NULL;
	FILE* fichier_graphe_gnu = NULL;
	int i,sommet1,sommet2;
	char nom_gnu[256];

	/* ouverture en lecture de nom.gnu */
	/* exemple : berlin52.gnu */
	/* ouverture en ecriture de "graphe.gnu" */
	sprintf(nom_gnu,"%s.gnu",c->nom);
	fichier_gnu = fopen(nom_gnu, "r");
	fichier_graphe_gnu = fopen("graphe.gnu", "w");

	/* generation de "graphe.gnu" a partir de nom.gnu et de g */
	char ligne[256];
	fgets(ligne,sizeof(ligne),fichier_gnu);
	fprintf(fichier_graphe_gnu,"%s",ligne);
	for(sommet1 = 0; sommet1 < g->n; sommet1++) {
		for (i = 0; i < g->alist[sommet1]->d; i++) {
			sommet2 = g->alist[sommet1]->list[i];
			if (sommet2 > sommet1) fprintf(fichier_graphe_gnu,"set arrow from %f,%f to %f,%f nohead\n",c->clist[sommet1]->coordx,c->clist[sommet1]->coordy,c->clist[sommet2]->coordx,c->clist[sommet2]->coordy);
		}
	}
	fprintf(fichier_graphe_gnu,"%s","replot\n");
	fprintf(fichier_graphe_gnu,"%s","pause -1\n");

	/*fermeture des differents fichiers */
	fclose(fichier_gnu);
	fclose(fichier_graphe_gnu);

	/* affichage */
	system("gnuplot graphe.gnu");
}

void prim(Coordonnees c, Graphe g){

	Tas tare = initialiser((c -> n) + 1);

	tare.indices[0] = -1;

	for(int i = 1; i < c -> n ; i++){

		ajouter(&tare,0,i,distance(0,i,c));
	}

	while(tare.pile[0].l > 0){


		int s1 = tare.pile[1].s1;
		int s2 = tare.pile[1].s2;

		graphe_ajouter_arete(g,s1,s2);
		recupMin(&tare);
		tare.indices[s2] = -1;


		for(int i = 1; i < c -> n; i++){
			int dist = distance(s2,i,c);
			if((tare.indices[i] != -1) && (dist < tare.pile[tare.indices[i]].l) ){

				supprime(&tare, i);
				ajouter(&tare, s2, i, dist);
			}
		}
	 }

}

int distance(int s1, int s2, Coordonnees c){

	return floor( sqrt(pow(c -> clist[s1] -> coordx - c-> clist[s2] -> coordx,2) +
			   pow(c -> clist[s1] -> coordy - c-> clist[s2] -> coordy,2)));

}

void arbre1(Coordonnees c, Graphe g){

	Tas tare = initialiser((c -> n) + 1);

	tare.indices[0] = -1;
	tare.indices[1] = -1;

	for(int i = 2; i < c -> n ; i++)
		ajouter(&tare,1,i,distance(1,i,c));

	while(tare.pile[0].l > 0){

		int s1 = tare.pile[1].s1;
		int s2 = tare.pile[1].s2;

		graphe_ajouter_arete(g,s1,s2);
		recupMin(&tare);
		tare.indices[s2] = -1;

		for(int i = 1; i < c -> n; i++){
			int dist = distance(s2,i,c);
			if((tare.indices[i] != -1) && (dist < tare.pile[tare.indices[i]].l) ){
				supprime(&tare, i);
				ajouter(&tare, s2, i, dist);

			}
		}
	}

	int d1 = distance(0,1,c);
	int d2 = distance(0,2,c);

	int r1 = 1;
	int r2 = 2;

	if( d2 < d1){
		int temp = d1;
		d1 = d2;
		d2 = temp;

		temp = r1;
		r1 = r2;
		r2 = temp;
	}

	for(int i = 3; i < c -> n ; i++){

		int dist = distance(0,i,c);
		if(dist < d1){
			d2 = d1;
			r2 = r1;

			d1 = dist;
			r1 = i;
		}else if(dist < d2){
			d2 = dist;
			r2 = i;
		}
	}
	graphe_ajouter_arete(g,0,r1);
	graphe_ajouter_arete(g,0,r2);
}

int primPoids(Coordonnees c, Graphe g,float* poids){

	int res = 0;
	Tas tare = initialiser((c -> n) + 1);

	tare.indices[0] = -1;
	tare.indices[1] = -1;

	for(int i = 2; i < c -> n ; i++)
		ajouter(&tare,1,i,distance(1,i,c)+ poids[1] + poids[i]);

	while(tare.pile[0].l > 0){

		int s1 = tare.pile[1].s1;
		int s2 = tare.pile[1].s2;

		graphe_ajouter_arete(g,s1,s2);
		res += distance(s1,s2,c) + poids[s1] + poids[s2];

		recupMin(&tare);
		tare.indices[s2] = -1;

		for(int i = 1; i < c -> n; i++){
			int prio = distance(s2,i,c) + poids[s2] + poids[i];
			if((tare.indices[i] != -1) && (prio < tare.pile[tare.indices[i]].l) ){

				supprime(&tare, i);
				ajouter(&tare, s2, i,prio );
			}
		}
	}
	int d1 = distance(0,1,c);
	int d2 = distance(0,2,c);

	int r1 = 1;
	int r2 = 2;

	if( d2 < d1){
		int temp = d1;
		d1 = d2;
		d2 = temp;

		temp = r1;
		r1 = r2;
		r2 = temp;
	}

	for(int i = 3; i < c -> n ; i++){

		int dist = distance(0,i,c);
		if(dist < d1){
			d2 = d1;
			r2 = r1;

			d1 = dist;
			r1 = i;
		}else if(dist < d2){
			d2 = dist;
			r2 = i;
		}
	}
	graphe_ajouter_arete(g,0,r1);
	graphe_ajouter_arete(g,0,r2);
	res += distance(0,r1,c) + poids[0] + poids[r1];
	res += distance(0,r2,c) + poids[0] + poids[r2];
	int sommep = 0;
	for(int i = 0; i < c -> n; i++)
		sommep += poids[i];


	return res - (2*sommep);

}

int borneMin(Coordonnees c, Graphe g,float UB){

	float poids[c -> n];

	for(int i = 0; i < c ->n; i++)
		poids[i] = 0;

	int iter = 2 * c -> n;
	float lambda = 2;

	int i= 0;

	int bInf;
	int sommep;
	int sommed;

	while(iter > 1){

		i++;
		for(int j = 0; j < iter; j++){

			Graphe temp = creer_graphe(c -> n);

			bInf = primPoids(c,temp,poids);

			sommep = 0;
			for(int n = 0; n < c -> n; n++)
				sommep += poids[n];

			sommep *= 2;

			sommed = 0;
			for(int n = 0; n < c->n; n++)
				sommed += pow(graphe_degre(temp,n) - 2,2);


			float ti = (lambda * (UB - bInf + sommep)) / sommed;


			for(int n = 0; n < c->n; n++){
                        poids[n] += ti * (graphe_degre(temp,n)-2);

                  }

			detruire_graphe(temp);
		}

		iter /= 2;
		lambda /= 2;
	}


	return primPoids(c,g,poids);
}

int UFGetRoot(int* tab,int s){

	int last = s;
	int curr = tab[s];

	while(curr != last){
		last = curr;
		curr = tab[last];
	}
	return last;
}

int UFIsConnected(int* tab, int s1, int s2){

	return UFGetRoot(tab,s1) == UFGetRoot(tab,s2);
}

void UFConnect(int* tab, int s1, int s2){
	tab[UFGetRoot(tab,s1)] = UFGetRoot(tab,s2);
}

int cycle(Coordonnees c, Graphe g){

	int res =0;

	int nb  = c -> n;
	int nbtot = (nb*(nb-1))/2;

	Arete* aretes = NULL;
	aretes = (Arete*)malloc(nbtot * sizeof(Arete));
	if(aretes == NULL)
		printf("fail\n");


	int* unionF;
	unionF = (int*)malloc(nb * sizeof(int));


	for(int i = 0 ; i < nb ; i++)
		unionF[i] = i;

	int k=0;

	for(int i = 0; i < nb; i++){
		for(int j = i+1; j < nb; j++){
			aretes[k] = creerA(i, j, distance(i,j,c));
			k++;
		}
	}

	if(nb > 150)
		fusion_para(aretes,0,nbtot);
	else
		tri_selec(aretes,nbtot);


	int compt=nb-1;
	int iter=0;
	while(compt > 0){

		int s1 = aretes[iter].s1;
		int s2 = aretes[iter].s2;


		if( (g -> alist[s1] -> d) < 2
		&&  (g -> alist[s2] -> d) < 2
		&& !UFIsConnected(unionF,s1,s2)){

			graphe_ajouter_arete(g,s1,s2);
			res += distance(s1,s2,c);
			UFConnect(unionF,s1,s2);
			compt--;

		}
		iter++;
	}


	int sa = -1;
	int sb = -1;
	for(int i = 0; i < nb; i++){
		if(g -> alist[i] -> d == 1){
			if(sa == -1)
				sa = i;
			else
				sb = i;
		}
	}
	graphe_ajouter_arete(g,sa,sb);
	res += distance(sa,sb,c);
	return res;

}

int arete_suivante(Graphe g,int s1,int s2){
	if(g -> alist[s2] -> list[0] == s1){
		return g -> alist[s2] -> list[1];
	}else{
		return g -> alist[s2] -> list[0];
	}
}


int opt2(Graphe g, Coordonnees c){

	int s1 = 0;
	int s2 = g -> alist[s1] -> list[0];
	int s3;
	int s4;
	int temp;
	int res = 0;

	while(s2 != 0){
		s3 = arete_suivante(g, s1, s2);
		s4 = arete_suivante(g, s2, s3);
		while(s3 != 0){

			if(distance(s1, s2, c) + distance(s3, s4, c)
			 > distance(s1, s3, c) + distance(s2 ,s4, c)){

				graphe_retirer_arete(g ,s1 ,s2);
				graphe_retirer_arete(g ,s3 ,s4);
				graphe_ajouter_arete(g ,s1 ,s3);
				graphe_ajouter_arete(g ,s2 ,s4);
			 	
				temp = s2;
				s2 = s3;
				s3 = temp;
				res = 1;
			}
			temp = s4;
			s4 = arete_suivante(g, s3, s4);
			s3=temp;

		}
		temp = s2;
		s2 = arete_suivante(g, s1, s2);
		s1 = temp;
	}
	return res;

}


int lissage(Graphe g, Coordonnees c){

	int s1 = 0;
	int s2 = g -> alist[s1] -> list[0];
	int s3;
	int s4;
	int s5;
	int temp;
	int res = 0;


	while(s2 != 0){
		s3 = arete_suivante(g, s1, s2);
		s4 = arete_suivante(g, s2, s3);
		s5 = arete_suivante(g, s3, s4);
		while(s5 != 0){
			if(distance(s1,s2,c) + distance(s3,s4,c) + distance(s4,s5,c)
			 > distance(s1,s4,c) + distance(s4,s2,c) + distance(s3,s5,c)
                   && s1 != s3 && s1 != s4 && s1 != s5 && s2 != s3 && s2 != s4 && s2 != s5){

				graphe_retirer_arete(g ,s1 ,s2);
				graphe_retirer_arete(g ,s3 ,s4);
				graphe_retirer_arete(g ,s4 ,s5);
				graphe_ajouter_arete(g ,s1 ,s4);
				graphe_ajouter_arete(g ,s4 ,s2);
				graphe_ajouter_arete(g ,s3 ,s5);

				res = 1;
				s2 = s4;
				s4 = s3;
				s3 = arete_suivante(g,s5,s4);


			 }

			temp = s5;
			s5 = arete_suivante(g, s4, s5);
			s4 = temp;
			s3 = arete_suivante(g, s5, s4);

		}

		temp = s2;
		s2 = arete_suivante(g, s1, s2);
		s1 = temp;
	}
	return res;

}

int longueur_cycle(Graphe g, Coordonnees c){

      int s1 = 0;
      int s2 = g -> alist[s1] -> list[0];

      int res = 0;
      int temp;

      do{
            res += distance(s1,s2,c);

            temp = s2;
		s2 = arete_suivante(g, s1, s2);
		s1 = temp;

      }while(s1 != 0);


      return res;
}
