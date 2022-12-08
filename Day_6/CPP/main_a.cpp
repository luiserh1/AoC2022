#include <iostream>		// Salida por consola
#include <fstream>		// Lectura de archivo de input
#include <string>		// std::string
#include <filesystem>	// Rutas canónicas

#define CHARS_FOR_MARKER 4

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
	size_t markerPos = -1;

	while (std::getline(file, line))
	{
		// Looking for the pattern: 4 consecutive and different chars
		for (size_t i = 0; i < line.size() - CHARS_FOR_MARKER; i++)
		{
			bool markerFound = true;
			for (size_t j = i; j < i + CHARS_FOR_MARKER; j++)
			{
				for (size_t k = j+1; k < i + CHARS_FOR_MARKER; k++)
				{
					if (line[j] == line[k])
					{
						markerFound = false;
						break;
					}
				}
				if (!markerFound)
				{
					break;
				}
			}
			
			if (markerFound)
			{
				markerPos = i + CHARS_FOR_MARKER;
				break;
			}
		}
		
		// Displaying the result
		if (markerPos == static_cast<size_t>(-1))
		{
			std::cout << "Marker not found!";
		}
		else
		{
			std::cout << "The marker position is: "<< "\033[32m"<< markerPos << "\033[m" << " -> ";
			for (size_t i = 0; i < line.size(); i++)
			{
				if (i >= markerPos - CHARS_FOR_MARKER && i < markerPos)
				{
					std::cout << "\033[32m";
				}
				else
				{
					std::cout << "\033[m";
				}
				std::cout << line[i];
			}
			std::cout << "\n";
		}
	}
	file.close();
	
	std::system("pause");
	return 0;
}
