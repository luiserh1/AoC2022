#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

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
		return 1;
	}
	
	std::string line;
	std::cout << "El contenido de " << canonical_path << " es:\n";
	while (std::getline(file, line))
	{
		std::cout << line << "\n";
	}
	file.close();
	
	std::system("pause");
	return 0;
}