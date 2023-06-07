#include <iostream>
#include <vector>
#include <fstream>
#include "ChaCha20.hpp"
#include <math.h>


int main()
{
    std::ifstream file("foil.wav", std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
     // allocate PCM audio buffer        
    std::vector<short> samples = std::vector<short>(size/2); 
    file.seekg(44, std::ios::beg);
    file.read((char*)&samples[0], size-44);
    
    std::ifstream file2("daylight_flag.wav", std::ios::binary | std::ios::ate);
    size = file2.tellg();
    // allocate PCM audio buffer        
    std::vector<short> samples2 = std::vector<short>(size/2); 
    file2.seekg(44, std::ios::beg);
    file2.read((char*)&samples2[0], size-44);
    
    //for (short& s : samples2)
    //{
    //	if (!s)
    //	    s = 1;
    //}
    
    int blocksize = 88100/2;
    
    while (samples.size() % blocksize != 0)
    	samples.push_back(1);
    	
    while (samples2.size() % blocksize != 0)
    	samples2.push_back(1);
    
    std::vector<int> blist1;
    std::vector<int> blist2;
    
    for (int i = 0; i < samples.size(); i+= blocksize)
    {
    	blist1.push_back(i);
    }
    
    for (int i = 0; i < samples2.size(); i+= blocksize)
    {
    	blist2.push_back(i);
    }
    
    //encode flag
    
    for (auto& b : blist2)
    	samples2[b] = 0; //zero out first of each block 	
    
    //interleaving
    for (auto& b: blist2){
    	blist1.push_back(b + samples.size());
    }
    	
    for (auto& s : samples2)
    {
    	samples.push_back(s);
    }
    
   
   
    for (int i = 0; i < blist1.size(); i++)
    {
    	int a = rand() % blist1.size();
    	int b = rand() % blist1.size();
    	while (b == a)
    		b = rand() % blist1.size();
    	std::swap(blist1[a], blist1[b]);
    	for (int j = 0; j < blocksize; j++)
    	{
    		std::swap((short&)samples[blist1[a] + j], (short&)samples[blist1[b] + j]);
    	}
    }
    std::ofstream fileout ("song.dat", std::ios::out|std::ios::binary);
    
    if (!fileout.is_open())
    {
    	std::cout<<"Failure to open song.dat\n";
    	exit(0);
    }
    fileout.seekp(0, std::ios::beg);
    fileout.write((char*)&samples[0], samples.size() * 2);
    fileout.close();
    for (auto& b : blist1)
    	std::cout<<b<<", ";
    
    return 1;
}
