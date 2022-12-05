//#define VERBOSE

#define ROCK_VALUE 1
#define PAPER_VALUE 2
#define SCISORS_VALUE 3

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
		char elfShape = line[0];
		char myShape  = line[2];
		
		int roundScore = 0;
		// Points for playing a specific shape
		if (myShape == 'X')
		{
			roundScore = ROCK_VALUE;
		}
		else if (myShape == 'Y')
		{
			roundScore = PAPER_VALUE;
		}
		else if (myShape == 'Z')
		{
			roundScore = SCISORS_VALUE;
		}
		
		// Points for the round's result
		if ((myShape == 'X' && elfShape == 'A') ||
			(myShape == 'Y' && elfShape == 'B') ||
			(myShape == 'Z' && elfShape == 'C')) 
		{
			// Draw coondition
			roundScore += DRAW_VALUE;
		}
		else if ((myShape == 'X' && elfShape == 'C') ||
				 (myShape == 'Y' && elfShape == 'A') ||
				 (myShape == 'Z' && elfShape == 'B')) 
		{
			// Win condition
			roundScore += WIN_VALUE;
		}
		else if ((myShape == 'X' && elfShape == 'B') ||
				 (myShape == 'Y' && elfShape == 'C') ||
				 (myShape == 'Z' && elfShape == 'A')) 
		{
			// Loss condition
			roundScore += LOSS_VALUE;
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
