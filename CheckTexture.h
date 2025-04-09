//CheckTexture.h
//20106_ìcíÜÅ@ò@
//25_02_07
#pragma once
#include "GameObject.h"
#include "ShaderManager.h"
#include <vector>
#include <string>

class CheckTexture : public GameObject {
	unsigned short _dispnum = 1;
	std::vector<ID3D11ShaderResourceView*> _srv;
	short _index = 0;
	ShaderName _shadername = ShaderName::Unlit;

public:
	CheckTexture(const std::string& label, const unsigned short& dispnum, std::vector<ID3D11ShaderResourceView*> srv, const ShaderName& shadername);
	CheckTexture(const std::string& label, ID3D11ShaderResourceView* srv, const ShaderName& shadername);
	~CheckTexture() {}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};