#ifndef LEXER_H
#define LEXER_H

#include "DrawableObject.h"
#include "Token.h"
#include <vector>

namespace grumpy
{
    class SourceCode;
	class SyntaxParser;

    enum LexerState : int
    {
		LEXER_STATE_WAITING,
        LEXER_STATE_SKIPPING,
        LEXER_STATE_READING
    };

    class Lexer : public puddi::DrawableObject
    {
    public:
        Lexer(Object* par, SourceCode *code, const std::vector<LexToken> &lToks);

        ~Lexer();

        void Update();

        float GetSkipVelocity() const;
        void SetSkipVelocity(float v);

        float GetReadVelocity() const;
        void SetReadVelocity(float v);

		void SetParser(SyntaxParser *pars);

		void Lex();

    private:
        SourceCode *sourceCode;
		SyntaxParser *parser;
        std::vector<LexToken> lTokens;
        std::vector<Token*> tokensProduced;
        size_t currentCharacterIndex;
		size_t currentTokenIndex;
        glm::vec4 currentTokenStartPos;

        float skipVelocity;
        float readVelocity;

        LexerState state;

        glm::vec4 scanBarColor;
        DrawableObject *scanBar;

		void produceToken(size_t glyphBegin, size_t glyphEnd);

		glm::vec4 chooseTokenColor(const LexToken& t);
    };
}

#endif
