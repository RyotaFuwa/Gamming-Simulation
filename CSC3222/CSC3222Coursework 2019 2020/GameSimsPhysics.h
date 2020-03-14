#pragma once
#include <vector>
#include "../../Common/Vector2.h"
#include "RigidBody.h"
#include "SimObject.h"
#include "CollisionVolume.h"
#include "QuadTreeNode.h"

namespace NCL {
	using namespace Maths;
	namespace CSC3222 {
		class GameSimsPhysics	{
		public:
			GameSimsPhysics();
			~GameSimsPhysics();
			void InitialiseQuadTree(const Vector2 &mapCenter, const Vector2 &mapHalfSize);

			void Update(float dt);

			void AddRigidBody(RigidBody* b);
			void RemoveRigidBody(RigidBody* b);

		protected:
			void IntegrateAcceleration(float dt);
			void IntegrateVelocity(float dt);

			void HandleCollision(float dt);

			std::vector<BodyPairs> BroadPhase();  
			bool NarrowPhase(CollisionVolume* cv1, CollisionVolume* cv2);
			bool CircleCircleCollision(Circle* circle1, Circle* circle2);
			bool CircleAABBCollision(Circle* cirle, AABB* aabb);
			bool ABBCircleCollision(AABB* aabb, Circle* circle);
			bool AABBAABBCollision(AABB* aabb1, AABB* aabb2);

			// collision resolution (logical aspect)
			bool CollisionResolution(SimObject* obj1, SimObject* obj2);

			// collision resolution (physical aspect)
			void Projection(RigidBody* body1, RigidBody* body2); //TODO use CollisionPair
			void Impluse(RigidBody* body1, RigidBody* body2);

			QuadTreeNode* root;
			std::vector<RigidBody*>			allDynamicBodies;
			std::vector<RigidBody*>			allStaticBodies;
			std::vector<BodyPairs>		allCollisionPairs;
			CollisionRegister cReg;
		};
	}
}

