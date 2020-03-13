#include "Coin.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"
#include "PlayerCharacter.h"
#include "Laser.h"


using namespace NCL;
using namespace CSC3222;

Vector2 animFrames[] = {
	Vector2(96 , 80),
	Vector2(112, 80),
	Vector2(128, 80),
	Vector2(144, 80),
};

Coin::Coin() : SimObject(State::SLEEP) {
	animFrameCount = 4;
	texture = texManager->GetTexture("Items.png");
	taken = false;
	elasticity = 1.0;

	CollisionVolume* cv = new Circle(8.0);
	cv->SetPos(&position);
	SetCollider(cv);
}

Coin::~Coin() {
}

bool Coin::UpdateObject(float dt) {
	UpdateAnimFrame(dt);
	return !taken;
}

void Coin::DrawObject(GameSimsRenderer& r) {
	Vector2 texPos	 = animFrames[currentanimFrame];
	Vector2 texSize	 = Vector2(16,16);
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position);
}

bool NCL::CSC3222::Coin::CollisionCallback(SimObject* other, const CollisionRegister& cReg)
{
	if (dynamic_cast<PlayerCharacter*>(other)) {
		taken = true;
		return false;
	}
	else if (dynamic_cast<Laser*>(other)) {
		return true;
	}
	else {
		return true;
	}
}
