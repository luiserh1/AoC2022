#include <iostream>		// Salida por consola
#include <fstream>		// Lectura de archivo de input
#include <string>		// std::string
#include <vector>		// std::vector
#include <filesystem>	// Rutas canónicas

void printStack(int id, std::vector<char> stack)
{
	std::cout << "\"" << id << "\": ";
	for (char c : stack)
	{
		std::cout << c << " ";
	}
	std::cout << "\n";
}

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
	
	std::vector<std::vector<char>> vecStacks;
	std::string line;
	size_t numStacks = -1;
	bool readinngInitialState = true;
	while (std::getline(file, line))
	{
		size_t lineSize = line.size();
		if (numStacks == static_cast<size_t>(-1))
		{
			numStacks = lineSize / 4 + 1;
			vecStacks.resize(numStacks);
			std::cout << "Initiallizing problem for " << numStacks << " stacks\n";
		}
		
		// Getting the input
		/* 
		 * There are 2 sections of differently formated data inside the input file
		 * 1: The initial state
		 * 2: The actions
		 *
		 * 1:
		 * Being S the number of stacks, each line is expected to have 4 * S - 1 total chars
		 * This is because each stack but the last one takes always 4 chars of the line
		 * The codification of a stack in a line will follow this format
		 *  - '[' + crate_char  + ']' + ' '   	// There is a crate in the stack at this height
		 *  - ' ' + ' '         + ' ' + ' '	 	// There is no crate in the stack at this height
		 * 	- ' ' + stack_num   + ' ' + ' '	 	// This is the last line of the initial state
		 * The last stack always lacks the 4th char, the separator (' '), so it onnly takes three chars
		 *
		 * 2:
		 * The actions consist in moving one ore more crates from one stack to another
		 * All the actions have the following format:
		 * "move " + num_crates + " from " + src_stack + " to " dst_stack
		 * Where num_crates is the number of crates that will be moved from src_stack to dst_stack
		 * The crates are moved one at a time
		 *
		 * Both sections are separated by a blank line
		*/
		
		if (readinngInitialState)
		{
			// 1: Initial State
			if (lineSize / 4 + 1 == numStacks)
			{
				for (size_t stack = 0; stack < numStacks; stack++)
				{
					size_t i = stack * 4;
					if (line[i] == '[')
					{
						// There is a crate
						vecStacks[stack].insert(vecStacks[stack].begin(), line[i+1]);
					}
					else if (line[i+1] != ' ')
					{
						// The last line, it contains the stack ID
						std::cout << "\"" << line[i+1] << "\": ";
						for (char c : vecStacks[stack]) std::cout << c << " ";
						std::cout << "\n";
					}
				}
			}
			else
			{
				// Blank line
				readinngInitialState = false;
			}
		}
		else
		{
			// 2: The actions
			static const std::string MOVE = "move ";
			static const std::string FROM = " from ";
			static const std::string TO   = " to ";
			int moveLinePos = line.find(MOVE);
			int fromLinePos = line.find(FROM);
			int toLinePos   = line.find(TO);
			std::string numCratesStr = line.substr(moveLinePos + MOVE.size(), fromLinePos - (moveLinePos + MOVE.size()));
			std::string srcStackStr  = line.substr(fromLinePos + FROM.size(), toLinePos - (fromLinePos + FROM.size()));
			std::string dstStackStr  = line.substr(toLinePos + TO.size());
			
			int numCrates, srcStack, dstStack;
			try
			{	
				numCrates = stoi(numCratesStr);
				srcStack  = stoi(srcStackStr);
				dstStack  = stoi(dstStackStr);
			}
			catch (const std::invalid_argument& e)
			{
				std::cout <<  "Error tratando de connvertir a entero: " << canonical_path;
				return 3;
			}
			
			std::cout << "Moving " << numCrates << " crates from: " << srcStack << " to " << dstStack << "\n";
			auto bottomIt = vecStacks[srcStack-1].end() - numCrates;
			auto topIt = vecStacks[srcStack-1].end();
			for (auto it = bottomIt; it != topIt; it++)
			{
				vecStacks[dstStack-1].push_back(*it);
			}
			vecStacks[srcStack-1].erase(bottomIt, topIt);
			for (size_t  i = 0; i < numStacks; i++)
			{
				printStack(i+1, vecStacks[i]);
			}
		}
		
	}
	file.close();
	
	std::cout << "Thi final stacks look like this:\n";
	for (size_t i = 0; i < vecStacks.size(); i++)
	{
		printStack(i+1, vecStacks[i]);
	}
	
	std::cout << "Being the final top crates: ";
	for (const auto& stack : vecStacks)
	{
		std::cout << stack[stack.size()-1];
	}
	std::cout << std::endl;
	
	std::system("pause");
	return 0;
}
