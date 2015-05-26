//la derniere version de ce code est meise à jour le 14/12/2013
//il est developpé sur un systeme d'exploitation linux ubuntu 13.04 
//l'editeur de texte utilise est le logiciel Sublime texte 2
// la complation est faite directement sur l'invite de commande et par le compilateur gcc installer sur

//Description du probleme 
//jeu de puzzles en utilisant les tableau d'enregistrements
//l'utilisateur est appelé a entrer le nom du fichier de son inputs 
// et apres il attend la fin de l'exsécution du programme 
//des que le prgramme acheve le fichier résutant sera trouver 
//dans son espace de travail 

//le nom fichier resulat est ***resultat.txt*** 
 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int n=0;
char *matp={" "};
char *dict_txt={" "};
int totale=0;
typedef struct point
{
	int x;
	int y;
}point;
typedef struct liste
{
	char nom;
	int nbpoint;
	point* ensemble;
	
}liste;


int test_doublon(liste *listeloc,int nb);
void initialisation_n();
void initialisation(char** mat);
void afficher(char** mat);
void remplissage_par_utilisateur(char **mat);
void conversion_mat_vers_tabdenregistrement(char **mat,liste **listeloc,int *k);
void lecture_d_fichier(liste *listeloc,int nbliste,int seek);
void ecriture_dans_fichier(char *chaine);
void coord_cara_dans_mat(liste **listeloc,char **mat,int pos_actu ,char c );
void duppliquer_mat(char **matsource,char **matrecep);
void voisin_dans_mat_par_enr(liste **listeloc_a , int pos_a,int pos_b );
void afficher_le_mot_dans_mat(liste *listll, int nb,char **mat);
int indice(liste *listeloc,int nbliste,char c);
void trouve_liste_mot(liste *listeloc,liste **listemot, char * mot);
int existe_tous_point(liste *listeloc,int nbliste,char *mot,int *tab_pos);
void chercher_mot(liste *listeloc,int nbliste,char *mot);

//******************* main *****************************************





int main(int argc, char const *argv[])
{	
	int j,i,nbenreg;
	liste *list=malloc(26*sizeof(liste));
	liste *list_aux=malloc(26*sizeof(liste));
	char **mat;
	int l,c;
	initialisation_n();
	mat = (char **)malloc(n*sizeof(char*));
	for (i = 0; i < n; ++i){
			mat[i] = (char *)malloc(n*sizeof(char));
	}

	initialisation(mat);	
	conversion_mat_vers_tabdenregistrement(mat,&list,&nbenreg);
	lecture_d_fichier(list,nbenreg,4);


	printf("%s\n", matp);

	free(list);
	for ( i = 0; i < 1; ++i)
	{
		free(*(mat+i));
	}
	free(mat);
	return 0;

}
//fonction qui initialise la taille de la matrice , le nom du fichier dictionnaire et le nom du fichier dict.txt 
void initialisation(char **mat){
	FILE* f;
	int k=0,i =0,j;
	char c;
	char * fichier_prob=malloc(30*sizeof(char));
	char * ch=malloc(30*sizeof(char));
	matp=(char*)malloc(30*sizeof(char));
	dict_txt=(char*)malloc(30*sizeof(char));
	
	printf("Donnez le fichier des inputs \n");
	scanf("%s",fichier_prob);

	f=fopen(fichier_prob,"r");
	fgets(ch,30,f);
	fgets(ch,30,f);
	strncpy(dict_txt,ch,strlen(ch)-2);
	printf("%s\n",dict_txt );

	while((!feof(f))&&(i<n)){
		fgets(ch,30,f);
		
		for (j = 0; j < n; j++)		
		{	if (((int)ch[j]>='A')&&((int)ch[j]<=(int)'Z'))
			{
				int x=(int)ch[j]+((int)'a'-(int)'A');
				ch[j]=(char)x;
			}
			matp[k]=ch[j];
			k++;
		}
		i++;
	}
	matp[strlen(matp)]='\n';
	
	int iter=0;
	for (i = 0; i < n; ++i){
		for (j = 0; j < n; j++)		
		{	
			mat[i][j]=matp[iter];
			iter++;
		}
	}
	afficher(mat);
	free(ch);
	close(f);
}



