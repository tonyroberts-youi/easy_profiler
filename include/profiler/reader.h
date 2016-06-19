/**
Lightweight profiler library for c++
Copyright(C) 2016  Sergey Yagovtsev

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef PROFILER_READER____H
#define PROFILER_READER____H

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <map>
#include "profiler/profiler.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct BlocksTree
{
    typedef std::vector<BlocksTree> children_t;

    profiler::SerilizedBlock* node;
    children_t children;

    BlocksTree():node(nullptr)
	{

    }

    BlocksTree(BlocksTree&& that)
    {
        makeMove(std::forward<BlocksTree&&>(that));
    }

    BlocksTree& operator=(BlocksTree&& that)
    {
        makeMove(std::forward<BlocksTree&&>(that));
        return *this;
    }

    ~BlocksTree()
    {
        if (node)
        {
            delete node;
        }
        node = nullptr;
    }

    bool operator < (const BlocksTree& other) const
    {
        if (!node || !other.node)
        {
            return false;
        }
        return node->block()->getBegin() < other.node->block()->getBegin();
    }

private:
    void makeMove(BlocksTree&& that)
    {
        node = that.node;
        children = std::move(that.children);

        that.node = nullptr;
    }

};


typedef std::map<size_t, BlocksTree> thread_blocks_tree_t;

extern "C"{
	int PROFILER_API fillTreesFromFile(const char* filename, thread_blocks_tree_t& threaded_trees);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // PROFILER_READER____H
