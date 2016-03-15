#ifndef TOKEN_H
#define TOKEN_H

#include "DrawableObject.h"
#include <string>

namespace grumpy
{
    class TokenQueue;
    class SyntaxParser;

    /** \brief A struct to contain the token information produced by the actual Grumpy lexer.
     * These are what are produced when reading in the lexer.in file. Each drawable Token object
     * inherits the properties of a LexToken.
     */
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

    /** \brief Token object. Produced by a Lexer and consumed by a SyntaxParser.
     */
    class Token : public puddi::DrawableObject
    {
    public:
        Token(puddi::Object *par, const LexToken &lTok, TokenQueue *tq, SyntaxParser *pars);

        Token(puddi::Object *par, const LexToken &lTok, TokenQueue *tq, SyntaxParser *pars, puddi::SchematicNode *schematic);

        LexToken LToken;

//        Token* GetNext() const;
//        void SetNext(Token *t);
//
//        Token* GetPrevious() const;
//        void SetPrevious(Token *t);

        void SetVelocity(float v);

        void SetTargetPosition(const glm::vec4 p);

        void Consume();

        void Update();

        void CreateGlyphs(std::string font);

//		glm::vec4 GetTokenConnectionHead() const;
//      glm::vec4 GetTokenConnectionTail() const;

    private:
        TokenQueue *tokenQueue;
        SyntaxParser *parser;
        float velocity;
		float rotationVelocity;
		std::vector<DrawableObject*> glyphs;
		glm::vec4 targetPosition;
		bool reachedTargetPosition;

        void init(const LexToken &lTok, TokenQueue *tq, SyntaxParser *pars);

		void rotateToward(const glm::vec4 &point);
    };
}

#endif
