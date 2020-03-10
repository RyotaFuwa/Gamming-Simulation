#include "StaticObstacle.h"

void NCL::CSC3222::StaticObstacle::CollisionCallback(SimObject* other, const CollisionRegister& cReg)
{
	if (!dynamic_cast<StaticObstacle*>(other)) {
		std::cout << "Hit Wall !! AT (";
		std::cout << GetPosition().x << ", " << GetPosition().y << ")" << std::endl; // prottype
	}
}
