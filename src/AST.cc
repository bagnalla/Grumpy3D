#include "AST.h"
#include "Texture.h"
#include "Cube.h"
#include "FpsTracker.h"
#include "Schematic.h"
#include <iostream>
#include <string>

using namespace puddi;
using namespace std;

namespace grumpy
{
	// PUBLIC

	ASTNode::ASTNode(Object *par, ASTNode *parentNode, DrawableObject *connector, queue<char> &input) : DrawableObject(par)
	{
        if (!input.size() || input.front() != '[')
        {
            cerr << "ASTNode constructor: expected '[' in first character of input.\n";
            return;
        }
        input.pop();

        if (parentNode != nullptr)
            parseIndex = parentNode->parseIndex;
        parent = parentNode;
        parentConnector = connector;
        parentConnectorLine = nullptr;
        SetPosition(vec4(0.0f, 0.0f, 0.0f, 1.0f));
		velocity = 0.05f;
		//velocity = 100.0f;

        container = new DrawableObject(this);
        glyphCount = 0;
        hidden = false;

        while (true)
        {
            char c = input.front();

            if (c == '\t' || c == '\r' || c == '\n')
            {
                input.pop();
                continue;
            }

            if (c == '`')
            {
                input.pop();
                string index_string = "";
                while ((c = input.front()) != '`')
                {
                    index_string += c;
                    input.pop();
                }

                parseIndex = atoi(index_string.c_str());

                //cout << parseIndex << endl;

                input.pop();
                continue;
            }

            if (c == '~')
            {
                input.pop();
                string index_string = "";
                while ((c = input.front()) != '~')
                {
                    index_string += c;
                    input.pop();
                }

                requiredTokenNumbers.push_back(atoi(index_string.c_str()));

                input.pop();
                continue;
            }

            //cout << c;

            if (c == ']')
                break;

            if (c == '[')
            {
                //DrawableObject *conn = new Cube(glyphContainer);
                //DrawableObject *conn = new Cube(container);
				DrawableObject *conn = new DrawableObject(container, Schematic::GetSchematicByName("rounded_cube"));
                conn->Scale(0.9f);
                conn->SetScaleX(1.9f);
                conn->RotateX(M_PI / 2.0f);
                conn->Translate(vec4(glyphCount + 1.0f, 0.0f, 0.0f, 0.0f));
                //conn->SetEmissive(true);
                //conn->SetEmissionColor(vec4(1.0f, 1.0f, 1.0f, 0.5f));
                //conn->SetBumpMap(Texture::GetBumpMapByName("rough5"));
                conn->SetRenderGraph(2);
                childConnectors.push_back(conn);

                glyphCount += 2;

                auto node = new ASTNode(container, this, conn, input);
                //node->Translate(vec4(0.0f, 0.0f, -1.0f, 0.0f));
                //node->Translate(conn->GetPosition() + vec4(0.0f, 0.0f, -1.0f, 0.0f));
                ChildNodes.push_back(node);
				//node->SetPosition(conn->GetPosition());
				node->SetPosition(conn->GetPosition() + vec4(0.0f, -2.0f, 0.0f, 0.0f));
            }
            else
            {
                if (c != ' ')
                {
                    //cout << c << endl;
                    DrawableObject *glyph = new DrawableObject(container, VertexMesh::GetVertexMeshPrototypeByName(string(1, c)));
                    glyph->SetTexture(Texture::GetTextureByName("myfont"));
                    glyph->RotateX(M_PI / 2.0f);
                    glyph->Translate(vec4(glyphCount + 0.5f, 0.0f, 0.0f, 0.0f));
                    glyphs.push_back(glyph);
                }
                glyphCount++;
            }

            input.pop();
        }

        container->Translate(vec4(-glyphCount / 2.0f, 0.0f, 0.0f, 0.0f));

        //width = std::max(resizeRecursive(), glyphCount + 1);
		//resizeRecursive();

        // create connector lines
        for (auto it = ChildNodes.begin(); it != ChildNodes.end(); ++it)
        {
            auto child = *it;
            child->parentConnectorLine = new Cube(child);
            child->parentConnectorLine->Scale(0.1f);
            child->parentConnectorLine->SetScaleX(length(child->position - child->parentConnector->GetPosition()));
            //child->parentConnectorLine->SetPosition((child->position + child->parentConnector->GetPosition()) / 2.0f);
			child->parentConnectorLine->SetPosition(vec4(-child->position.x, -child->position.y, -child->position.z, 1.0f) + (child->position + child->parentConnector->GetPosition()) / 2.0f);
			//child->parentConnectorLine->SetPosition(child->parentConnector->GetPosition() + child->position / 2.0f);
            float theta = atan2(child->parentConnector->GetPosition().z - child->position.z, child->parentConnector->GetPosition().x - child->position.x);
			//float theta = atan2(child->position.z, child->position.x);
            child->parentConnectorLine->SetRotationY(theta);
            child->parentConnectorLine->SetEmissive(true);
            child->parentConnectorLine->SetEmissionColor(vec4(0.5f, 0.0f, 0.25f, 0.4f));
            child->parentConnectorLine->SetRenderGraph(1);
        }

		// create connector line
		//if (parentConnector != NULL)
		//{
		//	parentConnectorLine = new Cube(parent->container);
		//	parentConnectorLine->Scale(0.1f);
		//	parentConnectorLine->SetScaleX(length(position - parentConnector->GetPosition()) / 2.0f);
		//	/*parentConnectorLine->SetPosition(position / 2.0f);
		//	float theta = atan2(position.z, position.x);
		//	parentConnectorLine->SetRotationY(theta);*/
		//	parentConnectorLine->SetEmissive(true);
		//	parentConnectorLine->SetEmissionColor(vec4(0.5f, 0.0f, 0.25f, 0.4f));
		//	parentConnectorLine->SetRenderGraph(1);
		//}

        // create body
        body = new Cube(this);
        //body->Translate(vec4(-0.5f, 0.0f, 0.0f, 0.0f));
        body->SetScaleX(glyphCount);
        body->SetEmissive(true);
        body->SetEmissionColor(vec4(0.25f, 0.25f, 0.25f, 0.25f));
//        if (tokenRequired)
//            body->SetEmissionColor(vec4(0.3f, 0.15f, 0.0f, 0.4f));
//        else
//            body->SetEmissionColor(vec4(0.25f, 0.25f, 0.25f, 0.25f));
        body->SetRenderGraph(3);

        Resize();

        Update();
	}

