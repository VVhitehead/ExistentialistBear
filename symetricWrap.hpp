#ifndef SYMETRICWRAP_H
#define SYMETRICWRAP_H

#include <iostream>
#include <vector>
#include <regex>
#include <sstream>
#include <fstream>
#include "textwrap.hpp"

class symtrcWrap : txtWrap {
  public:
    void setChar (std::string c) { wrpChar = c; }
    virtual void setText (std::vector<std::string> txt) { text = txt; }
    int utf8len(std::string s) {
      int len = 0;
      for (auto & element : s) {
        len += (element & 0xc0) != 0x80;
      }
      return len;
    }
    std::string center(const std::string& s, const int w) {
      std::stringstream ss, spaces;
      int pad = w - utf8len(s); // count excess room to pad
      for (int i = 0; i < pad / 2; ++i)
        spaces << " ";
      if (w % 2 == 0) { ss << " "; } // if width is not odd, add 1 more space
      ss << spaces.str() << s << spaces.str(); // format with padding
      if (pad > 0 && pad % 2 != 0) { ss << " "; } // if pad odd #, add 1 more space
      return ss.str();
    }
    std::vector<std::string> wrapImpl () {
      int width = utf8len(text[0]), height = text.size(), tmp, half;
      std::string cloudS, strtend, conn;
      std::vector<std::string> cloud;
      for (int i = 0; i < text.size(); ++i) {
        tmp = utf8len(text[i]);
        if (tmp > width) {
          width = tmp;
        }
      }
      half = height / 2;
      if (height % 2 != 0) {
        cloudS = std::string((half + 1), ' ');
        conn = cloudS + " \\";
        for (int i = 0; i < width + 1; ++i) {
          i % 2 == 0 ? cloudS += wrpChar : cloudS += ' ';
        }
        strtend = cloudS;
        cloud.push_back(strtend);
        for (int j = height - 1, i = 0; i < height; ++i) {
          if (i < height / 2 + 1) {
            cloudS = std::string(half - i, ' ') + wrpChar + center(text[i], (width + 2 * i)) + wrpChar;
            cloud.push_back(cloudS);
          }
          else {
            j -= 2;
            cloudS = std::string(i - half, ' ') + wrpChar + center(text[i], (width + j)) + wrpChar;
            cloud.push_back(cloudS);
          }
        }
        cloud.push_back(strtend);
        cloud.push_back(conn);
        conn = std::string(half, ' ') + "   \\" + std::string(12 - half, ' ') + "__";
        cloud.push_back(conn);
        return cloud;
      }
      cloudS = std::string(half + 1, ' ');
      for (int i = 0; i < width + 1; ++i) {
        i % 2 == 0 ? cloudS += wrpChar : cloudS += ' ';
      }
      cloud.push_back(cloudS);
      for (int j = height, i = 0; i < height; ++i) {
        if (i < height / 2 + 1) {
          cloudS = std::string(half - i, ' ') + wrpChar + center(text[i], (width + 2 * i)) + wrpChar;
          cloud.push_back(cloudS);
        }
        else {
          j -= 2;
          cloudS = std::string(i - half, ' ') + wrpChar + center(text[i], width + j) + wrpChar;
          cloud.push_back(cloudS);
        }
      }
      strtend = std::string(half, ' ');
      cloudS = strtend + " \\";
      for (int i = 0; i < width + 3; ++i) {
        i % 2 == 0 ? strtend += wrpChar : strtend += ' ';
      }
      cloud.push_back(strtend);
      cloud.push_back(cloudS);
      cloudS = std::string(half, ' ') + "  \\" + std::string(13 - half, ' ') + "__";
      cloud.push_back(cloudS);
      return cloud;
    }
    std::vector<std::string> AArt() {
      std::vector<std::string> AA;
      std::string line;
      rAA.exceptions(std::ifstream::badbit);
      try {
        rAA.open("Data/Bear");
        while(!rAA.eof()) {
          getline(rAA, line);
          AA.push_back(line);
        }
      }
      catch (std::ifstream::failure e) {
        std::cerr << "Failure reading/opening file" << std::endl;
      }
      return AA;
    }
};

#endif
