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
      for (auto & element : s)
        len += (element & 0xc0) != 0x80;
      return len;
    }
    std::string center(const std::string& s, const int w) {
      std::stringstream ss, spaces;
      int pad = w - utf8len(s); // count excess room to pad
      if (pad > 0 && pad % 2 != 0) {
        for (int i = 0; i < pad / 2; ++i)
          ss << ' ';
        ss << s;
        for (int i = 0; i < pad / 2 + 1; ++i)
          ss << ' ';
      }
      else {
        for (int i = 0; i < pad / 2; ++i)
          ss << ' ';
        ss << s;
        for (int i = 0; i < pad / 2; ++i)
          ss << ' ';
      }
      return ss.str();
    }
    std::vector<std::string> wrapImpl () {
      int width = utf8len(text[0]), height = text.size(), tmp, half, longl;
      std::string cloudS, strtend, conn;
      std::vector<std::string> cloud;
      for (int i = 0; i < text.size(); ++i) {
        tmp = utf8len(text[i]);
        if (tmp > width) {
          width = tmp;
          longl = i;
        }
      }
      half = height / 2;
      if (width == utf8len(text.front()) || utf8len(text.back()) == width || text.size() < 3) {
        width % 2 == 0 ? ++width : width;
        if (height % 2 != 0) {
          cloudS = std::string((half + 1), ' ');
          conn = cloudS + " \\";
          for (int i = 0; i < width; ++i)
            i % 2 == 0 ? cloudS += wrpChar : cloudS += ' ';
          strtend = cloudS;
          cloud.push_back(strtend);
          for (int j = height - 1, i = 0; i < height; ++i) {
            if (i <= height / 2) {
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
        for (int i = 0; i < width; ++i)
          i % 2 == 0 ? cloudS += wrpChar : cloudS += ' ';
        cloud.push_back(cloudS);
        for (int j = height, i = 0; i < height; ++i) {
          if (i <= height / 2) {
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
        for (int i = 0; i < width + 3; ++i)
          i % 2 == 0 ? strtend += wrpChar : strtend += ' ';
        cloud.push_back(strtend);
        cloud.push_back(cloudS);
        cloudS = std::string(half, ' ') + "  \\" + std::string(13 - half, ' ') + "__";
        cloud.push_back(cloudS);
        return cloud;
      }
      else {
        int t = 0, m = 0, j = 0;
        if (longl <= half) {
          width -= longl * 2;
          if (height % 2 == 0)
            j = height;
          else
            j = height - 1;
        }
        else {
          if (height % 2 == 0) {
            width -= (height + 1 - longl);
            j = height;
          }
          else {
            width -= (height - longl);
            j = height - 1;
          }
        }
        for (int i = 0; i < height; ++i) {
          if (i <= half) {
            if (utf8len(text[i]) > (width + 2 * i)) {
              m = utf8len(text[i]) - (width + 2 * i);
              if (m > t)
                t = m;
            }
          }
          else {
            j -= 2;
            if (utf8len(text[i]) > (width + j)) {
              m = utf8len(text[i]) - (width + j);
              if (m > t)
                t = m;
            }
          }
        }
        width += t;
        width % 2 == 0 ? ++width : width;
        cloudS = std::string((half + 1), ' ');
        conn = cloudS + " \\";
        height % 2 == 0 ? j = height : j = height - 1;
        for (int i = 0; i < width; ++i) {
          i % 2 == 0 ? cloudS += wrpChar : cloudS += ' ';
        }
        strtend = cloudS;
        cloud.push_back(strtend);
        for (int i = 0; i < height; ++i) {
          if (i <= half) {
            cloudS = std::string(half - i, ' ') + wrpChar + center(text[i], (width + 2 * i)) + wrpChar;
            cloud.push_back(cloudS);
          }
          else {
            j -= 2;
            cloudS = std::string(i - half, ' ') + wrpChar + center(text[i], width + j) + wrpChar;
            cloud.push_back(cloudS);
          }
        }
        if (text.size() % 2 != 0)
          cloud.push_back(strtend);
        else {
          strtend = std::string(half, ' ');
          for (int i = 0; i < width + 3; ++i) {
            i % 2 == 0 ? strtend += wrpChar : strtend += ' ';
          }
          cloud.push_back(strtend);
        }
        cloud.push_back(conn);
        conn = std::string(half, ' ') + "   \\" + std::string(12 - half, ' ') + "__";
        cloud.push_back(conn);
        return cloud;
      }
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
