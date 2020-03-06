#pragma once
#include "SimObject.h"

namespace NCL {
	namespace Maths {
		class Vector2;
	}
	namespace CSC3222 {
		class Laser : public SimObject {
		public:
			Laser(Vector2 direction, int playerID);
			~Laser();

			void DrawObject(GameSimsRenderer &r) override;
			bool UpdateObject(float dt) override;

			int GetPlayerId() { return playerId; }
			void CollisionCallback(SimObject* other, const CollisionRegister& cReg) override;
		private:
			int playerId;
		};
	}
}

