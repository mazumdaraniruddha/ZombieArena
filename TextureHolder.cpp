#include "stdafx.h"
#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::mInstance = nullptr;

TextureHolder::TextureHolder() {
	assert(mInstance == nullptr);
	mInstance = this;
}

Texture& TextureHolder::GetTexture(string const& filename) {
	auto& m = mInstance->mTextures;
	auto keyValue = m.find(filename);

	if (keyValue != m.end()) {
		// Texture key found
		return keyValue->second;
	}
	else {
		// Texture key not found in map
		auto& texture = m[filename];
		texture.loadFromFile(filename);
		return texture;
	}
}