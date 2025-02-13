//Wave.h
//20106_田中　蓮
//24_11_11
#include "Wave.h"
#include "Time.h"
#include "ShaderManager.h"
#include "ErrorHandler.h"
#include "Input.h"

const float Wave::WAVE_AMPLITUDE = 100.0f;
const float Wave::WAVE_LENGTH = 70.0f;
const float Wave::WAVE_CYCLE = 2.0f;

void Wave::Init() {
	SetPosition({ 0.0f, 0.5f, 0.0f });

	//頂点バッファ生成
	{
		for (int x = 0; x < VERTEX_NUM; x++) {
			for (int z = 0; z < VERTEX_NUM; z++) {
				_vertex[x][z].Position = XMFLOAT3((x - (VERTEX_NUM * 0.5f)) * 5.0f, 0.0f, (z - (VERTEX_NUM * 0.5f)) * -5.0f);
				_vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				_vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				_vertex[x][z].TexCoord = XMFLOAT2(x * 0.5f, z * 0.5f);
				_vertex[x][z].Tangent = { 1.0f, 0.0f, 0.0f };
			}
		}

		//法線ベクトル算出
		for (int x = 1; x < VERTEX_NUM - 1; x++) {
			for (int z = 1; z < VERTEX_NUM - 1; z++) {
				XMFLOAT3	vx, vz, vn;
				vx = _vertex[x + 1][z].Position - _vertex[x - 1][z].Position;

				vz = _vertex[x][z - 1].Position - _vertex[x][z + 1].Position;

				//外積
				vn.x = vz.y * vx.z - vz.z * vx.y;
				vn.y = vz.z * vx.x - vz.x * vx.z;
				vn.z = vz.x * vx.y - vz.y * vx.x;

				float len = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
				vn /= len;

				_vertex[x][z].Normal = vn;
			}
		}



		D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * VERTEX_NUM * VERTEX_NUM;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = _vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_vertexbuffer);
	}

	//インデックスバッファ生成
	unsigned int index[((VERTEX_NUM + 1) * 2) * (VERTEX_NUM - 1) - 2];

	int i = 0;
	for (int x = 0; x < VERTEX_NUM - 1; x++) {
		for (int z = 0; z < VERTEX_NUM; z++) {
			index[i] = x * VERTEX_NUM + z;
			i++;

			index[i] = (x + 1) * VERTEX_NUM + z;
			i++;
		}
		if (x == VERTEX_NUM - 2) {
			break;
		}
		index[i] = (x + 1) * VERTEX_NUM + VERTEX_NUM - 1;
		i++;

		index[i] = (x + 1) * VERTEX_NUM;
		i++;
	}
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * (((VERTEX_NUM + 1) * 2) * (VERTEX_NUM - 1) - 2);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = index;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_indexbuffer);


	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	ErrorHandler::GetInstance()->LoadTex(_texname, metadata, image);
	//LoadFromWICFile(_texname, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_texture);
	assert(_texture);

	ErrorHandler::GetInstance()->LoadTex(L"asset\\texture\\water_normal.dds", metadata, image);
	//LoadFromDDSFile(L"asset\\texture\\water_normal.dds", DDS_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_normaltexture);
	assert(_normaltexture);

	ErrorHandler::GetInstance()->LoadTex(L"asset\\model\\sky02.jpg", metadata, image);
	//LoadFromWICFile(L"asset\\model\\sky02.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_envtexture);
	assert(_envtexture);


	_param = { 0.0f, 0.0f, 1.0f, 0.0f };
}

void Wave::Uninit() {
	_vertexbuffer->Release();
	_texture->Release();
	_normaltexture->Release();
	_envtexture->Release();
}

void Wave::Update() {
	float dx, dz, length;
	float dt = Time::GetDeltaTime();

	for (int x = 0; x < VERTEX_NUM; x++) {
		for (int z = 0; z < VERTEX_NUM; z++) {
			//振動原点からの距離を求める
			dx = _vertex[z][x].Position.x - (_vertex[0][0].Position.x - 2.5f);
			dz = _vertex[z][x].Position.z - (_vertex[0][0].Position.z + 2.5f);
			length = sqrtf(dx * dx + dz * dz);

			_vertex[z][x].Position.y = WAVE_AMPLITUDE / length * sinf(XM_2PI * (length / WAVE_LENGTH) - (_time / WAVE_CYCLE));
		}
	}
	for (int x = 1; x < VERTEX_NUM - 1; x++) {
		for (int z = 1; z < VERTEX_NUM - 1; z++) {
			//法線ベクトル算出
			XMFLOAT3	vx, vz, vn;
			vx = _vertex[x + 1][z].Position - _vertex[x - 1][z].Position;

			vz = _vertex[x][z - 1].Position - _vertex[x][z + 1].Position;

			//外積
			vn.x = vz.y * vx.z - vz.z * vx.y;
			vn.y = vz.z * vx.x - vz.x * vx.z;
			vn.z = vz.x * vx.y - vz.y * vx.x;

			float len = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
			vn /= len;

			_vertex[x][z].Normal = vn;


			//Tangentの再計算
			XMFLOAT3 edge1 = _vertex[x + 1][z].Position - _vertex[x][z].Position;
			XMFLOAT3 edge2 = _vertex[x][z + 1].Position - _vertex[x][z].Position;

			XMFLOAT2 delta1 = _vertex[x + 1][z].TexCoord - _vertex[x][z].TexCoord;
			XMFLOAT2 delta2 = _vertex[x][z + 1].TexCoord - _vertex[x][z].TexCoord;

			float f = 1.0f / (delta1.x * delta2.y - delta2.x * delta1.y);

			XMFLOAT3 tangent;
			tangent.x = f * (delta2.y * edge1.x - delta1.y * edge2.x);
			tangent.y = f * (delta2.y * edge1.y - delta1.y * edge2.y);
			tangent.z = f * (delta2.y * edge1.z - delta1.y * edge2.z);

			// Tangentを正規化
			len = sqrtf(tangent.x * tangent.x + tangent.y * tangent.y + tangent.z * tangent.z);
			tangent /= len;

			_vertex[x][z].Tangent = tangent;
		}
	}

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(_vertexbuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	memcpy(vertex, _vertex, sizeof(VERTEX_3D) * VERTEX_NUM * VERTEX_NUM);

	Renderer::GetDeviceContext()->Unmap(_vertexbuffer, 0);

	_time += dt;

	_param.x += 0.5f * dt;
	_param.y += 0.5f * dt;
	if (_param.x >= 1.0f) {
		_param.x = 0.0f;
	}
	if (_param.y >= 1.0f) {
		_param.y = 0.0f;
	}

	if (Input::GetKeyTrigger(VK_SPACE)) {
		_param.z *= -1;
	}
}

void Wave::Draw() {
	Shader::SetShader(ShaderName::Water);

	Renderer::SetParameter(_param);

	//ワールドマトリクス設定
	XMMATRIX world, scale, trans, rot;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffer, &stride, &offset);

	//インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(_indexbuffer, DXGI_FORMAT_R32_UINT, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(0.2f, 0.2f, 0.5f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_texture);
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &_normaltexture);
	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &_envtexture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed(((VERTEX_NUM + 1) * 2) * (VERTEX_NUM - 1) - 2, 0, 0);
}