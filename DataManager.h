//DataManager.h
//20106_ìcíÜÅ@ò@
//24_09_04
#pragma once

#include <fstream>
#include <string>

class DataManager {

public:
	static void SaveData(int rot, int twist, float dot) {
		std::ofstream file("rotation_data.dat", std::ios::binary);
		if (file.is_open()) {
			file.write(reinterpret_cast<char*>(&rot), sizeof(rot));
			file.write(reinterpret_cast<char*>(&twist), sizeof(twist));
			file.write(reinterpret_cast<char*>(&dot), sizeof(dot));
			file.close();
		}
	}

	static void LoadData(int& rot, int& twist, float& dot) {
		std::ifstream file("rotation_data.dat", std::ios::binary);
		if (file.is_open()) {
			file.read(reinterpret_cast<char*>(&rot), sizeof(rot));
			file.read(reinterpret_cast<char*>(&twist), sizeof(twist));
			file.read(reinterpret_cast<char*>(&dot), sizeof(dot));
			file.close();
		}
	}
	static void SaveScore(float score) {
		std::ofstream file("score_data.dat", std::ios::binary);
		if (file.is_open()) {
			file.write(reinterpret_cast<char*>(&score), sizeof(score));
			file.close();
		}
	}

	static void LoadScore(float& score) {
		std::ifstream file("score_data.dat", std::ios::binary);
		if (file.is_open()) {
			file.read(reinterpret_cast<char*>(&score), sizeof(score));
			file.close();
		}
	}
};