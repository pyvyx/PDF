#pragma once
#include <iostream>
#include <Windows.h>

namespace pfo {

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#if _DEBUG
#define pfo_NewLine std::cout << '\n'
#define pfo_Log(message, code) pfo::LogR(message, code)

	// Log current File + Line for debug builds
#define pfo_LogL(message, code) \
				pfo::useLogColorFor(5, code);\
				pfo::LogR("-------------------------------------Debug log-------------------------------------\n", code); \
				pfo::LogR("Error occured in file: " __FILE__ " on line " TOSTRING(__LINE__) "\n", code);\
				pfo::LogR("Function<" __FUNCTION__ ">: ", code);\
				pfo::LogR(message, code);\
				pfo::LogR("-----------------------------------------------------------------------------------\n", code)

#define pfo_Log_If(if_statement) \
				if if_statement
		/*
			Usage:
				pfo_Log_If((3 < 4)	// Statement
				{	// Body
					std::cout << "3 is less than 4\n";
				})
		*/
#else
#define pfo_NewLine
#define pfo_Log(message, code)
#define pfo_LogL(message, code)
#define pfo_Log_If(if_statement)
#endif

	// release macros
#define pfo_NewLineR std::cout << '\n'
#define pfo_LogR(message, code) pfo::LogR(message, code)

// Log current File + Line for debug and release builds
#define pfo_LogLR(message, code) \
			pfo::useLogColorFor(5, code);\
			pfo::LogR("-------------------------------Release and Debug log-------------------------------\n", code);\
			pfo::LogR("Error occured in file: " __FILE__ " on line " TOSTRING(__LINE__) "\n", code);\
			pfo::LogR("Function<" __FUNCTION__ ">: ", code);\
			pfo::LogR(message, code);\
			pfo::LogR("-----------------------------------------------------------------------------------\n", code)

#define pfo_LogR_If(if_statement) \
			if if_statement


	// global variables for the counter function
	static unsigned int counter_G = 0;
	static char code_G = 0;
	static unsigned int current_count_G = 0;

	// color codes
#define PFO_BLACK			0b0000
#define PFO_BLUE			0b0001
#define PFO_GREEN			0b0010
#define PFO_AQUA			0b0011
#define PFO_RED				0b0100
#define PFO_PURPLE			0b0101
#define PFO_YELLOW			0b0110
#define PFO_WHITE			0b0111
#define PFO_GRAY			0b1000
#define PFO_LIGHT_BLUE		0b1001
#define PFO_LIGHT_GREEN		0b1010
#define PFO_LIGHT_AQUA		0b1011
#define PFO_LIGHT_RED		0b1100
#define PFO_LIGHT_PURPLE	0b1101
#define PFO_LIGHT_YELLOW	0b1110
#define PFO_BRIGHT_WHITE	0b1111

// Status codes
#define PFO_INFO		PFO_LIGHT_GREEN
#define PFO_HINT		PFO_YELLOW
#define PFO_ERROR		PFO_RED
#define OpenGL_ERROR	PFO_AQUA
#define PFO_ALLOC		PFO_PURPLE
#define PFO_DEFAULT		PFO_WHITE

	template<class T>
	static inline void LogR(const T& message, const char& code = PFO_DEFAULT)
	{
		switch (code)
		{
			// check color codes
		case PFO_BLACK:
		case PFO_BLUE:
		case PFO_GREEN:
		case PFO_AQUA:
		case PFO_RED:
		case PFO_PURPLE:
		case PFO_YELLOW:
		case PFO_WHITE:
		case PFO_GRAY:
		case PFO_LIGHT_BLUE:
		case PFO_LIGHT_GREEN:
		case PFO_LIGHT_AQUA:
		case PFO_LIGHT_RED:
		case PFO_LIGHT_PURPLE:
		case PFO_LIGHT_YELLOW:
		case PFO_BRIGHT_WHITE:
			break;
		default:
			pfo_Log("LOG[ERROR]<" __FUNCTION__ ">: Invalid logging code", PFO_ERROR);
			throw;
			return;
		}

		if (current_count_G < counter_G)
		{
			if (current_count_G == 0)
			{
				HANDLE hConsole;
				hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

				SetConsoleTextAttribute(hConsole, code_G);
			}

			std::cout << message;

			if (current_count_G == counter_G - 1)
			{
				HANDLE hConsole;
				hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, PFO_DEFAULT);
			}
			++current_count_G;
		}
		else if (code == PFO_DEFAULT)
			std::cout << message;
		else
		{
			HANDLE hConsole;
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			SetConsoleTextAttribute(hConsole, code);
			std::cout << message;
			SetConsoleTextAttribute(hConsole, PFO_DEFAULT);
		}
	}


	static inline void useLogColorFor(const unsigned int& times, const char& code = PFO_DEFAULT)
	{
		switch (code)
		{
			// check color codes
		case PFO_BLACK:
		case PFO_BLUE:
		case PFO_GREEN:
		case PFO_AQUA:
		case PFO_RED:
		case PFO_PURPLE:
		case PFO_YELLOW:
		case PFO_WHITE:
		case PFO_GRAY:
		case PFO_LIGHT_BLUE:
		case PFO_LIGHT_GREEN:
		case PFO_LIGHT_AQUA:
		case PFO_LIGHT_RED:
		case PFO_LIGHT_PURPLE:
		case PFO_LIGHT_YELLOW:
		case PFO_BRIGHT_WHITE:
			break;
		default:
			pfo_Log("LOG[ERROR]<" __FUNCTION__ ">: Invalid logging code", PFO_ERROR);
			throw;
			return;
		}

		counter_G = times;
		code_G = code;
		current_count_G = 0;
	}
}