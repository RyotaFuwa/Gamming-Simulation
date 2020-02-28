#pragma once
#include <vector>
#include "../../Common/Vector2.h"

namespace NCL {
	namespace CSC3222 {
		class RigidBody;
		class CollisionVolume;

		class GameSimsPhysics	{
		public:
			GameSimsPhysics();
			~GameSimsPhysics();

			void Update(float dt);

			void AddRigidBody(RigidBody* b);
			void RemoveRigidBody(RigidBody* b);

			void AddCollider(CollisionVolume* c); //TODO what does collider mean?
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

			std::vector<RigidBody*>			allBodies;
			std::vector<CollisionVolume*>	allColliders;

			struct  {
				int bodyIndex1;
				int bodyIndex2; // TODO add another info
			} collisionRegsister;
		};
	}
}

