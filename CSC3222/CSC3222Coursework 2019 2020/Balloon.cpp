#include "Balloon.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

Balloon::Balloon() : SimObject() {
	texture = texManager->GetTexture("TL_Creatures.png");
	owner = nullptr;
	springConstant = 16.0f;
	naturalLength = 20.0f;
	dampingConstant = 4.0;

	CollisionVolume* cv = new Circle(8.0);
	SetCollider(cv);
}

Balloon::~Balloon() {

}

bool Balloon::UpdateObject(float dt) {
	if (owner) {
		Vector2 relativePos = position - owner->GetPosition();
		float deltaX = relativePos.Length() - naturalLength;
		Vector2 distance = relativePos.Normalised() * deltaX;
		force = -distance * springConstant -velocity * dampingConstant;
	}
	return true;
}

void Balloon::DrawObject(GameSimsRenderer& r) {
	Vector2 texPos	= Vector2(73, 99);
	Vector2 texSize = Vector2(14, 28);
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position);
}

void NCL::CSC3222::Balloon::CollisionCallback(SimObject* other, const CollisionRegister& cReg)
{
	if (dynamic_cast<PlayerCharacter*>(other)) {
		if (!owner) {
			owner = (PlayerCharacter*)other;
		}
	}
}
