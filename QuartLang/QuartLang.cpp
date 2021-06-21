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
	bool testResult = false;
	try {
		testResult = doAllTests();
	}
	catch (const std::exception & expt) {
		std::cout << std::endl << expt.what() << std::endl;
		throw;
	}



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

	std::cout << "Copyright (c) 2021, MATIAS CHARA CRONEMBOLD \n"
		"All rights reserved.\n\n"
		"Redistribution and use in source and binary forms, with or without \n"
		"modification, are permitted provided that the following conditions are met : \n"
		"1. Redistributions of source code must retain the above copyright \n"
		"notice, this list of conditionsand the following disclaimer. \n"
		"2. Redistributions in binary form must reproduce the above copyright \n"
		"notice, this list of conditionsand the following disclaimer in the \n"
		"documentationand /or other materials provided with the distribution. \n"
		"3. All advertising materials mentioning features or use of this software \n"
		"must display the following acknowledgement : \n"
		"This product includes software developed by MATIAS CHARA CRONEMBOLD. \n"
		"4. Neither the name of MATIAS CHARA CRONEMBOLD nor the \n"
		"names of its contributors may be used to endorse or promote products \n"
		"derived from this software without specific prior written permission. \n\n"
		"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER ''AS IS'' AND ANY \n"
		"EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED \n"
		"WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE \n"
		"DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE \n"
		"FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL \n"
		"DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR \n"
		"SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER \n"
		"CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, \n"
		"OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE \n"
		"USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\n";

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


			try{
				Ishleng ishlengObj(&logger, &dict, source);
				ishlengObj.lex();
				ishlengObj.validate();
				ishlengObj.parse();
				ishlengObj.run();
			}
			catch (const std::exception &expt) {
				std::cout << std::endl << expt.what() << std::endl;
			}

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
	Engine::cleanUpThreads();
}
  