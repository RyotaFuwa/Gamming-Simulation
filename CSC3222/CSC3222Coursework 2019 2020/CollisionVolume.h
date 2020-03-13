#pragma once
#include "..//..//Common/Vector2.h"

namespace NCL {
	using namespace Maths;
	namespace CSC3222 {

		class CollisionVolume {
		public:
			CollisionVolume();
			virtual ~CollisionVolume();
			virtual Vector2 GetHalfSize() const = 0;  // all colliders are supposed to be tested as AABB in broad phase

			Vector2 GetPos() const { return *pos; }
			void SetPos(Vector2* p) { pos = p; }
		protected:
			Vector2* pos = nullptr;
		};


		class AABB : public CollisionVolume {
		public:
			AABB(Vector2 hs);
			~AABB();
			Vector2 GetHalfSize() const override { return halfSize; }
		protected:
			Vector2 halfSize;
		};


		class Circle : public CollisionVolume {
		public:
			Circle(float r);
			~Circle();
			Vector2 GetHalfSize() const override { return Vector2(radius, radius); }
			float GetRadius() const{ return radius; }
			
		private:
			float radius;
		};
	}
}

