#include "E:\Program Files\BMRT\shaders\reflections.h"
#include "kgUtils.h"


surface kgEnv (string envname = ""; float expandDynRange = 2.0;) 
{

float Ka = 0.02;
float Kd = 0.05;
float Ks = 0.03;
float Kr = 0.9;
/*
float Ka = 0.1;
float Kd = 0.3;
float Ks = 0.1;
float Kr = 0.5;
*/

float roughness = 0.2;
float blur = 0;
string envspace="world";
uniform float envrad = 100;
uniform float rayjitter = 0;
uniform  float raysamples =1;


normal Nf = normalize(faceforward(N, I));
vector ni = normalize(I);
vector V = -ni;
vector R = normalize(reflect(I, N));
color c1  =  (Ka*ambient() + Kd*diffuse(Nf) +
                        Ks*specular(Nf,V,roughness) +
		  SampleEnvironment(P, R, Kr, blur, envname, envspace, 
			envrad, rayjitter, raysamples)
	);
//float f1 = kgComputeLuminance(c1);

Ci = c1;/*kgExpandDynamicRange(c1, 0.5, 2.0, expandDynRange);*/
Oi=Os;

} 
