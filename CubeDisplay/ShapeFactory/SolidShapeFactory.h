#pragma once
#include "ShapeFactory.h"

class CSolidShapeFactory : public CShapeFactory
{
public:
	// CShapeFactory��(��) ���� ��ӵ�
	virtual CShape CreateCube() override;

};