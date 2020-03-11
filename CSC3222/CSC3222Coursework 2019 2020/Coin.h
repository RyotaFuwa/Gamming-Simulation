#pragma once
#include "SimObject.h"

namespace NCL {
	namespace CSC3222 {
		class Coin :	public SimObject
		{
		public:
			Coin();
			~Coin();

			bool UpdateObject(float dt) override;
			void DrawObject(GameSimsRenderer& r) override;
			bool CollisionCallback(SimObject* other, const CollisionRegister& cReg) override;

		protected:
			bool taken;
		};
	}
}