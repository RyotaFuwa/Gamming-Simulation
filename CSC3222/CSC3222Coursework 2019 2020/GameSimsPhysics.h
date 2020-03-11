#pragma once
#include <vector>
#include "../../Common/Vector2.h"

namespace NCL {
	using namespace Maths;
	namespace CSC3222 {
		class RigidBody;
		class CollisionVolume;

		struct  CollisionRegister {
			int bodyIndex1;
			int bodyIndex2; // TODO add another info
			Vector2 collisionNormal;  // body1 to body2
			float penetrationDepth;
		};

		class GameSimsPhysics	{
		public:
			GameSimsPhysics();
			~GameSimsPhysics();

			void Update(float dt);

			void AddRigidBody(RigidBody* b);
			void RemoveRigidBody(RigidBody* b);

			void AddCollider(CollisionVolume* c); 
			void RemoveCollider(CollisionVolume* c);

		protected:
			void IntegrateAcceleration(float dt);
			void IntegrateVelocity(float dt);

			void HandleCollision(float dt);

			// collision detection for different shapes
			bool CollideCheck(int i, int j);
			bool CircleCircleCollision(int i, int j); 
			bool CircleAABBCollision(int i, int j);
			bool AABBAABBCollision(int i, int j);

			// collision resolution (physical aspect)
			void Projection();
			void Impluse();

			std::vector<RigidBody*>			allBodies;
			std::vector<CollisionVolume*>	allColliders;
			CollisionRegister cReg;

		};
	}
}

