//#define VERBOSE
#define NUM_ELFS 3

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
	
	int maxCalories[NUM_ELFS];
	int maxCaloriesElfIndex[NUM_ELFS];
	int currentElfCalories 	= 0;
	int currentElfIndex    	= 0;
	
	std::string line;
#ifdef VERBOSE
	std::cout << "El contenido de " << canonical_path << " es:\n";
#endif
	while (std::getline(file, line))
	{
		if (line.empty())
		{
#ifdef VERBOSE
			if (currentElfIndex)
			{
				std::cout << "El elfo [" << currentElfIndex << "] lleva " << currentElfCalories << " calorías\n";
			}
#endif
			// Checking if the elf has reached the top
			for  (int i = NUM_ELFS-1; i >= 0; i--)
			{
				if (currentElfCalories > maxCalories[i])
				{
					int auxValue1 = maxCalories[i];
					int auxIndex1  = maxCaloriesElfIndex[i];
					for (int j = i; j > 0; j--)
					{
						// updating values
						int auxValue2  	 = maxCalories[j-1];
						maxCalories[j-1] = auxValue1;
						auxValue1 	   	 = auxValue2;
						
						// updating inndices
						int auxIndex2 		   	 = maxCaloriesElfIndex[j-1];
						maxCaloriesElfIndex[j-1] = auxIndex1;
						auxIndex1 			   	 = auxIndex2;
					}
					maxCalories[i]         = currentElfCalories;
					maxCaloriesElfIndex[i] = currentElfIndex;
					break;
				}
			}

			currentElfIndex++;
			currentElfCalories = 0;
			continue;
		}
		
		try
		{	
			int foodCalories = stoi(line);
			currentElfCalories += foodCalories;			
		}
		catch (const std::invalid_argument& e)
		{
			std::cout << "El archivo " << canonical_path << " contiene una línea que no se puede tratar como entero: \"" << line << "\"\n";
			return 3;
		}
	}
	file.close();
	
	std::cout << "======================\n";
	std::cout << "Top " << NUM_ELFS << " de elfos cargando calorías:\n";
	for (int i = NUM_ELFS-1; i >= 0; i--)
	{
		std::cout << NUM_ELFS - i << ": Elfo " << "\033[33m" << maxCaloriesElfIndex[i] << "\033[m" << " con " << "\033[32m" << maxCalories[i] << "\033[m" << " calorías.\n";
	}
	std::cout << "La suma total de calorías entre los " << NUM_ELFS << " es: ";
	int sum = 0;
	for (int i = 0; i < NUM_ELFS; i++) sum += maxCalories[i];
	std::cout << "\033[32m" << sum << "\033[m" << std::endl;
	
	std::system("pause");
	return 0;
}
