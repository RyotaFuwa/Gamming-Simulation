#pragma once
#include "SimObject.h"

namespace NCL {
	namespace CSC3222 {
		class StaticObstacle : public SimObject
		{
		public:
			StaticObstacle(Vector2 pos, CollisionVolume* cv);
			~StaticObstacle();
			void DrawObject(GameSimsRenderer& r) override {};
			bool UpdateObject(float dt) override { return true; };
			bool CollisionCallback(SimObject* other, const CollisionRegister& cReg) override;
		};
	}
}
