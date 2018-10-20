#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Pixel //elementele matricei citite din fisier
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}pixel;

typedef struct QuadtreeArray { //elementele vectorului memorat dupa cerinta 1
	unsigned char blue,green,red ;
	uint32_t area ;
	int32_t top_left , top_right ;
	int32_t bottom_left,bottom_right ;
} __attribute__ ( ( packed ) ) QuadtreeArray ;

typedef struct QuadtreeNode { //elementele arborelui cuaternar
	unsigned char r,g,b;
	long long int area;
	struct QuadtreeNode *ul; //up left
	struct QuadtreeNode *ur; //up right
	struct QuadtreeNode *dl; //down left
	struct QuadtreeNode *dr; //down right	
}*QuadtreeNode ;

//am pus aici antetele functiilor pentru a le putea folosi in cod oricand, indiferent de ordine
QuadtreeNode initNode(unsigned char r,unsigned char g, unsigned char b, long long int area);
int mediumRed(pixel **p,int x,int y,int size);
int mediumGreen(pixel **p,int x,int y,int size);
int mediumBlue(pixel **p,int x,int y,int size);
QuadtreeNode build(QuadtreeNode q,pixel **p,int x,int y, int size,int eps);
void buildArray(QuadtreeArray **a,QuadtreeNode q,int *i);
void build2(QuadtreeNode *q, QuadtreeArray p,QuadtreeArray *a,int nr,int *i);
int  sqroot(int nr);
QuadtreeNode buildv(QuadtreeNode q,pixel **p,int x,int y, int size,int eps);
QuadtreeNode buildh(QuadtreeNode q,pixel **p,int x,int y, int size,int eps);
QuadtreeNode overlay(QuadtreeNode q1,QuadtreeNode q2,QuadtreeNode q,int size);
void build3(pixel ***p, QuadtreeNode q,int size,int x,int y)

int mean(pixel **p,int x,int y,int size)//functia care calculeaza pragul "mean"
{
	int i,j,dr,dg,db;
	long long s=0;
	int r=mediumRed(p,x,y,size);
	int g=mediumGreen(p,x,y,size);
	int b=mediumBlue(p,x,y,size);
	for(i=y;i<y+size;i++)
		for(j=x;j<x+size;j++)
		{
			dr=(r-p[i][j].r)*(r-p[i][j].r);//delta red
			dg=(g-p[i][j].g)*(g-p[i][j].g);//deltga green
			db=(b-p[i][j].b)*(b-p[i][j].b);//delta blue
			s=s+dr+dg+db;
		}		
	return s/(3*size*size);
}

