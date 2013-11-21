//ManaWorld
//see LICENSE
//RtMF

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
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
typedef struct particleField
{
	particle * current; //current status of field
	particle * new; //storage for new status of field
	unsigned int maxx; //width is maxx*2+1
	unsigned int maxy; //height is maxy*2+1
	unsigned int pcount; //particle count
} particleField;
unsigned int freeField(particleField * pf)
{
	if (pf==NULL) return 1;
	if (pf->current!=NULL) free(pf->current);
	if (pf->new!=NULL) free(pf->new);
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
	if (pf->current==NULL || pf->new==NULL) return freeField(pf);
	pf->maxx=maxx;
	pf->maxy=maxy;
	pf->pcount=pcount;
	scatterParticles(pf);
	return 0;
};
void printParticle(particleDefinition p)
{
	const char * pFormat="[%s] %s(%s) (%+1d,%+1d,%+1d)\n";
	printf(pFormat,p.combo,p.element,p.force,p.r,p.g,p.b);
}

int main(int argc, char ** argv)
{
	int i;
	particleField pf;
	pf.current=NULL;
	pf.new=NULL;
	for (i=0;i<6;i++)
	{
		printParticle(particleDef[i]);
	}
	return initField(&pf,127,127,1000);
}