//fonction qui initialise la taille de la matrice  
void initialisation_n(){
	FILE* f;
	char *ch=malloc(30*sizeof(char));
	f=fopen("probleme.txt","r");
	fgets(ch,30,f);
	n=atoi(ch);
	free(ch);
	close(f);
}



//Afficher un mot dans un tableau d'enregistrement 
void afficher_le_mot_dans_mat(liste *listeloc_ici, int nliste,char **matt){
	point *p;
	int i,j,nmat=n;
	
	for (i = 0; i < nmat; ++i){
		for (j = 0; j < nmat; ++j)
		{
			matt[i][j] ='*';
		}
	}
	for ( i = 0; i < nliste; ++i)
	{
		p=(listeloc_ici+i)->ensemble;
		for (j = 0; j < (listeloc_ici+i)->nbpoint; ++j)
		{
			*(*(matt+(p+j)->x)+(p+j)->y) =(listeloc_ici+i)->nom;
		}		
	}
	afficher(matt);
}

//*********************************************************


//affichage de la matrice 
void afficher(char** mat){
	ecriture_dans_fichier("\t\t*******************\n");

	int i,j;
	FILE* f;
	f=fopen("resultat.txt","a");
	for (i = 0; i < n; ++i){
		for (j = 0; j < n; ++j)
		{
			fputc(mat[i][j],f);
			 fputc('\t',f);
			
		}
		fputc('\n',f);
	}
	close(f);
	printf("\n");
}



//fonction pour le remplissage de la matrice de façon par l'utilisateur
void remplissage_par_utilisateur(char **mat){
	printf("%s %d\n",matp,n );
	int iter=0,i,j;
	for (i = 0; i < n; ++i){
		for (j = 0; j < n; j++)		
		{	
			mat[i][j]=matp[iter];
			iter++;
		}
	}
	afficher(mat);
}



//cette fonction a comme but d'extaraire la matrice vers une tableau d'enregistrement 
//les cararcteres ne soient pas redendant dans le tableau 
void conversion_mat_vers_tabdenregistrement(char **matloc,liste **listeloc,int *nbenreg){
	int i,j,k,m,nbc=0; 
	char **mat=malloc(n*sizeof(char*));
	for (i = 0; i < n; ++i){
		mat[i]= (char *)malloc(n*sizeof(char));
	}
	duppliquer_mat(matloc,mat);
	

	for (i = 0; i < n; ++i)
	{	for (j = 0; j < n; ++j)
		{	char c;			
			while(((mat[i][j]=='*')&&(i<n))||((mat[i][j]=='*')&&(n>=n)))
			{
				if(j==n){
					i++;
					j=0;
				}else{
					j++;
				}
			}

			if (j<n)
			{	(*listeloc+nbc)->ensemble=(point*)malloc(20*sizeof(point)); 
				coord_cara_dans_mat(listeloc,mat,nbc,mat[i][j] );
				nbc++;
			}			
		}
	}
	*nbenreg=nbc;

	for ( i = 0; i < n; ++i)
	{
		free(*(mat+i));
	}
	free(mat);

}



//fonction qui dupplique dux matrices
void duppliquer_mat(char **matsource,char **matrecep){
	int i,j;
	for ( i = 0; i < n ; ++i)
	{
		for ( j = 0; j < n; ++j)
		{
			matrecep[i][j]=matsource[i][j];
		}
	}
}


