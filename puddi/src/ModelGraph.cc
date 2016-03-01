#include "ModelGraph.h"
#include "Object.h"
#include <algorithm>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <iostream>

namespace puddi
{
	// PUBLIC

	ModelNode::ModelNode(ModelNode *par, Object *o)
	{
		parent = par;
		object = o;
		parallel = false;

		if (parent != NULL)
			parent->AddChild(this);
	}

	ModelNode::~ModelNode()
	{
		//for (auto it = children.begin(); it != children.end(); ++it)
		//	delete *it;

		for (; 0 < children.size();)
			delete children[0];

		if (parent != NULL)
			parent->RemoveChild(this);
	}

	void ModelNode::Update()
	{
		if (!object->GetCulled())
		{
			if (object->UpdateModel())
			{
				//concurrency::parallel_for_each(begin(children), end(children), [&](ModelNode *child)
				//{
					//object->PassDownParentModel(child->object);
					//child->Update();
				//}, concurrency::static_partitioner());
				
				//__gnu_parallel::for_each(children.begin(), children.end(), [&](ModelNode *child)
				//{
					////std::cout << "threads = " << omp_get_num_threads() << std::endl;
					//object->PassDownParentModel(child->object);
					//child->Update();
				//});
				if (parallel)
				{
					#pragma omp parallel for
					for (int i = 0; i < children.size(); ++i)
					{
						//std::cout << "i = " << i << ", threads = " << omp_get_num_threads() << std::endl;
						object->PassDownParentModel(children[i]->object);
						children[i]->Update();
					}
				}
				else
				{
					for (auto it = children.begin(); it != children.end(); ++it)
					{
						auto child = *it;
						object->PassDownParentModel(child->object);
						child->Update();
					}
				}
			}
			else
			{
				if (parallel)
				{
					#pragma omp parallel for
					for (int i = 0; i < children.size(); ++i)
					{
						children[i]->Update();
					}
				}
				else
				{
					for (auto it = children.begin(); it != children.end(); ++it)
						(*it)->Update();
				}
			}
		}
	}

	void ModelNode::AddChild(ModelNode *child)
	{
		children.push_back(child);
	}

	void ModelNode::RemoveChild(ModelNode *child)
	{
		auto it = std::find(children.begin(), children.end(), child);
		if (it != children.end())
			children.erase(it);
	}
}
