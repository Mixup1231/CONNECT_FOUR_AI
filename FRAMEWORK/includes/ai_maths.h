#pragma once
#include <math.h>

template <typename T>
double average(const T& container) {
	if (container.empty()) {
		return 0;
	}

	double result = 0;
	for (auto const& element : container)
		result += element;
	return result / container.size();
}

template <typename T>
double variance(const T& container) {
	if (container.size() < 2) {
		return 0;
	}

	double mean = average(container);
	double result = 0;
	for (auto const& element : container) {
		result += (element - mean) * (element - mean);
	}
	result /= container.size() - 1;
}

template <typename T>
double standardDeviation(const T& container) {
	return sqrt(variance(container));
}

template <typename T>
bool withinRange(T value, T lower, T upper) {
	return (lower <= value && value <= upper);
}