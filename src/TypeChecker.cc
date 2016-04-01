#include "TypeChecker.h"
#include "GrumpyConfig.h"
#include "AST.h"
#include "RTLCompiler.h"
#include "Schematic.h"
#include <iostream>
#include <algorithm>

using namespace puddi;
using namespace std;

namespace grumpy
{
    // PUBLIC

    TypeChecker::TypeChecker(Object *par, ASTNode *root) : DrawableObject(par)
    {
		init(root);
    }

	TypeChecker::TypeChecker(Object* par, ASTNode *root, SchematicNode *schematic) : DrawableObject(par, schematic)
	{
		init(root);
	}

	void TypeChecker::Update()
    {
        if (state == TYPECHECKER_STATE_DONE)
        {
            if (position == homePosition)
                return;

            float moveAmount = velocity * GrumpyConfig::GetGameSpeedFactor();

			MoveToPoint(homePosition, moveAmount, nullptr);

            return;
        }

		if (currentNodeIndex >= nodesVector.size())
		{
			finish();
			return;
		}

        ASTNode *targetNode = nodesVector[currentNodeIndex];

        if (state == TYPECHECKER_STATE_WAITING)
        {

        }
        else if (state == TYPECHECKER_STATE_TYPECHECKING)
        {
			vec4 targetPosition = targetNode->GetWorldPosition();

            float moveAmount = velocity * GrumpyConfig::GetGameSpeedFactor();

			MoveToPoint(targetPosition, moveAmount, [&]()
			{
			    targetNode->GiveTypeColor();

				if (++currentNodeIndex >= nodesVector.size())
                {
                    finish();
                    return;
                }
			});
        }
    }

    float TypeChecker::GetVelocity() const
    {
        return velocity;
    }
    void TypeChecker::SetVelocity(float v)
    {
        velocity = v;
    }

	void TypeChecker::SetHomePosition(glm::vec4 v)
	{
        homePosition = v;
	}

	void TypeChecker::SetRTLCompiler(RTLCompiler *c)
	{
	    rtlCompiler = c;
	}

	void TypeChecker::Start()
	{
	    currentNodeIndex = 0;
	    state = TYPECHECKER_STATE_TYPECHECKING;
	}

    // PRIVATE

    void TypeChecker::init(ASTNode *root)
	{
		astRoot = root;
		currentNodeIndex = 0;
		velocity = 0.0f;
		state = TYPECHECKER_STATE_WAITING;
		homePosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);

		createNodesVector();
	}

    void TypeChecker::createNodesVector()
    {
        nodesVector.clear();

        addToNodesVectorRecursive(astRoot);

        sort(nodesVector.begin(), nodesVector.end(),
        [](ASTNode *a, ASTNode *b) -> bool
        {
            return a->GetTypeIndex() < b->GetTypeIndex();
        });

        /*for (auto it = nodesVector.begin(); it != nodesVector.end(); ++it)
            cout << (*it)->GetType() << " " << (*it)->GetTypeIndex() << endl;*/
    }

    void TypeChecker::addToNodesVectorRecursive(ASTNode *node)
    {
        if (node->GetTypeIndex() != -1)
            nodesVector.push_back(node);
        for (auto it = node->ChildNodes.begin(); it != node->ChildNodes.end(); ++it)
            addToNodesVectorRecursive(*it);
    }

    void TypeChecker::finish()
    {
        state = TYPECHECKER_STATE_DONE;
        rtlCompiler->Start();
    }
}
