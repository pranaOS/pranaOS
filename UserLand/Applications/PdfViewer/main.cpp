// includes
#include "common.hpp"
#include "reader.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
	if (argc != 2)
  {
		std::cout << "pdfviewer [pdf-file]" << std::endl;
		return jklim::ec::invalid_arguments;
	}

  // test
  {
    jklim::reader tester(argv[1]);

    std::cout << tester << std::endl;
  }

  std::cout << "press enter to exit...";
	std::cin.get();
	
	return 0;
}