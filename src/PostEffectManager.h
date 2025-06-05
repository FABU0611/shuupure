//PostEffectManager.h
//20106_ìcíÜÅ@ò@
//25_05_28
#pragma once
#include <list>
#include "PostEffectBase.h"

class PostEffectManager {
	PostEffectManager() = default;
	~PostEffectManager() = default;
	PostEffectManager(const PostEffectManager&) = delete;
	PostEffectManager& operator=(const PostEffectManager&) = delete;
	PostEffectManager(PostEffectManager&&) = delete;
	PostEffectManager& operator=(PostEffectManager&&) = delete;

	static PostEffectManager*	_instance;
	std::list<PostEffectBase*>	_postEffects;
	class Gaussian* _gaussian = nullptr;

public:
	static PostEffectManager* GetInstance();
	static void DeleteInstance();

	void AddPostEffect(PostEffectBase* effect) {
		_postEffects.push_back(effect);
	}
	void InsertPostEffect(const int& index, PostEffectBase* effect) {
		auto it = _postEffects.begin();
		std::advance(it, index);
		if (it == _postEffects.end()) {
			_postEffects.push_back(effect);
			return;
		}
		_postEffects.insert(it, effect);
	}

	void RemovePostEffect(const int& index) {
		auto it = _postEffects.begin();
		std::advance(it, index);
		if (it != _postEffects.end()) {
			delete* it;
			_postEffects.erase(it);
		}
	}

	void Init();
	void Uninit();
	void Update();
	void Draw(ID3D11ShaderResourceView* initialSRV);

	Gaussian* GetGaussian();
	void SetSlider();
};