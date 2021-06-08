typedef float4 point;
typedef float4 vector;
typedef float4 color;
typedef float4 sphere;

vector
Bounce( vector in, vector n )
{
	vector out = in - n*(vector)( 2.*dot(in.xyz, n.xyz) );
	out.w = 0.;
	return out;
}

vector
BounceSphere( point p, vector v, sphere s )
{
	vector n;
	n.xyz = fast_normalize( p.xyz - s.xyz );
	n.w = 0.;
	return Bounce( v, n );
}

bool
IsInsideSphere( point p, sphere s )
{
	float r = fast_length( p.xyz - s.xyz );
	return  ( r < s.w );
}

color rgb(vector v) {

	int i = (int)(((v.x * v.x) + (v.y * v.y) + (v.z * v.z)) / 200.) % 256;

	int normalized = (int)(i * 6);

	int x = normalized % 256;

	int red = 0, grn = 0, blu = 0;
	switch(normalized / 256)     {
		case 0: red = 255;      grn = x;        blu = 0;       break;//red
		case 1: red = 255 - x;  grn = 255;      blu = 0;       break;//yellow
		case 2: red = 0;        grn = 255;      blu = x;       break;//green
		case 3: red = 0;        grn = 255 - x;  blu = 255;     break;//cyan
		case 4: red = x;        grn = 0;        blu = 255;     break;//blue
		case 5: red = 255;      grn = 0;        blu = 255 - x; break;//magenta
	}

	return (color)(red / 255., grn / 255., blu / 255., 1.);
}

kernel
void
Particle( global point *dPobj, global vector *dVel, global color *dCobj )
{
	const float4 G       = (float4) ( 0., -9.8, 0., 0. );
	const float  DT      = 0.1;
	const sphere Sphere1 = (sphere)( -100., -800., 0.,  600. );
	const sphere Sphere2 = (sphere)(400, -1200., 700., 800.);
	int gid = get_global_id( 0 );

	// extract the position and velocity for this particle:
	point  p = dPobj[gid];
	vector v = dVel[gid];

	// remember that you also need to extract this particle's color
	// and change it in some way that is obviously correct

	dCobj[gid] = rgb(v);

	// advance the particle:

	point  pp = p + v*DT + G*(point)( .5*DT*DT );
	vector vp = v + G*DT;
	pp.w = 1.;
	vp.w = 0.;

	// test against the first sphere here:

	if( IsInsideSphere( pp, Sphere1 ) )
	{
		vp = BounceSphere( p, v, Sphere1 );
		pp = p + vp*DT + G*(point)( .5*DT*DT );
	}

	if(IsInsideSphere(pp, Sphere2)) 	{
		vp = BounceSphere(p, v, Sphere2);
		pp = p + vp * DT + G * (point)(.5 * DT * DT);
	}

	// test against the second sphere here:

	dPobj[gid] = pp;
	dVel[gid]  = vp;

}
