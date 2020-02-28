#pragma once
#include "SimObject.h"

namespace NCL {
	namespace CSC3222 {
		class BadFoodGame;
		class PlayerCharacter : public SimObject
		{
		public:
			enum class CharacterType {
				TYPE_A,
				TYPE_B
			};
			PlayerCharacter();
			~PlayerCharacter();

			bool UpdateObject(float dt) override;

			void DrawObject(GameSimsRenderer& r) override;

			void SetCharacterType(CharacterType t);
			
			int GetPlayerId() { return playerId; }

			void CollisionCallback(const SimObject* other, const CollisionRegister& cReg) override;

		protected:
			enum class MovementDir {
				Up,
				Down,
				Left,
				Right,
				Idle
			};
			void DrawCharacterA(GameSimsRenderer& r);
			void DrawCharacterB(GameSimsRenderer& r);

			MovementDir		currentAnimDir;
			CharacterType		charType;
			int playerId;
		private:
			static int NumOfPlayers;
		};
	}
}

