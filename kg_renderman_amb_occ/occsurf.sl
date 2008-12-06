vector bendvector(vector in;
			vector ref;
			float theta;
			float gammar;){
			
	vector side=in^ref;
	
	float ntheta=theta*(1-0.1*random());
	
	vector go= rotate(in,ntheta,(0,0,0),side);
	go = rotate(go,gammar,(0,0,0),in);

	
	return go;
}

surface occsurf (float samples = 6; 
			float far= 6;)
{
	extern normal N;
	extern vector dPdu;
	normal Nn = normalize(N);
	
	float sum = 0;
	float i,j;
	
	point Ph;
	normal Nh;
		
	float div=0;
	float dist;
	
	vector ref= normalize(dPdu);
	float theta,gammar;
	
	
	float jsamples,jstep;
	//stratified sampling	
	for (i=1; i<=samples; i+=1){
		
		theta = sqrt(i/samples)*radians(89);
		
		jsamples=1+4*samples*sin(theta);
		jstep=PI*2/jsamples;
		
		for (j=0; j<jsamples; j+=1){
		
		
		
		gammar = j*jstep;
		
		vector dir = bendvector(Nn,ref,theta,gammar);
		div+=1;
		
		
		dist = rayhittest(P,dir,Ph,Nh);
		
		dist = smoothstep(0,far,dist);
		
			sum+=dist;
		}
		
	}
	sum/=div;
	Ci=sum;
}

