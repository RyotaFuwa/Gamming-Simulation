#include "GameSimsPhysics.h"
#include "RigidBody.h"
#include "CollisionVolume.h"
#include "../../Common/Vector2.h"

using namespace NCL;
using namespace CSC3222;

GameSimsPhysics::GameSimsPhysics()	{

}

GameSimsPhysics::~GameSimsPhysics()	{

}

void GameSimsPhysics::Update(float dt) {
	IntegrateAcceleration(dt);

	if (CollisionDetection(dt))
		std::cout << "Collision Detected !!" << std::endl; // For Prottype. CollisionResolution Installed in CW2;

	IntegrateVelocity(dt);
}

void GameSimsPhysics::AddRigidBody(RigidBody* b) {
	allBodies.emplace_back(b);
}

void GameSimsPhysics::RemoveRigidBody(RigidBody* b) {
	auto at = std::find(allBodies.begin(), allBodies.end(), b);
	if (at != allBodies.end()) {
		//maybe delete too?
		allBodies.erase(at);
	}
}

void GameSimsPhysics::AddCollider(CollisionVolume* c) {
	allColliders.emplace_back(c);
}

void GameSimsPhysics::RemoveCollider(CollisionVolume* c) {
	auto at = std::find(allColliders.begin(), allColliders.end(), c);
	if (at != allColliders.end()) {
		//maybe delete too?
		allColliders.erase(at);
	}
}


void NCL::CSC3222::GameSimsPhysics::IntegrateAcceleration(float dt)
{
	for (auto each : allBodies) { // acceleration(use appropriate numerical approximation for v = a * dt)
		Vector2 acceleration = each->force * each->inverseMass;
		each->velocity += acceleration * dt;
	}
}

void NCL::CSC3222::GameSimsPhysics::IntegrateVelocity(float dt)
{
	for (auto each : allBodies) { // change position by integrating the velocity
		each->position += each->velocity * dt;
		each->velocity *= 0.999f; // bit inaccurate for air resistance
	}
}

bool GameSimsPhysics::CollisionDetection(float dt) {
	for (int i = 0; i < allColliders.size(); i++) {
		for (int j = i + 1; j < allColliders.size(); j++) {
			return CollideCheck(i, j);
		}
	}
}

bool NCL::CSC3222::GameSimsPhysics::CollideCheck(int i, int j)
{
	if (dynamic_cast<Circle*>(allColliders[i])) {
		if (dynamic_cast<Circle*>(allColliders[j]))
			return CircleCircleCollision(i, j);
		else if (dynamic_cast<AABB*>(allColliders[j]))
			return CircleAABBCollision(i, j);
		else
			return false; //TODO raise error for no collision detection defined for this shape 
	} else if (dynamic_cast<AABB*>(allColliders[i])) 
	    if (dynamic_cast<Circle*>(allColliders[j])) 
			return CircleAABBCollision(j, i);
		else if (dynamic_cast<AABB*>(allColliders[j]))
			return CircleAABBCollision(i, j);
		else
			return false; //TODO raise error for no collision detection defined for this shape 

	else {
		return false; //TODO raise error for no collision detection defined for this shape 
	}
}

bool NCL::CSC3222::GameSimsPhysics::CircleCircleCollision(int i, int j)
{
	return false;
}

bool NCL::CSC3222::GameSimsPhysics::CircleAABBCollision(int i, int j)
{
	return false;
}

bool NCL::CSC3222::GameSimsPhysics::AABBAABBCollision(int i, int j)
{
	return false;
}

