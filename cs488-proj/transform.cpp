#include "transform.hpp"

// Return a matrix to represent a counterclockwise rotation of "angle"
// degrees around the axis "axis", where "axis" is one of the
// characters 'x', 'y', or 'z'.
Matrix4x4 rotation(double angle, char axis) 
{
	Matrix4x4 r;
	switch (axis){
		case 'x':
			r = Matrix4x4(
					Vector4D(1,	0,        	0,        			0),
					Vector4D(0, cos(angle), (-1)*sin(angle), 	0),
					Vector4D(0, sin(angle),	cos(angle), 		0),
					Vector4D(0,	0,        	0,        			1)
				);
			break;
		case 'y':
		
			r = Matrix4x4(
					Vector4D(cos(angle), 		0, 	sin(angle), 0),
					Vector4D(0,        			1,	0,        	0),
					Vector4D((-1)*sin(angle), 	0, 	cos(angle), 0),
					Vector4D(0,     			0,	0,        	1)
				); 
			break;
		case 'z':
			r = Matrix4x4(
					Vector4D(cos(angle), 	(-1)*sin(angle),0, 	0),
					Vector4D(sin(angle),	cos(angle),		0, 	0),
					Vector4D(0,           	0,       		1, 	0),
					Vector4D(0,          	0, 		      	0, 	1)
				);

			break;
		default:
			break;
	}
  return r;
}

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 translation(const Vector3D& displacement)
{
	Matrix4x4 t;
	t = Matrix4x4(
			Vector4D(	1,	0,	0,	displacement[0]	),
			Vector4D(	0,	1,	0,	displacement[1]	),
			Vector4D(	0,	0,	1,	displacement[2]	),
			Vector4D(	0,	0,	0,	1    			)
		);
	return t;
}

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 scaling(const Vector3D& scale)
{
	Matrix4x4 s;
	s = Matrix4x4(
			Vector4D(	scale[0],	0,     		0,    		0	),
			Vector4D(	0,      	scale[1],  	0,    		0	),
			Vector4D(	0,          0, 			scale[2],	0	),
			Vector4D(	0,          0,     		0,    		1	)
		);
	return s;
}
