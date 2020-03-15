#pragma once
#include <vector>
using namespace std;

namespace NCL {
	namespace CSC3222 {
		namespace StateMachine {
			class State {
			public:
				virtual void Update(float time) = 0;
			};

			class Transition {
			public:
				virtual bool CanTransition() = 0;
				State* GetSourceState() { return sourceState; }
				State* GetDestState() { return destState; }
			private:
				State* sourceState;
				State* destState;
			};

			class StateMachine
			{
			public:
				virtual void Update(float time) {
					activeState->Update(time);
					for (Transition*& t : allTransitions) {
						if (t->GetSourceState() != activeState) {
							continue;
						}
						if (t->CanTransition()) {
							activeState = t->GetDestState();
							break;
						}
					}
				}
			protected:
				State* activeState;
				vector<State*> allStates;
				vector<Transition*> allTransitions;
			};
		}
	}
}

