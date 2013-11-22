//ManaWorld
//see LICENSE
//RtMF

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
	   poles refers to which of the poles it attracts
	    array goes cw from top, FAWE
	 */
	unsigned int attracts[6];
	unsigned int repels[6];
	unsigned int poles[4];
	/*
	   r, g and b define the color of the particle for rendering
	 */
	signed int r,g,b;
} particleDefinition;
particleDefinition particleDef[6]={
	{"AF","Lightning","Change",
		{1,0,0,0,0,0},
		{0,0,0,1,0,0},
		{1,1,0,0},
		1,0,0},
	{"AE","Light","Extropy",
		{0,1,0,0,0,0},
		{0,0,0,0,1,0},
		{0,1,0,1},
		0,1,0},
	{"AW","Mist","Dynamism",
		{0,0,1,0,0,0},
		{0,0,0,0,0,1},
		{0,1,1,0},
		0,0,1},
	{"WE","Ice","Stasis",
		{0,0,0,1,0,0},
		{1,0,0,0,0,0},
		{0,0,1,1},
		-1,0,0},
	{"FW","Shadow","Entropy",
		{0,0,0,0,1,0},
		{0,1,0,0,0,0},
		{1,0,1,0},
		0,-1,0},
	{"FE","Metal","Tradition",
		{0,0,0,0,0,1},
		{0,0,1,0,0,0},
		{1,0,0,1},
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
	unsigned int count;
} triple;
typedef struct vect
{
	double x,y;
} vect;
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
void printParticleDef(particleDefinition p)
{
	const char * pFormat="[%s] %s(%s) (%+1d,%+1d,%+1d)\n";
	printf(pFormat,p.combo,p.element,p.force,p.r,p.g,p.b);
}
void printParticles(particleField * pf)
{
	const char * pFormat="(%+3d,%+3d) %1d:";
	int i;
	for (i=0;i<pf->pcount;i++)
	{
		printf(pFormat,pf->current[i].x,pf->current[i].y,pf->current[i].class);
		printParticleDef(particleDef[pf->current[i].class]);
	}
}
triple * tripleAtPos(particleField * pf, signed int x, signed int y)
{
	int px=x+pf->maxx;
	int py=y+pf->maxy;
	int idx=px+py*(pf->maxx*2+1);
	return &(pf->field[idx]);
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
			t->count=0;
		}
	}
	for (i=0;i<pf->pcount;i++)
	{
		p=particleDef[pf->current[i].class];
		t=tripleAtPos(pf,pf->current[i].x,pf->current[i].y);
		t->r+=p.r;
		t->g+=p.g;
		t->b+=p.b;
		t->count++;
	}
}
void printSimpleRender(particleField * pf)
{
	signed int x,y;
	double r,g,b;
	unsigned int tr,tg,tb;
	triple * t;
	if (pf==NULL || pf->current==NULL || pf->field==NULL) return;
	x=-pf->maxx;
	y=pf->maxy;
	printf("\e[HSimple Render:\n");
	while(y>=-pf->maxy)
	{
		t=tripleAtPos(pf,x,y);
		if (t->count<1)
		{
			tr=2;
			tg=2;
			tb=2;
		}
		else
		{
			r=t->r/(double)t->count+0.5;
			g=t->g/(double)t->count+0.5;
			b=t->b/(double)t->count+0.5;
			tr=r*6;
			tg=g*6;
			tb=b*6;
			tr=tr>5?5:tr;
			tg=tg>5?5:tg;
			tb=tb>5?5:tb;
		}
		printf("\e[48;5;%dm \e[48;5;0m",16+36*tr+6*tg+tb);
		//printf("[%+3d,%+3d,%+3d]",t->r,t->g,t->b);
		x++;
		if (x>pf->maxx)
		{
			x=-pf->maxx;
			printf("\n");
			y--;
		}
	}
}
void addForceFrom(particleField * pf, int p, vect * v, signed int x, signed int y, double mag)
{
	double xf,yf;
	double dist;
	xf=x-pf->current[p].x;
	yf=y-pf->current[p].y;
	dist=sqrt(xf*xf+yf*yf);
	if (dist<1) return;
	if (abs(mag)>dist) mag=dist*(mag/abs(mag));
	xf=xf/dist;
	yf=yf/dist;
	v->x+=xf*mag;
	v->y+=yf*mag;
}
void addPoleForce(particleField * pf, int p, vect * v)
{
	particleDefinition pd=particleDef[pf->current[p].class];
	addForceFrom(pf,p,v,0,pf->maxy,pd.poles[0]*pf->maxy);
	addForceFrom(pf,p,v,pf->maxx,0,pd.poles[1]*pf->maxx);
	addForceFrom(pf,p,v,0,-pf->maxy,pd.poles[2]*pf->maxy);
	addForceFrom(pf,p,v,-pf->maxx,0,pd.poles[3]*pf->maxx);
}
#define SCALE 2
void addParticleForce(particleField * pf, int p, vect * v)
{
	int i;
	particle pt;
	double mag;
	particleDefinition pd=particleDef[pf->current[p].class];
	for (i=0;i<pf->pcount;i++)
	{
		pt=pf->current[i];
		if (pd.attracts[pt.class]) mag=SCALE;
		else if (pd.repels[pt.class]) mag=-SCALE;
		else continue;
		addForceFrom(pf,p,v,pt.x,pt.y,mag);
	}
}
void mutateParticle(particleField * pf, int p)
{
	vect v;
	v.x=0;
	v.y=0;
	pf->new[p]=pf->current[p];
	addPoleForce(pf,p,&v);
	addParticleForce(pf,p,&v);
	//addForceFrom(pf,p,&v,0,0,8);
	pf->new[p].x+=v.x;
	while (pf->new[p].x>pf->maxx) pf->new[p].x-=(pf->maxx*2+1);
	while (pf->new[p].x<-pf->maxx) pf->new[p].x+=(pf->maxx*2+1);
	//if (pf->new[p].x>pf->maxx) pf->new[p].x=pf->maxx;
	//if (pf->new[p].x<-pf->maxx) pf->new[p].x=-pf->maxx;
	pf->new[p].y+=v.y;
	while (pf->new[p].y>pf->maxy) pf->new[p].y-=(pf->maxy*2+1);
	while (pf->new[p].y<-pf->maxy) pf->new[p].y+=(pf->maxy*2+1);
	//if (pf->new[p].y>pf->maxy) pf->new[p].y=pf->maxy;
	//if (pf->new[p].y<-pf->maxy) pf->new[p].y=-pf->maxy;
}
void mutateField(particleField * pf)
{
	int i;
	if (pf==NULL || pf->current==NULL || pf->new==NULL) return;
	for (i=0;i<pf->pcount;i++)
		mutateParticle(pf,i);
	for (i=0;i<pf->pcount;i++)
		pf->current[i]=pf->new[i];
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
		printParticleDef(particleDef[i]);
	}
	if (initField(&pf,20,20,1000)==0)
	{
		while(1)
		{
			renderFieldSimple(&pf);
			printSimpleRender(&pf);
			//printParticles(&pf);
	//		sleep(1);
			mutateField(&pf);
		}
		return 0;
	}
	else return 1;
}
