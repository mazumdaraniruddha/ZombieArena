#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class TextureHolder {
private:
	// Map of Texture String keys and Texture objects
	map<string, Texture> mTextures;
	// Singleton instance pointer
	static TextureHolder* mInstance;
public:
	TextureHolder();
	static Texture& GetTexture(string const& filename);
};
#endif