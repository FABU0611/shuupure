//ErrorHandler.h
//20106_�c���@�@
//25_02_12
#pragma once
#include <Windows.h>
#include <unordered_map>
#include <string>
#include "Main.h"

class ErrorHandler {
	ErrorHandler() = default;
	~ErrorHandler() = default;
	ErrorHandler(const ErrorHandler&) = delete;
	ErrorHandler& operator=(const ErrorHandler&) = delete;

	static ErrorHandler* _instance;
	std::unordered_map<int, std::string> _errormsgs;

	void LoadErrorMessages();
	std::string GetErrorMessage(const short& errorCode);
	void DispErrorMessageBox(const short& errorcode, const HRESULT& hr);
	void DispErrorMessageBox(const short& errorcode, const std::string str);

	static void DeleteInstance() {
		if (!_instance) {
		}
		_instance->_errormsgs.clear();
		delete _instance;
		_instance = nullptr;
	}
	friend class Manager;

public:
	static ErrorHandler* GetInstance() {
		if (!_instance) {
			_instance = new ErrorHandler();
			_instance->LoadErrorMessages();
		}
		return _instance;
	}
	void LoadTex(const std::wstring& filename, TexMetadata& metadata, ScratchImage& image);
};