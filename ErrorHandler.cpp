//ErrorHandler.cpp
//20106_田中　蓮
//25_02_12
#include "ErrorHandler.h"
#include "Main.h"
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iomanip>

std::unordered_map<int, std::string> errorMessages;

void LoadErrorMessages() {
	std::ifstream file("ErrorCode.csv");
	if (!file) {
		MessageBoxA(NULL, "エラーメッセージの CSV を開けませんでした。", "エラー", MB_OK | MB_ICONERROR);
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream ss(line);
		std::string codeStr, message;

		if (std::getline(ss, codeStr, ',') && std::getline(ss, message)) {
			int code = std::stoi(codeStr);
			errorMessages[code] = message;
		}
	}

	file.close();
}

std::string GetErrorMessage(int errorCode) {
	if (errorMessages.empty()) {
		LoadErrorMessages();
	}

	if (errorMessages.find(errorCode) != errorMessages.end()) {
		return errorMessages[errorCode];
	}
	return "不明なエラー (" + std::to_string(errorCode) + ")";
}

void DispErrorMessageBox(const short& errorcode) {;
	std::string errorMessage = GetErrorMessage(errorcode);

	char formattedMessage[256];
	snprintf(formattedMessage, sizeof(formattedMessage), "Error Code: %03d\n%s", errorcode, errorMessage.c_str());

	if (MessageBoxA(GetWindow(), formattedMessage, "Error", MB_OK | MB_ICONERROR) == IDOK) {
		DestroyWindow(GetWindow());
	}
}