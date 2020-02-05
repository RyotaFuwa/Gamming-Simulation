#include "Balloon.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

using namespace NCL;
using namespace CSC3222;

Balloon::Balloon() : SimObject() {
	texture = texManager->GetTexture("TL_Creatures.png");
}

Balloon::~Balloon() {

}

bool Balloon::UpdateObject(float dt) {
	return true;
}

void Balloon::DrawObject(GameSimsRenderer& r) {
	Vector2 texPos	= Vector2(73, 99);
	Vector2 texSize = Vector2(14, 28);
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position);
}