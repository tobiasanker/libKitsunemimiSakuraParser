﻿/**
 * @file        sakura_items.h
 *
 * @author      Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright   Apache License Version 2.0
 *
 *      Copyright 2019 Tobias Anker
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#ifndef SAKURA_ITEMS_H
#define SAKURA_ITEMS_H

#include <vector>
#include <string>

#include <libKitsunemimiSakuraParser/value_item_map.h>

namespace Kitsunemimi
{
class DataItem;
class DataMap;
class DataBuffer;

namespace Sakura
{

//===================================================================
// SakuraItem
//===================================================================
class SakuraItem
{
public:
    enum ItemType
    {
        UNDEFINED_ITEM = 0,
        BLOSSOM_ITEM = 1,
        BLOSSOM_GROUP_ITEM = 2,
        TREE_ITEM = 4,
        SUBTREE_ITEM = 5,
        SEED_ITEM = 6,
        SEED_TRIGGER_ITEM = 7,
        SEQUENTIELL_ITEM = 8,
        PARALLEL_ITEM = 9,
        IF_ITEM = 10,
        FOR_EACH_ITEM = 11,
        FOR_ITEM = 12,
        SEED_PART = 13
    };

    SakuraItem();
    virtual ~SakuraItem();
    virtual SakuraItem* copy() = 0;

    ItemType getType() const;
    ValueItemMap values;

protected:
    ItemType type = UNDEFINED_ITEM;
};

//===================================================================
// BlossomItem
//===================================================================
class BlossomItem : public SakuraItem
{
public:
    BlossomItem();
    ~BlossomItem();
    SakuraItem* copy();

    std::string blossomType = "";

    std::string blossomName = "";
    std::string blossomGroupType = "";
    std::string blossomPath = "";

    DataItem* blossomOutput = nullptr;
    DataMap* parentValues = nullptr;

    // result
    std::vector<std::string> nameHirarchie;
    bool skip = false;
    bool success = true;
    std::string outputMessage = "";
};

//===================================================================
// BlossomGroupItem
//===================================================================
class BlossomGroupItem : public SakuraItem
{
public:
    BlossomGroupItem();
    ~BlossomGroupItem();
    SakuraItem* copy();

    std::string id = "";
    std::string blossomGroupType = "";
    std::vector<std::string> nameHirarchie;

    std::vector<BlossomItem*> blossoms;
};

//===================================================================
// SeedPart
//===================================================================
class SeedPart : public SakuraItem
{
public:
    SeedPart();
    ~SeedPart();
    SakuraItem* copy();

    std::string id = "";
};

//===================================================================
// SeedTriggerItem
//===================================================================
class SeedTriggerItem : public SakuraItem
{
public:
    SeedTriggerItem();
    ~SeedTriggerItem();
    SakuraItem* copy();

    std::string treeId = "";
    std::string tag = "";
};

//===================================================================
// SeedInitItem
//===================================================================
class SeedInitItem : public SakuraItem
{
public:
    SeedInitItem();
    ~SeedInitItem();
    SakuraItem* copy();

    std::string id = "";
    std::vector<SeedPart*> childs;
};

//===================================================================
// TreeItem
//===================================================================
class TreeItem : public SakuraItem
{
public:
    TreeItem();
    ~TreeItem();
    SakuraItem* copy();

    std::string id = "";

    std::string unparsedConent = "";
    std::string relativePath = "";
    std::string rootPath = "";

    SakuraItem* childs;
};

//===================================================================
// SubtreeItem
//===================================================================
class SubtreeItem : public SakuraItem
{
public:
    SubtreeItem();
    ~SubtreeItem();
    SakuraItem* copy();

    std::string nameOrPath = "";
    std::map<std::string, ValueItemMap> internalSubtrees;

    DataMap* parentValues = nullptr;

    // result
    std::vector<std::string> nameHirarchie;
};

//===================================================================
// IfBranching
//===================================================================
class IfBranching : public SakuraItem
{
public:
    enum compareTypes {
        EQUAL = 0,
        GREATER_EQUAL = 1,
        GREATER = 2,
        SMALLER_EQUAL = 3,
        SMALLER = 4,
        UNEQUAL = 5
    };

    IfBranching();
    ~IfBranching();
    SakuraItem* copy();

    ValueItem leftSide;
    compareTypes ifType = EQUAL;
    ValueItem rightSide;

    SakuraItem* ifContent = nullptr;
    SakuraItem* elseContent = nullptr;
};

//===================================================================
// ForEachBranching
//===================================================================
class ForEachBranching : public SakuraItem
{
public:
    ForEachBranching();
    ~ForEachBranching();
    SakuraItem* copy();

    std::string tempVarName = "";
    ValueItemMap iterateArray;
    bool parallel = false;

    SakuraItem* content = nullptr;
};

//===================================================================
// ForBranching
//===================================================================
class ForBranching : public SakuraItem
{
public:
    ForBranching();
    ~ForBranching();
    SakuraItem* copy();

    std::string tempVarName = "";
    ValueItem start;
    ValueItem end;
    bool parallel = false;

    SakuraItem* content = nullptr;
};

//===================================================================
// SequentiellPart
//===================================================================
class SequentiellPart : public SakuraItem
{
public:
    SequentiellPart();
    ~SequentiellPart();
    SakuraItem* copy();

    std::vector<SakuraItem*> childs;
};

//===================================================================
// ParallelPart
//===================================================================
class ParallelPart : public SakuraItem
{
public:
    ParallelPart();
    ~ParallelPart();
    SakuraItem* copy();

    SakuraItem* childs;
};

//===================================================================
// SakuraGarden
//===================================================================
class SakuraGarden
{
public:
    SakuraGarden();
    ~SakuraGarden();

    std::string rootPath = "";
    std::map<std::string, TreeItem*> trees;
    std::map<std::string, std::string> templates;
    std::map<std::string, Kitsunemimi::DataBuffer*> files;

    TreeItem* getTreeById(const std::string id);
    TreeItem* getTreeByPath(const std::string relativePath);
    const std::string getTemplate(const std::string relativePath);
    DataBuffer* getFile(const std::string relativePath);
};

}
}

#endif // SAKURA_ITEMS_H