//fonction qui retourne les cordonnes de chaque caractere dana la matrice 
void coord_cara_dans_mat(liste **listeloc,char **mat,int pos_actu ,char c ){

	int i,j,m,k;
	int nbc=0;
	point *pointt=(*listeloc+pos_actu)->ensemble; 
	for (k = 0; k < n; ++k)
	{	for (m = 0; m < n; m++)
		{	if(mat[k][m]==c)
			{	
				mat[k][m]='*';
				(pointt+nbc)->x=k;
				(pointt+nbc)->y=m;
				nbc++;
			}
		}
	}
	(*listeloc+pos_actu)->ensemble=pointt;
	(*listeloc+pos_actu)->nbpoint=nbc;
	(*listeloc+pos_actu)->nom=c;

}



//la fonction ci dessous extraire deux points voisins dans la matrice
void voisin_dans_mat_par_enr(liste **listeloc_a ,int pos_a,  int pos_b ){
	int i ,j ,a=0,b=0 ,nouveautaille=0;

 	
	point *point_b=(*listeloc_a+pos_b)->ensemble;
	point *pointaux=malloc(20*sizeof(point));
	point *point_a=(*listeloc_a+pos_a)->ensemble;
	


	for ( i = 0; i < (*listeloc_a+pos_a)->nbpoint; ++i)
	{ 	int exist=0,j=0;
		while((exist==0)&&(j<(*listeloc_a+pos_b)->nbpoint))
		{	 a=abs((point_a+i)->x-(point_b+j)->x);
			 b=abs((point_a+i)->y-(point_b+j)->y);
			if ((a<2)&&(b<2)&&((a+b)!=0)){

				exist=1;
				(pointaux+nouveautaille)->x=(point_a+i)->x;
				(pointaux+nouveautaille)->y=(point_a+i)->y;
				nouveautaille++;	
			}
			j++;
		}
	}
 	for ( i = 0; i < nouveautaille; ++i)
 	{
 		(point_a+i)->x=(pointaux+i)->x;
 		(point_a+i)->y=(pointaux+i)->y;
 	}
	(*listeloc_a+pos_a)->ensemble=point_a;
	(*listeloc_a+pos_a)->nbpoint=nouveautaille;
	




	free(pointaux);

}


void lecture_d_fichier(liste *listeloc,int nbliste,int seek){
	int i=0,j,somme=0,trouve=0;
	char x,*mot=malloc(20*sizeof(char));
	char *chaux=malloc(30*sizeof(char));
	FILE* f;
	char *dic=malloc(30*sizeof(char));
	f=fopen(dict_txt,"r");
	int pos,nbmot=0;


	while(!feof(f))
	{	
		pos=ftell(f);
		nbmot++;
		fseek(f, sizeof(char)*pos, SEEK_SET) ;
		fgets(mot,30,f);
		printf("*** position %d %s\n",nbmot,mot );
		somme++;
		if (strlen(mot)>1)
		{
			chercher_mot(listeloc,nbliste,mot);	
		}
	}

	fclose(f);
	
	printf("le nombre nombre de mots trouvé est  %d\n", totale);
	printf("la fermeture de fichier est faite\n");

}


//la fonction suivante prendre un mot en parametre et la 
//et la rendre dans un tableau d'enregistrement
//cette fonction affiche une matrice du mot si elle la trouve

