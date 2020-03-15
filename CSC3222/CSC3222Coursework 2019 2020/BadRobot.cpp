#include "BadRobot.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"
#include "Laser.h"

using namespace NCL;
using namespace CSC3222;

Vector4 animFramesGreen[] = {
	Vector4(71,142,18,18),
	Vector4(102,142,20,18),
	Vector4(135,142,19,18),
	Vector4(166,142,19,18),
	Vector4(198,140,20,18)
};

Vector4 animFramesWhite[] = {
	Vector4(71,266,17,22),
	Vector4(104,265,19,22),
	Vector4(137,266,17,22),
	Vector4(168,266,17,22),
	Vector4(200,264,17,23)
};

BadRobot::BadRobot() : SimObject(State::ACTIVE) {
	texture = texManager->GetTexture("TL_Creatures.png");
	animFrameCount	= 4;

	int r = rand() % 2;

	if (r == 0) {
		type = RobotType::Green;
		stamina = 30;
	}
	else {
		type = RobotType::White;
		stamina = 90;
	}

	CollisionVolume* cv = new Circle(8.0);
	cv->SetPos(&position);
	SetCollider(cv);

}

BadRobot::~BadRobot() {
}

bool BadRobot::UpdateObject(float dt) {
	UpdateAnimFrame(dt);
	return true;
}

void BadRobot::DrawObject(GameSimsRenderer& r) {
	Vector2	texPos;
	Vector2 texSize;

	switch (type) {
		case RobotType::Green:{
			texPos	= Vector2(animFramesGreen[currentanimFrame].x, animFramesGreen[currentanimFrame].y);
			texSize = Vector2(animFramesGreen[currentanimFrame].z, animFramesGreen[currentanimFrame].w);
		}break;
		case RobotType::White:{
			texPos	= Vector2(animFramesWhite[currentanimFrame].x, animFramesWhite[currentanimFrame].y);
			texSize = Vector2(animFramesWhite[currentanimFrame].z, animFramesWhite[currentanimFrame].w);
		}break;
	}

	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position, false);
}

bool NCL::CSC3222::BadRobot::CollisionCallback(SimObject* other, const CollisionRegister& cReg)
{
	if (dynamic_cast<Laser*>(other)) {
		Laser* laser = (Laser*)other;
		stamina -= laser->GetAttackPower();
	}

	if (stamina > 0)
		return true;
	else
		return false;
}
