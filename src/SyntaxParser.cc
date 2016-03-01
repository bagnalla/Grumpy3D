#include "SyntaxParser.h"
#include "FpsTracker.h"
#include "Util.h"
#include "Lexer.h"
#include <iostream>
#include <algorithm>

using namespace puddi;
using namespace std;

namespace grumpy
{
    // PUBLIC

	SyntaxParser::SyntaxParser(Object *par, Lexer *lex, ASTNode *root) : DrawableObject(par)
    {
		init(lex, root);
    }

	SyntaxParser::SyntaxParser(Object* par, Lexer *lex, ASTNode *root, SchematicNode *schematic) : DrawableObject(par, schematic)
	{
		init(lex, root);
	}

    void SyntaxParser::Update()
    {
        if (state == SYNTAXPARSER_STATE_DONE)
        {
            if (position == homePosition)
                return;

            float moveAmount = velocity * FpsTracker::GetFrameTimeMs();

			MoveToPoint(homePosition, moveAmount, nullptr);

            return;
        }

		if (currentNodeIndex >= nodesVector.size())
		{
			state = SYNTAXPARSER_STATE_DONE;
			return;
		}

        ASTNode *targetNode = nodesVector[currentNodeIndex];

        if (state == SYNTAXPARSER_STATE_WAITING)
        {
            auto requiredTokens = targetNode->GetRequiredTokenNumbers();
            int tokensNeeded = 0;
            for (auto it = requiredTokens.begin(); it != requiredTokens.end(); ++it)
            {
                tokensNeeded += !std::any_of(tokenQueue.begin(), tokenQueue.end(), [&](Token *t) { return t->LToken.number == *it; });
            }
            if (!tokensNeeded)
            {
                state = SYNTAXPARSER_STATE_MOVING;
            }
            //targetNode->Show();
			//astRoot->Resize();
            //state = SYNTAXPARSER_STATE_MOVING;
        }
        else if (state == SYNTAXPARSER_STATE_MOVING)
        {
//            if (targetNode->GetParent() != nullptr && targetNode->GetParent()->GetHidden())
//            {
//                nodesVector[currentNodeIndex]->Show();
//				astRoot->Resize();
//				currentNodeIndex++;
//				return;
//            }

			vec4 targetPosition;
			if (targetNode->GetParentConnector() != nullptr)
                targetPosition = targetNode->GetParentConnector()->GetWorldPosition();
			else
				targetPosition = targetNode->GetWorldPosition();

            float moveAmount = velocity * FpsTracker::GetFrameTimeMs();

			MoveToPoint(targetPosition, moveAmount, [&]()
			{
				nodesVector[currentNodeIndex]->Show();
				astRoot->Resize();

				auto requiredTokens = targetNode->GetRequiredTokenNumbers();
				for (auto it = requiredTokens.begin(); it != requiredTokens.end(); ++it)
				{
                    auto tNum = *it;
                    for (size_t i = 0; i < tokenQueue.size();)
                    {
                        if (tokenQueue[i]->LToken.number == tNum)
                        {
                            auto tok = tokenQueue[i];
                            tok->Consume();
                            tokenQueue.erase(tokenQueue.begin() + i);
                            tok->Cull();
                            break;
                        }
                        else
                            ++i;
                    }
				}

				if (++currentNodeIndex >= nodesVector.size())
                {
                    state = SYNTAXPARSER_STATE_DONE;
                    return;
                }

				targetNode = nodesVector[currentNodeIndex];
				requiredTokens = targetNode->GetRequiredTokenNumbers();
                int tokensNeeded = 0;
                for (auto it = requiredTokens.begin(); it != requiredTokens.end(); ++it)
                {
                    tokensNeeded += !std::any_of(tokenQueue.begin(), tokenQueue.end(), [&](Token *t) { return t->LToken.number == *it; });
                    if (!std::any_of(tokenQueue.begin(), tokenQueue.end(), [&](Token *t) { return t->LToken.number == *it; }))
                        cout << "parser needs token " << *it << endl;
                }
                if (tokensNeeded)
                {
                    cout << "parser waiting on " << tokensNeeded << " tokens from the lexer. tokenQueue size = " << tokenQueue.size() << "\n";
					state = SYNTAXPARSER_STATE_WAITING;
                    // request x number of tokens from lexer
					lexer->Lex();
                }
			});
        }
    }

    float SyntaxParser::GetVelocity() const
    {
        return velocity;
    }
    void SyntaxParser::SetVelocity(float v)
    {
        velocity = v;
    }

	void SyntaxParser::SetHomePosition(glm::vec4 v)
	{
        homePosition = v;
	}

	void SyntaxParser::AddToken(Token *t)
	{
        tokenQueue.push_back(t);
        cout << "adding token " << t->LToken.number << " to parser\n";

        ASTNode *targetNode = nodesVector[currentNodeIndex];
        auto requiredTokens = targetNode->GetRequiredTokenNumbers();
        int tokensNeeded = 0;
        for (auto it = requiredTokens.begin(); it != requiredTokens.end(); ++it)
        {
            tokensNeeded += !std::any_of(tokenQueue.begin(), tokenQueue.end(), [&](Token *t) { return t->LToken.number == *it; });
        }
        if (tokensNeeded)
        {
            cout << "parser waiting on " << tokensNeeded << " tokens from the lexer. tokenQueue size = " << tokenQueue.size() << "\n";
            //state = SYNTAXPARSER_STATE_WAITING;
            // request x number of tokens from lexer
            lexer->Lex();
        }
	}

	Token* SyntaxParser::GetTokenTail()
	{
        if (tokenQueue.size())
            return tokenQueue[tokenQueue.size() - 1];
        else
            return nullptr;
	}

    // PRIVATE

	void SyntaxParser::init(Lexer *lex, ASTNode *root)
	{
		lexer = lex;
		astRoot = root;
		currentNodeIndex = 0;
		velocity = 0.0f;
		state = SYNTAXPARSER_STATE_WAITING;
		homePosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);

		createNodesVector();

		lexer->Lex();
	}

    void SyntaxParser::createNodesVector()
    {
        nodesVector.clear();

        addToNodesVectorRecursive(astRoot);

        sort(nodesVector.begin(), nodesVector.end(),
        [](ASTNode *a, ASTNode *b) -> bool
        {
            return a->GetParseIndex() < b->GetParseIndex();
        });

        for (int i = 0; i < nodesVector.size(); ++i)
        {
            nodesVector[i]->Hide();
        }

        for (int i = 0; i < nodesVector.size(); ++i)
        {
            nodesVector[i]->SetNodeColor(Util::InterpolateRainbow(i / static_cast<float>(nodesVector.size()), 0.25f));
        }
    }

    void SyntaxParser::addToNodesVectorRecursive(ASTNode *node)
    {
        nodesVector.push_back(node);
        for (auto it = node->ChildNodes.begin(); it != node->ChildNodes.end(); ++it)
            addToNodesVectorRecursive(*it);
    }
}
