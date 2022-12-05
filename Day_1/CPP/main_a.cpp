#define VERBOSE 0

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
	
	int maxCalories 		= 0;
	int currentElfCalories 	= 0;
	int maxCaloriesElfIndex = 0;
	int elfIndex    		= 0;
	
	std::string line;
#ifdef VERBOSE
	std::cout << "El contenido de " << canonical_path << " es:\n";
#endif
	while (std::getline(file, line))
	{
		if (line.empty())
		{
#ifdef VERBOSE
			if (elfIndex)
			{
				std::cout << "El elfo [" << elfIndex << "] lleva " << currentElfCalories << " calorías\n";
			}
#endif
			elfIndex++;
			currentElfCalories = 0;
			continue;
		}
		
		try
		{	
			int foodCalories = stoi(line);
			currentElfCalories += foodCalories;
			if (currentElfCalories > maxCalories)
			{
				maxCalories = currentElfCalories;
				maxCaloriesElfIndex = elfIndex;
			}
		}
		catch (const std::invalid_argument& e)
		{
			std::cout << "El archivo " << canonical_path << " contiene una línea que no se puede tratar como entero: \"" << line << "\"\n";
			return 3;
		}
	}
	file.close();
	
	std::cout << "======================\nEl elfo que transporta más cantidad de calorías en comida lleva " << "\033[32m" << maxCalories 
		<< "\033[m" << " calorías. Es el elfo: " << "\033[33m" << maxCaloriesElfIndex << "\033[m" << "\n======================\n";
	
	std::system("pause");
	return 0;
}
