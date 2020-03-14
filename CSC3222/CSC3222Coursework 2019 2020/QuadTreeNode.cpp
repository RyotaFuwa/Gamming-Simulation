#include "QuadTreeNode.h"

QuadTreeNode::QuadTreeNode(Vector2 pos, Vector2 halfsize, int maxDepth, int capacity, vector<RigidBody*> staticBodies)
{
	QuadTreeNode::maxDepth = maxDepth;
	QuadTreeNode::capacity = capacity;
	depth = 0;
	this->center = new Vector2(pos);
	shape = new AABB(halfsize);
	shape->SetPos(center);
	leaf = true;
	for (int division = 0; division < 4; division++) {
		children[division] = nullptr;
	}
	for (RigidBody* staticBody : staticBodies) {
		InsertStatic(staticBody);
	}
}

QuadTreeNode::QuadTreeNode(Vector2 pos, Vector2 halfsize, int depth) {
	this->depth = depth;
	this->center = new Vector2(pos);
	shape = new AABB(halfsize);
	shape->SetPos(center);
	leaf = true;
	for (int division = 0; division < 4; division++) {
		children[division] = nullptr;
	}
}

QuadTreeNode::~QuadTreeNode()
{
	delete shape;
	delete center;
	for (int division = 0; division < 4; division++) {
		delete children[division];
	}
}

void QuadTreeNode::Insert(RigidBody* body) {
	if (CheckOverlap(body->GetCollider())) {
		if (!leaf) {
			for (int division = 0; division < 4; division++) {
				children[division]->Insert(body);
			}
		}
		else if (depth >= maxDepth) {
			dynamicBodies.push_back(body);
		}
		else if (dynamicBodies.size() + staticBodies.size() < capacity) {
			dynamicBodies.push_back(body);
		}
		else {
			Split();
			for (int division = 0; division < 4; division++) {
				children[division]->Insert(body);
			}
		}
	}
}

vector<BodyPairs> QuadTreeNode::RetriveCollisionPairs() {
	vector<BodyPairs> container;
	GetPairs(container);
	return container;
}

void QuadTreeNode::Refresh() {
	dynamicBodies.clear();
	for (int division = 0; division < 4; division++) {
		if (!children[division])
			children[division]->Refresh();
	}
}

void QuadTreeNode::Split() {
	leaf = false;
	Vector2 newSize = shape->GetHalfSize() / 2.0;
	Vector2 newPos = shape->GetPos() - newSize;
	children[Division::TOP_LEFT] = new QuadTreeNode(newPos, newSize, depth+1);
	newPos.y += 2 * newSize.y;
	children[Division::BOTTOM_LEFT] = new QuadTreeNode(newPos, newSize, depth+1);
	newPos.x += 2 * newSize.x;
	children[Division::BOTTOM_RIGHT] = new QuadTreeNode(newPos, newSize, depth+1);
	newPos.y -= 2 * newSize.y;
	children[Division::TOP_RIGHT] = new QuadTreeNode(newPos, newSize, depth+1);

	for (auto body : dynamicBodies) {
		for (int division = 0; division < 4; division++) {
			children[division]->Insert(body);
		}
	}
	dynamicBodies.clear();

	for (auto body : staticBodies) {
		for (int division = 0; division < 4; division++) {
			children[division]->Insert(body);
		}
	}
	staticBodies.clear();
}

void QuadTreeNode::GetPairs(vector<BodyPairs> &container)
{
	if (!leaf) {
		for (int division = 0; division < 4; division++) {
			children[division]->GetPairs(container);
		}
	}
	else {

		// seems like O(n**2) but the sizes of both vectors are limited to be small -> O(1)
		for (auto staticBody : staticBodies) {
			for (auto dynamicBody : dynamicBodies) {
				if (dynamicBody->GetState() == State::ACTIVE) {
					BodyPairs cp;
					cp.body1 = staticBody;
					cp.body2 = dynamicBody;
					container.push_back(cp);
				}
			}
		}

		for (unsigned int i = 0; i < dynamicBodies.size(); i++) {
			for (unsigned int j = i + 1; j < dynamicBodies.size(); j++) {
				RigidBody* body1 = dynamicBodies[i];
				RigidBody* body2 = dynamicBodies[j];
				State s1 = body1->GetState();
				State s2 = body2->GetState();
				if (s1 == State::ACTIVE || s2 == State::ACTIVE) {
					BodyPairs cp;
					cp.body1 = body1;
					cp.body2 = body2;
					container.push_back(cp);
				}
			}
		}
	}
}

void QuadTreeNode::InsertStatic(RigidBody* body)
{
	if (CheckOverlap(body->GetCollider())) {
		if (!leaf) {
			for (int division = 0; division < 4; division++) {
				children[division]->Insert(body);
			}
		}
		else if (depth >= maxDepth) {
			staticBodies.push_back(body);
		}
		else if (staticBodies.size() + staticBodies.size() < capacity) {
			staticBodies.push_back(body);
		}
		else {
			Split();
			for (int division = 0; division < 4; division++) {
				children[division]->Insert(body);
			}
		}
	}
}

bool QuadTreeNode::CheckOverlap(CollisionVolume* cv) {
	Vector2 pos1 = shape->GetPos();
	Vector2 pos2 = cv->GetPos();
	Vector2 halfsize1 = shape->GetHalfSize();
	Vector2 halfsize2 = cv->GetHalfSize();
	float depthX = (halfsize1.x + halfsize2.x) - abs(pos1.x - pos2.x);
	float depthY = (halfsize1.y + halfsize2.y) - abs(pos1.y - pos2.y);
	return (depthX > 0) && (depthY > 0);
}

int QuadTreeNode::maxDepth = 0;
int QuadTreeNode::capacity = 0;
