#pragma once
#include "../../Common/Vector2.h"
#include "CollisionVolume.h"

namespace NCL {
	using namespace Maths;
	namespace CSC3222 {
		enum class State {
			ACTIVE,
			SLEEP,
			STATIC
		};
		class RigidBody	{
			friend class GameSimsPhysics;
		public:
			RigidBody(State s);
			~RigidBody();

			Vector2 GetPosition() const {
				return position;
			}

			void SetPosition(const Vector2& newPosition) {
				position = newPosition;
			}

			Vector2 GetVelocity() const {
				return velocity;
			}

			void SetVelocity(const Vector2& newVelocity) {
				velocity = newVelocity;
			}

			void AddForce(const Vector2& newForce) {
				force += newForce;
			}

			void SetForceToZero() {
				force = Vector2(0, 0);
			}

			void SetCollider(CollisionVolume * c) { //TODO call setCollider in constructor of each object
				c->SetPos(&position);
				collider = c;
			}

			CollisionVolume* GetCollider() const {
				return collider;
			}
			State GetState() {
				return state;
			}

			void SetState(State s) {
				state = s;
			}

		protected:
			Vector2 position;
			Vector2 velocity;
			Vector2 force;
			float inverseMass;
			float elasticity;

			CollisionVolume* collider;
			State state;

		};
	}
}
