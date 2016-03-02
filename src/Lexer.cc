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
#include <iostream>

using namespace puddi;
using namespace std;

namespace grumpy
{
    // PUBLIC

    Lexer::Lexer(Object* par, SourceCode *code, const vector<LexToken> &lToks) : DrawableObject(par)
    {
        sourceCode = code;
        lTokens = lToks;
        currentCharacterIndex = 0;
        currentTokenIndex = 0;
        currentTokenStartPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        skipVelocity = 0.0f;
        readVelocity = 0.0f;
        state = LEXER_STATE_WAITING;

        scanBarColor = vec4(1.0f, 1.0f, 0.0f, 0.0f);

        scanBar = new Cube(this);
        scanBar->RotateX(static_cast<float>(M_PI / 2.0));
        scanBar->SetEmissive(true);
        scanBar->SetEmissionColor(scanBarColor);
        scanBar->Scale(1.0f);
        scanBar->SetRenderGraph(2);
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

            vec4 targetPosition = targetGlyph->GetWorldPosition() - vec4(targetGlyph->GetScaleX() / 2.0f, 0.0f, 0.0f, 0.0f);
            //vec4 targetPosition = targetGlyph->GetPosition();

            float moveAmount = skipVelocity * FpsTracker::GetFrameTimeMs();

            vec4 displacement = targetPosition - position;
            if (Util::Length(displacement) <= moveAmount)
            {
                SetPosition(targetPosition);

                if (currentCharacterIndex == currentToken.start)
                {
                    cout << "reached token " << currentTokenIndex << " at pos " << currentCharacterIndex << ". entering read state until pos " << currentToken.end << endl;
                    currentTokenStartPos = targetPosition;
                    scanBarColor.w = 0.5f;
                    scanBar->SetEmissionColor(scanBarColor);
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
            scanBar->SetPosition(vec4(-scanBar->GetScaleX() / 2.0f, 0.0f, 0.0f, 1.0f));

            DrawableObject *targetGlyph = sourceCode->glyphs[currentToken.end];

            vec4 targetPosition = targetGlyph->GetWorldPosition() + vec4(targetGlyph->GetScaleX() / 2.0f, 0.0f, 0.0f, 0.0f);
            //vec4 targetPosition = targetGlyph->GetPosition();

            float moveAmount = readVelocity * FpsTracker::GetFrameTimeMs();

			displacement = targetPosition - position;
            if (Util::Length(displacement) <= moveAmount)
            {
                SetPosition(targetPosition);

                cout << "reached end of token" << currentTokenIndex << ". entering skip state" << endl;
                scanBarColor.w = 0.0f;
                scanBar->SetEmissionColor(scanBarColor);
                scanBar->SetScaleX(0.0f);
                scanBar->SetPosition(vec4(0.0f, 0.0f, 0.0f, 1.0f));
                state = LEXER_STATE_WAITING;

                for (;currentCharacterIndex <= currentToken.end; currentCharacterIndex++)
                {
                    sourceCode->glyphs[currentCharacterIndex]->Cull();
                }

				produceToken();

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
			produceToken();
        else
            state = LEXER_STATE_SKIPPING;
	}

    // PRIVATE

	void Lexer::produceToken()
	{
		Token *t = new Token(Puddi::GetRootObject(), lTokens[currentTokenIndex], parser, Schematic::GetSchematicByName("rounded_cube"));
		Token *parserTokenTail = parser->GetTokenTail();
		t->SetNext(parserTokenTail);
		if (parserTokenTail != nullptr)
			parserTokenTail->SetPrevious(t);
		//t->SetVelocity(0.1f);
		t->SetVelocity(0.2f);
		t->SetMaterial(Material::Medium(vec4(0.0f, 1.0f, 1.0f, 1.0f)));
		t->SetBumpMap(Texture::GetBumpMapByName("rough4"));
		t->SetPosition(position);
		Puddi::ForceModelUpdate();
		tokensProduced.push_back(t);
	}
}
