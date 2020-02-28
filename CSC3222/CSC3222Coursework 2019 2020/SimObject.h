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

		struct CollisionRegister {
			int i; //TODO define collision info other than those in simObject
			int j; 
		};

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

			virtual void CollisionCallback(const SimObject* other, const CollisionRegister& cReg) {  //TODO define callback for each object
				std::cout << "Collision Detected !! AT (";
				std::cout << GetPosition().x << ", " << GetPosition().y << ")" << std::endl; // prottype
			}

			static void InitObjects(BadFoodGame* game, TextureManager* manager);

		protected:

			CollisionVolume*		collider;
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

