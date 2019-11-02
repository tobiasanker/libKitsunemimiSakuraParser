/**
 *  @file    sakura_parser_test.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "sakura_parser_test.h"

#include <sakura_converter.h>
#include <test_strings/branch_test_string.h>
#include <test_strings/forest_test_string.h>
#include <test_strings/tree_test_string.h>

#include <common_items/data_items.h>

using Kitsunemimi::Common::DataItem;
using Kitsunemimi::Common::DataArray;
using Kitsunemimi::Common::DataValue;
using Kitsunemimi::Common::DataMap;

namespace Kitsunemimi
{
namespace Sakura
{

ParsingTest::ParsingTest() : Kitsunemimi::Common::Test("ParsingTest")
{
    initTestCase();
    parseBranchTest();
    parseTreeTest();
    parseForestTest();
    cleanupTestCase();
}

void ParsingTest::initTestCase()
{
    m_parser = new SakuraConverter(false);
}

void ParsingTest::parseBranchTest()
{
    std::pair<DataItem*, bool> result = m_parser->parse(testBranchString);
    TEST_EQUAL(result.second, true);
    std::string output = result.first->toString(true);
    //std::cout<<"output: "<<output<<std::endl;;

}

void ParsingTest::parseTreeTest()
{
    std::pair<DataItem*, bool> result = m_parser->parse(testTreeString);
    TEST_EQUAL(result.second, true);
    std::string output = result.first->toString(true);
    //std::cout<<"output: "<<output<<std::endl;;
}

void ParsingTest::parseForestTest()
{
    std::pair<DataItem*, bool> result = m_parser->parse(testForestString);
    TEST_EQUAL(result.second, true);
    std::string output = result.first->toString(true);
    //std::cout<<"output: "<<output<<std::endl;;
}

void ParsingTest::cleanupTestCase()
{
    delete m_parser;
}

}  // namespace Sakura
}  // namespace Kitsunemimi
