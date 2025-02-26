//ErrorHandler.h
//20106_ìcíÜÅ@ò@
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
	std::unordered_map<unsigned short, std::string> _errormsgs;

	std::string GetCSVPathFromConfig(const std::string& configFile);
	void LoadErrorMessages();
	std::string GetErrorMessage(const unsigned short& errorCode);

	static void DeleteInstance() {
		if (!_instance) {
			return;
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
	void DispErrorMessageBox(const std::string& errorcode, const HRESULT& hr);
	void DispErrorMessageBox(const std::string& errorcode, const std::string str);
	void DispErrorMessageBox(const std::string& errorcode);
};