int countNodes(QuadtreeNode q)//functie ajutatoare care returneaza nr de noduri dintr-un arbore
{
	if(q==NULL) return 0;
	return 1+countNodes(q->ul)+countNodes(q->ur)+countNodes(q->dl)+countNodes(q->dr);
}
//fumctie esentiala care creeaza un arbore din matricea de pixeli citita
QuadtreeNode build(QuadtreeNode q,pixel **p,int x,int y, int size,int eps){
	int r=mediumRed(p,x,y,size);
	int g=mediumGreen(p,x,y,size);
	int b=mediumBlue(p,x,y,size);
	int area=size*size;
	q=initNode(r,g,b,area);
	if(mean(p,x,y,size)>eps)
	{
		q->ul=build(q->ul,p,x,y,size/2,eps);
		q->ur=build(q->ur,p,x+size/2,y,size/2,eps);
		q->dl=build(q->dl,p,x,y+size/2,size/2,eps);
		q->dr=build(q->dr,p,x+size/2,y+size/2,size/2,eps);
	}
	return q;
}
//functie identica cu "build", dar care creeaza arborele oglindit vertical
QuadtreeNode buildv(QuadtreeNode q,pixel **p,int x,int y, int size,int eps){
	int r=mediumRed(p,x,y,size);
	int g=mediumGreen(p,x,y,size);
	int b=mediumBlue(p,x,y,size);
	int area=size*size;
	q=initNode(r,g,b,area);
	if(mean(p,x,y,size)>eps)
	{
		q->ul=buildv(q->ul,p,x,y+size/2,size/2,eps);
		q->ur=buildv(q->ur,p,x+size/2,y+size/2,size/2,eps);
		q->dl=buildv(q->dl,p,x,y,size/2,eps);
		q->dr=buildv(q->dr,p,x+size/2,y,size/2,eps);
	}
	return q;
}
//functie identica cu "build", dar care creeaza arborele oglindit orizontal
QuadtreeNode buildh(QuadtreeNode q,pixel **p,int x,int y, int size,int eps){
	if(size==0) return q;
	int r=mediumRed(p,x,y,size);
	int g=mediumGreen(p,x,y,size);
	int b=mediumBlue(p,x,y,size);
	int area=size*size;
	q=initNode(r,g,b,area);
	if(mean(p,x,y,size)>eps)
	{
		q->ul=buildh(q->ul,p,x+size/2,y,size/2,eps);
		q->ur=buildh(q->ur,p,x,y,size/2,eps);
		q->dl=buildh(q->dl,p,x+size/2,y+size/2,size/2,eps);
		q->dr=buildh(q->dr,p,x,y+size/2,size/2,eps);
	}
	return q;
}
//functie importanta de initializare a unui nod cu valorile r,g,b si cu suprafata area
QuadtreeNode initNode(unsigned char r,unsigned char g, unsigned char b, long long int area){
	QuadtreeNode new=malloc(sizeof(struct QuadtreeNode));
	new->ul=NULL;
	new->ur=NULL;
	new->dl=NULL;
	new->dr=NULL;
	new->r=r;
	new->g=g;
	new->b=b;
	new->area=area;
	return new;
}
//functie care returneaza valoarea medie a componentei de rosu pe o suprafata a imaginii
int mediumRed(pixel **p,int x,int y,int size){
	int i,j,s=0,a;
	a=(size)*(size);
	for(i=y;i<y+size;i++)
		for(j=x;j<x+size;j++)
		{
			s=s+p[i][j].r;
		}
	return s/a;
}
//functie care returneaza valoarea medie a componentei de verde pe o suprafata a imaginii
int mediumGreen(pixel **p,int x,int y,int size){
	int i,j,s=0,a;
	a=(size)*(size);
	for(i=y;i<y+size;i++)
		for(j=x;j<x+size;j++)
		{
			s=s+p[i][j].g;
		}
	return s/a;
}
//functie care returneaza valoarea medie a componentei de albastru pe o suprafata a imaginii
int mediumBlue(pixel **p,int x,int y,int size){
	int i,j,s=0,a;
	a=(size)*(size);
	for(i=y;i<y+size;i++)
		for(j=x;j<x+size;j++)
		{
			s=s+p[i][j].b;
		}
	return s/a;
}
//functie auxiliara care returneaza numarul de frunze din arborele q
int countColors(QuadtreeNode q)
{
	if(q->ul==NULL&&q->ur==NULL&&q->dl==NULL&&q->dr==NULL) return 1; 
	return countColors(q->ul)+countColors(q->ur)+countColors(q->dl)+countColors(q->dr);
}
//functii de eliberare de memorie pentru un valgrind prietenos
void freeQuad(QuadtreeNode q)//elibereaza memoria alocata pt arbore
{
	if (q==NULL) return;

	freeQuad(q->ul);
	freeQuad(q->ur);
	freeQuad(q->dl);
	freeQuad(q->dr);

	free(q);
}

void freeMatrix(pixel **p,int w)//elibereaza memoria alocata pt matrice
{
	int i;
	for(i=0;i<w;i++)
	{
			free(p[i]);
	}
	free(p);
}

