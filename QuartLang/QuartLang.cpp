
#include "Parser.h"
#include <iostream>
#include <string>

#include "Engine.h"

int main()
{
	Logger logger;
	logger.toggleLogging(false);
	Parser parser("Source.txt", &logger);
	Recognizer recognizer(&parser, &logger);
	
	Engine engine(recognizer.getProgram(), &logger);
	engine.run();
	std::cout << std::endl;
	system("pause");
}
  