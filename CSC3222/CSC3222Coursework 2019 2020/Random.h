#pragma once

namespace Random {
	float rand01() {
		float i = (rand() % 1000) / 1000.0;
		return i;
	}

	template <typename T>
	T randBetween(T a, T b) {
		float t = rand01();
		return a * t + b * (1 - t);
	}

	int randSign() {
		return rand01() < 0.5 ? 1 : -1;
	}

	float randUniform() {
		return randSign() * rand01();
	}
}

