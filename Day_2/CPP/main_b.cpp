//#define VERBOSE

#define ROCK_VALUE 1
#define PAPER_VALUE 2
#define SCISSORS_VALUE 3

#define LOSS_VALUE 0
#define DRAW_VALUE 3
#define WIN_VALUE 6

#include <iostream>		// Salida por consola
#include <fstream>		// Lectura de archivo de input
#include <string>		// std::string
#include <filesystem>	// Rutas canónicas

int main()
{
	const char* inputFilepath = "../data/input.txt";
	
	std::filesystem::path canonical_path;
	try
	{
		canonical_path = std::filesystem::canonical(inputFilepath);
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		std::cout << "Error: no se puede obtener la ubicación de: " << inputFilepath;
		return 1;
	}
	
	std::ifstream file;
	file.open(canonical_path);
	if  (!file.is_open())
	{
		std::cout <<  "Error abriendo " << canonical_path;
		return 2;
	}
	
	std::string line;
	int totalScore = 0;
	while (std::getline(file, line))
	{
		if (line.size() != 3)
		{
			std::cout << "Error: El formato de la línea no es válido\n";
			return 3;
		}
		char elfShape 	 = line[0];
		char roundResult = line[2];
		
		int roundScore = 0;
		// Points for the round's result
		if (roundResult == 'X')
		{
			// loss
			roundScore = LOSS_VALUE;
		}
		else if (roundResult == 'Y')
		{
			// Draw
			roundScore = DRAW_VALUE;
		}
		else if (roundResult == 'Z')
		{
			// WIN
			roundScore = WIN_VALUE;
		}
		
		// Points for my shape
		if ((roundResult == 'X' && elfShape == 'B') || 	// Loses against Paper
			(roundResult == 'Y' && elfShape == 'A') ||	// Draws against Rock
			(roundResult == 'Z' && elfShape == 'C')) 	// Wins against Scissors
		{
			// My shape is Rock
			roundScore += ROCK_VALUE;
		}
		else if ((roundResult == 'X' && elfShape == 'C') || // Loses against Scissors
				 (roundResult == 'Y' && elfShape == 'B') ||	// Draws against Paper
				 (roundResult == 'Z' && elfShape == 'A')) 	// Wins against Rock
		{
			// My shape is Paper
			roundScore += PAPER_VALUE;
		}
		else if ((roundResult == 'X' && elfShape == 'A') || // Loses against Rock
				 (roundResult == 'Y' && elfShape == 'C') ||	// Draws against Scissors
				 (roundResult == 'Z' && elfShape == 'B')) 	// Wins against Paper
		{
			// My shape is Scissors
			roundScore += SCISSORS_VALUE;
		}
		else
		{
			// Wrong format
			std::cout << "Error: El formato de la línea no es válido\n";
			return 3;
		}
		
		totalScore += roundScore;
#ifdef VERBOSE
		std::cout << line << ": " << "\033[33m" << roundScore << "\033[m" << std::endl;
#endif
	}
	file.close();
	
	std::cout << "======================\n";
	std::cout << "Siguiendo la estrategia mi puntuación sería de: " << "\033[32m" << totalScore << "\033[m" << std::endl;
	
	std::system("pause");
	return 0;
}
