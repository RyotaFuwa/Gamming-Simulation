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
			SimObject();
			~SimObject();

			void SetCollider(CollisionVolume * c) { //TODO call setCollider in constructor of each object
				collider = c;
			}

			CollisionVolume* GetCollider() const {
				return collider;
			}

			virtual bool UpdateObject(float dt) = 0;

			virtual void DrawObject(GameSimsRenderer &r) = 0;

			virtual bool CollisionCallback(SimObject* other, const CollisionRegister& cReg) {  //TODO define callback for each object
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