	int ASTNode::GetWidth() const
	{
        return width;
	}

    void ASTNode::Hide()
    {
        if (hidden)
            return;
        //DisableRender();
        body->DisableRender();
        for (auto it = glyphs.begin(); it != glyphs.end(); ++it)
            (*it)->DisableRender();
        for (auto it = childConnectors.begin(); it != childConnectors.end(); ++it)
            (*it)->DisableRender();
        if (parentConnectorLine != nullptr)
            parentConnectorLine->DisableRender();
        hidden = true;
    }

    void ASTNode::Show()
    {
        if (!hidden)
            return;
//        EnableRender();
//        for (auto it = ChildNodes.begin(); it != ChildNodes.end(); ++it)
//            (*it)->Hide();
        body->EnableRender();
        for (auto it = glyphs.begin(); it != glyphs.end(); ++it)
            (*it)->EnableRender();
        for (auto it = childConnectors.begin(); it != childConnectors.end(); ++it)
            (*it)->EnableRender();
        if (parentConnectorLine != nullptr)
        {
            parentConnectorLine->EnableRender();
            rePositionConnectorLine();
        }
        //if (parent != nullptr)
        //    parent->Show();
        hidden = false;
    }

	void ASTNode::Resize()
	{
		resizeRecursive();
	}

	void ASTNode::Update()
	{
		if (position == assignedLocation || hidden)
			return;

		float moveAmount = velocity * FpsTracker::GetFrameTimeMs();

		MoveToPoint(assignedLocation, moveAmount, nullptr);

		rePositionConnectorLine();
	}

	void ASTNode::SetAssignedLocation(const glm::vec4 &l)
	{
		assignedLocation = l;
	}

    int ASTNode::GetParseIndex() const
    {
        return parseIndex;
    }

    void ASTNode::SetNodeColor(glm::vec4 c)
    {
        //body->SetEmissionColor(c);
		if (parentConnector != nullptr)
			parentConnector->SetMaterial(Material::Vibrant(c));
		//for (auto it = childConnectors.begin(); it != childConnectors.end(); ++it)
		//	(*it)->SetMaterial(Material::Vibrant(c));
    }

	DrawableObject* ASTNode::GetParentConnector() const
	{
		return parentConnector;
	}

	bool ASTNode::GetHidden() const
	{
        return hidden;
	}

	ASTNode* ASTNode::GetParent() const
	{
        return parent;
	}

	std::vector<int> ASTNode::GetRequiredTokenNumbers() const
	{
        return requiredTokenNumbers;
	}

	// PRIVATE

	int ASTNode::resizeRecursive()
	{
		int childrenWidthSum = 0;

		for (auto it = ChildNodes.begin(); it != ChildNodes.end(); ++it)
			childrenWidthSum += (*it)->resizeRecursive(); // recursive call

        float deltaY = hidden ? 0.0f : -3.0f;
		vec4 cursor = vec4(glyphCount / 2.0f - childrenWidthSum / 2.0f, 0.0f, deltaY, 1.0f);
		if (ChildNodes.size())
			cursor.x += ChildNodes[0]->width / 2.0f;
		for (int i = 0; i < ChildNodes.size(); ++i)
		{
			auto child = ChildNodes[i];
			child->assignedLocation = cursor;
			//child->SetPosition(cursor);
			//child->rePositionConnectorLine();
			cursor.x += child->width / 2.0f;
			if (i < ChildNodes.size() - 1)
				cursor.x += ChildNodes[i + 1]->width / 2.0f;
		}

        int personalWidth = hidden ? 0 : glyphCount + 1;

		return width = std::max(childrenWidthSum, personalWidth);
	}

	void ASTNode::rePositionConnectorLine()
	{
		if (parentConnectorLine == nullptr)
			return;

        if (parent->hidden)
            parentConnectorLine->DisableRender();
        else if (!parentConnectorLine->GetRenderEnabled())
            parentConnectorLine->EnableRender();

		parentConnectorLine->SetScaleX(length(position - parentConnector->GetPosition()));
		parentConnectorLine->SetPosition(vec4(-position.x, -position.y, -position.z, 1.0f) + (position + parentConnector->GetPosition()) / 2.0f);
		float theta = atan2(parentConnector->GetPosition().z - position.z, parentConnector->GetPosition().x - position.x);
		parentConnectorLine->SetRotationY(theta);

//		theta = atan2(parentConnector->GetPosition().y - position.y, parentConnector->GetPosition().x - position.x);
//		parentConnectorLine->SetRotationZ(theta);
	}
}
