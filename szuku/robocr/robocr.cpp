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


int main(int argc, char **argv)
{
	string s(argv[1]);
	string dir = "letters";


					 texture t(s);
					 t.special_reduce();
					 t.noise_reduction();
					 vector<pair<int, int> > positions = t.find_positions();
					 texture t2 = t.find_letters(positions);
					 texture tmp(s);
					 texture tmp2 = tmp.crop_gray(positions);
					 tmp.clear();
					 texture tmp3(tmp2.width, tmp2.height);
					 tmp3.fill(0);
					 
					 string ret = "";
					 vector<string> files = getdir(dir);

					 for(unsigned int j = 0; j < 6; ++j)
					 {
						 vector<pair<pair<unsigned int, pair<unsigned int, unsigned int> >, string> > values;
						 for(unsigned int i = 0; i < files.size(); ++i)
						 {
							if(files[i][files[i].length() - 1] == 'p')
							{
#ifdef _WIN32
								pair<unsigned int, pair<unsigned int, unsigned int> > val = t2.special_xor(files[i], my_width*j, 0, false);
#else
								pair<unsigned int, pair<unsigned int, unsigned int> > val = t2.special_xor(dir + "/" + files[i], my_width*j, 0, false);
#endif
								values.push_back(pair<pair<unsigned int, pair<unsigned int, unsigned int> >, string>(val, files[i]));
							}
						 }
						 if(!values.empty())
						 {
							 sort(values.begin(), values.end());
							 tmp2.leters_recover(tmp3, values, j);
						 }
					 }

					 t2.add(tmp3, -512, 2);
//					 t2.smooth();

 					 for(unsigned int j = 0; j < 6; ++j)
					 {
						 vector<pair<pair<unsigned int, pair<unsigned int, unsigned int> >, string> > values;
						 for(unsigned int i = 0; i < files.size(); ++i)
						 {
							if(files[i][files[i].length() - 1] == 'p')
							{
#ifdef _WIN32
								pair<unsigned int, pair<unsigned int, unsigned int> > val = t2.special_xor(files[i], my_width*j, 0, false);
#else
								pair<unsigned int, pair<unsigned int, unsigned int> > val = t2.special_xor(dir + "/" + files[i], my_width*j, 0, false);
#endif
								values.push_back(pair<pair<unsigned int, pair<unsigned int, unsigned int> >, string>(val, files[i]));
							}
						 }
						 if(!values.empty())
						 {
							 sort(values.begin(), values.end());
							 ret += values[0].second[values[0].second.length() - 6];
						 }
					 }


	cout << ret << endl;
//	t2.write("tmp.bmp", 100);

	return 0;
}
