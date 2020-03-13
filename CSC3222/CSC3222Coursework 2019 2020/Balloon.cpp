#include "Balloon.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"
#include "Laser.h"

using namespace NCL;
using namespace CSC3222;

Balloon::Balloon() : SimObject(State::SLEEP) {
	texture = texManager->GetTexture("TL_Creatures.png");
	owner = nullptr;
	cracked = false;
	springConstant = 32.0f;
	naturalLength = 20.0f;
	dampingConstant = 4.0;

	inverseMass = 2;

	CollisionVolume* cv = new Circle(8.0);
	cv->SetPos(&position);
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
	return !cracked;
}

void Balloon::DrawObject(GameSimsRenderer& r) {
	Vector2 texPos	= Vector2(73, 99);
	Vector2 texSize = Vector2(14, 28);
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position);
}

bool NCL::CSC3222::Balloon::CollisionCallback(SimObject* other, const CollisionRegister& cReg)
{
	if (dynamic_cast<PlayerCharacter*>(other)) {
		if (!owner) {
			owner = (PlayerCharacter*)other;
		}
	}
	else if (dynamic_cast<Laser*>(other)) {
		Laser* l = (Laser*)other;
		if (!owner) {
			cracked = true;
			return false;
		}
		else if (owner->GetPlayerId() != l->GetPlayerId()) {
			cracked = true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}
