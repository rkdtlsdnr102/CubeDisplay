#include "SolidShapeFactory.h"
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

// 위쪽 면 반시계방향으로 정점 4개
// 아래쪽면 반시계방향으로 정점 4개
static float tri_vtx[] = { 0.5,-0.5,0,
							0.5, 0.5, 0,
							-0.5, 0.5, 0 };

static constexpr int vtx_coord_cnt = 3;

// 윗면
// 옆면1
// 옆면2
// 옆면3
// 옆면4
// 아랫면
//unsigned int cube_indices[] = { 0,1,2,3,
//				0,4,5,1,
//				1,5,6,2,
//				1,6,7,3,
//				3,7,4,0,
//				4,5,6,7 };
static unsigned int cube_indices[] = { 0,1,2,
				0,2,3,
				0,4,5,
				0,5,1,
				1,5,6,
				1,6,2,
				2,6,7,
				2,7,3,
				3,7,4,
				3,4,0,
				4,5,6,
				4,6,7 };
//unsigned int cube_indices[] = { 0,1,2 };


template<typename T> inline void copy_vtx(T* pDestArr, rsize_t destPos, rsize_t destSz, const T* pSrcArr, rsize_t srcPos)
{
	memcpy_s(pDestArr + destPos * 3, destSz, pSrcArr + srcPos * 3, sizeof(T) * 3);
}
//
//CShape CBasicShapeFactory::CreateCube()
//{
//	CShape shape;
//	shape.data.reset((float*)nullptr);
//
//	try {
//
//		constexpr rsize_t nBufferByte = sizeof(tri_vtx);
//
//		shape.data.reset(new float[nBufferByte / sizeof(float)]);
//		shape.nVertex = nBufferByte / vtx_coord_cnt / sizeof(float);
//		shape.dim = vtx_coord_cnt;
//
//		float* ptr_raw = shape.data.get();
//
//		copy_vtx(ptr_raw, 0, nBufferByte, cube_vtx, 0);
//		copy_vtx(ptr_raw, 1, nBufferByte, cube_vtx, 1);
//		copy_vtx(ptr_raw, 2, nBufferByte, cube_vtx, 2);
//	}
//	catch (std::bad_alloc& ex)
//	{
//		fprintf_s(stderr, ex.what());
//		shape.data.reset((float*)nullptr);
//	}
//
//	return shape;
//}

//
//CShape CBasicShapeFactory::CreateCube()
//{
//	CShape shape;
//	shape.data.reset((float*)nullptr);
//
//	// cube 생성, 윗면, 옆면 1, 2, 3, 4, 아랫면
//	// 반시계방향으로 생성
//	try {
//		
//		// 면 6개 * 면당 점 4개 * 점당 float 개수 * float 사이즈
//		constexpr rsize_t nBufferByte = 6 * 4 * vtx_coord_cnt * sizeof(float);
//
//		shape.data.reset(new float[nBufferByte / sizeof(float)]);
//		shape.nVertex = nBufferByte / vtx_coord_cnt / sizeof(float);
//		shape.dim = vtx_coord_cnt;
//
//		float* ptr_raw = shape.data.get();
//		
//		// 윗면
//		copy_vtx(ptr_raw, 0, nBufferByte, cube_vtx, 0);
//		copy_vtx(ptr_raw, 1, nBufferByte, cube_vtx, 1);
//		copy_vtx(ptr_raw, 2, nBufferByte, cube_vtx, 2);
//		copy_vtx(ptr_raw, 3, nBufferByte, cube_vtx, 3);
//
//		// 옆면1
//		copy_vtx(ptr_raw, 4, nBufferByte, cube_vtx, 0);
//		copy_vtx(ptr_raw, 5, nBufferByte, cube_vtx, 4);
//		copy_vtx(ptr_raw, 6, nBufferByte, cube_vtx, 5);
//		copy_vtx(ptr_raw, 7, nBufferByte, cube_vtx, 1);
//
//		// 옆면2
//		copy_vtx(ptr_raw, 8, nBufferByte, cube_vtx, 1);
//		copy_vtx(ptr_raw, 9, nBufferByte, cube_vtx, 5);
//		copy_vtx(ptr_raw, 10, nBufferByte, cube_vtx, 6);
//		copy_vtx(ptr_raw, 11, nBufferByte, cube_vtx, 2);
//
//		// 옆면3
//		copy_vtx(ptr_raw, 12, nBufferByte, cube_vtx, 1);
//		copy_vtx(ptr_raw, 13, nBufferByte, cube_vtx, 6);
//		copy_vtx(ptr_raw, 14, nBufferByte, cube_vtx, 7);
//		copy_vtx(ptr_raw, 15, nBufferByte, cube_vtx, 3);
//
//		// 옆면4
//		copy_vtx(ptr_raw, 16, nBufferByte, cube_vtx, 3);
//		copy_vtx(ptr_raw, 17, nBufferByte, cube_vtx, 7);
//		copy_vtx(ptr_raw, 18, nBufferByte, cube_vtx, 4);
//		copy_vtx(ptr_raw, 19, nBufferByte, cube_vtx, 0);
//
//		// 아랫면
//		copy_vtx(ptr_raw, 20, nBufferByte, cube_vtx, 4);
//		copy_vtx(ptr_raw, 21, nBufferByte, cube_vtx, 5);
//		copy_vtx(ptr_raw, 22, nBufferByte, cube_vtx, 6);
//		copy_vtx(ptr_raw, 23, nBufferByte, cube_vtx, 7);
//	}
//	catch( std::bad_alloc &ex)
//	{
//		fprintf_s(stderr, ex.what());
//		shape.data.reset((float*)nullptr);
//	}
//
//	return shape;
//}

CShape CSolidShapeFactory::CreateCube()
{
	CShape shape;
	shape.data.reset((float*)nullptr);

	// cube 생성, 윗면, 옆면 1, 2, 3, 4, 아랫면
	// 반시계방향으로 생성
	try {
		
		constexpr rsize_t nBufferByte = sizeof(cube_vtx);

		shape.data.reset(new float[nBufferByte / sizeof(float)]);
		shape.nVertices = nBufferByte / vtx_coord_cnt / sizeof(float);
		shape.dim = vtx_coord_cnt;
		
		shape.pIndices = cube_indices;
		shape.drawmode = GL_TRIANGLES;
		shape.nIndices = sizeof(cube_indices) / sizeof(unsigned int);

		float* ptr_raw = shape.data.get();

		for (int i = 0; i < shape.nVertices; i++)
		{
			//vertex copy
			copy_vtx(ptr_raw, i, nBufferByte, cube_vtx, i);
		}
	}
	catch( std::bad_alloc &ex)
	{
		fprintf_s(stderr, ex.what());
		shape.data.reset((float*)nullptr);
	}

	return shape;
}
