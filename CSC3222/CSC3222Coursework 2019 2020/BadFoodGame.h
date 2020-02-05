#pragma once
#include <vector>

namespace NCL {
	namespace Maths {
		class Vector2;
	}
	namespace CSC3222 {
		class GameMap;
		class SimObject;
		class GameSimsRenderer;
		class GameSimsPhysics;
		class TextureBase;
		class TextureManager;
		class PlayerCharacter;

		class BadFoodGame {
		public:
			BadFoodGame();
			~BadFoodGame();

			void Update(float dt);

			void AddNewObject(SimObject* object);

			float getGameTime();
			void EnableRandomSpawn(float rate);


		protected:
			void InitialiseGame();

			TextureManager*		texManager;
			GameSimsRenderer*	renderer;
			GameSimsPhysics*	physics;

			GameMap* currentMap;
			int cellsize;

			PlayerCharacter* player;

			float gameTime;
			int second;

			int currentScore;
			int foodCount;
			int coins;
			int balloons;
			int lives;

			int spawned; // for random spawned

			std::vector<SimObject*> gameObjects;
			std::vector<SimObject*> newObjects;
		};
	}
}

