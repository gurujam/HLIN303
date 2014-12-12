//HLIN303:Huffman

#include"./arbre/arbre.h"

int main(int argc, char* argv[]){
  /*Test nombre d'arguments*/
  if (argc!=2){
    printf("Syntax Error.\n");
    return 1;
  }
  int occ[256];
  int nbfeuille=0;
  int i=0;
  float tailledep=0;
  
  /*Initialisation des occurences a 0*/
  for(i=0;i<256;i++){
    occ[i]=0;
  }

  /*Test du fichier*/
  FILE *f=fopen(argv[1],"r");
  int c=0;
  if(!f){
    printf("Fichier non trouvé.\n");
    return 2;
  }
  c=fgetc(f);
  if(c==EOF){
    printf("Fichier vide.\n");
    fclose(f);
    return 3;
  }

  fclose(f);

  /*Creation du tableau de caractères et de leur fréquence*/
  f=fopen(argv[1],"r");
  while(EOF!=(c=fgetc(f))){
    occ[c]+=1;
    tailledep+=8;
  }
  fclose(f);
  for(i=0;i<256;i++){
    if(occ[i]!=0){
      nbfeuille+=1;
    }
  }


  int indice=0;
  int val[(2*nbfeuille)-1];
  int fg[(2*nbfeuille)-1]; 
  int fd[(2*nbfeuille)-1]; 
  int parent[(2*nbfeuille)-1];
  char car[nbfeuille];
  NoeudSC tabNoeud[2*nbfeuille-1];
 
  for(i=0;i<2*nbfeuille-1;i++){
    val[i]=-1;
    parent[i]=-1;
    fg[i]=-1;
    fd[i]=-1;
  }

  printf("Indice (caractère) : occurences\n");

  for(i=0;i<256;i++){
    if(occ[i]!=0){
      val[indice]=occ[i];
      car[indice]=i;
      printf("%d (%c):%d\n",indice,i,occ[i]);
      indice+=1;
    }
  }

  printf("nbfeuille=%d\nindice=%d\n",nbfeuille,indice);

  printf("\n***_Ordonnancement Feuilles_***\n");
  int imini=-1, imini2=-1, nn=2*nbfeuille-1;
  for(nn=nbfeuille;nn<2*nbfeuille-1;nn++){
    for(i=0;i<nn;i++){
      if(parent[i]==-1){
	if(imini==-1){
	  imini=i;
	}
	else if(imini2==-1){
	  if(val[i]<val[imini]){
	    imini2=imini;
	    imini=i;
	  }
	  else{
	    imini2=i;
	  }
	}
	else if(val[i]<val[imini2]){
	  if(val[i]<val[imini]){
	    imini2=imini;
	    imini=i;
	  }
	  else{
	    imini2=i;
	  }
	}
      }
    }
    val[nn]=val[imini]+val[imini2];
    fg[nn]=imini;
    fd[nn]=imini2;
    parent[imini]=nn;
    parent[imini2]=nn;
    imini=-1;
    imini2=-1;
  }
 
  printf("Affichage tableaux\n");
  for(i=0;i<2*nbfeuille-1;i++){
    if(i<nbfeuille){
      printf("Ind:%d Car:%c Val:%d fg:%d fd:%d parent:%d\n",i,car[i],val[i],fg[i],fd[i],parent[i]);
    }
    else{
      printf("Ind:%d   /   Val:%d fg:%d fd:%d parent:%d\n",i,val[i],fg[i],fd[i],parent[i]);
    }
  }
  printf("***_Remplissage Arbre_***\n");
  for(i=0;i<2*nbfeuille-1;i++)
    {
      tabNoeud[i].val=val[i];
      if(i!=2*nbfeuille-2)
	{
	  tabNoeud[i].parent=&tabNoeud[parent[i]];
	}
      else
	{
	  tabNoeud[i].parent=NULL;
	}
    }

  for(i=0;i<nbfeuille;i++)
    {
      tabNoeud[i].symbole=car[i];
    }
  for(i=0;i<nbfeuille;i++)
    {
      tabNoeud[i].fd=NULL;
      tabNoeud[i].fg=NULL;
    }   
  for(i=nbfeuille;i<2*nbfeuille-1;i++)
    {
      tabNoeud[i].fd=&tabNoeud[fd[i]];
      tabNoeud[i].fg=&tabNoeud[fg[i]];
    }
  for(i=0;i<2*nbfeuille-1;i++)
    {
      tabNoeud[i].code=NULL;
    }
   

  printf("***_Remplissage codes_***\n");
  if(2*nbfeuille-2==0)
    {
      tabNoeud[0].code="0";
      printf("symbole : %c nombre d'occurence : %d code:%s \n",tabNoeud[0].symbole,tabNoeud[0].val,tabNoeud[0].code);
    }
  else
    {
      ArbreBin A=&(tabNoeud[2*nbfeuille-2]);
      //printf("val:%d code:%s  fgval:%d fdval:%d \n",A->val,A->code,A->fg->val,A->fd->val);
      code(A);
      for(i=0;i<nbfeuille;i++)
	{
	  printf("symbole : %c nombre d'occurence : %d code:%s \n",tabNoeud[i].symbole,tabNoeud[i].val,tabNoeud[i].code);
	}
    }
  /*
    printf("_Crée un nouveau fichier texte en fonction du code associé à chaque lettre_\n");
    FILE *g=fopen(argv[1],"r");
    FILE *h=fopen("file.bin","wb");
    int lu=0;
    i=0;
    int j=0;
    float taillefin=0;
    while(EOF!=(lu=fgetc(g))){
    while(lu!=tabNoeud[i].symbole){
    i++;
    }
    while(tabNoeud[i].code[j]!='\0'){
    j++;
    }
    fwrite(tabNoeud[i].code,j,1,h);
    taillefin+=1;
    j=0;
    }
    fclose(g);
    fclose(h);
  */

  //affichage de la longeur moyenne de codage
  float lm=0;
  for(i=0;i<nbfeuille;i++){
    while(tabNoeud[i].code[j]!='\0')
      {
	j+=1;
      }
    lm+=j*tabNoeud[i].val;
    j=0;
  }
  lm=lm/(tailledep/8);
  printf("Longeur moyenne de codage=%.2f bits\n",lm);
  //affichage des tailles
  printf("Taille originelle : %d bits soit %d octets\nTaille compressée: %d bits soit %d octets\nGain=%.2f\%\n",(int)tailledep,(int)tailledep/8,(int)taillefin,(int)taillefin/8,((tailledep-taillefin)/tailledep)*100);

  return 0;
}

