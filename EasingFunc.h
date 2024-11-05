//EasingFunc.h	�C�[�W���O�֐��B
//20106_�c���@�@
//24_09_14
#pragma once
#include "Main.h"

class Easing {
public:
	static float EaseOutCirc(float x) {
		x = fmax(0, fmin(1, x)); // x��0����1�͈̔͂ɐ���
		return sqrtf(1 - powf(x - 1, 2));
	}
	static float EaseOutBounce(float x) {
		float n1 = 7.5625;
		float d1 = 2.75;

		if (x < 1 / d1) {
			return n1 * x * x;
		}
		else if (x < 2 / d1) {
			return n1 * (x -= 1.5f / d1) * x + 0.75f;
		}
		else if (x < 2.5f / d1) {
			return n1 * (x -= 2.25f / d1) * x + 0.9375f;
		}
		else {
			return n1 * (x -= 2.625f / d1) * x + 0.984375f;
		}
	}
	static float EaseOutExpo(float x) {
		return x == 1 ? 1 : 1 - powf(2, -10 * x);
	}
};