#ifndef TOKENQUEUE_H
#define TOKENQUEUE_H

#include "Object.h"
#include <vector>

namespace grumpy
{
    class Token;

    class TokenQueue : public puddi::Object
    {
    public:
        TokenQueue(puddi::Object *par);

        std::vector<Token*> GetAllTokens();
        std::vector<Token*> GetReadyTokens();

        void AddToken(Token *t);

        void TokenIsReady(Token *t);

        void ConsumeToken(Token *t);

    private:
        std::vector<Token*> tokens; // all tokens in the queue
        std::vector<Token*> readyTokens; // tokens that are ready for consumption

        void assignPositions();
    };
}

#endif
