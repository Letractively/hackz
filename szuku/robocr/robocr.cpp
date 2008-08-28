#include <vector>
#include <string>
#include <iostream>
#include <dirent.h>
#include <errno.h>

#include "texture.h"
#include "utils.h"

using namespace std;

extern unsigned int my_width;
extern unsigned int my_height;
extern unsigned int tname_len;
texture t2;



int main(int argc, char **argv)
{
	string s(argv[1]);
	string dir = "letters";

	texture t(s);
	t.special_reduce();
	t.noise_reduction();
	vector<pair<int, int> > positions = t.find_positions();
	t2 = t.find_letters(positions);
	texture tmp(s);
	texture tmp2 = tmp.crop_gray(positions);
	tmp.clear();
	texture tmp3(tmp2.width, tmp2.height);
	tmp3.fill(0);
					 
	string ret = "";
	vector<string> files = getdir(dir);
					 vector<pair<pair<unsigned int, pair<unsigned int, unsigned int> >, string> > values[6];

					 for(unsigned int j = 0; j < 6; ++j)
					 {
						 for(unsigned int i = 0; i < files.size(); ++i)
						 {
							if(files[i][files[i].length() - 1] == 'p')
							{
#ifdef _WIN32
								pair<unsigned int, pair<unsigned int, unsigned int> > val = t2.special_xor(files[i], my_width*j, 0, false);
#else
								pair<unsigned int, pair<unsigned int, unsigned int> > val = t2.special_xor(dir + "/" + files[i], my_width*j, 0, false);
#endif
								values[j].push_back(pair<pair<unsigned int, pair<unsigned int, unsigned int> >, string>(val, files[i]));
							}
						 }
						 if(!values[j].empty())
						 {
							 sort(values[j].begin(), values[j].end());
							 tmp2.leters_recover(tmp3, values[j], j);
						 }
					 }

					 t2.add(tmp3, -512, 2);
//					 t2.smooth();

 					 for(unsigned int j = 0; j < 6; ++j)
					 {
						 vector<string> my_files;
						 for(unsigned int i = 0; i < (values[j].size()/4); ++i)
						 {
							 my_files.push_back(values[j][i].second);
						 }
						 vector<pair<pair<unsigned int, pair<unsigned int, unsigned int> >, string> > values2;
						 for(unsigned int i = 0; i < my_files.size(); ++i)
						 {
							if(my_files[i][my_files[i].length() - 1] == 'p')
							{
#ifdef _WIN32
								pair<unsigned int, pair<unsigned int, unsigned int> > val = t2.special_xor(my_files[i], my_width*j, 0, false);
#else
								pair<unsigned int, pair<unsigned int, unsigned int> > val = t2.special_xor(dir + "/" + my_files[i], my_width*j, 0, false);
#endif
								values2.push_back(pair<pair<unsigned int, pair<unsigned int, unsigned int> >, string>(val, my_files[i]));
							}
						 }
						 if(!values2.empty())
						 {
							 sort(values2.begin(), values2.end());
							 ret += values2[0].second[values2[0].second.length() - tname_len];
						 }
					 }



	cout << ret << endl;

	return 0;
}
