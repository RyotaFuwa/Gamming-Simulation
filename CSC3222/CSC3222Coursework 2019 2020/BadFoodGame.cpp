#include "BadFoodGame.h"
#include "SimObject.h"
#include "GameMap.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "GameSimsPhysics.h"
#include "PlayerCharacter.h"
#include "BadRobot.h"
#include "Laser.h"
#include "Food.h"
#include "Coin.h"
#include "Balloon.h"

#include "../../Common/Window.h"
#include "../../Common/TextureLoader.h"

using namespace NCL;
using namespace CSC3222;

BadFoodGame::BadFoodGame()	{
	renderer	= new GameSimsRenderer();
	texManager	= new TextureManager();
	physics		= new GameSimsPhysics();
	SimObject::InitObjects(this, texManager);
	InitialiseGame();
}

BadFoodGame::~BadFoodGame()	{
	delete currentMap;
	delete texManager;
	delete renderer;
	delete physics;
}

void BadFoodGame::Update(float dt) {
	for (auto i : newObjects) {
		gameObjects.emplace_back(i);
	}
	newObjects.clear();

	if (gameTime > second)
		second++;
	gameTime += dt;

	renderer->Update(dt);
	physics->Update(dt);
	currentMap->DrawMap(*renderer);

	srand((int)(gameTime * 1000.0f));

	for (auto i = gameObjects.begin(); i != gameObjects.end(); ) {
		if (!(*i)->UpdateObject(dt)) { //object has said its finished with
			delete (*i);
			gameObjects.erase(i);
		}
		else {
			(*i)->DrawObject(*renderer);
			++i;
		}
	}	

	// show stats
	renderer->DrawString("Score: " + std::to_string(currentScore), Vector2(10, 10));
	renderer->DrawString("Lives left: " + std::to_string(lives), Vector2(10, 30));
	renderer->DrawString("Food: " + std::to_string(foodCount), Vector2(10, 50));
	renderer->DrawString("Coins: " + std::to_string(coins), Vector2(10, 70));
	renderer->DrawString("Balloons: " + std::to_string(balloons), Vector2(10, 90));
	renderer->DrawString("NumberOfEnemy: " + std::to_string(numOfEnemy), Vector2(10, 110));
	renderer->DrawString("second: " + std::to_string(second), Vector2(10, 130));


	/*

	// Some examples of debug rendering!
	renderer->DrawBox(Vector2(16,16), Vector2(8, 8), Vector4(1, 0, 0, 1));
	renderer->DrawLine(Vector2(16, 16), Vector2(192, 192), Vector4(1, 1, 0, 1));
	renderer->DrawCircle(Vector2(100, 100), 10.0f, Vector4(1, 0, 1, 1));

	*/
	renderer->Render();

}

void BadFoodGame::InitialiseGame() {
	delete currentMap;
	for (auto o : gameObjects) {
		delete o;
	}
	gameObjects.clear();

	currentMap = new GameMap("BadFoodMap.txt", gameObjects, *texManager); 

	cellsize = 16;
	renderer->SetScreenProperties(cellsize, currentMap->GetMapWidth(), currentMap->GetMapHeight());

	player = new PlayerCharacter();
	player->SetCharacterType(PlayerCharacter::CharacterType::TYPE_B);
	player->SetPosition(Vector2(100, 200));
	AddNewObject(player);

	/* Test: Object Placement

	BadRobot* testRobot = new BadRobot();
	testRobot->SetPosition(Vector2(150, 200));
	AddNewObject(testRobot);


	Laser* testLaser = new Laser(Vector2(1,0));
	testLaser->SetPosition(Vector2(200, 200));
	AddNewObject(testLaser);

	Food* testFood = new Food();
	testFood->SetPosition(Vector2(250, 200));
	AddNewObject(testFood);

	Coin* testCoin = new Coin();
	testCoin->SetPosition(Vector2(300, 200));
	AddNewObject(testCoin);

	Balloon* testBalloon = new Balloon();
	testBalloon->SetPosition(Vector2(350, 200));
	AddNewObject(testBalloon);

	*/

	gameTime		= 0;
	currentScore	= 0;
	foodCount		= 0;
	coins			= 0;
	balloons		= 1;
	lives			= 3;
}

void BadFoodGame::AddNewObject(SimObject* object) {
	newObjects.emplace_back(object);
	physics->AddRigidBody(object);
	if (object->GetCollider()) {
		physics->AddCollider(object->GetCollider());
	}
}

/*


Added By Me.


*/

float BadFoodGame::getGameTime()
{
	return gameTime;
}

void NCL::CSC3222::BadFoodGame::AutoSpawnEnemy(float possibility, int limit)
{
	if (second < gameTime && numOfEnemy <= limit) {
		if (rand() % 100 < possibility * 100) {
			float randX = (rand() % 100) / 100.0;
			float randY = (rand() % 100) / 100.0;
			BadRobot* testRobot = new BadRobot();
			testRobot->SetPosition(Vector2(randX * cellsize * currentMap->GetMapWidth(),
				randY * cellsize * currentMap->GetMapHeight()));
			AddNewObject(testRobot);
			numOfEnemy++;
		}
	}
}

