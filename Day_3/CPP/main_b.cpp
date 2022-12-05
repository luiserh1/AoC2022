#define VERBOSE
#define LINES_PER_SET 3

#include <iostream>		// Salida por consola
#include <fstream>		// Lectura de archivo de input
#include <string>		// std::string
#include <array>		// std::array
#include <set>			// std::set
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
	std::array<std::string, LINES_PER_SET> lineSet;
	size_t largestLineLenngth = 0;
	int linesInSet = 0;
	int prioritySum = 0;
	// Loopinng all over the file
	while (std::getline(file, line))
	{
		// Format check
		if (line.empty() || line.size() % 2 != 0)
		{
			std::cout << "Error: La línea no está bien formateada\n";
			return 3;			
		}
		
		// Collecting the lines
		if (linesInSet < LINES_PER_SET)
		{
			lineSet[linesInSet] = line;
			linesInSet++;
			largestLineLenngth = line.size() > largestLineLenngth ? line.size() : largestLineLenngth; 
			continue;
		}
		
		// Searching for the badge. It's a char present in all the lines in the set
		char badgeChar = '-';
		std::array<std::set<char>, LINES_PER_SET> charsInLines;
		for (size_t i = 0; i < largestLineLenngth; i++)
		{
			for (size_t j = 0; j < LINES_PER_SET; j++)
			{
				if (i > lineSet[j].size() - 1) continue;
				
				char charIlineJ = lineSet[j][i];
				
				bool foundInAll = true;
				for (size_t k = 0; k < LINES_PER_SET; k++)
				{
					if (j == k) continue;
					
					if (charsInLines[k].find(charIlineJ) == charsInLines[k].end())
					{
						foundInAll = false;
						break;
					}
				}
				if (foundInAll)
				{
					badgeChar = charIlineJ;
					break;
				}
				
				charsInLines[j].emplace(charIlineJ);
			}
			
			if (badgeChar != '-') break;
		}
		
		// Getting the priority
		int badgePriority = 0;
		
		int repeatedCharASCIIInndex = static_cast<int>(badgeChar);
		if (repeatedCharASCIIInndex > 64 && repeatedCharASCIIInndex < 91)		// A..Z
		{
			// 27..52
			badgePriority = repeatedCharASCIIInndex - 64 + 26;
		}
		else if (repeatedCharASCIIInndex > 96 && repeatedCharASCIIInndex < 123)	// a..z
		{
			// 1..26
			badgePriority = repeatedCharASCIIInndex - 96;
		}
		else //if (!badgeChar)
		{
			std::cout << "Error: La línea no está bien formateada. EL caracter \"" << badgeChar << "\" no es válido";
			return 4;
		}
#ifdef VERBOSE
		for (size_t j = 0; j < LINES_PER_SET; j++)
		{
			std::cout << lineSet[j] << " ";
		}
		std::cout << "-> " << badgeChar << "(" << badgePriority << "). La línea más larga eran " << largestLineLenngth << " caracateres " << std::endl;
#endif
		prioritySum += badgePriority;
		
		// Resetting
		linesInSet = 0;
		largestLineLenngth = 0;
	}
	file.close();
	
	if (linesInSet > 0)
	{
		std::cout << "Error:El archivo no está bien formateado. EL caracter\n";
		return 4;
	}
	
	std::cout << "La suma total de prioridades es: " << "\033[33m" << prioritySum << "\033[m" << std::endl;
	
	std::system("pause");
	return 0;
}
