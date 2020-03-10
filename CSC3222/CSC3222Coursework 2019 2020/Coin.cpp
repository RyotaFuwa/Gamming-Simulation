#include "Coin.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"
#include "PlayerCharacter.h"


using namespace NCL;
using namespace CSC3222;

Vector2 animFrames[] = {
	Vector2(96 , 80),
	Vector2(112, 80),
	Vector2(128, 80),
	Vector2(144, 80),
};

Coin::Coin() : SimObject() {
	animFrameCount = 4;
	texture = texManager->GetTexture("Items.png");
	taken = false;

	CollisionVolume* cv = new Circle(8.0);
	SetCollider(cv);
}

Coin::~Coin() {

}

bool Coin::UpdateObject(float dt) {
	UpdateAnimFrame(dt);

	return true;
}

void Coin::DrawObject(GameSimsRenderer& r) {
	Vector2 texPos	 = animFrames[currentanimFrame];
	Vector2 texSize	 = Vector2(16,16);
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position);
}

void NCL::CSC3222::Coin::CollisionCallback(SimObject* other, const CollisionRegister& cReg)
{
	if (dynamic_cast<PlayerCharacter*>(other)) {
		std::cout << "Taken By Player !! AT (";
		std::cout << GetPosition().x << ", " << GetPosition().y << ")" << std::endl; // prottype
	}
}
