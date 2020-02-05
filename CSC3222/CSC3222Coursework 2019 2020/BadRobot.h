#pragma once
#include "SimObject.h"

namespace NCL {
	namespace CSC3222 {
		class BadRobot : public SimObject
		{
		public:
			BadRobot();
			~BadRobot();

			bool UpdateObject(float dt) override;
			void DrawObject(GameSimsRenderer& r) override;

		protected:
			enum class RobotType {
				Green,
				White,
			};
			RobotType type;
		};
	}
}

