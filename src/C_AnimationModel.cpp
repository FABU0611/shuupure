#include "main.h"
#include "renderer.h"
#include "C_AnimationModel.h"
#include "Manager.h"

void AnimationModel::Draw() {
	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	material.NormalTextureEnable = true;
	Renderer::SetMaterial(material);

	for (unsigned int m = 0; m < _aiscene->mNumMeshes; m++) {
		aiMesh* mesh = _aiscene->mMeshes[m];


		// マテリアル設定
		aiString diffuseTexture;
		aiString normalTexture;
		//aiString texture;
		aiColor3D diffuse;
		aiColor3D specularcolor;
		float opacity;
		float shininess = 0.0f;

		aiMaterial* aimaterial = _aiscene->mMaterials[mesh->mMaterialIndex];
		aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		aimaterial->Get(AI_MATKEY_OPACITY, opacity);
		if (AI_SUCCESS == aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularcolor)) {
			material.Specular = XMFLOAT4(specularcolor.r, specularcolor.g, specularcolor.b, 1.0f);
		}
		else {
			material.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (AI_SUCCESS == aimaterial->Get(AI_MATKEY_SHININESS, shininess)) {
			material.Shininess = shininess;
		}
		else{
			material.Shininess = 0.0f;
		}

		//Diffuse (アルベド) テクスチャを取得
		if (AI_SUCCESS == aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexture)) {
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_texture[diffuseTexture.data]);
		}
		else {
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_dummytex);
			material.TextureEnable = false; // ディフューズテクスチャがない場合の処理
		}

		//Normal map (ノーマルマップ) を取得
		if (AI_SUCCESS == aimaterial->GetTexture(aiTextureType_NORMALS, 0, &normalTexture)) {
			Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &_texture[normalTexture.data]);
		}
		else {
			Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &_dummyntex);
			material.NormalTextureEnable = false; // ディフューズテクスチャがない場合の処理
		}
		//上書きすれば他のモデルのテクスチャが貼られることはないっぽい

		if (Manager::GetisDrawFromLight()) {
			for (int i = 0; i < Renderer::CASCADE_NUM; i++) {
				ID3D11ShaderResourceView* shadow = Renderer::GetCameraDepthTexture(i);
				Renderer::GetDeviceContext()->PSSetShaderResources(2 + i, 1, &shadow);
			}
		}

		//if (texture == aiString(""))
		//{
		//	material.TextureEnable = false;
		//}
		//else
		//{
		//	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_texture[texture.data]);
		//	material.TextureEnable = true;
		//}

		material.Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, opacity);
		material.Ambient = material.Diffuse;
		Renderer::SetMaterial(material);


		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffer[m], &stride, &offset);

		// インデックスバッファ設定
		Renderer::GetDeviceContext()->IASetIndexBuffer(_indexbuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// ポリゴン描画
		Renderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void AnimationModel::LoadAnimation(const char* FileName, const char* Name) {
	_animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(_animation[Name]);
}

void AnimationModel::CreateBone(aiNode* node) {
	BONE bone;

	_bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++) {
		CreateBone(node->mChildren[n]);
	}
}

void AnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix) {
	//再帰的に実行することで親子関係をもって繋がる
	BONE* bone = &_bone[node->mName.C_Str()];

	//マトリックスの乗算順番に注意
	//DirectXとは逆になっている
	aiMatrix4x4 worldMatrix;

	worldMatrix *= matrix;
	worldMatrix *= bone->AnimationMatrix;


	bone->Matrix = worldMatrix;
	bone->Matrix *= bone->OffsetMatrix;

	for (unsigned int n = 0; n < node->mNumChildren; n++) {
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

void AnimationModel::Load(const char* FileName) {
	const std::string modelPath(FileName);

	_aiscene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(_aiscene);

	_vertexbuffer = new ID3D11Buffer * [_aiscene->mNumMeshes];
	_indexbuffer = new ID3D11Buffer * [_aiscene->mNumMeshes];


	//変形後頂点配列生成
	_deformvertex = new std::vector<DEFORM_VERTEX>[_aiscene->mNumMeshes];

	//再帰的にボーン生成
	CreateBone(_aiscene->mRootNode);

	for (unsigned int m = 0; m < _aiscene->mNumMeshes; m++) {
		aiMesh* mesh = _aiscene->mMeshes[m];

		// 頂点バッファ生成
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
				vertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				//従法線があるなら読みこむ
				if (mesh->HasTangentsAndBitangents()) {
					vertex[v].Tangent = XMFLOAT3(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z);
				}
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd,
				&_vertexbuffer[m]);

			delete[] vertex;
		}


		// インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_indexbuffer[m]);

			delete[] index;
		}



		//変形後頂点データ初期化
		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
			DEFORM_VERTEX deformVertex;
			deformVertex.Position = mesh->mVertices[v];
			deformVertex.Normal = mesh->mNormals[v];
			deformVertex.BoneNum = 0;

			for (unsigned int b = 0; b < 4; b++) {
				deformVertex.BoneName[b] = "";
				deformVertex.BoneWeight[b] = 0.0f;
			}

			_deformvertex[m].push_back(deformVertex);
		}


		//ボーンデータ初期化
		for (unsigned int b = 0; b < mesh->mNumBones; b++) {
			aiBone* bone = mesh->mBones[b];

			_bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

			//変形後頂点にボーンデータ格納
			for (unsigned int w = 0; w < bone->mNumWeights; w++) {
				aiVertexWeight weight = bone->mWeights[w];

				int num = _deformvertex[m][weight.mVertexId].BoneNum;

				_deformvertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
				_deformvertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
				_deformvertex[m][weight.mVertexId].BoneNum++;

				assert(_deformvertex[m][weight.mVertexId].BoneNum <= 4);
			}
		}
	}



	//テクスチャ読み込み
	for (unsigned int i = 0; i < _aiscene->mNumTextures; i++) {
		aiTexture* aitexture = _aiscene->mTextures[i];

		ID3D11ShaderResourceView* texture;

		// テクスチャ読み込み
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICMemory(aitexture->pcData, aitexture->mWidth, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
		assert(texture);

		_texture[aitexture->mFilename.data] = texture;
	}
	if (!_dummytex) {
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICFile(L"asset\\texture\\dummy.png", WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_dummytex);
		assert(_dummytex);
	}
	if (!_dummyntex) {
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICFile(L"asset\\texture\\dummynormal.png", WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_dummyntex);
		assert(_dummyntex);
	}


}




void AnimationModel::Uninit() {
	for (unsigned int m = 0; m < _aiscene->mNumMeshes; m++) {
		_vertexbuffer[m]->Release();
		_indexbuffer[m]->Release();
	}

	delete[] _vertexbuffer;
	delete[] _indexbuffer;

	delete[] _deformvertex;


	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : _texture) {
		pair.second->Release();
	}



	aiReleaseImport(_aiscene);


	for (std::pair<const std::string, const aiScene*> pair : _animation) {
		aiReleaseImport(pair.second);
	}

}

