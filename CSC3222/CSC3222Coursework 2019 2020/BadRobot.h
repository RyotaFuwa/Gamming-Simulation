#pragma once
#include "SimObject.h"
#include "StateMachine.h"

namespace NCL {
	namespace CSC3222 {
		class BadRobot : public SimObject
		{
		public:
			BadRobot();
			~BadRobot();

			bool UpdateObject(float dt) override;
			void DrawObject(GameSimsRenderer& r) override;
			bool CollisionCallback(SimObject* other, const CollisionRegister& cReg) override;

		protected:
			enum class RobotType {
				Green,
				White,
			};
			RobotType type;
			int stamina;
			StateMachine::StateMachine stateMachine;
		};
	}
}

