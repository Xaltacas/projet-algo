
typedef struct arete{
	int s1;
	int s2;
	int l;
}Arete;


typedef struct tas{

	Arete* pile;
	int* indices;

}Tas;


Tas initialiser(int taillemax);

double log2(double x);

Arete creerA(int s1, int s2, int l);

void inverser(Tas* K,int i,int j);

int est_vide(Tas K);

int est_feuille(Tas K, int i);

int a_un_fils_droit(Tas K, int i);

int fgauche(Tas K, int i);

int fdroite(Tas K, int i);

int pere(Tas K, int i);

void ajouter(Tas* K, int s1, int s2, int l);

void supprime(Tas* K,int s2);

int recupMin(Tas* K);

void afficheTas(Tas* K);

void interclassement(Arete* Tab, int start, int mid, int end);

void fusion_para(Arete* Tab, int start, int end);

void tri_selec(Arete* tab, int taille);

