/***********************************/
/* les types Graphe et Coordonnees */
/***********************************/

/* type struct graphe */
struct graphe {
    int n;              /* nombre de sommets */
    int m;              /* nombre d'aretes */
    struct voisins {
        int d;          /* degre */
        int nbcell;        /* nombre de cellules dans le tableau */
        char est_trie; /* vrai si la liste est deja triee */
        int list[1];    /* liste des voisins, sous forme d'un tableau dynamique */
    } *alist[1];
};

/*type graphe non-oriente */
typedef struct graphe *Graphe;

/* type struct coordonnees */
struct coordonnees {
	int n;		      /* nombre de points */
	char nom[256];	      /* nom de l'instance */
	struct coord {
		float coordx; /* coordonnee sur l'axe x */
		float coordy; /* coordonnee sur l'axe y */
	} *clist[1];
};

/*type coordonnees */
typedef struct coordonnees *Coordonnees;

/*************************************/
/* 	!!!!!!!	ATTENTION !!!!!!     */
/* 	!!!!!!!	ACHTUNG !!!!!!!!     */
/*	!!!!!!!	BE CAREFUL !!!!!     */
/* les sommets sont indices de 0     */
/* n-1 dans Coordonnees et graph     */
/* MAIS ils sont indicés de 1 a n    */
/* dans les differents fichiers	     */
/*************************************/

/**********************************************************************************/
/* les signatures des fonctions et procedures liees aux graphes et aux affichages */
/**********************************************************************************/

/* cree un nouveau graphe avec n sommets numerotes de 0 a n-1 et aucune arete */
Graphe creer_graphe(int n);

/* libere la memoire utilisee par le graphe g*/
void detruire_graphe(Graphe g);

/* ajoute une arete (s1,s2) a un graphe existant */
/* le faire plus d'une fois peut conduire a des resultats imprevisibles */
void graphe_ajouter_arete(Graphe g, int s1, int s2);

void graphe_retirer_arete(Graphe g, int s1,int s2);

/* retourne le nombre de sommets/aretes d'un graphe g*/
int graphe_nb_sommets(Graphe g);
int graphe_nb_aretes(Graphe g);

/* retourne le degre d'un sommet s dans g*/
int graphe_degre(Graphe g, int s);

/* retourne 1 si l'arete (s1,s2) existe dans g, 0 sinon */
int graphe_arete_existe(Graphe g, int s1, int s2);

/* conversion d'un fichier tour nom_tour.tour en un graphe */
Graphe conversion_tour_graphe(char *nom_tour);

/* cree un tableau de n coordonnees numerotees de 0 a n-1 et toutes a (0,0) */
Coordonnees creer_coordonnees(int n,char *nom);

/* libere la memoire utilisee par les coordonnees c*/
void detruire_coordonnees(Coordonnees c);

/* lit une instance dans le fichier nom.tsp, la charge en memoire dans Coordonnees */
/* et cree un fichier nom.gnu pour l'affichage */
Coordonnees lecture_instance(char* nom);

/* affiche une instance apres lecture de l'instance nom.tsp */
/* a partir du fichier nom.gnu qui a ete genere par lecture_instance */
void afficher_instance(char* nom);

/* affiche une tournee nom_tour.tour */
void afficher_tour(Coordonnees c, char* nom_tour);

/* affiche le graphe g */
void afficher_graphe(Coordonnees c, Graphe g);

//cree un arbre couvrant de valeur minimum
void prim(Coordonnees c, Graphe g);

//renvoie la distance entre deux sommets
int distance(int s1, int s2, Coordonnees c);

void arbre1(Coordonnees c, Graphe g);

int primPoids(Coordonnees c, Graphe g,float* poids);

int primPoidsNoG(Coordonnees c,float* poids);

int borneMin(Coordonnees c, Graphe g,float UB);

int UFGetRoot(int* tab, int s);

int UFIsConnected(int* tab, int s1, int s2);

void UFConnect(int* tab,int s1, int s2);

int cycle(Coordonnees c, Graphe g);

int opt2(Graphe g,Coordonnees c);

int opt2bis(Graphe g,Coordonnees c);

int arete_suivante(Graphe g,int s1,int s2);

int intersection2(int a, int b, int x, int y, Coordonnees c);

double prodSca(double ux, double uy, double vx, double vy);

int lissage(Graphe g, Coordonnees c);

int longueur_cycle(Graphe g, Coordonnees c);
