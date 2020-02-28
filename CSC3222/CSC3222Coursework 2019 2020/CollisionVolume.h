#pragma once
#include "..//..//Common/Vector2.h"

namespace NCL {
	using namespace Maths;
	namespace CSC3222 {

		class CollisionVolume {
		public:
			CollisionVolume();
			virtual ~CollisionVolume();
		};

		class Circle : public CollisionVolume {
		public:
			Circle(float r) : radius(r) {};
			~Circle() {};
			float GetRadius() { return radius; }
		private:
			float radius;
		};

		class AABB : public CollisionVolume {
		public:
			AABB(Vector2 hs): halfSize(hs) {};
			~AABB() {};
			Vector2 GetHalfSize() { return halfSize; }
		protected:
			Vector2 halfSize;
		};
	}
}

