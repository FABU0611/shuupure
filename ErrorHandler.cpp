//ErrorHandler.cpp
//20106_�c���@�@
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
		DispErrorMessageBox(000, "�ݒ�t�@�C�����J���܂���ł���");
		return "";
	}

	size_t dataSize;
	file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));  // �f�[�^�T�C�Y��ǂݍ���

	if (file.fail() || dataSize == 0) {
		DispErrorMessageBox(002, "�ݒ�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B");
		return "";
	}

	std::string data(dataSize, '\0');  // �w��T�C�Y�̕�������m��
	file.read(&data[0], dataSize);  // ������f�[�^��ǂݍ���
	str = data;

	if (file.fail()) {
		DispErrorMessageBox(003, "�ݒ�t�@�C���̓��e�𐳂����ǂݎ��܂���ł����B");
		return "";
	}
	
	if (data.find("ErrorCSVPath=") == 0) {
		return data.substr(13);     //"ErrorCSVPath="��13���������O���ĕԂ�
	}	

	DispErrorMessageBox(001, "�ݒ�t�@�C���Ɂ@'ErrorCSVPath'�@�̍��ڂ�����܂���B");
	return "";
}

//csv����G���[���b�Z�[�W���擾
void ErrorHandler::LoadErrorMessages() {
	if (!_errormsgs.empty()) {
		return;
	}
	std::string csvpath = GetCSVPathFromConfig("config.dat");
	std::ifstream file(csvpath);
	if (!file) {
		DispErrorMessageBox(002, "csv�t�@�C�����J���܂���ł���");
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

//�G���[�R�[�h����csv�̃G���[���b�Z�[�W�������Ă���
std::string ErrorHandler::GetErrorMessage(const short& errorCode) {
	if (_errormsgs.empty()) {
		LoadErrorMessages();
	}

	if (_errormsgs.find(errorCode) != _errormsgs.end()) {
		return _errormsgs[errorCode];
	}
	return "�s���ȃG���[ (" + std::to_string(errorCode) + ")";
}

//HRESULT��csv����G���[���b�Z�[�W�\��
void ErrorHandler::DispErrorMessageBox(const short& errorcode, const HRESULT& hr) {
	if (FAILED(hr)) {
		//�G���[�R�[�h�𕶎���ɕϊ�
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

//�C�ӂ̕�����ŃG���[���b�Z�[�W�\��
void ErrorHandler::DispErrorMessageBox(const short& errorcode, const std::string str) {
	char formattedMessage[256];
	snprintf(formattedMessage, sizeof(formattedMessage), "Error Code: %03d\n%s", errorcode, str.c_str());

	if (MessageBoxA(GetWindow(), formattedMessage, "Error", MB_OK | MB_ICONERROR) == IDOK) {
		DestroyWindow(GetWindow());
	}
}

//csv�̕�����ŃG���[���b�Z�[�W�\��
void ErrorHandler::DispErrorMessageBox(const short& errorcode) {
	std::string errorMessage = GetErrorMessage(errorcode);
	char formattedMessage[256];
	snprintf(formattedMessage, sizeof(formattedMessage), "Error Code: %03d\n%s", errorcode, errorMessage.c_str());

	if (MessageBoxA(GetWindow(), formattedMessage, "Error", MB_OK | MB_ICONERROR) == IDOK) {
		DestroyWindow(GetWindow());
	}
}