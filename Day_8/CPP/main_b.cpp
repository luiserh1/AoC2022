#include <iostream>		// Salida por consola
#include <fstream>		// Lectura de archivo de input
#include <string>		// std::string
#include <vector>		// std::vector
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
	std::vector<std::vector<int>> trees;
	// The grids are expected to have the same amount of rowa and columns
	while (std::getline(file, line))
	{
		std::vector<int> treesLine;
		treesLine.reserve(line.size());
		for (char c : line)
		{
			int asciiValue = static_cast<int>(c);
			int treeHeight = asciiValue - 48;
			if (treeHeight < 0 || treeHeight > 10)
			{
				std::cout <<  "Wrong format! Chars must be integers from 0 to 9";
				return 4;
			}
			treesLine.push_back(treeHeight);
		}
		
		trees.push_back(std::move(treesLine));
	}
	file.close();
	
	int treesPerLine = trees.size();
	
	bool okFormat = true;
	if (trees.empty())
	{
		okFormat = false;
	}
	for (int i = 0; i < treesPerLine; i++)
	{
		int treesThisLine = trees[i].size();
		std::cout << "The tree column number " << "\033[33m" << (i+1) << "\033[m" << " has " << "\033[34m" << treesThisLine << "\033[m" << " trees in it\n";
		if (treesThisLine != treesPerLine)
		{
			okFormat = false;
			break;
		}
	}
	if  (!okFormat)
	{
		std::cout <<  "Wrong format! Rows and columns must share the same size";
		return 4;
	}
	std::cout << "The format is correct! Solving the problem...\n\n";
	
	// Looking left
	std::vector<int> treesSeenLookingLeft;
	treesSeenLookingLeft.reserve(treesPerLine);
	treesSeenLookingLeft[0].resize(treesPerLine, 0);
	treesSeenLookingLeft[treesPerLine - 1].resize(treesPerLine, 0);
	for (int y = 1; y < treesPerLine - 1; y++)
	{
		treesSeenLookingLeft[0][y] = 0;
		for (int x = 1; x < treesPerLine; x++)
		{
			int treeHeight   = trees[x][y];
			int visibleTrees = 1;
			while (treeHeight >= trees[x-visibleTrees][y] && x - visibleTrees > 0)
			{
				visibleTrees += trees[x-visibleTrees][y];
			}
			treesSeenLookingLeft[x][y] = visibleTrees;
		}
	}
	
	std::system("pause");
	return 0;
}
