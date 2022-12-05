//#define VERBOSE

#include <iostream>		// Salida por consola
#include <fstream>		// Lectura de archivo de input
#include <string>		// std::string
#include <map>			// std::map
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
		
		// Searching for the repeated char
		size_t bottomIndex = 0;
		size_t upperIndex  = line.size() - 1;
		
		char repeatedChar = '\0';
		std::set<char> lSet, rSet;
		for (size_t i = 0; i < line.size() / 2; i++)
		{
			char leftChar  = line[bottomIndex + i];
			if (rSet.find(leftChar) != rSet.end()) 
			{
				repeatedChar = leftChar;
				break;
			}
			lSet.emplace(leftChar);
				
			char rightChar = line[upperIndex - i];
			if (lSet.find(rightChar) != lSet.end()) 
			{
				repeatedChar = rightChar;
				break;
			}
			rSet.emplace(rightChar);
		}

		// Getting the priority
		int linePriority = 0;
		
		int repeatedCharASCIIInndex = static_cast<int>(repeatedChar);
		if (repeatedCharASCIIInndex > 64 && repeatedCharASCIIInndex < 91)		// A..Z
		{
			// 27..52
			linePriority = repeatedCharASCIIInndex - 64 + 26;
		}
		else if (repeatedCharASCIIInndex > 96 && repeatedCharASCIIInndex < 123)	// a..z
		{
			// 1..26
			linePriority = repeatedCharASCIIInndex - 96;
		}
		else //if (!repeatedChar)
		{
			std::cout << "Error: La línea no está bien formateada. EL caracter \"" << repeatedChar << "\" no es válido";
			return 4;	
		}
#ifdef VERBOSE
		std::cout << line << ": " << repeatedChar << "(" << linePriority << ")" << std::endl;
#endif
		prioritySum += linePriority;
	}
	file.close();
	
	std::cout << "The total priority sum is: " << "\033[33m" << prioritySum << "\033[m" << std::endl;
	
	std::system("pause");
	return 0;
}
