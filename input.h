#include <string>
#include <sstream>

std::stringstream input() {
  char* input = (char*)EM_ASM_PTR({
    var input = prompt("Input:");
    if (input == null) return "";
    else return stringToNewUTF8(input)
  });
  std::string instr(input);
  free(input);
  std::stringstream instream(instr);
  return instream;
}
