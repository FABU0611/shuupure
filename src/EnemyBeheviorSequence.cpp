//EnemyBeheviorSequence.cpp
//20106_“c’†@˜@
//25_05_16
#include "EnemyBeheviorSequence.h"


BEHEVIOR_RESULT EnemyBeheviorSequence::Update() {
	BEHEVIOR_RESULT result = _child[_index]->Update();

	switch (result) {
	case BEHEVIOR_RESULT::CONTINUE:
		return BEHEVIOR_RESULT::CONTINUE;

	case BEHEVIOR_RESULT::SUCCESS:
		if ((int)_child.size() != _index + 1) {
			_index++;
		}
		//Œ©Ž¸‚Á‚½‚ç
		else {
			_index = 0;
		}
		if ((int)_child.size() < _index) {
			return BEHEVIOR_RESULT::CONTINUE;
		}
		else {
			return BEHEVIOR_RESULT::SUCCESS;
		}

	case BEHEVIOR_RESULT::FAILRE:
		_index = 0;
		return BEHEVIOR_RESULT::FAILRE;
	}
	return BEHEVIOR_RESULT::FAILRE;
}
