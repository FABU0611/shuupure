#pragma once

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp-vc143-mt.lib")

#include "component.h"


//変形後頂点構造体
struct DEFORM_VERTEX {
	aiVector3D Position{};
	aiVector3D Normal{};
	int				BoneNum{};
	std::string		BoneName[4]{};//本来はボーンインデックスで管理するべき
	float			BoneWeight[4]{};
};

//ボーン構造体
struct BONE {
	aiMatrix4x4 Matrix{};
	aiMatrix4x4 AnimationMatrix{};
	aiMatrix4x4 OffsetMatrix{};
};

class AnimationModel : public Component {
private:
	const aiScene* _aiscene = nullptr;
	std::unordered_map<std::string, const aiScene*> _animation{};

	ID3D11Buffer** _vertexbuffer{};
	ID3D11Buffer** _indexbuffer{};

	std::unordered_map<std::string, ID3D11ShaderResourceView*> _texture{};
	ID3D11ShaderResourceView* _dummytex{};
	ID3D11ShaderResourceView* _dummyntex{};

	std::vector<DEFORM_VERTEX>* _deformvertex{};		//変形後頂点データ
	std::unordered_map<std::string, BONE> _bone{};		//ボーンデータ（名前で参照）

	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

public:
	using Component::Component;

	void Load(const char* FileName);
	void LoadAnimation(const char* FileName, const char* Name);
	void Uninit() override;
	void Update(const char* AnimationName1, int Frame1);
	void Draw() override;
};