void chercher_mot(liste *listeloc,int nbliste,char *mot){
	
	int *tab_pos=malloc((strlen(mot)-1)*sizeof(int));
	
	if(existe_tous_point(listeloc,nbliste,mot,tab_pos)!=0){
		int indicee=0;
		int i=0,j=0;
		char **mat;


		liste *liste_mot=malloc(26*sizeof(liste));			

		mat = (char **)malloc(n*sizeof(char*));
		for (i = 0; i < n; ++i){
			mat[i] = (char *)malloc(n*sizeof(char));
		}


		point *pointaux,*point_sour;
		for ( i = 0; i < strlen(mot)-1; ++i)
		{	(liste_mot+i)->ensemble=malloc((listeloc+tab_pos[i])->nbpoint*sizeof(point)); //allocation 
			(liste_mot+i)->nbpoint=(listeloc+tab_pos[i])->nbpoint;  //affectation de nbpoint
			(liste_mot+i)->nom=(listeloc+tab_pos[i])->nom;	//affectation de nom
								//affectation de tableau de coordonneés
			point_sour=(listeloc+tab_pos[i])->ensemble;
			pointaux=(liste_mot+i)->ensemble;
			for ( j = 0; j < (listeloc+tab_pos[i])->nbpoint; ++j)
			{
				(pointaux+j)->x=(point_sour+j)->x;
				(pointaux+j)->y=(point_sour+j)->y;
			}
		}
		
		for ( i = 0; i < strlen(mot)-2; ++i)
		{	voisin_dans_mat_par_enr(&liste_mot,i,i+1);
			voisin_dans_mat_par_enr(&liste_mot,i+1,i);
		}

		for ( i = strlen(mot)-3; i >=0; --i)
		{	
			voisin_dans_mat_par_enr(&liste_mot,i,i+1);
			voisin_dans_mat_par_enr(&liste_mot,i+1,i);
		}

		int trouve=1;
		i=0;
		while((trouve)&&(i<strlen(mot)-1)){
			if ((liste_mot+i)->nbpoint==0)
			{
				trouve=0;
			}
			i++;
		}

		if ((trouve)&&(test_doublon( liste_mot,strlen(mot)-1)==0))
		{	
			totale++;
			int x=strlen(mot)-1;
			afficher_le_mot_dans_mat(liste_mot,x,mat);
			strcat(mot,"\n");
			ecriture_dans_fichier(mot);
			ecriture_dans_fichier("\n\n");

			
		}
		else printf(" introuvable %s\n",mot );
		

		for ( i = 0; i < strlen(mot)-1; ++i)
		{
			(liste_mot+i)->nbpoint=0;
		}


	free(liste_mot);
	for ( i = 0; i < 1; ++i)
	{
		free(*(mat+i));
	}
	free(mat);
	}
	else 
		printf("introuvable :  %s \n",mot );
}



int test_doublon(liste *listeloc,int nb){
 	int i=0,j=0,valret=0;
 	for (i = 0; i < nb-1; ++i)
 	{
 		char a =((listeloc+i)->nom);
 		for (j = i+1; j < nb; ++j)
 		{
 			if(a==(listeloc+j)->nom){
 				if(((listeloc+j)->nbpoint==1)&&(listeloc+j)->nbpoint==(listeloc+i)->nbpoint){
 					point *pointa=(listeloc+i)->ensemble;
 					point *pointb=(listeloc+j)->ensemble;
 					int k ; 
 					for ( k = 0; k < (listeloc+j)->nbpoint; ++k)
 					{
 						if (((pointa+k)->x==(pointa+k)->x)&&(pointa+k)->y==(pointa+k)->y)
 						{	
 							return 1;
 						}
 					}
 					
 				}
 			}
 		}
 		i++;

 	}
 	//sleep(4);

 	return 0;
 }



// foncton pour nous retourner un 0 si le mot n'existe pas dans la matrice 
//si non elle nous retourne un tableau contenant tous les postions de caractere dans le tableau d'enregistrement
int existe_tous_point(liste *listeloc,int nbliste,char *mot,int *tab_pos){
	int i;
	int indicee;
	for ( i = 0; i < strlen(mot)-1; ++i)
	{	indicee=indice(listeloc,nbliste,mot[i]);
		if (indicee!=-1)
		{
			tab_pos[i]=indicee;
		}
		else return 0;
	}
	
	return 1;
}


//***********************************
int indice(liste *listeloc,int nbliste,char c){
	int i=0; 
	while(((listeloc+i)->nom!=c)&&(i<nbliste))
	{
		i++;
	}
	if(i==nbliste)return -1;
	return i; 
}

//ecriture dans un fichier 
void ecriture_dans_fichier(char *chaine){
	FILE* f;
	//strcat(chaine,"\n");
	f=fopen("resultat.txt","a");
	fputs(chaine,f);
	close(f);
	
}
