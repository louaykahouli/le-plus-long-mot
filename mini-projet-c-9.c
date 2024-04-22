#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//////////joureur/////////
typedef struct joueur{
	int score[6];
	char nom[10];
}joueur;
///////////initialisation////////////////////// permet d'initialiser la structure du joureur
void initialiser_joueur(joueur *j) {
    int i;
    for(i = 0; i < 6; i++) {
        j->score[i] = 0;
    }
    strcpy(j->nom, "");
}

//////////////////nb-voyelle/////////////////// demande le nombre de voyelle souhaité
int lire_voyelle(){
	int v;
	do{
		printf("\t\t\t\t\t\tdonner le nombre de voyelle :");
		scanf("%d",&v);
	}while(v<2);
	return v;
}
////////////////////tirage//////////////////donner les lettres aléatoirement


char* tirage(int nbvoyelle){
    srand(time(NULL)); 

    char* lettres=(char*)malloc(11);
    char* voyelles="AEIUYO";
    char* consonnes="ZRTPQSDFGHJKLMWXCVBN";
    int i;
    lettres[0]='\0';
    for(i=0;i<nbvoyelle;i++){
        lettres[i] = voyelles[rand() % 6];
    }
    for(i=nbvoyelle;i<10;i++){
        lettres[i] = consonnes[rand() % 20];
    }
    lettres[10]='\0';
    return lettres;
}

////////////////////création du fichier index////////////////
void crieindex(){
	FILE *f,*g;
	char *ch= malloc(70);;
	int n,n1;
	char c;
	f=fopen("index.txt","w");
	g=fopen("dictionnaire.txt","r");
	rewind( f );
	if(f!=NULL && g!=NULL){
		fgets(ch,70,g);
	
		n1=0; 
		c=ch[0];
		while(feof(g) == 0){ 
			fgets(ch,70,g);
			if(ch[0]!=c){//permet de distinguer quand la lettre se change
				fprintf(f,"%c %d \n",c,n1);
				c=ch[0];
				n1=ftell(g)-strlen(ch)-1;
			
			}	
		}
		fprintf(f,"%c %d \n",c,n1);
		fclose(g);
		fclose(f);
		
	}
	else{
		printf("\t\t\t\t\t\terreure d'ouverture du fichier ");
	}
	
	free(ch); 
}
/////////////majuscule///////////////////////////////////  le role de cette fonction est de covertir le mot passé de minuscule en majuscule
void majus(char *ch) {
	int i;
	for (i=0;i<strlen(ch);i++)
	ch[i]=toupper(ch[i]);
}
///////////////////verification de lettres//////////////////////
/* L'idee est simple , on construit deux tables chaque case correspond a un caractere tels que son index dans les tableaux correspond
a celle dans l'alphabet puis on compare la présence et l'occurence */
int verif_lettre(char *ch,char *mot) {
    int count_mot[26] = {0}, count_ch[26] = {0};
    int i;
    for(i = 0; ch[i]; i++) // Compter le nombre d'occurrences de chaque lettre dans 'ch'
        count_ch[toupper(ch[i])-'A']++;
        
	for(i = 0; mot[i]; i++) // Compter le nombre d'occurrences de chaque lettre dans 'mot'
        count_mot[toupper(mot[i])-'A']++;

    
    

    
    for(i = 0; i < 26; i++) // Vérifier si le nombre d'occurrences de chaque lettre dans 'mot' est inférieur ou égal à celui dans 'ch'
        if(count_mot[i] > count_ch[i])
            return 0;

    return 1;
}

