//CheckTexture.h
//20106_ìcíÜÅ@ò@
//25_02_07
#pragma once
#include "GameObject.h"
#include "ShaderManager.h"
#include <list>
#include <string>

class CheckTexture : public GameObject {
	unsigned short _dispnum = 1;
	std::list<ID3D11ShaderResourceView*> _srv;
	class CheckBox* _checkbox;
	ShaderName _shadername = ShaderName::Unlit;

public:
	CheckTexture(const std::string& label, std::list<ID3D11ShaderResourceView*>& srv, const ShaderName& shadername, const XMFLOAT3& dispsize);
	CheckTexture(const std::string& label, ID3D11ShaderResourceView* srv, const ShaderName& shadername, const XMFLOAT3& dispsize);
	~CheckTexture() {}

	void Init() override {}
	void Uninit() override;
	void Update() override;
	void Draw() override;
};