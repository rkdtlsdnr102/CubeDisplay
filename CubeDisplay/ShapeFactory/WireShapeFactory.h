#pragma once

#include "ShapeFactory.h"

class CWireShapeFactory : public CShapeFactory
{
public:
	// CShapeFactory��(��) ���� ��ӵ�
	virtual CShape CreateCube() override;
};