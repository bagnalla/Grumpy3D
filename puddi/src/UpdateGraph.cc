#include "UpdateGraph.h"
#include "Object.h"
#include <algorithm>

#ifdef _OPENMP
    #include <omp.h>
#endif

#include <iostream>

namespace puddi
{
	// PUBLIC

	UpdateNode::UpdateNode(UpdateNode *par, Object *o)
	{
		parent = par;
		object = o;
		parallel = false;

		if (parent != NULL)
			parent->AddChild(this);
	}

	UpdateNode::~UpdateNode()
	{
		for (size_t i = 0; i < children.size(); ++i)
				delete children[i];

		if (parent != NULL)
			parent->RemoveChild(this);
	}

	void UpdateNode::Update()
	{
		object->Update();

		if (parallel)
		{
			// update children in parallel
			//concurrency::parallel_for_each(begin(children), end(children), [&](UpdateNode *child)
			//{
				//child->Update();
			//}, concurrency::static_partitioner());

			//__gnu_parallel::for_each(begin(children), end(children), [&](UpdateNode *child)
			//{
				////std::cout << "threads = " << omp_get_num_threads() << std::endl;
				//child->Update();
			//});

			#pragma omp parallel for
			for (int i = 0; i < static_cast<int>(children.size()); ++i)
			{
				//if (omp_get_num_threads() != 1)
				//	std::cout << "i = " << i << ", threads = " << omp_get_num_threads() << std::endl;
				auto child = children[i];
				if (child != nullptr)
					child->Update();
			}
		}
		else
		{
			// update children sequentially
			for (size_t i = 0; i < children.size(); ++i)
			{
				auto child = children[i];
				if (child != nullptr)
					child->Update();
			}
		}

		// remove null or dead children
		for (size_t i = 0; i < children.size();)
		{
			if (children[i] == nullptr)
				children.erase(children.begin() + i);
			else
				++i;
		}
	}

	void UpdateNode::AddChild(UpdateNode *child)
	{
		children.push_back(child);
	}

	void UpdateNode::RemoveChild(UpdateNode *child)
	{
        for (size_t i = 0; i < children.size(); ++i)
        {
            if (children[i] == child)
                children[i] = nullptr;
        }
	}
}
