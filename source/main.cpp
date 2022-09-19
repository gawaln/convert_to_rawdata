#include <Windows.h>
#include <filesystem>
#include <iostream>
#include <fstream>

void ConvertToRawData(const std::string& fileName)
{
	size_t find = fileName.rfind(".");
	const auto str = fileName.substr(0, find) + "_rawdata.txt";

	std::filesystem::path p(str);
	if (std::filesystem::exists(p))
	{
		printf("[-] The file already exists\n");
		return;
	}

	std::ofstream txt(str, std::ios::binary | std::ios::out);
	std::ifstream dump(fileName, std::ios::binary | std::ios::in);

	if (txt.is_open() && dump.is_open())
	{
		dump.seekg(0, std::ios::end);
		size_t fileSize = dump.tellg();
		dump.seekg(0, std::ios::beg);

		char* buffer = new char[fileSize];

		printf("[+] File size = 0x%x\n", fileSize);

		dump.read(buffer, fileSize);

		printf("[+] Converting...\n");

		txt << "std::uint8_t rawData[] = {\n";

		char rawBuf[0x10];

		for (int i = 0; i < fileSize; i++)
		{
			sprintf(rawBuf, "0x%X, ", (std::uint8_t)buffer[i]);
			txt << rawBuf;
		}

		txt << "\n};";

		printf("[+] Done!\n");

		delete[] buffer;
	}

	txt.close();
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("usage: convert_to_raw.exe [filename.extension]\n");
		return -1;
	}

	ConvertToRawData(argv[1]);

	return 0;
}