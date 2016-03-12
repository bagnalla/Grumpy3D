#ifndef SYNTAXPARSER_H
#define SYNTAXPARSER_H

#include "DrawableObject.h"
#include <vector>
#include <queue>

namespace grumpy
{
	class Lexer;
	class ASTNode;
	class Token;
	class TypeChecker;

    enum SyntaxParserState : int
    {
        SYNTAXPARSER_STATE_WAITING,
        SYNTAXPARSER_STATE_MOVING,
        SYNTAXPARSER_STATE_DONE
    };

    /** \brief Syntax parser object. "Constructs" a given AST by calling Lex() on a Lexer
     * object and consuming Token objects to display and position the nodes of the AST.
     */
    class SyntaxParser : public puddi::DrawableObject
    {
    public:
		SyntaxParser(Object* par, Lexer *lex, ASTNode *root);
		SyntaxParser(Object* par, Lexer *lex, ASTNode *root, puddi::SchematicNode *schematic);

        void Update();

        float GetVelocity() const;
        void SetVelocity(float v);

		void SetHomePosition(glm::vec4 v);

		void AddToken(Token *t);

		void SetTypeChecker(TypeChecker *tc);

		Token* GetTokenTail();

    private:
		Lexer *lexer;
		TypeChecker *typeChecker;
        ASTNode *astRoot;
        std::vector<ASTNode*> nodesVector;
        size_t currentNodeIndex;
        float velocity;
        SyntaxParserState state;
        glm::vec4 homePosition;

        std::vector<Token*> tokenQueue;

		void init(Lexer *lex, ASTNode *root);
        void createNodesVector();
        void addToNodesVectorRecursive(ASTNode *node);

        void finishParsing();
    };
}

#endif
