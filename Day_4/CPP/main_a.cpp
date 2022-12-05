#include <iostream>		// Salida por consola
#include <fstream>		// Lectura de archivo de input
#include <ostream>		// std::ostream para operator<<
#include <string>		// std::string
#include <vector>		// std::vector
#include <algorithm>	// std::sort
#include <filesystem>	// Rutas canónicas

struct Range
{
	int start, end;
	
	Range(int start, int end) : start(start), end(end) {}
	
	int size() const { return end - start; }
	
	bool isContained(const Range& o) const { return o.start <= start && o.end >= end; }
	
	bool operator<(const Range& o) const
	{ 
		if (size() < o.size())
			return true; 
		else if (size() == o.size())
			return start < o.start;
		else 
			return false;
	}
	
};

std::ostream& operator<<(std::ostream& os, const Range &range)
{
	os << range.start << "-" << range.end;
	return os;
}

int main()
{
	const char* inputFilepath = "../data/test.txt";
	
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
	std::vector<Range> inputRanges;
	while (std::getline(file, line))
	{
		// The format of a line is: S1-E1,S2-E2
		// A '-' sets apart the start and end of every range
		// A ',' sets apart the range of each elf in a pair
		// And: S1, E1, S2, E2 are all natural numbers
		int range1Separator = line.find("-");
		int range2Separator = line.rfind("-");
		int elvesSeparator = line.find(",");
		std::string s1Str = line.substr(0, range1Separator);
		std::string e1Str = line.substr(range1Separator+1, elvesSeparator - range1Separator - 1);
		std::string s2Str = line.substr(elvesSeparator+1, range2Separator - elvesSeparator - 1);
		std::string e2Str = line.substr(range2Separator+1);
		
		int s1, e1, s2, e2;
		try
		{	
			s1 = stoi(s1Str);
			e1 = stoi(e1Str);
			s2 = stoi(s2Str);
			e2 = stoi(e2Str);
		}
		catch (const std::invalid_argument& e)
		{
			std::cout <<  "Error tratando de connvertir a entero: " << canonical_path;
			return 3;
		}
		inputRanges.emplace_back(s1, e1);
		inputRanges.emplace_back(s2, e2);
	}
	file.close();
	
	// Sorting from lower to greater disntannce between start and end. In case of draw, the one
	// with the earlier start is lower. This helps us make some assumptions when looping
	std::sort(inputRanges.begin(), inputRanges.end());
	int fullyOverlapedRanges = 0;
	for (size_t i = 0; i < inputRanges.size(); i++)
	{
		const auto& range1 = inputRanges[i];
		bool rannge1IsContained = false;
		for (size_t j = i+1; j < inputRanges.size(); j++)
		{
			const auto& range2 = inputRanges[j];
			if (range1.isContained(range2))
			{
				std::cout << range1 << " is contained in " << range2 << "\n";
				rannge1IsContained = true;
				break;
			}
		}
		if (rannge1IsContained)
		{
			fullyOverlapedRanges++;
		}
	}
	std::cout << "There are " << fullyOverlapedRanges << " fully overlaped ranges" << std::endl;
	
	std::system("pause");
	return 0;
}