void AnimationModel::Update(const char* AnimationName1, int Frame1) {
	if (_animation.count(AnimationName1) == 0)
		return;

	if (!_animation[AnimationName1]->HasAnimations())
		return;

	//アニメーションデータからボーンマトリクス算出
	aiAnimation* animation1 = _animation[AnimationName1]->mAnimations[0];


	for (auto pair : _bone) {
		BONE* bone = &_bone[pair.first];

		aiNodeAnim* nodeAnim1 = nullptr;

		for (unsigned int c = 0; c < animation1->mNumChannels; c++) {
			if (animation1->mChannels[c]->mNodeName == aiString(pair.first)) {
				nodeAnim1 = animation1->mChannels[c];
				break;
			}
		}

		int f;

		aiQuaternion rot1;
		aiVector3D pos1;

		if (nodeAnim1) {
			f = Frame1 % nodeAnim1->mNumRotationKeys;	//簡易実装
			rot1 = nodeAnim1->mRotationKeys[f].mValue;

			f = Frame1 % nodeAnim1->mNumPositionKeys;	//簡易実装
			pos1 = nodeAnim1->mPositionKeys[f].mValue;
		}

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot1, pos1);
	}//end for

	//再帰的にボーンマトリクスを更新
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	UpdateBoneMatrix(_aiscene->mRootNode, rootMatrix);

	//頂点変換(CPUスキニング)皮膚をくっつける
	//メッシュの数だけループ　人と武器とか
	for (unsigned int m = 0; m < _aiscene->mNumMeshes; m++) {
		aiMesh* mesh = _aiscene->mMeshes[m];

		D3D11_MAPPED_SUBRESOURCE ms;
		Renderer::GetDeviceContext()->Map(_vertexbuffer[m], 0,
			D3D11_MAP_WRITE_DISCARD, 0, &ms);

		VERTEX_3D* vertex = (VERTEX_3D*)ms.pData;

		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
			DEFORM_VERTEX* deformVertex = &_deformvertex[m][v];

			aiMatrix4x4 matrix[4];
			aiMatrix4x4 outMatrix;
			matrix[0] = _bone[deformVertex->BoneName[0]].Matrix;
			matrix[1] = _bone[deformVertex->BoneName[1]].Matrix;
			matrix[2] = _bone[deformVertex->BoneName[2]].Matrix;
			matrix[3] = _bone[deformVertex->BoneName[3]].Matrix;

			//ウェイトを考慮してマトリクス算出
			//outMatrix = matrix[0] * deformVertex->BoneWeight[0]
			//			+ matrix[1] * deformVertex->BoneWeight[1]
			//			+ matrix[2] * deformVertex->BoneWeight[2]
			//			+ matrix[3] * deformVertex->BoneWeight[3]
			{
				outMatrix.a1 = matrix[0].a1 * deformVertex->BoneWeight[0]
					+ matrix[1].a1 * deformVertex->BoneWeight[1]
					+ matrix[2].a1 * deformVertex->BoneWeight[2]
					+ matrix[3].a1 * deformVertex->BoneWeight[3];

				outMatrix.a2 = matrix[0].a2 * deformVertex->BoneWeight[0]
					+ matrix[1].a2 * deformVertex->BoneWeight[1]
					+ matrix[2].a2 * deformVertex->BoneWeight[2]
					+ matrix[3].a2 * deformVertex->BoneWeight[3];

				outMatrix.a3 = matrix[0].a3 * deformVertex->BoneWeight[0]
					+ matrix[1].a3 * deformVertex->BoneWeight[1]
					+ matrix[2].a3 * deformVertex->BoneWeight[2]
					+ matrix[3].a3 * deformVertex->BoneWeight[3];

				outMatrix.a4 = matrix[0].a4 * deformVertex->BoneWeight[0]
					+ matrix[1].a4 * deformVertex->BoneWeight[1]
					+ matrix[2].a4 * deformVertex->BoneWeight[2]
					+ matrix[3].a4 * deformVertex->BoneWeight[3];


				outMatrix.b1 = matrix[0].b1 * deformVertex->BoneWeight[0]
					+ matrix[1].b1 * deformVertex->BoneWeight[1]
					+ matrix[2].b1 * deformVertex->BoneWeight[2]
					+ matrix[3].b1 * deformVertex->BoneWeight[3];

				outMatrix.b2 = matrix[0].b2 * deformVertex->BoneWeight[0]
					+ matrix[1].b2 * deformVertex->BoneWeight[1]
					+ matrix[2].b2 * deformVertex->BoneWeight[2]
					+ matrix[3].b2 * deformVertex->BoneWeight[3];

				outMatrix.b3 = matrix[0].b3 * deformVertex->BoneWeight[0]
					+ matrix[1].b3 * deformVertex->BoneWeight[1]
					+ matrix[2].b3 * deformVertex->BoneWeight[2]
					+ matrix[3].b3 * deformVertex->BoneWeight[3];

				outMatrix.b4 = matrix[0].b4 * deformVertex->BoneWeight[0]
					+ matrix[1].b4 * deformVertex->BoneWeight[1]
					+ matrix[2].b4 * deformVertex->BoneWeight[2]
					+ matrix[3].b4 * deformVertex->BoneWeight[3];


				outMatrix.c1 = matrix[0].c1 * deformVertex->BoneWeight[0]
					+ matrix[1].c1 * deformVertex->BoneWeight[1]
					+ matrix[2].c1 * deformVertex->BoneWeight[2]
					+ matrix[3].c1 * deformVertex->BoneWeight[3];

				outMatrix.c2 = matrix[0].c2 * deformVertex->BoneWeight[0]
					+ matrix[1].c2 * deformVertex->BoneWeight[1]
					+ matrix[2].c2 * deformVertex->BoneWeight[2]
					+ matrix[3].c2 * deformVertex->BoneWeight[3];

				outMatrix.c3 = matrix[0].c3 * deformVertex->BoneWeight[0]
					+ matrix[1].c3 * deformVertex->BoneWeight[1]
					+ matrix[2].c3 * deformVertex->BoneWeight[2]
					+ matrix[3].c3 * deformVertex->BoneWeight[3];

				outMatrix.c4 = matrix[0].c4 * deformVertex->BoneWeight[0]
					+ matrix[1].c4 * deformVertex->BoneWeight[1]
					+ matrix[2].c4 * deformVertex->BoneWeight[2]
					+ matrix[3].c4 * deformVertex->BoneWeight[3];


				outMatrix.d1 = matrix[0].d1 * deformVertex->BoneWeight[0]
					+ matrix[1].d1 * deformVertex->BoneWeight[1]
					+ matrix[2].d1 * deformVertex->BoneWeight[2]
					+ matrix[3].d1 * deformVertex->BoneWeight[3];

				outMatrix.d2 = matrix[0].d2 * deformVertex->BoneWeight[0]
					+ matrix[1].d2 * deformVertex->BoneWeight[1]
					+ matrix[2].d2 * deformVertex->BoneWeight[2]
					+ matrix[3].d2 * deformVertex->BoneWeight[3];

				outMatrix.d3 = matrix[0].d3 * deformVertex->BoneWeight[0]
					+ matrix[1].d3 * deformVertex->BoneWeight[1]
					+ matrix[2].d3 * deformVertex->BoneWeight[2]
					+ matrix[3].d3 * deformVertex->BoneWeight[3];

				outMatrix.d4 = matrix[0].d4 * deformVertex->BoneWeight[0]
					+ matrix[1].d4 * deformVertex->BoneWeight[1]
					+ matrix[2].d4 * deformVertex->BoneWeight[2]
					+ matrix[3].d4 * deformVertex->BoneWeight[3];
			}

			deformVertex->Position = mesh->mVertices[v];
			deformVertex->Position *= outMatrix;

			//法線変換用に移動成分を削除
			outMatrix.a4 = 0.0f;
			outMatrix.b4 = 0.0f;
			outMatrix.c4 = 0.0f;

			deformVertex->Normal = mesh->mNormals[v];
			deformVertex->Normal *= outMatrix;


			//頂点バッファへ書き込み
			vertex[v].Position.x = deformVertex->Position.x;
			vertex[v].Position.y = deformVertex->Position.y;
			vertex[v].Position.z = deformVertex->Position.z;

			vertex[v].Normal.x = deformVertex->Normal.x;
			vertex[v].Normal.y = deformVertex->Normal.y;
			vertex[v].Normal.z = deformVertex->Normal.z;

			vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
			vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

			vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		Renderer::GetDeviceContext()->Unmap(_vertexbuffer[m], 0);
	}
}


