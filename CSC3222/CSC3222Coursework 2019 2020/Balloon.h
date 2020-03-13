#pragma once
#include "SimObject.h"
#include "PlayerCharacter.h"


namespace NCL {
	namespace CSC3222 {
		class Balloon : public SimObject
		{
		public:
			Balloon();
			~Balloon();

			bool UpdateObject(float dt) override;
			void DrawObject(GameSimsRenderer& r) override;
			bool CollisionCallback(SimObject* other, const CollisionRegister& cReg) override;

			PlayerCharacter* GetOwner() { return owner; }

		protected:
			PlayerCharacter* owner;
			float springConstant;
			float naturalLength;
			float dampingConstant;
			bool cracked;
		};
	}
}

