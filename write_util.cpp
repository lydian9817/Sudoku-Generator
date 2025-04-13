#include <fstream>
#include <string>
#include <filesystem>
#include "write_util.h"

using namespace std;

void writeToFile(std::unordered_map<int, StrBoard>* bs, string& fileName)
{
	string outDir = "generated_boards";
	filesystem::create_directory(outDir);
	std::ofstream f(outDir + "/" + fileName, ios::trunc);
	if (f.is_open())
	{
		json data;

		for (auto& b : *bs)
			data[to_string(b.first)] = {b.second.solved, b.second.puzzle};

		string content = data.dump(2);

		f.write(content.c_str(), content.size());
	}
}
