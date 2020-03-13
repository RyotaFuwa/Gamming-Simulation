#pragma once
#include "../../Common/Vector2.h"
#include "RigidBody.h"
#include "BadFoodGame.h"

namespace NCL {
	using namespace Maths;
	namespace Rendering {
		class TextureBase;
	}
	namespace CSC3222 {
		class GameSimsRenderer;
		class TextureManager;
		class RigidBody;
		class CollisionVolume;
		struct CollisionRegister;

		class SimObject	: public RigidBody {
		public:
			SimObject(State s);
			~SimObject();

			virtual bool UpdateObject(float dt) = 0;

			virtual void DrawObject(GameSimsRenderer &r) = 0;

			virtual bool CollisionCallback(SimObject* other, const CollisionRegister& cReg) {  //return true if physical resolution is needed
				return true;
			}

			static void InitObjects(BadFoodGame* game, TextureManager* manager);

		protected:
			Rendering::TextureBase* texture;

			void UpdateAnimFrame(float dt);

			int				currentanimFrame;
			int				animFrameCount;
			float			frameTime;

			static TextureManager*  texManager;
			static BadFoodGame*		game;
		};
	}
}

