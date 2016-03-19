#ifndef UPDATEGRAPH_H
#define UPDATEGRAPH_H

#include <vector>

namespace puddi
{
	class Object;

	struct UpdateNode
	{
		UpdateNode *parent;
		std::vector<UpdateNode*> children;
		Object *object;
		bool parallel;

		UpdateNode(UpdateNode *par, Object *o);

		~UpdateNode();

		void Update();

		void AddChild(UpdateNode *child);

		void RemoveChild(UpdateNode *child);
	};
}

#endif
