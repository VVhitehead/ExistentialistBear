#include <iostream>
#include "symetricWrap.hpp"
#include "ParseFF.hpp"

#define MAX 1500

/* "º", "Ø", "×", Ô", "Ò", "o", "O", "0", "·", "¤", "œ", "ƨ", "ɂ", "ϭ", "•" */
/* txt.uniRndmInt() */

bool cmp(const char* arg, const char* flag) {
  for (size_t i = 0; flag[i] != '\0'; ++i)
    if (flag[i] != arg[i]) return false;
  return true;
}

void prntO ( std::vector<std::string> v ) {
  for (auto & element : v)
    std::cout << element << std::endl;
}

int main(int argc, char const *argv[]) {
  /* add command line arguments */
  parseFF txt;
  txt.createCF("Data/Base");
  symtrcWrap ord;
  ord.setChar("•");
  std::string say = "";
  if ( argc > 2 ) {
    if (cmp(argv[1], "-say")) {
      for (int i = 2; i < argc; ++i) {
        if (ord.utf8len(argv[i]) > MAX) {
          std::cerr << "Won't accept strings longer then: " << MAX << "!\n";
          exit(-1);
        }
        say += std::string(argv[i]) + " ";
      }
      ord.setText(txt.foldLines(say));
      prntO(ord.wrapImpl());
      prntO(ord.AArt());
      //printf("\n%d", argc);
    }
    else
      std::cout <<"Usage: " << argv[0] << " flag." << std::endl;
  }
  else if (argc == 2) {
    if (cmp(argv[1], "-s"))
      txt.retrieves(argv[1]);
  }
  else if (argc == 1) { // txt.uniRndmInt()
    ord.setText(txt.retrieveq("Data/Content.txt", txt.uniRndmInt())); // 96
    prntO(ord.wrapImpl());
    prntO(ord.AArt());
  }
  return 0;
}
