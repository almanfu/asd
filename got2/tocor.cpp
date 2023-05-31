#include <iostream>
#include <fstream>
#include <sstream>

int main()
{
  std::ifstream inFile("input.txt");
  std::ofstream outFile("inputcor.txt");

  if (!inFile.is_open() || !outFile.is_open())
  {
    std::cout << "Error opening file" << std::endl;
    return 1;
  }

  std::string line;
  bool isFirstLine = true;
  outFile << "p cep ";
  while (std::getline(inFile, line))
  {
    std::istringstream iss(line);
    int a, b;

    if (!(iss >> a >> b))
    {
      std::cout << "Invalid data in line: " << line << std::endl;
      continue;
    }

    if (!isFirstLine)
    {
      a++;
      b++;
    }

    outFile << a << " " << b << "\n";

    isFirstLine = false;
  }

  inFile.close();
  outFile.close();

  return 0;
}