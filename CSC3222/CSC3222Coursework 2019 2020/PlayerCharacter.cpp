#include "PlayerCharacter.h"
#include "TextureManager.h"
#include "../../Common/Window.h"
#include "GameSimsRenderer.h"
#include "Laser.h"

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

PlayerCharacter::PlayerCharacter() : SimObject() {
	currentAnimDir		= MovementDir::Left;
	SetCharacterType(CharacterType::TYPE_A);
}

PlayerCharacter::~PlayerCharacter() {

}

bool PlayerCharacter::UpdateObject(float dt) {// Define how object moves 

	float constSpeed = 64; // Now it uses constant speed (64 [Length][Time]^-1).
	// float testSpeed = 16.0 * 32.0; //cellsize * # of cells = field size. character runs from one side to the other in 1s

	Vector2 newVelocity;

	float constForce = 32;
	Vector2 newForce;

	currentAnimDir = MovementDir::Idle;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
		currentAnimDir = MovementDir::Up;
		UpdateAnimFrame(dt);
		
		AddForce(Vector2(0,32));
		
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
		currentAnimDir = MovementDir::Down;
		UpdateAnimFrame(dt);

		newForce.y = constForce;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
		currentAnimDir = MovementDir::Left;
		UpdateAnimFrame(dt);

		newForce.x = -constForce;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
		currentAnimDir = MovementDir::Right;
		UpdateAnimFrame(dt);

		newForce.x = constForce;
	}

	
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		currentAnimDir = MovementDir::Up;
		UpdateAnimFrame(dt);

		newVelocity.y = -constSpeed;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		currentAnimDir = MovementDir::Down;
		UpdateAnimFrame(dt);

		newVelocity.y = constSpeed;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		currentAnimDir = MovementDir::Left;
		UpdateAnimFrame(dt);

		newVelocity.x = -constSpeed;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		currentAnimDir = MovementDir::Right;
		UpdateAnimFrame(dt);

		newVelocity.x = constSpeed;
	}

	// shoot laser
	if (Window::GetMouse()->ButtonPressed(MouseButtons::LEFT)) {
		Laser* l = new Laser(Vector2(1, 0));
		game->AddNewObject();
	}

	// mouse control prottype
	/*
	if (Window::GetMouse()->ButtonHeld(MouseButtons::LEFT)) {
		Vector2 mousePos = Window::GetMouse()->GetAbsolutePosition(); //TODO
		mousePos.x *= game->cellsize * game->currentMap->GetMapWidth() / Window::GetWindow()->GetScreenSize().x;
		mousePos.y *= game->cellsize * game->currentMap->GetMapWidth() / Window::GetWindow()->GetScreenSize().y;
		Vector2 dir = mousePos - position; 
		if (dir.y > dir.x) {
			if (dir.y >= 0)
				currentAnimDir = MovementDir::Up;
			else
				currentAnimDir = MovementDir::Down;
		}
		else {
			if (dir.x >= 0)
				currentAnimDir = MovementDir::Right;
			else
				currentAnimDir = MovementDir::Left;
		}
		UpdateAnimFrame(dt);
		newForce = dir * 0.1;
	}
	*/

	// velocity = newVelocity;
	// force = newForce;
	

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
	}

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