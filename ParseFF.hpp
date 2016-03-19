#ifndef PARSEFF_H
#define PARSEFF_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <random>
#include <regex>
#include <sys/types.h>
#include <sys/stat.h>


class parseFF {
  private:
    std::string tmp;
    std::ifstream source, read;
    std::ofstream cont;
    int n;
  public:
    __time_t lastmod(const char *filepath) {
      struct stat64 fileinfo;
      if(stat64(filepath, &fileinfo) != -1)
        return fileinfo.st_mtime;
      return -1;
    }
    std::istream& safeGetline(std::istream& is, std::string& t) {
      t.clear();
      /*The characters in the stream are read one-by-one using a std::streambuf.
      That is faster than reading them one-by-one using the std::istream.
      Code that uses streambuf this way must be guarded by a sentry object.
      The sentry object performs various tasks,
      such as thread synchronization and updating the stream state.*/
      std::istream::sentry se(is, true);
      std::streambuf* sb = is.rdbuf();
      for(;;) {
          int c = sb->sbumpc();
          switch (c) {
            case '\n':
              return is;
            case '\r':
              if(sb->sgetc() == '\n')
                  sb->sbumpc();
              return is;
            /* handle the case when the last line has no line ending */
            case EOF:
              if(t.empty())
                  is.setstate(std::ios::eofbit);
              return is;
          default:
              t += (char)c;
          }
      }
    }
    std::string trim(const std::string& str, const std::string& whitespace = " \t") {
      const auto strBegin = str.find_first_not_of(whitespace);
      if (strBegin == std::string::npos) { return ""; }
      const auto strEnd = str.find_last_not_of(whitespace);
      const auto strRange = strEnd - strBegin + 1;
      return str.substr(strBegin, strRange);
    }
    std::string reduce(const std::string& str, const std::string& fill = " ", const std::string& whitespace = " \t") {
      auto result = trim(str, whitespace); // trim first
      auto beginSpace = result.find_first_of(whitespace); // replace sub ranges
      while (beginSpace != std::string::npos) {
        const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;
        result.replace(beginSpace, range, fill);
        const auto newStart = beginSpace + fill.length();
        beginSpace = result.find_first_of(whitespace, newStart);
      }
      return result;
    }
    void createCF(const char* filepath) {
      bool first = true;
      n = 1;
      std::string num;
      std::stringstream convert;
      source.exceptions(std::ifstream::badbit);
      cont.exceptions(std::ofstream::badbit);
      if (lastmod("./Content.txt") <= lastmod(filepath)) {
        try {
          source.open(filepath);
          try {
            cont.open("./Content.txt");
            cont << "1.\n";
            while (!source.eof()) {
              safeGetline(source, tmp);
              if (!std::regex_match(tmp, std::regex("^\\s*$"))) {
                cont << reduce(tmp) << "\n";
                first = true;
              }
              else if (first && !source.eof()) {
                /* maybe replace all this crap with << operator overloaded function */
                ++n;
                convert << n;
                cont << convert.str() << ".\n";
                convert.str(std::string());
                first = false;
              }
            }
            source.close();
            cont.close();
          }
          catch (std::ofstream::failure e) {
            std::cerr << "Exception creating/reading/writing file" << std::endl;
            throw;
          }
        }
        catch (std::ifstream::failure e) {
          std::cerr << "Exception opening file" << std::endl;
        }
      }
      else {
        try {
          source.open("./Content.txt", std::ios::binary);
          source.seekg(-1, std::ios::end);
          char c;
          bool flag = false, readnum = false, check = false;
          for (; source.get(c); source.seekg(-2, std::ios::cur)) {
            if (c == '\n') {
              if (check) { break; }
              flag = true;
            }
            else if (flag && c == '.') { readnum = true; }
            else if (readnum && isdigit(c)) {
              num = c + num;
              check = true;
            }
          }
          std::istringstream(num) >> n;
          std::cout << n << std::endl;
        }
        catch (std::ifstream::failure e) {
          std::cerr << "Exception reading/opening file" << std::endl;
        }
      }
    }
    int uniRndmInt() {
      std::random_device dev;
      std::mt19937 gen(dev());
      std::uniform_int_distribution<> dis(1, n);
      return dis(gen);
    }
    std::vector<std::string> retriveq(const char* filepath, int qnum) {
      std::vector<std::string> quote;
      std::string line, snum, end;
      std::stringstream tstr;
      std::cout << qnum << std::endl;
      tstr << qnum << ".";
      snum = tstr.str();
      std::cout << snum << std::endl;
      tstr.str(std::string());
      bool flag = false;
      read.exceptions(std::ifstream::badbit);
      try {
        read.open("./Content.txt");
        while (!read.eof()) {
          safeGetline(read, line);
          if (line == snum) {
            flag = true;
            tstr << ++qnum << ".";
            end = tstr.str();
            std::cout << end << std::endl;
          }
          else if (line.substr(0, 1) != "#" && flag) {
            if (line == end || line.length() == 0) { break; }
            if (line == "$$") { quote.push_back(""); }
            else { quote.push_back(line); }
          }
        }
      }
      catch (std::ifstream::failure e){
        std::cerr << "Exception opening/reading/ file" << std::endl;
      }
      return quote;
    }
};

#endif
