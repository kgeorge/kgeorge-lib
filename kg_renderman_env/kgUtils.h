

float  kgComputeLuminance(color c) {

	return 0.3 * comp(c, 0) + 0.59 * comp(c, 1) + 0.11 * comp (c, 2);

}

color kgExpandDynamicRange( color c1; float dynRangeStartLum;
	 float  dynRangeExponent; float expandDynRange) {
uniform color cmax = color(1.0, 1.0, 1.0);
uniform color cmin = color(0.0, 0.0, 0.0);
if(expandDynRange <= 1.0) return c1;
float lum = kgComputeLuminance(c1);
if(lum > dynRangeStartLum) {
	lum =  (lum - dynRangeStartLum) / (1.0 - dynRangeStartLum);
	float dynMix = pow(lum, dynRangeExponent);
	color c2 = c1 * expandDynRange;
	c2 = clamp(c2, cmin, cmax);
	c1 = mix(c1, c2,  dynMix);
}
return c1;
}
