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

//csv����G���[���b�Z�[�W���擾
void ErrorHandler::LoadErrorMessages() {
	if (!_errormsgs.empty()) {
		return;
	}
	std::ifstream file("asset\\ErrorCode.csv");
	if (!file) {
		DispErrorMessageBox(000, "csv�t�@�C�����J���܂���ł���");
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

//�C�ӂ̕�����ŃG���[���b�Z�[�W�\��
void ErrorHandler::DispErrorMessageBox(const short& errorcode, const std::string str) {
	char formattedMessage[256];
	snprintf(formattedMessage, sizeof(formattedMessage), "Error Code: %03d\n%s", errorcode, str.c_str());

	if (MessageBoxA(GetWindow(), formattedMessage, "Error", MB_OK | MB_ICONERROR) == IDOK) {
		DestroyWindow(GetWindow());
	}
}


void ErrorHandler::LoadTex(const std::wstring& filename, TexMetadata& metadata, ScratchImage& image) {
	std::wstring file(filename);
	size_t pos = file.rfind(L'.');

	std::wstring extension = file.substr(pos);
	if (extension == L".dds") {
		HRESULT hr = LoadFromDDSFile(filename.c_str(), DDS_FLAGS_NONE, &metadata, image);
		if (FAILED(hr)) {
			DispErrorMessageBox(101, hr);
		}
		return;
	}

	HRESULT hr = LoadFromWICFile(filename.c_str(), WIC_FLAGS_NONE, &metadata, image);
	if (FAILED(hr)) {
		DispErrorMessageBox(100, hr);
	}
}
