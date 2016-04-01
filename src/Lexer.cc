#include "Lexer.h"
#include "SourceCode.h"
#include "GlmHeaders.h"
#include "FpsTracker.h"
#include "Rectangle.h"
#include "Cube.h"
#include "SyntaxParser.h"
#include "Util.h"
#include "Puddi.h"
#include "Texture.h"
#include "Schematic.h"
#include "GrumpyConfig.h"
#include "TokenQueue.h"
#include <iostream>
#include <regex>

using namespace puddi;
using namespace std;

namespace grumpy
{
    // PUBLIC

    Lexer::Lexer(Object* par, SourceCode *code, const vector<LexToken> &lToks, TokenQueue *tq) : DrawableObject(par)
    {
        init(code, lToks, tq);
    }

    Lexer::Lexer(Object* par, SourceCode *code, const std::vector<LexToken> &lToks, TokenQueue *tq, SchematicNode *schematic) : DrawableObject(par, schematic)
    {
        init(code, lToks, tq);
    }

    Lexer::~Lexer()
    {
        for (size_t i = 0; i < tokensProduced.size(); ++i)
            delete tokensProduced[i];
    }

    void Lexer::Update()
    {
        if (currentCharacterIndex >= sourceCode->characters.size() || currentTokenIndex >= lTokens.size())
            return;

        // get current token in token stream
        LexToken currentToken = lTokens[currentTokenIndex];

		if (state == LEXER_STATE_WAITING)
		{
		}
        else if (state == LEXER_STATE_SKIPPING)
        {
            // get current glyph in character stream
            DrawableObject *targetGlyph = sourceCode->glyphs[currentCharacterIndex];
            while (targetGlyph == nullptr)
            {
                if (++currentCharacterIndex >= sourceCode->characters.size())
                    return;

                targetGlyph = sourceCode->glyphs[currentCharacterIndex];
            }

            vec4 targetPosition = targetGlyph->GetWorldPosition() - vec4(targetGlyph->GetScaleX() / 2.0f, 0.0f, 0.0f, 0.0f) + vec4(0.0f, 0.0f, scale.z / 2.0f, 0.0f);
            //vec4 targetPosition = targetGlyph->GetPosition();

            float moveAmount = skipVelocity * GrumpyConfig::GetGameSpeedFactor();

            vec4 displacement = targetPosition - position;
            if (Util::Length(displacement) <= moveAmount)
            {
                SetPosition(targetPosition);

                if (currentCharacterIndex == currentToken.start)
                {
                    //cout << "reached token " << currentTokenIndex << " at pos " << currentCharacterIndex << ". entering read state until pos " << currentToken.end << endl;
                    currentTokenStartPos = targetPosition;
                    scanBarColor.w = 0.5f;
                    scanBar->SetEmissionColor(scanBarColor);
                    scanBar->SetPosition(vec4(-scanBar->GetScaleX() / 2.0f, -scale.z / 2.0f, 0.0f, 1.0f));
                    scanBar->UpdateModel();
                    scanBar->UnCull();
                    state = LEXER_STATE_READING;
                }
                else
                {
                    sourceCode->glyphs[currentCharacterIndex]->Cull();
                    currentCharacterIndex++;
                }
            }
            else
            {
                Translate(moveAmount * glm::normalize(displacement));
            }
		}
		else if (state == LEXER_STATE_READING)
		{
            //update scan bar size/position
            vec4 displacement = currentTokenStartPos - position;
            scanBar->SetScaleX(Util::Length(displacement));
            scanBar->SetPosition(vec4(-scanBar->GetScaleX() / 2.0f, -scale.z / 2.0f, 0.0f, 1.0f));

            DrawableObject *targetGlyph = sourceCode->glyphs[currentToken.end];

            vec4 targetPosition = targetGlyph->GetWorldPosition() + vec4(targetGlyph->GetScaleX() / 2.0f, 0.0f, 0.0f, 0.0f) + vec4(0.0f, 0.0f, scale.z / 2.0f, 0.0f);
            //vec4 targetPosition = targetGlyph->GetPosition();

            float moveAmount = readVelocity * GrumpyConfig::GetGameSpeedFactor();

			displacement = targetPosition - position;
            if (Util::Length(displacement) <= moveAmount)
            {
                SetPosition(targetPosition);

                //cout << "reached end of token" << currentTokenIndex << ". entering skip state" << endl;
                scanBarColor.w = 0.0f;
                scanBar->SetEmissionColor(scanBarColor);
                scanBar->SetScaleX(0.0f);
                scanBar->Cull();
                state = LEXER_STATE_WAITING;

                size_t glyphBegin = currentCharacterIndex;
                for (;currentCharacterIndex <= currentToken.end; currentCharacterIndex++)
                {
                    sourceCode->glyphs[currentCharacterIndex]->Cull();
                }

				produceToken(glyphBegin, currentCharacterIndex);

                currentTokenIndex++;
            }
            else
            {
                Translate(moveAmount * glm::normalize(displacement));
            }
		}
		else
		{
            std::cerr << "invalid lexer state\n";
            return;
		}
    }

