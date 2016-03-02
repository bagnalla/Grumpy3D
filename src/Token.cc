#include "Token.h"
#include "SyntaxParser.h"
#include "FpsTracker.h"
#include "Schematic.h"
#include "Util.h"
#include <iostream>

using namespace puddi;
using namespace std;

namespace grumpy
{
    // LexToken PUBLIC

    LexToken::LexToken() : LexToken(0, "", 0, 0, "") {}

    LexToken::LexToken(size_t num, const std::string &n, size_t s, size_t e, const std::string &v)
    {
        number = num;
        name = n;
        start = s;
        end = e;
        value = v;
    }

    // Token PUBLIC

    Token::Token(Object *par, const LexToken &lTok, SyntaxParser *pars) : DrawableObject(par)
    {
        init(lTok, pars);
    }

    Token::Token(Object *par, const LexToken &lTok, SyntaxParser *pars, SchematicNode *schematic) : DrawableObject(par, schematic)
    {
        init(lTok, pars);
    }

    Token* Token::GetNext() const
    {
        return next;
    }
    void Token::SetNext(Token *t)
    {
        next = t;
    }

    Token* Token::GetPrevious() const
    {
        return previous;
    }
    void Token::SetPrevious(Token *t)
    {
        previous = t;
    }

    void Token::SetVelocity(float v)
    {
        velocity = v;
    }

    void Token::Consume()
    {
        if (previous != nullptr)
            previous->SetNext(next);
        if (next != nullptr)
            next->SetPrevious(previous);
    }

    void Token::Update()
    {
//		Object *target;
//		if (next != nullptr)
//			target = next;
//		else
//			target = parser;
//
//		float moveAmount = velocity * FpsTracker::GetFrameTimeMs();
//
//		vec4 displacement = target->GetPosition() - GetPosition();
//		float len = Util::Length(displacement);
//		if (len <= scale.x / 2.0f + target->GetScaleX() / 2.0f)
//		{
//            if (len == 0.0f)
//                displacement = vec4(1.0f, 0.0f, 0.0f, 0.0f);
//			SetPosition(target->GetPosition() + Util::Normalize(-displacement) * (scale.x / 2.0f + target->GetScaleX() / 2.0f));
//
//			if (!addedToParser)
//			{
//				addedToParser = true;
//				parser->AddToken(this);
//			}
//		}
//		else
//		{
//            vec4 oldPos = position;
//			Translate(Util::Normalize(displacement) * moveAmount);
//			if(Util::Length(target->GetPosition() - GetPosition()) == 0.0f)
//                SetPosition(target->GetPosition() + Util::Normalize(target->GetPosition() - oldPos) * (scale.x / 2.0f + target->GetScaleX() / 2.0f));
//        }
//
//		LookAt(target->GetPosition());
    }

	vec4 Token::GetTokenConnectionHead() const
	{
		return parentModel * model * vec4(scale.x / 2.0f, 0.0f, 0.0f, 1.0f);
	}

    vec4 Token::GetTokenConnectionTail() const
    {
        return parentModel * model * vec4(-scale.x / 2.0f, 0.0f, 0.0f, 1.0f);
    }

    // PRIVATE

    void Token::init(const LexToken &lTok, SyntaxParser *pars)
    {
        LToken = lTok;
        parser = pars;
        next = nullptr;
        previous = nullptr;
        velocity = 1.0f;
		rotationVelocity = 1.0f;
        addedToParser = false;

        addedToParser = true;
        parser->AddToken(this);
    }

	void Token::rotateToward(const vec4 &point)
	{
		vec4 displacement = point - position;

		vec4 lookDirection = finalModel * vec4(1.0f, 0.0f, 0.0f, 0.0f);

		vec3 crossProduct = cross(vec3(lookDirection.x, lookDirection.y, 0.0f), vec3(displacement.x, displacement.y, 0.0f));

		if (crossProduct.z > 0.0f)
			RotateZ(rotationVelocity * FpsTracker::GetFrameTimeMs());
		else
			RotateZ(-rotationVelocity * FpsTracker::GetFrameTimeMs());

		//SetRotationZ(atan2(-displacement.y, displacement.x));
		//SetRotationY(atan2(displacement.z, Util::Length(vec2(displacement))));
	}
}
