#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>

#include <emscripten/emscripten.h>

#include "parse.h"
#include "interpreter.h"

std::string help = "Turl Help:\nUsage: turl <filename>\nFlags:\ndebug: Displays Debug Info\nturl <filename> debug";

std::string getturl() {
  std::cout << "[Turl Code]" << std::endl;
  std::stringstream output;
  while (true) {
    char* input = (char*)EM_ASM_PTR({
      var input = prompt("Enter Turl Code [Type nothing to finish]:");
      if (input == null) return "";
      else return stringToNewUTF8(input);
    });
    std::string instr = (input);
    free(input);
    if (instr == "") break;
    std::cout << instr << std::endl;
    output << instr << std::endl;
  }
  std::cout << "[End Of Turl Code]\n" << std::endl;
  std::string ret = output.str();
  return ret;
}

int main(int argc,char *argv[]) {

    std::cout << "Welcome to the Turl Web Playground!" << std::endl;
    std::cout << "Type your code and run it to see how it works!\n" << std::endl;

    std::string turlCode = getturl();

		std::stringstream turlSource(turlCode);

		std::stringstream buffer;
		buffer << turlSource.rdbuf();

		std::vector<std::vector<std::string>> turlFile = parse(buffer.str());	//From "parse.h"

		if (!preProcessor(&turlFile)) {
			std::cout << "[Pre Processor Error]" << std::endl;
			return 3;
		}

		bool debug = false;	//Debug mode
		if (argc > 2 && strcmp(argv[2],"debug") == 0) debug = true;

		if (debug) for (int i = 0; i < numbers.size(); i++) std::cout << "mark " << locations.at(i).name << " = " << locations.at(i).line << std::endl;

		bool running = true;
		for (int line = 0; line < turlFile.size(); line++) {
			if (debug) std::cout << "[Debug] Running instruction..." << std::endl;
			try {
				if (!interpret(turlFile.at(line),line,&line)) {
					running = false;
					break;
				}
			}
			catch (std::out_of_range) {
				std::cout << "[Invalid Instruction] (Line " << line << ") " << std::endl;
			}

			if (debug) {
				//Display numbers
				for (int i = 0; i < numbers.size(); i++) std::cout << "number " << numbers.at(i).name << " = " << numbers.at(i).value << std::endl;

				//Display strings
				for (int i = 0; i < strings.size(); i++) std::cout << "string " << strings.at(i).name << " = " << strings.at(i).value << std::endl;
			}

      //Detect Software End Instruction
      if (line == -1) {
        break;
      }

		}

    std::cout << std::endl;

		//End Of Program
		if (running) {
			if (debug) std::cout << "[Program Ended]" << std::endl;
		}
		else {
			std::cout << "[Program Stopped]" << std::endl;
		}
}
