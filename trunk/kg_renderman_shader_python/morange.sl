#define ORANGE     color(1,0.5,0.1)
#define PALEG      color(0.772, 1, 0.465)
#define WHITEMOULD color(1,1,1)
#define GREENMOULD color(0.3,0.4,0.3)

surface morange(float Ka=1,Kd=1,Ks=1;
                  float roughness=0.1;
                  color specularcolor = color(1,1,1);
                  float noiseFreq=0.01;
			float opacity=1.0;
			float namp)
{
  point Nf,PP,DP;
  point V=normalize(-I);
  float d,ks,kd,a,m;
	color cs;
 
  PP=transform("shader",P);
  /* Make dimples on surface */
  d=abs(noise(noiseFreq*PP));
  d=pow(d,6); /* make surface mostly flat */
  /* d=sin(0.5*3.14*d); round them off */
  DP=P-namp*d*N;

  
  /* combine underlying surface colour and mould colour */
  cs=ORANGE;

  N=calculatenormal(DP);
  Nf=faceforward(normalize(N),I);
  Oi= opacity;
  Ci=Os*(cs*(Ka*ambient()+Kd*diffuse(Nf))+
     specularcolor*Ks*specular(Nf,V,roughness));
}


