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

std::string ErrorHandler::GetCSVPathFromConfig(const std::string& configFile) {
	std::string str;
	std::ifstream file(configFile, std::ios::binary);
	if (!file) {
		DispErrorMessageBox(000, "設定ファイルを開けませんでした");
		return "";
	}

	size_t dataSize;
	file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));  // データサイズを読み込む

	if (file.fail() || dataSize == 0) {
		DispErrorMessageBox(002, "設定ファイルの読み込みに失敗しました。");
		return "";
	}

	std::string data(dataSize, '\0');  // 指定サイズの文字列を確保
	file.read(&data[0], dataSize);  // 文字列データを読み込む
	str = data;

	if (file.fail()) {
		DispErrorMessageBox(003, "設定ファイルの内容を正しく読み取れませんでした。");
		return "";
	}
	
	if (data.find("ErrorCSVPath=") == 0) {
		return data.substr(13);     //"ErrorCSVPath="の13文字を除外して返す
	}	

	DispErrorMessageBox(001, "設定ファイルに　'ErrorCSVPath'　の項目がありません。");
	return "";
}

//csvからエラーメッセージを取得
void ErrorHandler::LoadErrorMessages() {
	if (!_errormsgs.empty()) {
		return;
	}
	std::string csvpath = GetCSVPathFromConfig("config.dat");
	std::ifstream file(csvpath);
	if (!file) {
		DispErrorMessageBox(002, "csvファイルを開けませんでした");
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