    float Lexer::GetSkipVelocity() const
    {
        return skipVelocity;
    }
    void Lexer::SetSkipVelocity(float v)
    {
        skipVelocity = v;
    }

    float Lexer::GetReadVelocity() const
    {
        return readVelocity;
    }
    void Lexer::SetReadVelocity(float v)
    {
        readVelocity = v;
    }

	void Lexer::SetParser(SyntaxParser *pars)
	{
		parser = pars;
	}

	void Lexer::Lex()
	{
		if (currentTokenIndex == lTokens.size() - 1)
			produceToken(lTokens.size() - 1, lTokens.size());
        else
            state = LEXER_STATE_SKIPPING;
	}

    LexerState Lexer::GetState() const
    {
        return state;
    }

    // PRIVATE

    void Lexer::init(SourceCode *code, const vector<LexToken> &lToks, TokenQueue *tq)
    {
        sourceCode = code;
        lTokens = lToks;
        tokenQueue = tq;
        currentCharacterIndex = 0;
        currentTokenIndex = 0;
        currentTokenStartPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        skipVelocity = 0.0f;
        readVelocity = 0.0f;
        parser = nullptr;
        state = LEXER_STATE_WAITING;

        scanBarColor = vec4(1.0f, 1.0f, 0.0f, 0.0f);

        scanBar = new Cube(this);
        scanBar->RotateX(static_cast<float>(M_PI / 2.0));
        scanBar->SetEmissive(true);
        scanBar->SetEmissionColor(scanBarColor);
        scanBar->Scale(1.0f);
        scanBar->SetRenderGraph(2);
    }

	void Lexer::produceToken(size_t glyphBegin, size_t glyphEnd)
	{
		Token *t = new Token(engine::GetRootObject(), lTokens[currentTokenIndex], tokenQueue, parser, Schematic::GetSchematicByName("rounded_cube"));
		//t->SetVelocity(0.2f);
		t->SetVelocity(0.01f);
		//t->SetMaterial(Material::Medium(vec4(0.0f, 1.0f, 1.0f, 1.0f)));
		t->SetMaterial(Material::Medium(chooseTokenColor(t->LToken)));
		t->SetBumpMap(Texture::GetBumpMapByName("rough4"));
		//t->SetScaleX((glyphEnd - glyphBegin) / 2.0f);
		t->SetScaleX(lTokens[currentTokenIndex].name.length() / 2.25f);
		t->SetPosition(position - vec4(t->GetScaleX() / 2.0f, 0.0f, 0.0f, 0.0f));
		t->CreateGlyphs(sourceCode->font);
		engine::ForceModelUpdate();
		tokensProduced.push_back(t);
		//parser->AddToken(t);
		tokenQueue->AddToken(t);
	}

	vec4 Lexer::chooseTokenColor(const LexToken& t)
	{
        vector<string> words({ "DEF", "LET", "WHILE", "IF", "THEN", "ELSE", "REF", "IN", "TT" });
        vector<string> types({ "INT", "FLOAT", "BOOL", "UNIT" });
        vector<string> values({ "INTCONST.*", "FLOATCONST.*", "BOOLCONST.*", "ID.*" });
        vector<string> unops({ "NOT", "DEREF" });
        vector<string> binops({ "PLUS", "MINUS", "TIMES", "DIV", "AND", "OR", "LT", "INT_EQ", "DEFEQ" });
        vector<string> separators({ "LPAREN", "RPAREN", "LBRACE", "RBRACE", "SEMI", "EQ", "COLON", "COMMA", "EOF" });

        for (auto it = words.begin(); it != words.end(); ++it)
        {
            std::regex pattern(*it);
            if (std::regex_match(t.name, pattern))
                return vec4(0.5f, 0.5f, 0.5f, 1.0f);
        }

        for (auto it = values.begin(); it != values.end(); ++it)
        {
            std::regex pattern(*it);
            if (std::regex_match(t.name, pattern))
                return vec4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        for (auto it = types.begin(); it != types.end(); ++it)
        {
            std::regex pattern(*it);
            if (std::regex_match(t.name, pattern))
                return vec4(0.0f, 0.0f, 0.75f, 1.0f);
        }

        for (auto it = unops.begin(); it != unops.end(); ++it)
        {
            std::regex pattern(*it);
            if (std::regex_match(t.name, pattern))
                return vec4(0.0f, 0.75f, 0.25f, 1.0f);
        }

        for (auto it = binops.begin(); it != binops.end(); ++it)
        {
            std::regex pattern(*it);
            if (std::regex_match(t.name, pattern))
                return vec4(0.75f, 0.25f, 0.0f, 1.0f);
        }

        for (auto it = separators.begin(); it != separators.end(); ++it)
        {
            std::regex pattern(*it);
            if (std::regex_match(t.name, pattern))
                return vec4(0.5f, 0.25f, 0.75f, 1.0f);
        }

        return vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}
