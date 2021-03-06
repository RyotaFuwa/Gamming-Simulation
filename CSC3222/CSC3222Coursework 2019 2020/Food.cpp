#include "Food.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"
#include "Laser.h"

using namespace NCL;
using namespace CSC3222;

Vector4 foodFrames[] = {
	Vector4(0,0,15,16), //Apple
	Vector4(17,0,14,16),//Can
	Vector4(31,0,18,16),//Pizza
	Vector4(49,0,15,16),//Sundae
	Vector4(64,0,16,16) //Bread
};

Food::Food() : SimObject(State::SLEEP) {
	texture = texManager->GetTexture("food_items16x16.png");

	foodItem = rand() % 5;
	eaten = false;

	CollisionVolume* cv = new Circle(8.0);
	cv->SetPos(&position);
	SetCollider(cv);
}

Food::~Food() {
}

bool Food::UpdateObject(float dt) {
	return !eaten;
}

void Food::DrawObject(GameSimsRenderer& r) {
	Vector2 texPos	= Vector2(foodFrames[currentanimFrame].x, foodFrames[currentanimFrame].y);
	Vector2 texSize = Vector2(foodFrames[currentanimFrame].z, foodFrames[currentanimFrame].w);
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position);
}

bool NCL::CSC3222::Food::CollisionCallback(SimObject* other, const CollisionRegister& cReg)
{
	if (dynamic_cast<PlayerCharacter*>(other)) {
		eaten = true;
		return false;
	}
	else if (dynamic_cast<Laser*>(other)) {
		eaten = true;
		return false;
	}
	else {
		return true;
	}
}
