#ifndef TEXTWRAP_H
#define TEXTWRAP_H


#include <string>
#include <vector>
#include <fstream>


class txtWrap {
  protected:
    std::string wrpChar;
    std::vector<std::string> text;
    std::ifstream rAA;
    virtual void setChar (std::string) = 0;
    virtual void setText (std::vector<std::string> txt) = 0;
    virtual std::vector<std::string> wrapImpl () = 0;
    virtual std::vector<std::string> AArt () = 0;
};


#endif
