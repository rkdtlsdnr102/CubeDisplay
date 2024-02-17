#pragma once

#include "ShapeFactory.h"

class CWireShapeFactory : public CShapeFactory
{
public:
	// CShapeFactory을(를) 통해 상속됨
	virtual CShape CreateCube() override;
};