//functie esentiala de obtinere a unui vector dintr-un arbore
void buildArray(QuadtreeArray **a,QuadtreeNode q,int *i)
{
	int aux=*i;
	if(q==NULL) return;
	(*a)[*i].area=q->area;
	(*a)[*i].red=q->r;
	(*a)[*i].green=q->g;
	(*a)[*i].blue=q->b;
	if(q->ul!=NULL)
	{
		(*i)++;
		(*a)[aux].top_left=(*i);
		buildArray(a,q->ul,i);
		(*a)[aux].top_right=(*i);
		buildArray(a,q->ur,i);
		(*a)[aux].bottom_right=(*i);
		buildArray(a,q->dr,i);
		(*a)[aux].bottom_left=(*i);
		buildArray(a,q->dl,i);
	}
	else
	{
		(*a)[*i].top_left=-1;
		(*a)[*i].top_right=-1;
		(*a)[*i].bottom_right=-1;
		(*a)[*i].bottom_left=-1;
		(*i)++;		
	}
}
//functie esentiala de obtinere a unui arbore dintr-un vector rezultat in urma task-ului 1
void build2(QuadtreeNode *q, QuadtreeArray p,QuadtreeArray *a,int nr,int *i)
{
	if(*i>nr) return;
	int aux=*i;
	*q=initNode(a[aux].red,a[aux].green,a[aux].blue,a[aux].area);
	if(a[aux].top_left!=-1)
	{
		(*i)++;
		build2(&((*q)->ul),a[a[aux].top_left],a,nr,i);
		(*i)++;
		build2(&((*q)->ur),a[a[aux].top_right],a,nr,i);
		(*i)++;
		build2(&((*q)->dr),a[a[aux].bottom_right],a,nr,i);
		(*i)++;
		build2(&((*q)->dl),a[a[aux].bottom_left],a,nr,i);
	}
}
//functie de obtinere a unei matrice dintr-un arbore
void build3(pixel ***p, QuadtreeNode q,int size,int x,int y)
{
 	int i,j;
  	if(x == size) return;
  	int d=sqroot(q->area);
 	if(q->ul == NULL && q->ur == NULL && q->dl == NULL && q->dr == NULL)
 	{
 		for(i=x;i<x+d;i++)
 			for(j=y;j<y+d;j++)
 			{
 				(*p)[i][j].r=q->r;
 				(*p)[i][j].g=q->g;
 				(*p)[i][j].b=q->b;
 			}
 		return;
 	}
 	else
 	{
 		build3(p,q->ul,size,x,y);
 		build3(p,q->ur,size,x,y+d/2);
 		build3(p,q->dr,size,x+d/2,y+d/2);
 		build3(p,q->dl,size,x+d/2,y);
 	}

}
// suprapune doi arbori intr-un arbore final
QuadtreeNode overlay(QuadtreeNode q1,QuadtreeNode q2,QuadtreeNode q,int size)
{
	q=initNode((q1->r+q2->r)/2,(q1->g+q2->g)/2,(q1->b+q2->b)/2,size*size);
	if(q1->ul == NULL && q2->ul == NULL)
	{
		q->ul=NULL;
		q->ur=NULL;
		q->dl=NULL;
		q->dr=NULL;
	}
	if(q1->ul == NULL && q2->ul != NULL) 
	{
		q->ul=overlay(q1,q2->ul,q->ul,size/2);
		q->ur=overlay(q1,q2->ur,q->ur,size/2);
		q->dl=overlay(q1,q2->dl,q->dl,size/2);
		q->dr=overlay(q1,q2->dr,q->dr,size/2);		
	}
	if(q2->ul==NULL && q1->ul != NULL) 
	{
		q->ul=overlay(q1->ul,q2,q->ul,size/2);
		q->ur=overlay(q1->ur,q2,q->ur,size/2);
		q->dl=overlay(q1->dl,q2,q->dl,size/2);
		q->dr=overlay(q1->dr,q2,q->dr,size/2);
	}
	if(q1->ul != NULL && q2->ul != NULL) 
	{
		q->ul=overlay(q1->ul,q2->ul,q->ul,size/2);
		q->ur=overlay(q1->ur,q2->ur,q->ur,size/2);
		q->dl=overlay(q1->dl,q2->dl,q->dl,size/2);
		q->dr=overlay(q1->dr,q2->dr,q->dr,size/2);
	}
	return q;
}
// extrage redicalul din nr
int  sqroot(int nr)
{
	int i;
	for(i=1;i<=nr/2;i++)
		if((i*i)==nr) break;
	return i;
}

