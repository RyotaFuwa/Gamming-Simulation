#include "StaticObstacle.h"

void NCL::CSC3222::StaticObstacle::CollisionCallback(SimObject* other, const CollisionRegister& cReg)
{
	if (!(StaticObstacle*)other) {
		std::cout << "Collision Detected !! AT (";
		std::cout << GetPosition().x << ", " << GetPosition().y << ")" << std::endl; // prottype
	}
}
