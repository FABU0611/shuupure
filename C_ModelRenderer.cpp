
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "Main.h"
#include "Renderer.h"
#include "C_ModelRenderer.h"



std::unordered_map<std::string, MODEL*> ModelRenderer::_modelpool;
float ModelRenderer::_radius;


void ModelRenderer::Draw() {

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_model->VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(_model->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	for (unsigned int i = 0; i < _model->SubsetNum; i++) {
		// マテリアル設定
		_model->SubsetArray[i].Material.Material.Roughness = 0.5f;
		_model->SubsetArray[i].Material.Material.Metallic = 0.5f;

		Renderer::SetMaterial(_model->SubsetArray[i].Material.Material);

		// テクスチャ設定
		if (_model->SubsetArray[i].Material.Texture)
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_model->SubsetArray[i].Material.Texture);

		// ポリゴン描画
		if (!_isinstance) {
			Renderer::GetDeviceContext()->DrawIndexed(_model->SubsetArray[i].IndexNum, _model->SubsetArray[i].StartIndex, 0);
		}
		else {
			Renderer::GetDeviceContext()->DrawIndexedInstanced(_model->SubsetArray[i].IndexNum, _instancenum, _model->SubsetArray[i].StartIndex, 0, 0);
		}
	}

}

void ModelRenderer::Preload(const char* FileName) {
	if (_modelpool.count(FileName) > 0) {
		return;
	}

	MODEL* model = new MODEL;
	LoadModel(FileName, model);

	_modelpool[FileName] = model;

}


void ModelRenderer::UnloadAll() {
	for (std::pair<const std::string, MODEL*> pair : _modelpool) {
		pair.second->VertexBuffer->Release();
		pair.second->IndexBuffer->Release();

		for (unsigned int i = 0; i < pair.second->SubsetNum; i++) {
			if (pair.second->SubsetArray[i].Material.Texture)
				pair.second->SubsetArray[i].Material.Texture->Release();
		}

		delete[] pair.second->SubsetArray;

		delete pair.second;
	}

	_modelpool.clear();
}


void ModelRenderer::Load(const char* FileName) {
	if (_modelpool.count(FileName) > 0) {
		_model = _modelpool[FileName];
		return;
	}

	_model = new MODEL;
	LoadModel(FileName, _model);

	_modelpool[FileName] = _model;
}

void ModelRenderer::LoadModel(const char* FileName, MODEL* Model) {

	MODEL_OBJ modelObj;
	LoadObj(FileName, &modelObj);



	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * modelObj.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = modelObj.VertexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &Model->VertexBuffer);
	}


	// インデックスバッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * modelObj.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = modelObj.IndexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &Model->IndexBuffer);
	}

	// サブセット設定
	{
		Model->SubsetArray = new SUBSET[modelObj.SubsetNum];
		Model->SubsetNum = modelObj.SubsetNum;

		for (unsigned int i = 0; i < modelObj.SubsetNum; i++) {
			Model->SubsetArray[i].StartIndex = modelObj.SubsetArray[i].StartIndex;
			Model->SubsetArray[i].IndexNum = modelObj.SubsetArray[i].IndexNum;

			Model->SubsetArray[i].Material.Material = modelObj.SubsetArray[i].Material.Material;

			Model->SubsetArray[i].Material.Texture = nullptr;

			// テクスチャ読み込み
			TexMetadata metadata;
			ScratchImage image;
			wchar_t wc[256];
			mbstowcs(wc, modelObj.SubsetArray[i].Material.TextureName, sizeof(wc));
			LoadFromWICFile(wc, WIC_FLAGS_NONE, &metadata, image);
			CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &Model->SubsetArray[i].Material.Texture);

			if (Model->SubsetArray[i].Material.Texture)
				Model->SubsetArray[i].Material.Material.TextureEnable = true;
			else
				Model->SubsetArray[i].Material.Material.TextureEnable = false;

			Model->SubsetArray[i].Material.Material.NormalTextureEnable = false;

		}
	}

	_radius = CalculateRadius(modelObj.VertexArray, modelObj.VertexNum);

	delete[] modelObj.VertexArray;
	delete[] modelObj.IndexArray;
	delete[] modelObj.SubsetArray;
}






