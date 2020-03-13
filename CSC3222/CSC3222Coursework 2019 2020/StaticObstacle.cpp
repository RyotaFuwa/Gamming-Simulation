#include "StaticObstacle.h"

NCL::CSC3222::StaticObstacle::StaticObstacle(Vector2 pos, CollisionVolume* cv) : SimObject(State::STATIC)
{
		position = pos;
		inverseMass = 0;
		cv->SetPos(&pos);
		SetCollider(cv);
}

NCL::CSC3222::StaticObstacle::~StaticObstacle()
{
}

bool NCL::CSC3222::StaticObstacle::CollisionCallback(SimObject* other, const CollisionRegister& cReg)
{
	if (!dynamic_cast<StaticObstacle*>(other)) {
		return true;
	} else {
		return false;
	}
}

