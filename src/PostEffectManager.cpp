//PostEffectManager.cpp
//20106_田中　蓮
//25_05_28
#include "PostEffectManager.h"
#include "MotionBlur.h"
#include "Gaussian.h"

PostEffectManager* PostEffectManager::_instance = nullptr;

PostEffectManager* PostEffectManager::GetInstance() {
	if (!_instance) {
		_instance = new PostEffectManager();
	}
	return _instance;
}

void PostEffectManager::DeleteInstance() {
	if (!_instance) {
		return;
	}
	_instance->Uninit();
	for (auto effect : _instance->_postEffects) {
		if (!effect) {
			continue;
		}
		effect->Uninit();
		delete effect;
		effect = nullptr;
	}
	_instance->_postEffects.clear();

	delete _instance;
	_instance = nullptr;
}

void PostEffectManager::Init() {
	// 必要なポストエフェクトを追加
	AddPostEffect(new MotionBlur());
	_gaussian = new Gaussian();
	AddPostEffect(_gaussian);

	// 各ポストエフェクトの初期化
	for (auto effect : _postEffects) {
		if (!effect) {
			continue;
		}
		effect->Init();
	}
}

void PostEffectManager::Uninit() {
	for (auto effect : _postEffects) {
		if (!effect) {
			continue;
		}
		effect->Uninit();
		delete effect;
		effect = nullptr;
	}
	_postEffects.clear();
}

void PostEffectManager::Update() {
	for (auto effect : _postEffects) {
		if (!effect) {
			continue;
		}
		effect->Update();
	}
}

void PostEffectManager::Draw(ID3D11ShaderResourceView* initialSRV) {
	ID3D11ShaderResourceView* inputSRV = initialSRV;
	for (auto effect : _postEffects) {
		if (!effect) {
			continue;
		}
		effect->Draw(inputSRV);
		inputSRV = effect->GetWriteSRV();
	}
}

Gaussian* PostEffectManager::GetGaussian() {
	return _gaussian;
}

void PostEffectManager::SetSlider() {
	_gaussian->SetSlider();
}
