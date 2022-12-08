#include <iostream>		// Salida por consola
#include <fstream>		// Lectura de archivo de input
#include <string>		// std::string
#include <vector>		// std::vector
#include <memory>		// std::shared_ptr
#include <iomanip>		// std::setw, std::left
#include <filesystem>	// Rutas canónicas

class DirectoryElement;
class File;
class Directory;

class DirectoryElement : public std::enable_shared_from_this<DirectoryElement>
{
public:
	explicit DirectoryElement(const std::string& name, int depth=0) : m_name(name), m_depth(depth) {}
	// Add virtual methods to make the class polymorphic
    virtual ~DirectoryElement() {}
	virtual void print() const = 0;
	virtual int getSize() const = 0;
	
	const std::string& getName() const { return m_name; }
	int getDepth() const { return m_depth; }
protected:
	std::string m_name;
	int m_depth;
};

class File : public DirectoryElement
{
public:
	explicit File(const std::string& name, int size, int depth=0) : DirectoryElement(name, depth), m_size(size) {}
	
	int getSize() const override { return m_size; }
	
	void print() const override
	{
		std::string padding;
		padding.insert(0, m_depth * 4, ' ');
		std::cout << padding << " - " << m_name << " (file, size=" << m_size << ")\n";
	}
	
private:
	int m_size; 
};

class Directory : public DirectoryElement
{
public:
	explicit Directory(const std::string& name, std::shared_ptr<Directory> parent = nullptr, int depth=0)
		: DirectoryElement(name, depth), m_parent(parent) {}
		
	int getSize() const override
	{
		int sizeSum = 0;
		for (auto e : m_elements)
		{
			sizeSum += e->getSize();
		}
		return sizeSum;
	}
		
	std::shared_ptr<Directory> shared_from_this()
	{
		return std::static_pointer_cast<Directory>(DirectoryElement::shared_from_this());
	}
	
	std::shared_ptr<Directory> getParent() const { return m_parent; }
	
	const std::vector<std::shared_ptr<DirectoryElement>>& getElements() const { return m_elements; }
	
	std::shared_ptr<Directory> addSubdir(const std::string& name)
	{
		for (auto e : m_elements)
		{
			auto subdir = std::dynamic_pointer_cast<Directory>(e);
			if (subdir && subdir->getName() == name)
			{
				// The suubdir already exists
				return subdir;
			}
		}
		
		// The subdir does not exist yet
		auto subdir = std::make_shared<Directory>(name, shared_from_this(), m_depth+1);
		m_elements.push_back(subdir);
		return subdir;
	}
	
	std::shared_ptr<File> addFile(const std::string& name, int size)
	{
		for (auto e : m_elements)
		{
			auto file = std::dynamic_pointer_cast<File>(e);
			if (file && file->getName() == name)
			{
				// The file already exists
				return file;
			}
		}
		
		// The file does not exist yet
		auto file = std::make_shared<File>(name, size, m_depth+1);
		m_elements.push_back(file);
		return file;
	}
	
	void print() const override
	{
		std::string padding;
		padding.insert(0, m_depth * 4, ' ');
		std::cout << padding << " - " << m_name << " (dir)"  << ":\n";
		for (auto e : m_elements)
		{
			e->print();
		}
	}
	
private:
	std::shared_ptr<Directory> m_parent;
	std::vector<std::shared_ptr<DirectoryElement>> m_elements;
};

int getSmallestDirOverGivenSize(std::shared_ptr<Directory> root, int currentMin, int lowerBound)
{
	for (auto e : root->getElements())
	{
		auto subdir = std::dynamic_pointer_cast<Directory>(e);
		if (subdir)
		{
			int subdirSize = subdir->getSize();
			std::cout << "Visiting " << subdir->getName() << " at depth " << subdir->getDepth() << " Its size is: " << subdirSize;
			if (subdirSize < currentMin && subdirSize > lowerBound)
			{
				currentMin = subdirSize;
				std::cout << ". New smallest dir over " << lowerBound << " found!";
			}
			std::cout << "\n";
			currentMin = getSmallestDirOverGivenSize(subdir, currentMin, lowerBound);				
		}
	}
	return currentMin;
}

int getSmallestDirOverGivenSize(std::shared_ptr<Directory> root, int lowerBound=8381165)
{
	return getSmallestDirOverGivenSize(root, root->getSize(), lowerBound);
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
	
	std::string line;
	std::shared_ptr<Directory> currentDir, rootDir;
	while (std::getline(file, line))
	{
		if (line.empty()) continue;
		
		bool isCommand = line[0] == '$';
		if (isCommand)
		{
			// There are two possible commands:
			// cd -> changes the cuurrent directory
			// ls -> lists the content of the new directory
			
			std::string command = line.substr(2, 2); // $ cd ... | $ cd ls ...
			if (command == "cd")
			{
				std::string dirName = line.substr(5);
				if (dirName == "..")
				{
					currentDir = currentDir->getParent();
				}
				else
				{
					if (!currentDir)
					{
						rootDir    = std::make_shared<Directory>(dirName); // direname is suposed to be equal to "/" here
						currentDir = rootDir;
					}
					else
					{
						currentDir = currentDir->addSubdir(dirName);
					}
				}
			}
			//else if (command == "ls")	{	}
		}
		else
		{
			// If the line does not represent a command, then we are reading the information of a directory element
			// There are two possible directory elements expected:
			// 1: Files:
			//   Its info is always the size of the file and its name and they are separated by a blank space ' '
			// 2: Directories:
			//	 Its info is always its name. It comes after the "dir " token
			
			int separatorPos = line.find(' ');
			std::string preSeparatorStr = line.substr(0, separatorPos);
			std::string pstSeparatorStr = line.substr(separatorPos+1);
			
			if (preSeparatorStr == "dir")
			{
				// We add the found directory to the current one, if it's not already contained
				currentDir->addSubdir(pstSeparatorStr);
			}
			else
			{
				int fileSize;
				try
				{	
					fileSize = stoi(preSeparatorStr);
				}
				catch (const std::invalid_argument& e)
				{
					std::cout << "Failed trying to parse the size of a file. " << preSeparatorStr << " froom the line: " << line;
					return 3;
				}
				
				// We add the found file to the current directory, if it's not already contained
				currentDir->addFile(pstSeparatorStr, fileSize);
			}
		}
	}
	file.close();
	
	rootDir->print();
	
	std::cout << "The total size of the root element is: " << rootDir->getSize() << "\n";
	int smallestDirOverGivenSize = getSmallestDirOverGivenSize(rootDir);
	std::cout << "The size of the smallest directory fitting the request is: " << smallestDirOverGivenSize << "\n";
	
	std::system("pause");
	return 0;
}
