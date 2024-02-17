#pragma once

struct CShape
{
	int nVertices;
	int dim;
	std::shared_ptr<float> data;
	
	const unsigned int *pIndices;
	int nIndices;
	
	unsigned int drawmode;

	void SetNumVertices(int _nVertices, int _dim)
	{
		nVertices = _nVertices;
		dim = _dim;
	}

	void SetIndices(const unsigned int* _pIndices, int _nIndices)
	{
		pIndices = _pIndices;
		nIndices = _nIndices;
	}

	void CopyVertices(const float* pDataSrc) noexcept
	{
		int nSizeByte = GetByteSize();
		int bytePerVertex = GetByteSizePerVertex();

		data.reset(new float[nSizeByte]);

		float* pDestData = data.get();
		
		for (int i = 0; i < nVertices; i++)
		{
			memcpy_s(pDestData + i * dim, nSizeByte, pDataSrc + i * dim, bytePerVertex);
		}
	}

	int GetByteSize() const
	{
		return nVertices * dim * sizeof(float);
	}

	int GetByteSizePerVertex() const
	{
		return dim * sizeof(float);
	}

	int GetIndexByteSize() const
	{
		return nIndices * sizeof(unsigned int);
	}

};
