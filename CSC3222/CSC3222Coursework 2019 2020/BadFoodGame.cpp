#include "BadFoodGame.h"
#include "SimObject.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "GameSimsPhysics.h"
#include "PlayerCharacter.h"
#include "BadRobot.h"
#include "Laser.h"
#include "Food.h"
#include "Coin.h"
#include "Balloon.h"
#include "StaticObstacle.h"


#include "../../Common/Window.h"
#include "../../Common/TextureLoader.h"

#include <iostream>
#include <fstream>

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
	srand((int)(gameTime * 1000.0f));


	physicsRemainingTime += dt;
	if (physicsRemainingTime > physicsFixedDeltaTime) { // physics engine: 120 Hz 
		physics->Update(physicsFixedDeltaTime); 
		physicsRemainingTime -= physicsFixedDeltaTime;
	}

	graphicsRemainingTime += dt;
	if (graphicsRemainingTime > graphicsFixedDeltaTime) { // graphics: 60 Hz
		renderer->Update(graphicsFixedDeltaTime);
		currentMap->DrawMap(*renderer);
		for (auto i = gameObjects.begin(); i != gameObjects.end(); ) {
			if (!(*i)->UpdateObject(graphicsFixedDeltaTime)) { //object has said its finished with
				delete (*i);
				i = gameObjects.erase(i);
			}
			else {
				(*i)->DrawObject(*renderer);
				++i;
			}
		}
		graphicsRemainingTime -= graphicsFixedDeltaTime;
	}

	// show stats
	renderer->DrawString("Score: " + std::to_string(currentScore), Vector2(10, 10));
	renderer->DrawString("Lives left: " + std::to_string(lives), Vector2(10, 30));
	renderer->DrawString("Food: " + std::to_string(foodCount), Vector2(10, 50));
	renderer->DrawString("Coins: " + std::to_string(coins), Vector2(10, 70));
	renderer->DrawString("Balloons: " + std::to_string(balloons), Vector2(10, 90));

	// shoot laser
	if (Window::GetMouse()->ButtonPressed(MouseButtons::LEFT)) {
		std::cout << "Button Held" << std::endl;
		Vector2 mousePos = Window::GetMouse()->GetAbsolutePosition(); //TODO
		float coordinateRatioX = currentMap->GetMapWidth() / Window::GetWindow()->GetScreenSize().x;
		float coordinateRatioY = currentMap->GetMapHeight() / Window::GetWindow()->GetScreenSize().y;
		mousePos.x *= cellsize * coordinateRatioX;
		mousePos.y *= cellsize * coordinateRatioY;
		Vector2 pPos = player->GetPosition();
		Vector2 dir = mousePos - pPos;
		Laser* l = new Laser(dir.Normalised() * 200, player->GetPlayerId());
		l->SetPosition(pPos);
		AddNewObject(l);
	}



	/*

	// Some examples of debug rendering!
	renderer->DrawBox(Vector2(16,16), Vector2(8, 8), Vector4(1, 0, 0, 1));
	renderer->DrawLine(Vector2(16, 16), Vector2(192, 192), Vector4(1, 1, 0, 1));
	renderer->DrawCircle(Vector2(100, 100), 10.0f, Vector4(1, 0, 1, 1));

	*/

	/*
	renderer->DrawBox(Vector2(296, 52), Vector2(8, 36), Vector4(1, 0, 0, 1));
	renderer->DrawBox(Vector2(272, 76), Vector2(16, 12), Vector4(1, 0, 0, 1));
	renderer->DrawBox(Vector2(120, 52), Vector2(8, 36), Vector4(1, 0, 0, 1));
	renderer->DrawBox(Vector2(176, 76), Vector2(48, 12), Vector4(1, 0, 0, 1));
	*/

	// collision debug
	for (auto obj : gameObjects) {
		CollisionVolume* collider = obj->GetCollider();
		if (dynamic_cast<AABB*>(collider)) {
			AABB* aabb = (AABB*)collider;
			renderer->DrawBox(obj->GetPosition(), aabb->GetHalfSize(), Vector4(0, 0, 1, 1));

		}
		else if (dynamic_cast<Circle*>(collider)) {
			Circle* circle = (Circle*)collider;
			renderer->DrawCircle(obj->GetPosition(), circle->GetRadius(), Vector4(0, 1, 0, 1));
		}
	}

	// debug tool
	if (Window::GetMouse()->ButtonPressed(MouseButtons::RIGHT)) {
		Vector2 mousePos = Window::GetMouse()->GetAbsolutePosition(); //TODO
		float coordinateRatioX = currentMap->GetMapWidth() / Window::GetWindow()->GetScreenSize().x;
		float coordinateRatioY = currentMap->GetMapHeight() / Window::GetWindow()->GetScreenSize().y;
		mousePos.x *= coordinateRatioX;
		mousePos.y *= coordinateRatioY;
		Vector2 pos(mousePos.x * cellsize, mousePos.y * cellsize);
		renderer->DrawBox(pos, Vector2(cellsize/2.0, cellsize/2.0), Vector4(1, 0, 0, 1));
		std::cout << pos << std::endl;
	}
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

	physicsFixedDeltaTime = 1.0 / 120;
	graphicsFixedDeltaTime = 1.0 / 60;

	AddStaticObstacle("Walls.txt");
	AddStaticObstacle("OtherObstacles.txt");

	player = new PlayerCharacter();
	player->SetCharacterType(PlayerCharacter::CharacterType::TYPE_B);
	player->SetPosition(Vector2(200, 200));
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
	Food* testFood = new Food();
	testFood->SetPosition(Vector2(250, 200));
	AddNewObject(testFood);
	Coin* testCoin = new Coin();
	testCoin->SetPosition(Vector2(300, 200));
	AddNewObject(testCoin);
	Balloon* testBalloon = new Balloon();
	testBalloon->SetPosition(Vector2(350, 200));
	AddNewObject(testBalloon);
	

	physicsRemainingTime    = 0;
	graphicsRemainingTime   = 0;
	gameTime				= 0;
	currentScore			= 0;
	foodCount				= 0;
	coins					= 0;
	balloons				= 1;
	lives					= 3;
}

void BadFoodGame::AddNewObject(SimObject* object) {
	newObjects.emplace_back(object);
	physics->AddRigidBody(object);
	if (object->GetCollider()) {
		physics->AddCollider(object->GetCollider());
	}
}

void NCL::CSC3222::BadFoodGame::AddStaticObstacle(const std::string filename)
{
	std::ifstream objFile(Assets::DATADIR + filename);

	if (!objFile) {
		std::cout << "CAN'T LOAD OBSTACLE FILE!" << std::endl;
		return;
	}

	int row;
	char objShape;

	objFile >> row;
	for (int i = 0; i < row; i++) {
		objFile >> objShape;
		switch (objShape) {
		case 'R': {
			Vector2 pos, halfsize;
			objFile >> pos.x;
			objFile >> pos.y;
			objFile >> halfsize.x;
			objFile >> halfsize.y;

			StaticObstacle* obj = new StaticObstacle(pos, new AABB(halfsize));
			AddNewObject(obj);
			break;
		}
		case 'C': {
			Vector2 pos;
			float radius;
			objFile >> pos.x;
			objFile >> pos.y;
			objFile >> radius;

			StaticObstacle* obj = new StaticObstacle(pos, new Circle(radius));
			AddNewObject(obj);
			break;
		}
		}
	}
}


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