///////////////////verification que le mot existe dans le dictionnaire //////////////// 
int verif_dict(char *mot){
    char res[70];
    char ch[70], c;
    FILE *f,*g;
    f=fopen("index.txt","r");
    rewind( f );
    c=mot[0];
    while(fgets(ch,70,f) != NULL){//le role de cette boucle est de prendre la position correcte a partir de la quelle on veux commencer la verification
        if (ch[0]==c){
            strncpy(res, ch+2, strlen(ch)-3); 
            res[strlen(ch)-3] = '\0'; 
            break;
        }
    }
    fclose(f);
    g=fopen("dictionnaire.txt","r");
    rewind( g );
    fseek(g,atoi(res),0);
    while(feof(g)==0){//verification d'existance dans le dictionnaire
    	fgets(ch,70,g);
		ch[strlen(ch) - 1] = '\0';

    	if (strstr(ch,mot)!=NULL && strstr(mot,ch)!=NULL){
            fclose(g);
    	    return 1;
        }
    	if(ch[0]!=c)
    	    break;
    }
    fclose(g);
    return 0;
}
//////////////verification de la validitée du mot////////////////
int verif(char *ch,char *mot){
	majus(mot);
	if(verif_lettre(ch,mot)==1){
		if(verif_dict(mot)==1)
		return 1;
	}
	return 0;
}
///////////////solver///////////////chercher les mot les plus long quand peut construire 
void solveur(char *letters) {
    FILE *f = fopen("dictionnaire.txt", "r");
    char s[30];
    char pluslong[100] = "";
    while (fgets(s, sizeof(s), f)) {
        s[strlen(s) - 1] = '\0';

        if (strlen(s) > strlen(pluslong) && verif_lettre(letters, s)) {
            strcpy(pluslong, s);
        }
    }
    if(pluslong==""){
    	printf("impossible de construire des mots a partir les lettres disponible; ");
    	}
    else {
rewind(f);
    while (fgets(s, sizeof(s), f)) {
        s[strlen(s) - 1] = '\0';

        if (strlen(s) == strlen(pluslong) && verif_lettre(letters, s)) {
            printf("\t\t\t\t\t\tLe mot le plus long est : %s\n", s);
        }
    }
   	}
    fclose(f);
}
////////////////////score////////////////
void calcul_points(char* mot_j1,char* mot_j2,char *ch,joueur *j1,joueur *j2,int i){
	int long_mot_j1,long_mot_j2,pointj1,pointj2,mot_j1_valide,mot_j2_valide;
	
	pointj1=0;
	pointj2=0;
	
	long_mot_j1= strlen(mot_j1);
	long_mot_j2= strlen(mot_j2);
	
	mot_j1_valide=verif(ch,mot_j1);
	mot_j2_valide=verif(ch,mot_j2);
	
	if(mot_j1_valide && mot_j2_valide){
		if(long_mot_j1>long_mot_j2)
			pointj1+=long_mot_j1;
		else if(long_mot_j1<long_mot_j2)
			pointj2+=long_mot_j2;
		else {
				pointj2+=long_mot_j2;
				pointj1+=long_mot_j1;
				}	
	}
	if(mot_j1_valide && !(mot_j2_valide)){
		if (long_mot_j2>long_mot_j1)
			pointj1+=long_mot_j2;
		else
			pointj1+=long_mot_j1;}
	else {
		if(mot_j2_valide && !(mot_j1_valide)){
			if (long_mot_j1>long_mot_j2)
			pointj2+=long_mot_j1;
		else
			pointj2+=long_mot_j2; 	}	
		
	}
	j1->score[i]=pointj1;
	j2->score[i]=pointj2;
	printf("\t\t\t\t\t\tPoints %s : %d\n",j1->nom, pointj1);
    printf("\t\t\t\t\t\tPoints %s : %d\n", j2->nom,pointj2);
	
}
//////////////gagnant////////////////

void winner(joueur j1,joueur j2){
	int i,scorej1,scorej2;
	for (scorej1=0,scorej2=0,i=0;i<6;i++){
	
		scorej1+=j1.score[i];
		scorej2+=j2.score[i];
	}
	if(scorej1>scorej2)
		
		printf("\t\t\t\t\t\t Felicitations %s, vous etes le gagnant\n \t\t\t\t\t\tvotre score est : %d",j1.nom,scorej1);
	else if(scorej2>scorej1)
		printf("\t\t\t\t\t\t Felicitations %s, vous etes le gagnant\n \t\t\t\t\t\t votre score est : %d",j2.nom,scorej2);
	else
		printf("\t\t\t\t\t\t egalite\n \t\t\t\t\t\t le score est %d",scorej1);
}

///////////////main/////////
int main(){
	//all rights are reserved to Louay Kahouli,Fatma Zahra Gaida & Nesrine Ben Tmessek
	int n,i;
	char *motj1=malloc(10);
	char *motj2=malloc(10);
	char *ch=malloc(11);
	joueur *j1 = malloc(sizeof(joueur));
	joueur *j2 = malloc(sizeof(joueur));
	initialiser_joueur(j1);
	initialiser_joueur(j2);
	crieindex();
	ch[0]='\0';
	motj1[0]='\0';
	motj2[0]='\0';
	printf("\t\t\t\t\t\tBienvenue a MotPlusLong !\n\t\t\t\t\t\tAmusez-vous bien et bonne chance !\n\n");
	printf("\t\t\t\t\t\tdonner le nom de premier joueur:");
	scanf("\t\t\t\t\t\t%s",j1->nom);
	printf("\t\t\t\t\t\tdonner le nom de deuxieme joueur:");
	scanf("\t\t\t\t\t\t%s",j2->nom);
	for(i=0;i<6;i++){
		printf("\n");

		printf("\t\t\t\t\t\t  ****************************\n ");
		printf("\t\t\t\t\t\t  *  le manche n:%d commence  *\n ",i+1);
		printf("\t\t\t\t\t\t  ****************************\n ");
		printf("\n");
		if(i%2==0)
			printf("\t\t\t\t\t\t%s commence \n",j1->nom);
		else
			printf("\t\t\t\t\t\t%s commence \n",j2->nom);
		n=lire_voyelle();
		ch=tirage(n);
		printf("\t\t\t\t\t\t %s \n",ch);
		
		if(i%2==0){
		printf("\t\t\t\t\t\t%s donnez votre mot :",j1->nom);
	    scanf("\t\t\t\t\t\t%s",motj1);
	    printf("\t\t\t\t\t\t%s donnez votre mot :",j2->nom);
	    scanf("\t\t\t\t\t\t%s",motj2);
		}
		else{
		printf("\t\t\t\t\t\t%s donnez votre mot :",j2->nom);
		scanf("\t\t\t\t\t\t%s",motj2);
		printf("\t\t\t\t\t\t%s donnez votre mot :",j1->nom);
		scanf("\t\t\t\t\t\t%s",motj1);
		}
		calcul_points(motj1,motj2,ch,j1,j2,i);
		solveur(ch);
		ch[0]='\0';
		motj1[0]='\0';
		motj2[0]='\0';
		printf("\n");
	}
	winner(*j1,*j2);
	free(j1);
	free(j2);
	free(motj1);
	free(motj2);
	free(ch);

	return 0;
}
