#ifndef TOKEN_H
#define TOKEN_H

#include "DrawableObject.h"
#include <string>

namespace grumpy
{
    class SyntaxParser;

    struct LexToken
    {
		size_t number;
        std::string name;
		size_t start;
        size_t end;
        std::string value;

        LexToken();
        LexToken(size_t num, const std::string &name, size_t start, size_t end, const std::string &value);


    };

    class Token : public puddi::DrawableObject
    {
    public:
        Token(puddi::Object *par, const LexToken &lTok, SyntaxParser *pars);

        Token(puddi::Object *par, const LexToken &lTok, SyntaxParser *pars, puddi::SchematicNode *schematic);

        LexToken LToken;

        Token* GetNext() const;
        void SetNext(Token *t);

        Token* GetPrevious() const;
        void SetPrevious(Token *t);

        void SetVelocity(float v);

        void Consume();

        void Update();

		glm::vec4 GetTokenConnectionHead() const;
        glm::vec4 GetTokenConnectionTail() const;

    private:
        SyntaxParser *parser;
        Token *next;
        Token *previous;
        float velocity;
		float rotationVelocity;
        bool addedToParser;

        void init(const LexToken &lTok, SyntaxParser *pars);

		void rotateToward(const glm::vec4 &point);
    };
}

#endif
