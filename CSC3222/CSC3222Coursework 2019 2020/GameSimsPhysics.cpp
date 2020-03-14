#include "GameSimsPhysics.h"
#include "..//..//Common/Maths.h"

using namespace NCL;
using namespace CSC3222;


GameSimsPhysics::GameSimsPhysics()	{
}

GameSimsPhysics::~GameSimsPhysics()	{
}

void NCL::CSC3222::GameSimsPhysics::InitialiseQuadTree(const Vector2 &mapCenter, const Vector2 &mapHalfSize)
{
	root = new QuadTreeNode(mapCenter, mapHalfSize, 2, 5, allStaticBodies);
}

void GameSimsPhysics::Update(float dt) {
	IntegrateAcceleration(dt);

	HandleCollision(dt);

	IntegrateVelocity(dt);
}

void GameSimsPhysics::AddRigidBody(RigidBody* b) {
	if(b->state == State::STATIC)
		allStaticBodies.emplace_back(b);
	else
		allDynamicBodies.emplace_back(b);
}

void GameSimsPhysics::RemoveRigidBody(RigidBody* b) {
	vector<RigidBody*> &vec = b->state == State::STATIC ? allStaticBodies : allDynamicBodies;
	auto at = std::find(vec.begin(), vec.end(), b);
	if (at != vec.end()) {
		vec.erase(at);
	}
}

void NCL::CSC3222::GameSimsPhysics::IntegrateAcceleration(float dt)
{
	for (auto each : allDynamicBodies) { // acceleration(use appropriate numerical approximation for v = a * dt)
		Vector2 acceleration = each->force * each->inverseMass;
		each->velocity += acceleration * dt;
	}
}

void NCL::CSC3222::GameSimsPhysics::IntegrateVelocity(float dt)
{
	for (auto each : allDynamicBodies) { // change position by integrating the velocity
		each->position += each->velocity * dt;
		each->velocity *= 0.999f; // bit inaccurate for air resistance
		
	}
}

void GameSimsPhysics::HandleCollision(float dt) {
	// broad phase
	// vector<BodyPairs> bpairs = BroadPhase();
	vector<BodyPairs> bpairs;
	for (auto sbody : allStaticBodies) {
		for (auto dbody : allDynamicBodies) {
			BodyPairs bp;
			bp.body1 = sbody;
			bp.body2 = dbody;
			bpairs.push_back(bp);
		}
	}

	for (int i = 0; i != allDynamicBodies.size(); i++) {
		for (int j = i + 1; j != allDynamicBodies.size(); j++) {
			BodyPairs bp;
			bp.body1 = allDynamicBodies[i];
			bp.body2 = allDynamicBodies[j];
			bpairs.push_back(bp);
		}
	}

	// narrow phase (direct checking)
	for (auto pair : bpairs) {
		if (NarrowPhase(pair.body1->GetCollider(), pair.body2->GetCollider())) {
			if (CollisionResolution((SimObject*)(pair.body1), (SimObject*)(pair.body2))) {
				Projection(pair.body1, pair.body2);
				Impluse(pair.body1, pair.body2);
			}
		}
	}
}

std::vector<BodyPairs> GameSimsPhysics::BroadPhase()
{
	root->Refresh();
	for (RigidBody* body : allDynamicBodies) {
		root->Insert(body);
	}
	return root->RetriveCollisionPairs();
}

bool NCL::CSC3222::GameSimsPhysics::NarrowPhase(CollisionVolume* body1, CollisionVolume* body2)
{
	if (dynamic_cast<Circle*>(body1)) {
		if (dynamic_cast<Circle*>(body2))
			return CircleCircleCollision((Circle*)body1, (Circle*)body2);
		else if (dynamic_cast<AABB*>(body2))
			return CircleAABBCollision((Circle*)body1, (AABB*)body2);
		else
			return false; 
	}
	else if (dynamic_cast<AABB*>(body1)) {
		if (dynamic_cast<Circle*>(body2))
			return ABBCircleCollision((AABB*)body1, (Circle*)body2);
		else if (dynamic_cast<AABB*>(body2))
			return AABBAABBCollision((AABB*)body1, (AABB*)body2);
		else
			return false;
	}
	else {
		return false; 
	}
}

