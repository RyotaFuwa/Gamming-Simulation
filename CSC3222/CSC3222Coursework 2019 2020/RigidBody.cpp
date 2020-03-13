#include "RigidBody.h"

using namespace NCL;
using namespace CSC3222;

RigidBody::RigidBody(State s): state(s) {
	inverseMass = 1.0f;
	elasticity  = 0.75f;
}


RigidBody::~RigidBody()	{

}