/* ***_Remplissage d'un tableau de codes_*** */
FILE *x=fopen(argv[1],"r");
int k=0;
i=0;
unsigned int *tabCodes=malloc(sizeof(unsigned int)*8);

init(tabCodes);

while(EOF!=(k=fgetc(x))){
  while(k!=tabNoeud[i].symbole){//cherche l'indice du tabNoeud correspondant au caractère lu
    i++;
  }
  int reste=reste(tabCodes);
  int lcode=taille_c(tabNoeud[i].code);
  int m=0;
  if(lcode <= reste){//si y'à assez d'espace dans tabCodes il y fou le tabNoeud[i].code correspondant et passe au char suivant
    for(i=0;i<lcode;i++){
      tabCodes[8-reste]=tabNoeud[i].code[m];
    }
  }
  else{//s'il n'y à pas assez de place
    while(m<=lcode){ //tant qu'on est pas au bout du code
      while(reste!=0){//et tant qu'il y a de la place, tu rempli
	tabCodes[8-reste]=tabNoeud[i].code[m];
	m+=1;
	reste-=1;
	lcode-=1;
      }
      fwrite(convert_b_d(tabCodes),x,1,h);
      FWRITE();//plus de place : vider le tabCodes plein en décimal dans un FILE
      init(tabCodes);//le reinitialiser
      reste=reste(tabCodes);
    }
  }
  taillefin+=1;
 }
if (k==EOF){
  reste=reste(tabCodes);
  while(reste != 0){
    tabCodes[8-reste]=0;
    reste-=1;
  }
  fwrite(convert_b_d(tabCodes),x,1,h);
 }
  delete[] tabCodes;

