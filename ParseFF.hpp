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

/* len of chat box:
----------------------------------------------------------------------------------
or 82 (79 max allowed len of str without the walls) */

#define LIM 79
#define MAXNL 4

class parseFF {
  private:
    std::string tmp;
    std::ifstream source, read;
    std::ofstream cont;
    int n;
    const char *content = "Data/Content.txt", *txtsource = "Data/Lastqs";
  public:
    __time_t lastmod(const char *filepath) {
      struct stat64 fileinfo;
      if(stat64(filepath, &fileinfo) != -1)
        return fileinfo.st_mtime;
      return -1;
    }
    int utf8len(std::string &s) {
      int len = 0;
      for (auto & element : s)
        len += (element & 0xc0) != 0x80;
      return len;
    }
    void reverse(std::string &s) {
        int c, i, j;
        for (i = 0, j = utf8len(s) - 1; i < j; i++, j--) {
            c = s[i];
            s[i] = s[j];
            s[j] = c;
        }
    }
    void sort(std::vector<size_t> &v) {
      size_t swap;
      for (int i = 0; i < v.size(); ++i) {
        for (int j = i; j < v.size(); ++j) {
          if (v[i] > v[j]) {
            swap = v[i];
            v[i] = v[j];
            v[j] = swap;
          }
        }
      }
    }
    std::string cnvrt2str(size_t num) {
      std::string cnvrted = "";
      for (;num / 10;) {
        cnvrted += (num % 10) + '0';
        num /= 10;
      }
      cnvrted += num + '0';
      reverse(cnvrted);
      return cnvrted;
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
      if (lastmod(content) <= lastmod(filepath)) {
        try {
          source.open(filepath);
          try {
            cont.open(content);
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
          source.open(content, std::ios::binary);
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
    void retrieves(std::string s) {
      if (s == "-S" || s == "-s") {
        read.exceptions(std::ifstream::badbit);
        try {
          read.open(txtsource);
          while (read.good()) {
            safeGetline(read, tmp);
            break;
          }
        }
        catch (std::ifstream::failure e) {
          std::cerr << "Run regular text before asking for its source!" << std::endl;
          return;
        }
        if (tmp.length() > 4) { std::cout << "Source:" << tmp << std::endl; }
        else { std::cerr << "Run regular text before asking for its source!" << std::endl; }
      }
      else {
        std::cerr << "Invalid argument." << std::endl;
      }
    }
    std::vector<std::string> retrieveq(const char* filepath, int qnum) {
      std::vector<std::string> quote;
      std::string line, snum, end;
      std::stringstream tstr;
      tstr << qnum << ".";
      snum = tstr.str();
      tstr.str(std::string());
      bool flag = false, src = true;
      read.exceptions(std::ifstream::badbit);
      try {
        read.open(content);
        while (!read.eof()) {
          safeGetline(read, line);
          if (line == snum) {
            flag = true;
            tstr << ++qnum << ".";
            end = tstr.str();
          }
          else if (line.substr(0, 5) == "#-#-#" && flag) {
            cont.open(txtsource);
            cont << line.substr(5, (line.length() - 10));
            src = false;
          }
          else if (line.substr(0, 1) != "#" && flag) {
            if (line == end || line.length() == 0) { break; }
            if (line == "$$") { quote.push_back(""); }
            else { quote.push_back(line); }
          }
        }
        if (src) {
          try {
            cont.open(txtsource);
            cont << " Unknown";
          }
          catch (std::ifstream::failure e) {
            std::cerr << "Exception opening/reading/ file" << std::endl;
            throw;
          }
        }
      }
      catch (std::ifstream::failure e) {
        std::cerr << "Exception opening/reading/ file" << std::endl;
      }
      return quote;
    }
    std::vector<std::string> foldLines(std::string s) {
      // if no '\n' in s and length of s <= 79:
      // return s
      std::vector<std::string> fin;
      if (utf8len(s) <= 79) {
        for (auto &element : s)
          if (element == '\n' || element == '\t')
            goto out;
        fin.push_back(reduce(s));
        return fin;
      }
      out:
      size_t i = 0, pos = 0, theone = 0;
      int siz = 0, dev = 0, tn = 0;
      std::string cut = "", tmp = s, t = "";
      std::vector<size_t> nls;
      std::map<size_t, int> sps;
      for (auto &elm : s) {
        if (elm == ' ' || elm == '\t' || elm == '\n') {
          cut += ' ';
          sps[++pos] = i;
        }
        else
          cut += elm;
        ++i;
        }
        //std::cout << utf8len(cut) << std::endl;
        if ((siz = utf8len(cut)) > LIM) {
          int lnlm = (siz - LIM) / 2;
          dev = std::abs(sps.begin() -> second - lnlm);
          for (auto const &it : sps) {
            tn = std::abs(it.second - lnlm);
            if (tn < dev) {
              dev = tn;
              theone = it.second;
            }
          }
          if (theone)
            nls.push_back(theone);
          int lmt = LIM, tmp = 0;
          for (int i = 0, lnlmd = lnlm, thetwo = theone; siz - theone >= lmt; i += 2) {
            lmt = LIM - i - 2;
            lnlm = theone + lmt;
            lnlmd = thetwo - lmt + 2;
            for (auto const &it : sps) {
              if (it.second > lnlm)
                break;
              theone = it.second;
            }
            nls.push_back(theone);
            if (thetwo <= lmt)
              break;
            for (auto const &it : sps) {
              if (it.second >= lnlmd) {
                nls.push_back(it.second);
                thetwo = it.second;
                break;
              }
            }
          }
          nls.push_back(i-1); //Push back the index of the last char in s.. not to skip the line after last \n
          sort(nls);
          for (int i = 0; i < nls.size() - 1; ++i)
            if (nls[i+1] - nls[i] > lmt)
              lmt = nls[i+1] - nls[i];
          lmt % 2 == 0 ? ++lmt : lmt;
          tmp = lmt;
          int j = 0;
          for (auto &elm : cut) {
            for (auto &nl : nls) {
              if (nl == j) {
                fin.push_back(reduce(t));
                t = "";
                goto bai;
              }
            }
            t += elm;
            bai:
            ++j;
          }
        }
        else {
          size_t limit = LIM, cntn = 0, i = 0, inwrd = 0, pos = 0;
          std::string line = "", tmp = "";
          for (auto &elm : s) {
            if (elm == '\n' && cntn < MAXNL) {
              limit -= 4;
              ++cntn;
              i = 0;
              fin.push_back(line);
              line = "";
              inwrd = pos = 0;
              continue;
            }
            else if (elm == '\n' && cntn >= MAXNL) {
              line += ' ';
              ++i;
              inwrd = pos = 0;
              continue;
            }
            if (i < limit) {
              if (elm != ' ')
                pos = i;
              if (elm == ' ' && pos < i)
                inwrd = pos + (i - pos);
              line += elm;
              ++i;
            }
            else {
              tmp = line.substr(0, inwrd);
              fin.push_back(reduce(tmp));
              line = line.substr(inwrd, limit);
              inwrd = pos = 0;
              limit -= 4;
              i = 0;
            }
          }
          fin.push_back(reduce(line));
        }
        return fin;
    }
};

#endif
