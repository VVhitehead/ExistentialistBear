#include <iostream>
#include "textwrap.hpp"
#include "ParseFF.hpp"


/* "º", "Ø", "×", Ô", "Ò", "o", "O", "0", "·", "¤", "œ", "ƨ", ɂ", ϭ" */


void prntO ( std::vector<std::string> v ) {
  for (auto & element : v) {
    std::cout << element << std::endl;
  }
}

int main(int argc, char const *argv[]) {
  /* add command line arguments */
  parseFF txt;
  txt.createCF("./Base");
  symtrcWrap ord;
  ord.setChar("·");
  ord.setText(txt.retriveq("./Content.txt", txt.uniRndmInt()));
  prntO(ord.wrapImpl());
  prntO(ord.AArt());
  return 0;
}