int main (int argc, const char **argv)
{
	int i,j,h,w,maxsize;
	uint32_t nrc,nr;
	char dump;
	if(strcmp(argv[1],"-c")==0)//task 1
	{
		int eps=atoi(argv[2]);
		char *t=(char*)malloc(3*sizeof(char));
		FILE *f = fopen(argv[3],"rb");
		fscanf(f,"%s",t);
		fscanf(f,"%d %d\n",&w,&h);
		fscanf(f,"%d%c",&maxsize,&dump);
		pixel **p = (pixel **)malloc(w * sizeof(pixel*));
		for(i = 0; i < w; i++)	p[i] = (pixel*)malloc(h*sizeof(pixel));
		for(i=0;i<h;i++)
			for(j=0;j<w;j++)
			{
				fread(&p[i][j],sizeof(pixel),1,f);
			}
		QuadtreeNode q=NULL;
		q=build(q,p,0,0,w,eps);//formarea arborelui
		nr = countNodes(q);
		QuadtreeArray *a=malloc(nr*sizeof(struct QuadtreeArray));
		i=0;
		buildArray(&a,q,&i);//formarea vectorului
		nrc = countColors(q);
		fclose(f);
		FILE *o = fopen(argv[4],"wb");
		fwrite(&nrc,sizeof(uint32_t),1,o);
		fwrite(&nr,sizeof(uint32_t),1,o);
		for(i=0;i<nr;i++)
		fwrite(&a[i],sizeof(struct QuadtreeArray),1,o);//printare in fisierul de iesire
		fclose(o);
		freeQuad(q);
		freeMatrix(p,w);
		free(a);
		free(t);//eliberarea memoriei
	}
	else if(strcmp(argv[1],"-d")==0)//task 2
	{
		FILE *f = fopen(argv[2],"rb");
		fread(&nrc,sizeof(int),1,f);
		fread(&nr,sizeof(int),1,f);
		QuadtreeArray *a=malloc(nr*sizeof(struct QuadtreeArray));
		for(i=0;i<nr;i++)
			fread(&a[i],sizeof(struct QuadtreeArray),1,f);
		fclose(f);//citirea vectorului
		QuadtreeNode q=NULL;
		QuadtreeArray l=a[0];
		i=0;
		build2(&q,l,a,nr,&i);//formarea arborelui din vector
		w=h=sqroot(q->area);
		pixel **p = (pixel **)malloc(w * sizeof(pixel*));
		for(i = 0; i < w; i++)	p[i] = (pixel*)malloc(h*sizeof(pixel));
		build3(&p,q,w,0,0);//formarea matricei din arbore
		FILE *o = fopen(argv[3],"wb");
		fprintf(o,"P6\n");
		fprintf(o,"%d %d\n255\n",w,h);
		for(i=0;i<w;i++)
			for(j=0;j<h;j++)
				fwrite(&p[i][j],sizeof(struct Pixel),1,o);
		fclose(o);//printarea matricei in fisierul de iesire
		freeMatrix(p,w);
		freeQuad(q);
		free(a);//eliberarea memoriei
	}
	else if(strcmp(argv[1],"-m")==0) //task 3
	{
		int eps=atoi(argv[3]);
		char *t=(char*)malloc(3*sizeof(char));
		FILE *f = fopen(argv[4],"rb");
		fscanf(f,"%s",t);
		fscanf(f,"%d %d\n",&w,&h);
		fscanf(f,"%d%c",&maxsize,&dump);
		pixel **p = (pixel **)malloc(w * sizeof(pixel*));
		for(i = 0; i < w; i++)	p[i] = (pixel*)malloc(h*sizeof(pixel));
		for(i=0;i<h;i++)
			for(j=0;j<w;j++)
			{
				fread(&p[i][j],sizeof(pixel),1,f);
			}
		fclose(f);//citirea matricei
		QuadtreeNode q=NULL;
		if(strcmp(argv[2],"v")==0) q=buildv(q,p,0,0,w,eps);//formarea arborelui oglindit vertical
		else q=buildh(q,p,0,0,w,eps);//formarea arborelui oglindit orizontal
		pixel **y = (pixel **)malloc(w * sizeof(pixel*));
		for(i = 0; i < w; i++)	y[i] = (pixel*)malloc(h*sizeof(pixel));
		build3(&y,q,w,0,0);//formarea matricei din arbore
		FILE *o = fopen(argv[5],"wb");
		fprintf(o,"P6\n");
		fprintf(o,"%d %d\n255\n",w,h);
		for(i=0;i<w;i++)
			for(j=0;j<h;j++)
				fwrite(&y[i][j],sizeof(struct Pixel),1,o);
		fclose(o);//printarea matricei
		freeMatrix(p,w);
		freeMatrix(y,w);
		freeQuad(q);
		free(t);
	}
	else //BONUS
	{
		int w1,w2,h1,h2,eps=atoi(argv[2]),maxsize1,maxsize2;
		char *t1=(char*)malloc(3*sizeof(char));
		char *t2=(char*)malloc(3*sizeof(char));
		FILE *f1 = fopen(argv[3],"rb");
		fscanf(f1,"%s",t1);
		fscanf(f1,"%d %d\n",&w1,&h1);
		fscanf(f1,"%d%c",&maxsize1,&dump);
		pixel **p1 = (pixel **)malloc(w1 * sizeof(pixel*));
		for(i = 0; i < w1; i++)	p1[i] = (pixel*)malloc(h1*sizeof(pixel));
		for(i=0;i<h1;i++)
			for(j=0;j<w1;j++)
			{
				fread(&p1[i][j],sizeof(pixel),1,f1);
			}
		fclose(f1);//citirea primei matrice
		FILE *f2 = fopen(argv[4],"rb");
		fscanf(f2,"%s",t2);
		fscanf(f2,"%d %d\n",&w2,&h2);
		fscanf(f2,"%d%c",&maxsize2,&dump);
		pixel **p2 = (pixel **)malloc(w2 * sizeof(pixel*));
		for(i = 0; i < w2; i++)	p2[i] = (pixel*)malloc(h2*sizeof(pixel));
		for(i=0;i<h2;i++)
			for(j=0;j<w2;j++)
			{
				fread(&p2[i][j],sizeof(pixel),1,f2);
			}
		fclose(f2);//citirea matricei 2
		QuadtreeNode q=NULL,q1=NULL,q2=NULL;
		q1=build(q1,p1,0,0,w1,eps);//crearea arborelui 1
		q2=build(q2,p2,0,0,w2,eps);//crearea arborelui 2
		q=overlay(q1,q2,q,w1);//suprapunerea arborilor
		pixel **y = (pixel **)malloc(w1 * sizeof(pixel*));
		for(i = 0; i < w1; i++)	y[i] = (pixel*)malloc(h1*sizeof(pixel));
		build3(&y,q,w1,0,0);//crearea matricei finale
		FILE *f3 = fopen(argv[5],"wb");
		fprintf(f3,"P6\n");
		fprintf(f3,"%d %d\n255\n",w1,h1);
		for(i=0;i<w1;i++)
				for(j=0;j<h1;j++)
					fwrite(&y[i][j],sizeof(struct Pixel),1,f3);
		fclose(f3);//printarea matricei in fisierul de iesire
		freeMatrix(p1,w1);
		freeMatrix(p2,w2);
		freeMatrix(y,w1);
		freeQuad(q1);
		freeQuad(q2);
		freeQuad(q);
		free(t1);
		free(t2);//eliberarea memoriei.

	}
	return 0;
}