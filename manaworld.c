//ManaWorld
//see LICENSE
//RtMF

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
/*
   Basic Commands:


 */
/*
   Particle Simulation:

   6 classes of particle
 */
typedef struct particleDefinition
{
	/*
	   combo refers to the combination of classical elements
	   	required to define the particle class.  This does not
		include anything but the 'positive' elements.  It is
		always two characters.
	   element refers to the named element defined by the combo
	   force refers to the force the element represents

	   all three are for display purposes only really
	 */
	char * combo;
	char * element;
	char * force;
	/*
	   repels and attracts have the same order as the particleDef
	    array and define which classes they repel and attract.
	 */
	unsigned int attracts[6];
	unsigned int repels[6];
	/*
	   r, g and b define the color of the particle for rendering
	 */
	signed int r,g,b;
} particleDefinition;
particleDefinition particleDef[6]={
	{"AF","Lightning","Change",
		{1,1,1,0,1,1},
		{0,0,0,1,0,0},
		1,0,0},
	{"AE","Light","Extropy",
		{1,1,1,1,0,1},
		{0,0,0,0,1,0},
		0,1,0},
	{"AW","Mist","Dynamism",
		{1,1,1,1,1,0},
		{0,0,0,0,0,1},
		0,0,1},
	{"WE","Ice","Stasis",
		{0,1,1,1,1,1},
		{1,0,0,0,0,0},
		-1,0,0},
	{"FW","Shadow","Entropy",
		{1,0,1,1,1,1},
		{0,1,0,0,0,0},
		0,-1,0},
	{"FE","Metal","Tradition",
		{1,1,0,1,1,1},
		{0,0,1,0,0,0},
		0,0,-1},
};
typedef struct particle
{
	signed int x,y;
	unsigned int class;
} particle;
typedef struct triple
{
	signed int r,g,b;
} triple;
typedef struct particleField
{
	particle * current; //current status of field
	particle * new; //storage for new status of field
	signed int maxx; //width is maxx*2+1
	signed int maxy; //height is maxy*2+1
	unsigned int pcount; //particle count
	triple * field; //rendering storage
} particleField;
unsigned int freeField(particleField * pf)
{
	if (pf==NULL) return 1;
	if (pf->current!=NULL) free(pf->current);
	if (pf->new!=NULL) free(pf->new);
	if (pf->field!=NULL) free(pf->field);
	return 0;
}
void scatterParticles(particleField * pf)
{
	unsigned int i;
	if (pf==NULL) return;
	if (pf->current==NULL) return;
	for (i=0;i<pf->pcount;i++)
	{
		pf->current[i].x=drand48()*pf->maxx;
		pf->current[i].x*=drand48()>0.5?1:-1;
		pf->current[i].y=drand48()*pf->maxx;
		pf->current[i].y*=drand48()>0.5?1:-1;
		pf->current[i].class=lrand48()%6;
	}
}
unsigned int initField(particleField * pf, unsigned int maxx, unsigned int maxy, unsigned int pcount)
{
	if (freeField(pf)) return 1;
	pf->current=malloc(pcount*sizeof(particle));
	pf->new=malloc(pcount*sizeof(particle));
	pf->field=malloc((maxx*2+1)*(maxy*2+1)*sizeof(triple));
	if (pf->current==NULL || pf->new==NULL || pf->field==NULL) return freeField(pf);
	pf->maxx=maxx;
	pf->maxy=maxy;
	pf->pcount=pcount;
	scatterParticles(pf);
	return 0;
}
void printParticle(particleDefinition p)
{
	const char * pFormat="[%s] %s(%s) (%+1d,%+1d,%+1d)\n";
	printf(pFormat,p.combo,p.element,p.force,p.r,p.g,p.b);
}
triple * tripleAtPos(particleField * pf, signed int x, signed int y)
{
	return &(pf->field[(x+pf->maxx)+(y+pf->maxy)*(pf->maxx*2+1)]);
}
void renderFieldSimple(particleField * pf)
{
	signed int x,y;
	int i;
	particleDefinition p;
	triple * t;
	if (pf==NULL || pf->current==NULL || pf->field==NULL) return;
	for (x=-pf->maxx;x<=pf->maxx;x++)
	{
		for (y=-pf->maxy;y<=pf->maxy;y++)
		{
			t=tripleAtPos(pf,x,y);
			t->r=0;
			t->g=0;
			t->b=0;
		}
	}
	for (i=0;i<pf->pcount;i++)
	{
		p=particleDef[pf->current[i].class];
		t=tripleAtPos(pf,pf->current[i].x,pf->current[i].y);
		t->r+=p.r;
		t->g+=p.g;
		t->b+=p.b;
	}
}
void printSimpleRender(particleField * pf)
{
	signed int x,y;
	triple * t;
	if (pf==NULL || pf->current==NULL || pf->field==NULL) return;
	x=-pf->maxx;
	y=-pf->maxy;
	while(y<=pf->maxy)
	{
		t=tripleAtPos(pf,x,y);
		printf("[%+3d,%+3d,%+3d]",t->r,t->g,t->b);
		x++;
		if (x>pf->maxx)
		{
			x=-pf->maxx;
			printf("\n");
			y++;
		}
	}
}
int main(int argc, char ** argv)
{
	int i;
	int x,y;
	particleField pf;
	pf.current=NULL;
	pf.new=NULL;
	pf.field=NULL;
	for (i=0;i<6;i++)
	{
		printParticle(particleDef[i]);
	}
	if (initField(&pf,4,4,1000)==0)
	{
		renderFieldSimple(&pf);
		printSimpleRender(&pf);
		return 0;
	}
	else return 1;
}
