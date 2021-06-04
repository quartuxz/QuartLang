#include "Lexer.h"
#include <iostream>
#include <string>
#include "allTests.h"
#include "Engine.h"
#include "Ishleng.h"


#define EXIT_COMMAND "exit"
#define CLEAR_COMMAND "cls"
#define RUN_COMMAND "run"
#define CHANGE_DICTIONARIES_COMMAND "changelex"

int main()
{
#ifdef MUST_TEST_ISHLENG

	bool testResult = doAllTests();

	std::cout << "\n\n\n\t";
	if (!testResult) {
		std::cout << "***A TEST HAS FAILED !***" << std::endl;
	}
	else {
		std::cout << "ALL TESTS PASSED !" << std::endl;
	}
	std::cout << "\n\n\n";

#else

	

	Logger logger;
	logger.toggleLogging(false);

	std::cout << "LOADING DICTIONARIES...";
	DictionaryLexer dict("words_alpha.txt", m_matches);
	std::cout << "DONE " << std::endl << "=== === ===" << std::endl;

	std::string input;

	while (input != EXIT_COMMAND) {


		std::cout << ">";
		std::cin >> input;

		if (input == RUN_COMMAND) {



			std::string source;
			std::cout << "enter source file: ";
			std::cin >> source;

			Ishleng ishleng(&logger, &dict, source);
			ishleng.lex();
			ishleng.parse();
			ishleng.run();

			std::cout << std::endl;

		}
		else if(input == CLEAR_COMMAND) {
			system("cls");
		}
		else if (input == EXIT_COMMAND) {
			break;
		}

	}



	std::cout << std::endl;

#endif // MUST_TEST_ISHLENG
	system("pause");
	DictionaryLexer::cleanupDictionaries();
}
  