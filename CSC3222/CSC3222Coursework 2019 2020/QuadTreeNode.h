#pragma once
#include <vector>
#include "../../Common/Vector2.h"
#include "CollisionVolume.h"
#include "RigidBody.h"

using namespace std;
using namespace NCL::CSC3222;
using namespace NCL::Maths;

enum Division {
	TOP_RIGHT,
	TOP_LEFT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT
};

struct BodyPairs {
	RigidBody* body1;
	RigidBody* body2;
};

class QuadTreeNode {
public:
	static int maxDepth;
	static int capacity;

	QuadTreeNode(Vector2 pos, Vector2 halfsize, int maxDepth, int capacity, vector<RigidBody*> staticBodies);  // for root node
	QuadTreeNode(Vector2 pos, Vector2 halfsize, int depth);
	~QuadTreeNode();

	inline void Insert(RigidBody* body);
	vector<BodyPairs> RetriveCollisionPairs();
	void Refresh();

private:
	void InsertStatic(RigidBody* body);
	bool CheckOverlap(CollisionVolume* cv);
	void Split();
	void GetPairs(vector<BodyPairs> &container);

	int depth;
	QuadTreeNode* children[4];
	bool leaf;
	Vector2* center;
	AABB* shape;
	vector<RigidBody*> dynamicBodies;
	vector<RigidBody*> staticBodies;
};