//モデル読込////////////////////////////////////////////
void ModelRenderer::LoadObj(const char* FileName, MODEL_OBJ* ModelObj) {

	char dir[MAX_PATH];
	strcpy(dir, FileName);
	PathRemoveFileSpec(dir);





	XMFLOAT3* positionArray;
	XMFLOAT3* normalArray;
	XMFLOAT2* texcoordArray;

	unsigned int	positionNum = 0;
	unsigned int	normalNum = 0;
	unsigned int	texcoordNum = 0;
	unsigned int	vertexNum = 0;
	unsigned int	indexNum = 0;
	unsigned int	in = 0;
	unsigned int	subsetNum = 0;

	MODEL_MATERIAL* materialArray = nullptr;
	unsigned int	materialNum = 0;

	char str[256];
	char* s;
	char c;


	FILE* file;
	file = fopen(FileName, "rt");
	assert(file);



	//要素数カウント
	while (true) {
		(void)fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "v") == 0) {
			positionNum++;
		}
		else if (strcmp(str, "vn") == 0) {
			normalNum++;
		}
		else if (strcmp(str, "vt") == 0) {
			texcoordNum++;
		}
		else if (strcmp(str, "usemtl") == 0) {
			subsetNum++;
		}
		else if (strcmp(str, "f") == 0) {
			in = 0;

			do {
				(void)fscanf(file, "%s", str);
				vertexNum++;
				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//四角は三角に分割
			if (in == 4)
				in = 6;

			indexNum += in;
		}
	}


	//メモリ確保
	positionArray = new XMFLOAT3[positionNum];
	normalArray = new XMFLOAT3[normalNum];
	texcoordArray = new XMFLOAT2[texcoordNum];


	ModelObj->VertexArray = new VERTEX_3D[vertexNum];
	ModelObj->VertexNum = vertexNum;

	ModelObj->IndexArray = new unsigned int[indexNum];
	ModelObj->IndexNum = indexNum;

	ModelObj->SubsetArray = new SUBSET[subsetNum];
	ModelObj->SubsetNum = subsetNum;




	//要素読込
	XMFLOAT3* position = positionArray;
	XMFLOAT3* normal = normalArray;
	XMFLOAT2* texcoord = texcoordArray;

	unsigned int vc = 0;
	unsigned int ic = 0;
	unsigned int sc = 0;


	fseek(file, 0, SEEK_SET);

	while (true) {
		(void)fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "mtllib") == 0) {
			//マテリアルファイル
			(void)fscanf(file, "%s", str);

			char path[256];
			strcpy(path, dir);
			strcat(path, "\\");
			strcat(path, str);

			LoadMaterial(path, &materialArray, &materialNum);
		}
		else if (strcmp(str, "o") == 0) {
			//オブジェクト名
			(void)fscanf(file, "%s", str);
		}
		else if (strcmp(str, "v") == 0) {
			//頂点座標
			(void)fscanf(file, "%f", &position->x);
			(void)fscanf(file, "%f", &position->y);
			(void)fscanf(file, "%f", &position->z);
			position++;
		}
		else if (strcmp(str, "vn") == 0) {
			//法線
			(void)fscanf(file, "%f", &normal->x);
			(void)fscanf(file, "%f", &normal->y);
			(void)fscanf(file, "%f", &normal->z);
			normal++;
		}
		else if (strcmp(str, "vt") == 0) {
			//テクスチャ座標
			(void)fscanf(file, "%f", &texcoord->x);
			(void)fscanf(file, "%f", &texcoord->y);
			texcoord->x = 1.0f - texcoord->x;
			texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if (strcmp(str, "usemtl") == 0) {
			//マテリアル
			(void)fscanf(file, "%s", str);

			if (sc != 0)
				ModelObj->SubsetArray[sc - 1].IndexNum = ic - ModelObj->SubsetArray[sc - 1].StartIndex;

			ModelObj->SubsetArray[sc].StartIndex = ic;


			for (unsigned int i = 0; i < materialNum; i++) {
				if (strcmp(str, materialArray[i].Name) == 0) {
					ModelObj->SubsetArray[sc].Material.Material = materialArray[i].Material;
					strcpy(ModelObj->SubsetArray[sc].Material.TextureName, materialArray[i].TextureName);
					strcpy(ModelObj->SubsetArray[sc].Material.Name, materialArray[i].Name);

					break;
				}
			}

			sc++;

		}
		else if (strcmp(str, "f") == 0) {
			//面
			in = 0;

			do {
				(void)fscanf(file, "%s", str);

				s = strtok(str, "/");
				ModelObj->VertexArray[vc].Position = positionArray[atoi(s) - 1];
				if (s[strlen(s) + 1] != '/') {
					//テクスチャ座標が存在しない場合もある
					s = strtok(nullptr, "/");
					ModelObj->VertexArray[vc].TexCoord = texcoordArray[atoi(s) - 1];
				}
				s = strtok(nullptr, "/");
				ModelObj->VertexArray[vc].Normal = normalArray[atoi(s) - 1];

				ModelObj->VertexArray[vc].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				ModelObj->IndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//四角は三角に分割
			if (in == 4) {
				ModelObj->IndexArray[ic] = vc - 4;
				ic++;
				ModelObj->IndexArray[ic] = vc - 2;
				ic++;
			}
		}
	}


	if (sc != 0)
		ModelObj->SubsetArray[sc - 1].IndexNum = ic - ModelObj->SubsetArray[sc - 1].StartIndex;


	fclose(file);


	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}




