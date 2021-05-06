
#include "Parser.h"
#include <iostream>
#include <string>
#include "allTests.h"
#include "Engine.h"




int main()
{
#ifdef MUST_TEST_ISHLENG
	doAllTests();
	system("pause");
#else
	Logger logger;
	logger.toggleLogging(true);
	Parser parser("Source.txt", &logger);
	Recognizer recognizer(&parser, &logger);

	Engine engine(recognizer.getProgram(), &logger);
	engine.run();
	std::cout << std::endl;
	system("pause");
#endif // MUST_TEST_ISHLEND

}
  