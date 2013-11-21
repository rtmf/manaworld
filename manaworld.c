//ManaWorld
//see LICENSE
//RtMF

#include <stdio.h>
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
	int attracts[6];
	int repels[6];
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

void printParticle(particleDefinition p)
{
	const char * pFormat="[%s] %s(%s) (%+1d,%+1d,%+1d)\n";
	printf(pFormat,p.combo,p.element,p.force,p.r,p.g,p.b);
}

int main(int argc, char ** argv)
{
	int i;
	for (i=0;i<6;i++)
	{
		printParticle(particleDef[i]);
	}
}
