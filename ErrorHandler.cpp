//ErrorHandler.cpp
//20106_田中　蓮
//25_02_12
#include "ErrorHandler.h"
#include "TextureManager.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iomanip>
#include <comdef.h>


ErrorHandler* ErrorHandler::_instance = nullptr;

//csvからエラーメッセージを取得
void ErrorHandler::LoadErrorMessages() {
	if (!_errormsgs.empty()) {
		return;
	}
	std::ifstream file("asset\\ErrorCode.csv");
	if (!file) {
		DispErrorMessageBox(000, "csvファイルを開けませんでした");
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream ss(line);
		std::string codeStr, message;

		if (std::getline(ss, codeStr, ',') && std::getline(ss, message)) {
			int code = std::stoi(codeStr);
			_errormsgs[code] = message;
		}
	}

	file.close();
}

//エラーコードからcsvのエラーメッセージを持ってくる
std::string ErrorHandler::GetErrorMessage(const short& errorCode) {
	if (_errormsgs.empty()) {
		LoadErrorMessages();
	}

	if (_errormsgs.find(errorCode) != _errormsgs.end()) {
		return _errormsgs[errorCode];
	}
	return "不明なエラー (" + std::to_string(errorCode) + ")";
}

//HRESULTとcsvからエラーメッセージ表示
void ErrorHandler::DispErrorMessageBox(const short& errorcode, const HRESULT& hr) {
	if (FAILED(hr)) {
		//エラーコードを文字列に変換
		_com_error err(hr);
		std::string errorMessage = GetErrorMessage(errorcode);

		errorMessage += err.ErrorMessage();

		char formattedMessage[256];
		snprintf(formattedMessage, sizeof(formattedMessage), "Error Code: %03d\n%s", errorcode, errorMessage.c_str());

		if (MessageBoxA(GetWindow(), formattedMessage, "Error", MB_OK | MB_ICONERROR) == IDOK) {
			DestroyWindow(GetWindow());
		}
	}
}

//任意の文字列でエラーメッセージ表示
void ErrorHandler::DispErrorMessageBox(const short& errorcode, const std::string str) {
	char formattedMessage[256];
	snprintf(formattedMessage, sizeof(formattedMessage), "Error Code: %03d\n%s", errorcode, str.c_str());

	if (MessageBoxA(GetWindow(), formattedMessage, "Error", MB_OK | MB_ICONERROR) == IDOK) {
		DestroyWindow(GetWindow());
	}
}

//csvの文字列でエラーメッセージ表示
void ErrorHandler::DispErrorMessageBox(const short& errorcode) {
	std::string errorMessage = GetErrorMessage(errorcode);
	char formattedMessage[256];
	snprintf(formattedMessage, sizeof(formattedMessage), "Error Code: %03d\n%s", errorcode, errorMessage.c_str());

	if (MessageBoxA(GetWindow(), formattedMessage, "Error", MB_OK | MB_ICONERROR) == IDOK) {
		DestroyWindow(GetWindow());
	}
}