#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>

// encode flag
int main()
{
	std::ifstream file2("daylight.wav", std::ios::binary | std::ios::ate);
	if (!file2.is_open())
	{
		std::cout << "Fail to open!!";
		return 1;
	}
	int size = file2.tellg();
	// allocate PCM audio buffer
	std::vector<short> samples2 = std::vector<short>(size / 2);
	file2.seekg(0, std::ios::beg);
	file2.read((char *)&samples2[0], size);

	std::string flag = "- .--- -.-. - ..-. -... .-. .- -.-. -.- . - .. -.. --- -. .----. - .-.. .. -.- . - --- ... .. -. --. .---- ..... ....- ----. ----- ---.. ----. --... -.... ----.";

	int currptr = 22;
	int len = 30000;
	for (auto &curr : flag)
	{
		// std::cout<<(int)flag[0]<<"\n";
		if (curr == '.')
		{
			for (int i = currptr; i < currptr + len / 3; i++)
			{
				samples2[i] += 1000 * sin((6.24 / 88100) * i * (curr + 5000));
				samples2[i] += 1000 * sin((6.24 / 88100) * i * (curr + 38000));
			}
			currptr += len / 3;
		}
		if (curr == '-')
		{
			for (int i = currptr; i < currptr + len; i++)
			{
				samples2[i] += 1000 * sin((6.24 / 88100) * i * (curr + 5000));
				samples2[i] += 1000 * sin((6.24 / 88100) * i * (curr + 38000));
			}
			currptr += len;
		}
		if (curr == ' ')
			currptr += len;

		currptr += len / 2;
	}

	std::ofstream fileout("daylight_flag.wav", std::ios::out | std::ios::binary);

	if (!fileout.is_open())
	{
		std::cout << "Failure to open song.dat\n";
		exit(0);
	}
	fileout.seekp(0, std::ios::beg);
	fileout.write((char *)&samples2[0], samples2.size() * 2);
	fileout.close();
	return 0;
}
