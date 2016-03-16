#include "TokenQueue.h"
#include "Token.h"
#include <algorithm>
#include <iostream>

namespace grumpy
{
    // PUBLIC

    TokenQueue::TokenQueue(Object *par) : Object(par)
    {
    }

    std::vector<Token*> TokenQueue::GetAllTokens()
    {
        return tokens;
    }

    std::vector<Token*> TokenQueue::GetReadyTokens()
    {
        return readyTokens;
    }

    void TokenQueue::AddToken(Token *t)
    {
        tokens.push_back(t);
        assignPositions();
    }

    void TokenQueue::TokenIsReady(Token *t)
    {
        auto it = std::find(readyTokens.begin(), readyTokens.end(), t);
        if (it == readyTokens.end())
        {
            readyTokens.push_back(t);
        }
    }

    void TokenQueue::ConsumeToken(Token *t)
    {
        auto it = std::find(readyTokens.begin(), readyTokens.end(), t);
		if (it != readyTokens.end())
        {
            readyTokens.erase(it);
        }

        it = std::find(tokens.begin(), tokens.end(), t);
		if (it != tokens.end())
        {
            (*it)->Consume();
            (*it)->Cull();
            tokens.erase(it);
        }

        assignPositions();
    }

    // PRIVATE

    void TokenQueue::assignPositions()
    {
        readyTokens.clear();

        vec4 pos = GetWorldPosition();

//        for (auto it = tokens.begin(); it != tokens.end(); ++it)
//        {
//            (*it)->SetTargetPosition(pos + vec4((*it)->GetScaleX() / 2.0f, 0.0f, 0.0f, 0.0f));
//
//            pos += vec4((*it)->GetScaleX(), 0.0f, 0.0f, 0.0f);
//        }
        for (auto it = tokens.rbegin(); it != tokens.rend(); ++it)
        {
            (*it)->SetTargetPosition(pos - vec4((*it)->GetScaleX() / 2.0f, 0.0f, 0.0f, 0.0f));

            pos -= vec4((*it)->GetScaleX(), 0.0f, 0.0f, 0.0f);
        }

    }
}
