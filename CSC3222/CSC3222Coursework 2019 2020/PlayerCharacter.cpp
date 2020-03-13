#include "PlayerCharacter.h"
#include "TextureManager.h"
#include "../../Common/Window.h"
#include "GameSimsRenderer.h"
#include "Laser.h"
#include "Food.h"
#include "Balloon.h"
#include "BadFoodGame.h"
#include "CollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

int  charARunY		= 128;
int  charARunX[]	= { 32,64,96,128,160,192,224,256 };
bool charAFlips[]	= { false, true, true, false, false };

Vector2 charAIdle(64,64);
Vector2 charBIdle(64,64);

int  charBRunY		= 96;
int  charBRunX[]	= { 128,160,192,224,256,288 };
bool charBFlips[]	= { false, true, true, false, false };

int PlayerCharacter::NumOfPlayers = 1;

PlayerCharacter::PlayerCharacter() : SimObject(State::ACTIVE), playerId(NumOfPlayers) {
	currentAnimDir		= MovementDir::Left;
	SetCharacterType(CharacterType::TYPE_A);

	CollisionVolume* cv = new Circle(7.5f); // determined by looking at the character size on screen
	cv->SetPos(&position);
	SetCollider(cv);

	NumOfPlayers++;
}

PlayerCharacter::~PlayerCharacter() {

}

bool PlayerCharacter::UpdateObject(float dt) {// Define how object moves 

	float constSpeed = 64; // Now it uses constant speed (64 [Length][Time]^-1).
	// float testSpeed = 16.0 * 32.0; //cellsize * # of cells = field size. character runs from one side to the other in 1saa

	float constForce = 32;
	Vector2 newForce;

	currentAnimDir = MovementDir::Idle;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
		currentAnimDir = MovementDir::Up;
		UpdateAnimFrame(dt);
		
		// AddForce(Vector2(0, -constForce));
		newForce.y = -constForce;
		
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
		currentAnimDir = MovementDir::Down;
		UpdateAnimFrame(dt);

		 // AddForce(Vector2(0, constForce));
		newForce.y = constForce;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
		currentAnimDir = MovementDir::Left;
		UpdateAnimFrame(dt);

		 // AddForce(Vector2(-constForce, 0));
		newForce.x = -constForce;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
		currentAnimDir = MovementDir::Right;
		UpdateAnimFrame(dt);

		 // AddForce(Vector2(constForce, 0));
		newForce.x = constForce;
	}

	
	// default movement setting (move character with constant velocity)
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		currentAnimDir = MovementDir::Up;
		UpdateAnimFrame(dt);

		force = Vector2(0, 0);
		velocity = Vector2(0, 0);
		position.y += -constSpeed * dt;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		currentAnimDir = MovementDir::Down;
		UpdateAnimFrame(dt);

		force = Vector2(0, 0);
		velocity = Vector2(0, 0);
		position.y += constSpeed * dt;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		currentAnimDir = MovementDir::Left;
		UpdateAnimFrame(dt);

		force = Vector2(0, 0);
		velocity = Vector2(0, 0);
		position.x += -constSpeed * dt;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		currentAnimDir = MovementDir::Right;
		UpdateAnimFrame(dt);

		force = Vector2(0, 0); 
		velocity = Vector2(0, 0);
		position.x += constSpeed * dt;
	}

	/*
	// moved this implementation to BadFoodGame
	// shoot laser
	if (Window::GetMouse()->ButtonPressed(MouseButtons::LEFT)) {
		std::cout << "Button Held" << std::endl;
		Vector2 mousePos = Window::GetMouse()->GetAbsolutePosition(); //TODO
		float coordinateRatioX = game->GetCurrentMap()->GetMapWidth() / Window::GetWindow()->GetScreenSize().x;
		float coordinateRatioY = game->GetCurrentMap()->GetMapHeight() / Window::GetWindow()->GetScreenSize().y;
		mousePos.x *= game->GetCellSize() * coordinateRatioX;
		mousePos.y *= game->GetCellSize() * coordinateRatioY;
		Vector2 dir = mousePos - position;
		Laser* l = new Laser(dir.Normalised() * 200, playerId);
		l->SetPosition(position);
		game->AddNewObject(l);
	}
	*/

force = newForce;
	return true;
}

void PlayerCharacter::DrawObject(GameSimsRenderer& r) {
	switch (charType) {
		case CharacterType::TYPE_A:DrawCharacterA(r); break;
		case CharacterType::TYPE_B:DrawCharacterB(r); break;
	}
}

void PlayerCharacter::DrawCharacterA(GameSimsRenderer& r) {
	Vector2 screenPos	= position;
	Vector2 texSize		= Vector2(32, 32);
	screenPos.y -= 8;

	Vector2	texPos;
	texPos.x = (float)charARunX[currentanimFrame];
	texPos.y = (float)charARunY;

	if (currentAnimDir == MovementDir::Idle) {
		texPos = charAIdle;
	}
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, screenPos, charAFlips[(int)currentAnimDir]);
}

void PlayerCharacter::DrawCharacterB(GameSimsRenderer& r) {
	Vector2 screenPos	= position;
	Vector2 texSize		= Vector2(32, 32);
	screenPos.y -= 8;

	Vector2	texPos;
	texPos.x = (float)charBRunX[currentanimFrame];
	texPos.y = (float)charBRunY;

	if (currentAnimDir == MovementDir::Idle) {
		texPos = charBIdle;
	}

	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, screenPos, charBFlips[(int)currentAnimDir]);
}

void PlayerCharacter::SetCharacterType(CharacterType t) {
	charType = t;
	switch (charType) {
		case CharacterType::TYPE_A:
			texture = texManager->GetTexture("TL_oldcharacters.png"); 
			animFrameCount = 8;
			break;
		case CharacterType::TYPE_B:
			texture = texManager->GetTexture("Twiggy_spritesheet.png"); 
			animFrameCount = 6;
			break;
	}
}

bool NCL::CSC3222::PlayerCharacter::CollisionCallback(SimObject* other, const CollisionRegister& cReg)
{
	if (dynamic_cast<Laser*>(other)) {
		Laser* laser = (Laser*)other;
		if (playerId != laser->GetPlayerId()) {
			return false;
		}
		else {
			return false;
		}
	}
	else if(dynamic_cast<Balloon*>(other)) {
		Balloon* balloon = (Balloon*)other;
		if (!balloon->GetOwner()) {
			NumOfBalloons++;
			return false;
		}
	}
	else if (dynamic_cast<Food*>(other)) {
			return false;
	}
	else {
		return true;
	}
}
