#pragma once
#include <glm/vec2.hpp>

namespace util {
	typedef long long LL;

	LL Factorial(int x) {
		LL ans = 1;
		for (int i = 1; i <= x; ++i) {
			ans *= static_cast<LL>(i);
		}
		return ans;
	}

	// for combination ($C_n^k$)
	LL C(int n, int m) {
		return (Factorial(n)) / (Factorial(m) * Factorial(n - m));
	}

	glm::vec2 Interpolate(glm::vec2 from, glm::vec2 to, float t)
	{
		return from * t + to * (1 - t);
	}
}