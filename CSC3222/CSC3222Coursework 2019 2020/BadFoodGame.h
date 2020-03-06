#pragma once
#include <vector>
#include "GameMap.h"
#include "../../Common/Assets.h"

namespace NCL {
	namespace Maths {
		class Vector2;
	}
	namespace CSC3222 {
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
			void AddStaticObstacle(const std::string filename);

			float getGameTime();
			void AutoSpawnEnemy(float possiblity, int limit);
			int GetCellSize() { return cellsize; }
			const GameMap* GetCurrentMap() { return currentMap; }

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
			float physicsRemainingTime;
			float physicsFixedDeltaTime;
			float graphicsRemainingTime;
			float graphicsFixedDeltaTime;

			int currentScore;
			int foodCount;
			int coins;
			int balloons;
			int lives;

			std::vector<SimObject*> gameObjects;
			std::vector<SimObject*> newObjects;

			// internal control variables
			int numOfEnemy;
		};
	}
}

