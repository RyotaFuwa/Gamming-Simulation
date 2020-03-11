#include "GameSimsPhysics.h"
#include "RigidBody.h"
#include "SimObject.h"
#include "CollisionVolume.h"
#include "..//..//Common/Maths.h"

using namespace NCL;
using namespace CSC3222;


GameSimsPhysics::GameSimsPhysics()	{
}

GameSimsPhysics::~GameSimsPhysics()	{

}

void GameSimsPhysics::Update(float dt) {
	IntegrateAcceleration(dt);

	HandleCollision(dt);

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

void GameSimsPhysics::HandleCollision(float dt) {
	for (unsigned int i = 0; i < allColliders.size(); i++) {
		for (int j = i + 1; j < allColliders.size(); j++) {
			if (CollideCheck(i, j)) {
				// logical collision resolution
				SimObject* body1 = (SimObject*)allBodies[i];
				SimObject* body2 = (SimObject*)allBodies[j];
				bool b1 = body1->CollisionCallback(body2, cReg); //TODO Resolve collision by calling callback defined in object class
				bool b2 = body2->CollisionCallback(body1, cReg); //TODO Resolve collision by calling callback defined in object class

				// physical collision resolution
				if (b1 && b2) {
					Projection();
					Impluse();
				}
			}
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
	Vector2 pos1 = allBodies[i]->GetPosition();
	Vector2 pos2 = allBodies[j]->GetPosition();
	Circle* body1 = (Circle*)allColliders[i];
	Circle* body2 = (Circle*)allColliders[j];

	float distance = (pos1 - pos2).Length();
	float sumOfRadius = body1->GetRadius() + body2->GetRadius();
	bool out = distance < sumOfRadius;

	if (out) {
		cReg.bodyIndex1 = i;
		cReg.bodyIndex2 = j;
		cReg.collisionNormal = (pos2 - pos1).Normalised();
		cReg.penetrationDepth = sumOfRadius - distance;
	}
	return out;
}

bool NCL::CSC3222::GameSimsPhysics::CircleAABBCollision(int i, int j) //TODO define it
{
	Vector2 pos1 = allBodies[i]->GetPosition();
	Vector2 pos2 = allBodies[j]->GetPosition();
	Circle* body1 = (Circle*)allColliders[i];
	AABB* body2 = (AABB*)allColliders[j];
	
	Vector2 halfsize = body2->GetHalfSize();
	Vector2 nearestPoint = Clamp(pos1, pos2 - halfsize, pos2 + halfsize);
	float distance = (pos1 - nearestPoint).Length();
	bool out = distance < body1->GetRadius();
	if (out) {
		cReg.bodyIndex1 = i;
		cReg.bodyIndex2 = j;
		cReg.collisionNormal = (nearestPoint - pos1).Normalised();
		cReg.penetrationDepth = body1->GetRadius() - distance;
	}
	return out;
}

bool NCL::CSC3222::GameSimsPhysics::AABBAABBCollision(int i, int j) //TODO define it
{
	Vector2 pos1 = allBodies[i]->GetPosition();
	Vector2 pos2 = allBodies[j]->GetPosition();
	AABB* body1 = (AABB*)allColliders[i];
	AABB* body2 = (AABB*)allColliders[j];

	Vector2 halfsize1 = body1->GetHalfSize();
	Vector2 halfsize2 = body2->GetHalfSize();
	float depthX = (halfsize1.x + halfsize2.x) - abs(pos1.x - pos2.x);
	float depthY = (halfsize1.y + halfsize2.y) - abs(pos1.y - pos2.y);
	bool checkX = depthX > 0;
	bool checkY = depthY > 0;
	bool out = checkX && checkY;
	if (out) {
		cReg.bodyIndex1 = i;
		cReg.bodyIndex2 = j;
		cReg.collisionNormal = depthX < depthY ? Vector2(1, 0) : Vector2(0, 1);
		cReg.penetrationDepth = depthX < depthY ? depthX : depthY;
	}
	return out;
}

void NCL::CSC3222::GameSimsPhysics::Projection()
{
	RigidBody* body1 = allBodies[cReg.bodyIndex1];
	RigidBody* body2 = allBodies[cReg.bodyIndex2];

	float inverseMassTotal = body1->inverseMass + body2->inverseMass;
	body1->position -= cReg.collisionNormal * cReg.penetrationDepth * body1->inverseMass / inverseMassTotal;
	body2->position += cReg.collisionNormal * cReg.penetrationDepth * body2->inverseMass / inverseMassTotal;
}

void NCL::CSC3222::GameSimsPhysics::Impluse()
{
	RigidBody* body1 = allBodies[cReg.bodyIndex1];
	RigidBody* body2 = allBodies[cReg.bodyIndex2];
	float inverseMassTotal = body1->inverseMass + body2->inverseMass;
	Vector2 relativeVec = body2->velocity - body1->velocity;
	float J = Vector2::Dot(relativeVec, cReg.collisionNormal) / inverseMassTotal;
	body1->velocity -= cReg.collisionNormal * -(1 + body1->elasticity) * J * body1->inverseMass;
	body2->velocity += cReg.collisionNormal * -(1 + body2->elasticity) * J * body2->inverseMass;
}

