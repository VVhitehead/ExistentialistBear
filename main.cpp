#include <iostream>
#include "symetricWrap.hpp"
#include "ParseFF.hpp"


/* "º", "Ø", "×", Ô", "Ò", "o", "O", "0", "·", "¤", "œ", "ƨ", "ɂ", "ϭ", "•" */
/* txt.uniRndmInt() */

void prntO ( std::vector<std::string> v ) {
  for (auto & element : v) {
    std::cout << element << std::endl;
  }
}

int main(int argc, char const *argv[]) {
  /* add command line arguments */
  parseFF txt;
  txt.createCF("Data/Base");
  symtrcWrap ord;
  ord.setChar("•");
  if ( argc > 2 )
    std::cout <<"usage: " << argv[0] << " flag." << std::endl;
  else if (argc == 2) { txt.retrieves(argv[1]); }
  else if (argc == 1) {
    ord.setText(txt.retrieveq("Data/Content.txt", txt.uniRndmInt()));
    prntO(ord.wrapImpl());
    prntO(ord.AArt());
  }
  return 0;
}
