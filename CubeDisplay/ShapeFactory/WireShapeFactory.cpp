#include "WireShapeFactory.h"

#include <glad/glad.h>
#include <memory.h>

static float cube_vtx[] = { -0.25,-0.25,0.25,
							0.25, -0.25, 0.25,
							0.25, 0.25, 0.25,
							-0.25, 0.25, 0.25,
							-0.25,-0.25,-0.25,
							0.25,-0.25,-0.25,
							0.25,0.25,-0.25,
							-0.25,0.25,-0.25 };

static unsigned int cube_indices[] = { 0,1,1,2,2,3,3,0,
								0,4,1,5,2,6,3,7,
								4,5,5,6,6,7,7,4 };
								
static constexpr int vtx_coord_cnt = 3;


CShape CWireShapeFactory::CreateCube()
{
	CShape shape;

	shape.SetNumVertices(sizeof(cube_vtx) / sizeof(float) / vtx_coord_cnt, vtx_coord_cnt);
	shape.CopyVertices(cube_vtx);
	shape.SetIndices(cube_indices, sizeof(cube_indices) / sizeof(unsigned int));
	shape.drawmode = GL_LINES;
	
	return shape;
}
