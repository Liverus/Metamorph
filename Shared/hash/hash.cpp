#pragma once

size_t simple_hash(char* value, int length) {
	size_t hash = 0;
	const size_t prime = 31;

	for (size_t i = 0; i < length; ++i) {
		hash = value[i] + (hash * prime);
	}

	return hash;
}