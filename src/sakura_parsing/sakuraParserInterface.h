/**
 *  @file    sakuraParserInterface.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef JINJA2PARSERINTERFACE_HH
#define JINJA2PARSERINTERFACE_HH

#include <vector>
#include <string>
#include <data_structure/dataItems.hpp>

#include <iostream>

namespace Kitsune
{
namespace Sakura
{
class location;

class SakuraParserInterface
{

public:
    SakuraParserInterface(const bool traceParsing);

    // connection the the scanner and parser
    void scan_begin(const std::string &inputString);
    void scan_end();
    bool parse(const std::string &inputString);

    // output-handling
    void setOutput(Common::DataItem* output);
    Common::DataItem* getOutput() const;

    // Error handling.
    void error(const Kitsune::Sakura::location &location,
               const std::string& message);
    Common::DataItem* getErrorMessage() const;

    std::string removeQuotes(std::string input);

private:
    bool m_traceParsing = false;

    std::string m_inputString = "";
    Common::DataItem* m_output = nullptr;
    Common::DataItem* m_errorMessage = nullptr;
};

}  // namespace Sakura
}  // namespace Kitsune

#endif // JINJA2PARSERINTERFACE_HH
