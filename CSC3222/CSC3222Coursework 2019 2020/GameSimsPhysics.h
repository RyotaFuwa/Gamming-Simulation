#pragma once
#include <vector>
#include "../../Common/Vector2.h"
#include "CollisionVolume.h"

namespace NCL {
	using namespace Maths;
	namespace CSC3222 {
		class RigidBody;

		struct  CollisionRegister {
			Vector2 collisionNormal;  // body1 to body2
			float penetrationDepth;
		};

		enum Division {
			TOP_RIGHT,
			TOP_LEFT,
			BOTTOM_RIGHT,
			BOTTOM_LEFT
		};

		struct CollisionPair {
			RigidBody* body1;
			RigidBody* body2;
		};

		class QuadTreeNode {
		public:
			QuadTreeNode(Vector2 pos, Vector2 halfsize, int md, int cs);  // for root node
			QuadTreeNode(Vector2 pos, Vector2 halfsize);
			~QuadTreeNode();
			void Insert(RigidBody* body);
			vector<CollisionPair> Retrieve();
			void Refresh();

		private:
			void PushDown(RigidBody* body);
			void Split();
			bool CheckOverlap(CollisionVolume* cv);

			QuadTreeNode(Vector2 pos, Vector2 halfsize, int md, int cs)
			{
				maximumDepth = md;
				depth = 0;
				contentsSize = cs;

				shape = new AABB(halfsize);
				shape->SetPos(&pos);
				depth = md;
				for (auto division : allDivision) {
					children[division] = nullptr;
				}
			}

			QuadTreeNode(Vector2 pos, Vector2 halfsize) {
				shape = new AABB(halfsize);
				shape->SetPos(&pos);
				for (auto division : allDivision) {
					children[division] = nullptr;
				}
			}

			~QuadTreeNode()
			{
				delete shape;
				for (auto divisoin : allDivision) {
					delete children[divisoin];
				}
			}

			void Insert(RigidBody* body) {
				if (CheckOverlap(body->GetCollider())) {
					if (!children[TOP_LEFT]) {
						PushDown(body);
					}
					else if (depth >= maximumDepth) {
						contents.push_back(body);
					}
					else if (contents.size() <= contentsSize) {
						contents.push_back(body);
					}
					else {
						Split();
					}
				}
			}

			void Refresh() {
				contents.clear();
				for (auto division : allDivision) {
					if (!children[division])
						children[division]->Refresh();
				}
			}
			void PushDown(RigidBody* body) {
				for (auto division : allDivision) {
					children[division]->Insert(body);
				}
			}

			void Split() {
				depth++;
				Vector2 newSize = shape->GetHalfSize() / 2.0;
				Vector2 newPos = shape->GetPos() - newSize;
				children[Division::TOP_LEFT] = new QuadTreeNode(newPos, newSize);
				newPos.y += 2 * newSize.y;
				children[Division::BOTTOM_LEFT] = new QuadTreeNode(newPos, newSize);
				newPos.x += 2 * newSize.x;
				children[Division::BOTTOM_RIGHT] = new QuadTreeNode(newPos, newSize);
				newPos.y -= 2 * newSize.y;
				children[Division::TOP_RIGHT] = new QuadTreeNode(newPos, newSize);

				for (auto body : contents) {
					PushDown(body);
				}
				contents.clear();
			}

			bool CheckOverlap(CollisionVolume* cv) {
				Vector2 pos1 = shape->GetPos();
				Vector2 pos2 = cv->GetPos();
				Vector2 halfsize1 = shape->GetHalfSize();
				Vector2 halfsize2 = cv->GetHalfSize();
				float depthX = (halfsize1.x + halfsize2.x) - abs(pos1.x - pos2.x);
				float depthY = (halfsize1.y + halfsize2.y) - abs(pos1.y - pos2.y);
				return depthX > 0 && depthY > 0;
			}

			static constexpr std::initializer_list<Division> allDivision = { TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, BOTTOM_LEFT };
			static int depth;
			static int maximumDepth;
			static int contentsSize;

			QuadTreeNode* children[4];
			AABB* shape;
			vector<RigidBody*> contents;
		};

		class GameSimsPhysics	{
		public:
			GameSimsPhysics();
			~GameSimsPhysics();

			void Update(float dt);

			void AddRigidBody(RigidBody* b);
			void RemoveRigidBody(RigidBody* b);

			void AddCollider(CollisionVolume* c); 
			void RemoveCollider(CollisionVolume* c);

		protected:
			void IntegrateAcceleration(float dt);
			void IntegrateVelocity(float dt);

			void HandleCollision(float dt);

			// broad phase
			vector<CollisionPair> GetCollisionPairs();  //quadTree & static dynamic sleep object state checking

			// narrow phase
			bool CollisionCheck(CollisionVolume* cv1, CollisionVolume* cv2);
			bool CircleCircleCollision(Circle* circle1, Circle* circle2);
			bool CircleAABBCollision(Circle* cirle, AABB* aabb);
			bool AABBAABBCollision(AABB* aabb1, AABB* aabb2);

			// collision resolution (physical aspect)
			void Projection(RigidBody* body1, RigidBody* body2); //TODO use CollisionPair
			void Impluse(RigidBody* body1, RigidBody* body2);

			QuadTreeNode* root;
			std::vector<RigidBody*>			allBodies;
			std::vector<CollisionVolume*>	allColliders;
			std::vector<CollisionPair>		allCollisionPairs;
			CollisionRegister cReg;
		};
	}
}

