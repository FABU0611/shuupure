#pragma once



// マテリアル構造体
struct MODEL_MATERIAL {
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;

};


// 描画サブセット構造体
struct SUBSET {
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};


// モデル構造体
struct MODEL_OBJ {
	VERTEX_3D*		VertexArray;
	unsigned int	VertexNum;

	unsigned int*	IndexArray;
	unsigned int	IndexNum;

	SUBSET*			SubsetArray;
	unsigned int	SubsetNum;
};

struct MODEL {
	ID3D11Buffer*	VertexBuffer;
	ID3D11Buffer*	IndexBuffer;

	SUBSET*			SubsetArray;
	unsigned int	SubsetNum;
};


#include "component.h"
#include <string>
#include <unordered_map>


class ModelRenderer : public Component {
	static std::unordered_map<std::string, MODEL*> _modelpool;
	static float	_radius;
	bool	_isinstance = false;
	int		_instancenum{};

	static void LoadModel(const char* FileName, MODEL* Model);
	static void LoadObj(const char* FileName, MODEL_OBJ* ModelObj);
	static void LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum);
	static float CalculateRadius(const VERTEX_3D* vertices, const int vertexnum);

	MODEL* _model{};

public:

	static void Preload(const char* FileName);
	static void UnloadAll();

	void SetInstanceNum(const int& num) {
		_instancenum = num;
		if (_instancenum == 1) {
			_isinstance = false;
		}
		else {
			_isinstance = true;
		}
	}

	float GetRadius()const { return _radius; }

	using Component::Component;

	void Load(const char* FileName);

	void Draw() override;
};