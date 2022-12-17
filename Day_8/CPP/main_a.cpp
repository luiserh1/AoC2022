#include <iostream>		// Salida por consola
#include <fstream>		// Lectura de archivo de input
#include <string>		// std::string
#include <vector>		// std::vector
#include <filesystem>	// Rutas canónicas

int main()
{
	const char* inputFilepath = "../data/input.txt";
	std::cout << "Starting..." << std::endl;
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
			if (c == '\r') continue;

			int asciiValue = static_cast<int>(c);
			int treeHeight = asciiValue - static_cast<int>('0');
			if (treeHeight < 0 || treeHeight > 10)
			{
				std::cout << "Wrong format! Chars must be integers from 0 to 9. Reading \"";
				std::cout << c << "\" from: " << line << std::endl;
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
	
	int visibleTreesNum = treesPerLine * 4 - 4;
	std::vector<std::vector<char>> treesVisibility;	 // 'y': visible, invisible otherwise
	treesVisibility.resize(treesPerLine);	// Trees per line = lines of trees
	treesVisibility[0].resize(treesPerLine, 'y');
	treesVisibility[treesPerLine - 1].resize(treesPerLine, 'y');
	for (int i = 1; i < treesPerLine-1; i++)
	{
		treesVisibility[i].resize(treesPerLine, 'n');
		treesVisibility[i][0]				 = 'y';
		treesVisibility[i][treesPerLine - 1] = 'y';
	}
	
	// Down
	for (int x = 1; x < treesPerLine - 1; x++)
	{
		int highestTreeInLine = trees[x][0];
		for (int y = 1; y < treesPerLine - 1; y++)
		{
			int treeHeight = trees[x][y];
			if (treeHeight > highestTreeInLine)
			{
				visibleTreesNum++;
				highestTreeInLine = treeHeight;
				treesVisibility[x][y] = 'y';
			}
		}
	}
	
	// Right
	for (int y = 1; y < treesPerLine - 1; y++)
	{
		int highestTreeInLine = trees[0][y];
		for (int x = 1; x < treesPerLine - 1; x++)
		{
			int treeHeight = trees[x][y];
			if (treeHeight > highestTreeInLine)
			{
				highestTreeInLine = treeHeight;
				if (treesVisibility[x][y] != 'y')
				{
					visibleTreesNum++;
					treesVisibility[x][y] = 'y';
				}
			}
		}
	}
	
	// Up
	for (int x = 1; x < treesPerLine - 1; x++)
	{
		int highestTreeInLine = trees[x][treesPerLine - 1];
		for (int y = treesPerLine - 2; y > 0; y--)
		{
			int treeHeight = trees[x][y];
			if (treeHeight > highestTreeInLine)
			{
				highestTreeInLine = treeHeight;
				if (treesVisibility[x][y] != 'y')
				{
					visibleTreesNum++;
					treesVisibility[x][y] = 'y';
				}
			}
		}
	}
	
	// Left
	for (int y = 1; y < treesPerLine - 1; y++)
	{
		int highestTreeInLine = trees[treesPerLine - 1][y];
		for (int x = treesPerLine - 2; x > 0; x--)
		{
			int treeHeight = trees[x][y];
			if (treeHeight > highestTreeInLine)
			{
				highestTreeInLine = treeHeight;
				if (treesVisibility[x][y] != 'y')
				{
					visibleTreesNum++;
					treesVisibility[x][y] = 'y';
				}
			}
		}
	}
	
	// Printinng the result
	for (int i = 0; i < treesPerLine; i++)
	{
		for (int j = 0; j < treesPerLine; j++)
		{
			if (treesVisibility[i][j] == 'y')
			{
				std::cout << "\033[32m";
			}
			else
			{
				std::cout << "\033[m";
			}
			std::cout << trees[i][j];
		}
		std::cout << "\033[m\n";
	}
	std::cout << "\t- The amount of visible trees from outside the grid is: " << "\033[32m" << visibleTreesNum << "\033[m\n";
	
	std::system("pause");
	return 0;
}
