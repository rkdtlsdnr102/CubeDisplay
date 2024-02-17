#pragma once

#include <memory>
#include "ShapeData.h"

class CShapeFactory
{
public:
	virtual CShape CreateCube() = 0;

	template <class Factory> static Factory& GetInstance()
	{
		Factory factory;

		return factory;
	}
};