//マテリアル読み込み///////////////////////////////////////////////////////////////////
void ModelRenderer::LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum) {

	char dir[MAX_PATH];
	strcpy(dir, FileName);
	PathRemoveFileSpec(dir);



	char str[256];

	FILE* file;
	file = fopen(FileName, "rt");
	assert(file);

	MODEL_MATERIAL* materialArray;
	unsigned int materialNum = 0;

	//要素数カウント
	while (true) {
		(void)fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;


		if (strcmp(str, "newmtl") == 0) {
			materialNum++;
		}
	}


	//メモリ確保
	materialArray = new MODEL_MATERIAL[materialNum];


	//要素読込
	int mc = -1;

	fseek(file, 0, SEEK_SET);

	while (true) {
		(void)fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;


		if (strcmp(str, "newmtl") == 0) {
			//マテリアル名
			mc++;
			(void)fscanf(file, "%s", materialArray[mc].Name);
			strcpy(materialArray[mc].TextureName, "");

			materialArray[mc].Material.Emission.x = 0.0f;
			materialArray[mc].Material.Emission.y = 0.0f;
			materialArray[mc].Material.Emission.z = 0.0f;
			materialArray[mc].Material.Emission.w = 0.0f;
		}
		else if (strcmp(str, "Ka") == 0) {
			//アンビエント
			(void)fscanf(file, "%f", &materialArray[mc].Material.Ambient.x);
			(void)fscanf(file, "%f", &materialArray[mc].Material.Ambient.y);
			(void)fscanf(file, "%f", &materialArray[mc].Material.Ambient.z);
			materialArray[mc].Material.Ambient.w = 1.0f;
		}
		else if (strcmp(str, "Kd") == 0) {
			//ディフューズ
			(void)fscanf(file, "%f", &materialArray[mc].Material.Diffuse.x);
			(void)fscanf(file, "%f", &materialArray[mc].Material.Diffuse.y);
			(void)fscanf(file, "%f", &materialArray[mc].Material.Diffuse.z);
			materialArray[mc].Material.Diffuse.w = 1.0f;
		}
		else if (strcmp(str, "Ks") == 0) {
			//スペキュラ
			(void)fscanf(file, "%f", &materialArray[mc].Material.Specular.x);
			(void)fscanf(file, "%f", &materialArray[mc].Material.Specular.y);
			(void)fscanf(file, "%f", &materialArray[mc].Material.Specular.z);
			materialArray[mc].Material.Specular.w = 1.0f;
		}
		else if (strcmp(str, "Ns") == 0) {
			//スペキュラ強度
			(void)fscanf(file, "%f", &materialArray[mc].Material.Shininess);
		}
		else if (strcmp(str, "d") == 0) {
			//アルファ
			(void)fscanf(file, "%f", &materialArray[mc].Material.Diffuse.w);
		}
		else if (strcmp(str, "map_Kd") == 0) {
			//テクスチャ
			(void)fscanf(file, "%s", str);

			char path[256];
			strcpy(path, dir);
			strcat(path, "\\");
			strcat(path, str);

			strcat(materialArray[mc].TextureName, path);
		}
	}

	fclose(file);

	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}

float ModelRenderer::CalculateRadius(const VERTEX_3D* vertices, const int vertexnum) {
	//中心位置を計算
	XMFLOAT3 center = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < vertexnum; i++) {
		center += vertices[i].Position;
	}
	center /= (float)vertexnum;

	//中心からの最大距離を計算
	float maxDistanceSquared = 0.0f;

	for (int i = 0; i < vertexnum; i++) {
		const XMFLOAT3& pos = vertices[i].Position;
		XMFLOAT3 d = pos - center;
		float distanceSquared = d.x * d.x + d.y * d.y + d.z * d.z;

		maxDistanceSquared = (std::max)(maxDistanceSquared, distanceSquared);
	}
	return sqrtf(maxDistanceSquared);
}

