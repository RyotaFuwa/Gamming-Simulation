#pragma once
#include "SimObject.h"
#include "PlayerCharacter.h"

namespace NCL {
	namespace CSC3222 {
		class Food : public SimObject
		{
		public:
			Food();
			~Food();

			bool UpdateObject(float dt) override;
			void DrawObject(GameSimsRenderer& r) override;
			void CollisionCallback(SimObject* other, const CollisionRegister& cReg) override;

		protected:
			int foodItem;
			bool eaten;
		};
	}
}

