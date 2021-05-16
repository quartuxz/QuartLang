#include "Parser.h"
#include <iostream>
#include <string>
#include "allTests.h"
#include "Engine.h"




int main()
{
#ifdef MUST_TEST_ISHLENG
	if (!doAllTests()) {
		std::cout << "A TEST HAS FAILED !" << std::endl;
	}

#else
	Logger logger;
	logger.toggleLogging(false);
	Parser parser("Source.txt", &logger);
	Recognizer recognizer(&parser, &logger);

	Engine engine(recognizer.getProgram(), &logger);
	engine.run();
	std::cout << std::endl;

#endif // MUST_TEST_ISHLEND
	system("pause");
}
  