bool NCL::CSC3222::GameSimsPhysics::CircleCircleCollision(Circle* circle1, Circle* circle2)
{
	Vector2 pos1 = circle1->GetPos();
	Vector2 pos2 = circle2->GetPos();
	float distance = (pos1 - pos2).Length();
	float sumOfRadius = circle1->GetRadius() + circle2->GetRadius();
	bool out = distance < sumOfRadius;

	if (out) {
		cReg.collisionNormal = (pos2 - pos1).Normalised();
		cReg.penetrationDepth = sumOfRadius - distance;
	}
	return out;
}

bool NCL::CSC3222::GameSimsPhysics::CircleAABBCollision(Circle* circle, AABB* aabb) //TODO define swap version
{
	Vector2 pos1 = circle->GetPos();
	Vector2 pos2 = aabb->GetPos();
	Vector2 halfsize = aabb->GetHalfSize();
	Vector2 nearestPoint = Clamp(pos1, pos2 - halfsize, pos2 + halfsize);
	float distance = (pos1 - nearestPoint).Length();
	bool out = distance < circle->GetRadius();
	if (out) {
		cReg.collisionNormal = (nearestPoint - pos1).Normalised();
		cReg.penetrationDepth = circle->GetRadius() - distance;
	}
	return out;
}

bool NCL::CSC3222::GameSimsPhysics::ABBCircleCollision(AABB* aabb, Circle* circle)
{
	bool out = CircleAABBCollision(circle, aabb);
	if (out) {
		cReg.collisionNormal = -cReg.collisionNormal;
	}
	return out;
}

bool NCL::CSC3222::GameSimsPhysics::AABBAABBCollision(AABB* aabb1, AABB* aabb2) 
{
	Vector2 pos1 = aabb1->GetPos();
	Vector2 pos2 = aabb2->GetPos();
	Vector2 halfsize1 = aabb1->GetHalfSize();
	Vector2 halfsize2 = aabb2->GetHalfSize();
	float depthX = (halfsize1.x + halfsize2.x) - abs(pos1.x - pos2.x);
	float depthY = (halfsize1.y + halfsize2.y) - abs(pos1.y - pos2.y);
	bool checkX = depthX > 0;
	bool checkY = depthY > 0;
	bool out = checkX && checkY;
	if (out) {
		cReg.collisionNormal = depthX < depthY ? Vector2(1, 0) : Vector2(0, 1);
		cReg.penetrationDepth = depthX < depthY ? depthX : depthY;
	}
	return out;
}

bool NCL::CSC3222::GameSimsPhysics::CollisionResolution(SimObject* obj1, SimObject* obj2)
{
	bool b1 = obj1->CollisionCallback(obj2, cReg); 
	bool b2 = obj2->CollisionCallback(obj1, cReg);
	return b1 && b2;
}

void NCL::CSC3222::GameSimsPhysics::Projection(RigidBody* body1, RigidBody* body2)
{
	float inverseMassTotal = body1->inverseMass + body2->inverseMass;
	body1->position -= cReg.collisionNormal * cReg.penetrationDepth * body1->inverseMass / inverseMassTotal;
	body2->position += cReg.collisionNormal * cReg.penetrationDepth * body2->inverseMass / inverseMassTotal;
}

void NCL::CSC3222::GameSimsPhysics::Impluse(RigidBody* body1, RigidBody* body2)
{
	float inverseMassTotal = body1->inverseMass + body2->inverseMass;
	Vector2 relativeVec = body2->velocity - body1->velocity;
	float J = Vector2::Dot(relativeVec, cReg.collisionNormal) / inverseMassTotal;
	body1->velocity -= cReg.collisionNormal * -(1 + body1->elasticity) * J * body1->inverseMass;
	body2->velocity += cReg.collisionNormal * -(1 + body2->elasticity) * J * body2->inverseMass;
}

