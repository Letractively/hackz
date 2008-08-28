#ifdef _WIN32
	#using <mscorlib.dll>
#endif

#include "StdAfx.h"


#include "texture.h"
#include "utils.h"
#include "EasyBMP.h"
#include <stdio.h>

#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
using namespace std;

unsigned int my_width = 28;
unsigned int my_height = 26;
unsigned int letter_width = 24;
unsigned int letter_height = 24;
unsigned int recover_letters = 5;

bool texture::Load(const char *filename)			// Loads A TGA File Into Memory
{    
#ifdef _WIN32
	System::Drawing::Bitmap *bitmap = new System::Drawing::Bitmap(STLToManaged(filename));
	this->width = bitmap->Width;
	this->height = bitmap->Height;
#else
	BMP *bitmap = new BMP();
	bitmap->ReadFromFile(filename);
	this->width = bitmap->TellWidth();
	this->height = bitmap->TellHeight();
#endif
	this->bpp = 24;
	unsigned int imageSize = this->width * this->height * this->bpp / 8;
	this->imageData = (unsigned char *)malloc(imageSize);
	for(unsigned int i = 0; i < this->width; ++i)
	{
		for(unsigned int j = 0; j < this->height; ++j)
		{
#ifdef _WIN32
			this->set_pixel(i, j, 0, bitmap->GetPixel(i, j).B);
			this->set_pixel(i, j, 1, bitmap->GetPixel(i, j).G);
			this->set_pixel(i, j, 2, bitmap->GetPixel(i, j).R);
#else

			this->set_pixel(i, j, 0, bitmap->GetPixel(i, j).Blue);
			this->set_pixel(i, j, 1, bitmap->GetPixel(i, j).Green);
			this->set_pixel(i, j, 2, bitmap->GetPixel(i, j).Red);
#endif
		}
	}
	return true;
}
 

texture::texture(string filename)
{
	Load(filename.c_str());
}


void texture::write(string filename, unsigned int quality)
{
	BMP output;
	output.SetSize(this->width, this->height);
	output.SetBitDepth(24);
 	for (unsigned int y = 0; y < height; ++y)
	{
		for(unsigned int x = 0; x < width; ++x)
		{
			RGBApixel newpixel;
			newpixel.Alpha = 255;
			newpixel.Red = this->pixel(x, y, 2);
			newpixel.Green = this->pixel(x, y, 1);
			newpixel.Blue = this->pixel(x, y, 0);
			output.SetPixel(x, y, newpixel);
		}
	}

	output.WriteToFile(filename.c_str());
}



texture::texture(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	this->bpp = 24;
	this->imageData = new unsigned char[width * height * bpp/8];
}

unsigned char texture::pixel(unsigned int pos_x, unsigned int pos_y, unsigned int rgb)
{
	return (imageData)[(pos_y * width + pos_x) * 3 + rgb];
}

void texture::set_pixel(unsigned int pos_x, unsigned int pos_y, unsigned int rgb, unsigned char value)
{
	(imageData)[(pos_y * width + pos_x) * 3 + rgb] = value;
}

void texture::clear()
{
	free(imageData);
}


texture::~texture(void)
{
}

void texture::add(texture &t, int val, double factor)
{
	for(unsigned int i = 0; i < this->width; ++i)
	{
		for(unsigned int j = 0; j < this->height; ++j)
		{
			int b = t.pixel(i, j, 0)*factor + this->pixel(i, j , 0) + val;
			int g = t.pixel(i, j, 1)*factor + this->pixel(i, j , 1) + val;
			int r = t.pixel(i, j, 2)*factor + this->pixel(i, j , 2) + val;
			if(r <= 0)   r = 0;
			if(r >= 255) r = 255;
			if(g <= 0)   g = 0;
			if(g >= 255) g = 255;
			if(b <= 0)   b = 0;
			if(b >= 255) b = 255;
			this->set_pixel(i, j, 0, b);
			this->set_pixel(i, j, 1, g);
			this->set_pixel(i, j, 2, r);
		}
	}
}



#ifdef _WIN32
System::Drawing::Bitmap* texture::toBitmap()
{
	System::Drawing::Bitmap *output = new System::Drawing::Bitmap(this->width, this->height);
	for(unsigned int i = 0; i < this->width; ++i)
	{
		for(unsigned int j = 0; j < this->height; ++j)
		{
			output->SetPixel(i, j, System::Drawing::Color::FromArgb(pixel(i, j, 2), pixel(i, j, 1), pixel(i, j, 0)));
		}
	}
	return output;
}
#endif


void texture::special_reduce()
{
	color c1(253, 250, 0);
	color c2(58, 255, 255);
	color c3(0, 0, 0);

	for(unsigned int i = 0; i < width; ++i)
		 for(unsigned int j = 0; j < height; ++j)
		 {
			 color p(pixel(i, j, 2), pixel(i, j, 1), pixel(i, j, 0));
			 if(p.color_close(c1))
			 {
				 unsigned int dst = p.color_dst(c1);
				 this->set_pixel(i, j, 0, dst/2);
				 this->set_pixel(i, j, 1, dst/2);
				 this->set_pixel(i, j, 2, dst/2);
			 }
			 else if(p.color_close(c2))
			 {
				 unsigned int dst = p.color_dst(c2);
				 color c(dst/2, dst/2, dst/2);
				 this->set_pixel(i, j, 0, dst/2);
				 this->set_pixel(i, j, 1, dst/2);
				 this->set_pixel(i, j, 2, dst/2);
			 }
			 else if(p.color_close(c3))
			 {
				 unsigned int dst = p.color_dst(c3);
				 color c(dst/2, dst/2, dst/2);
				 this->set_pixel(i, j, 0, dst/2);
				 this->set_pixel(i, j, 1, dst/2);
				 this->set_pixel(i, j, 2, dst/2);
			 }
			 else
			 {
				 this->set_pixel(i, j, 0, 255);
				 this->set_pixel(i, j, 1, 255);
				 this->set_pixel(i, j, 2, 255);
			 }
		 }
}

void texture::noise_reduction()
{
	for(unsigned int i = 0; i < (this->width/4); ++i)
		for(unsigned int j = 0; j < (this->height/4); ++j)
		{
			unsigned int rval = 0;
			for(unsigned int k = 0; k < 4; ++k)
			{
				for(unsigned int l = 0; l < 4; ++l)
				{
					if(pixel(i*4 + k, j*4+l, 0) < 128)
					{
						rval++;
					}
				}
			}
			if(rval < 4)
			{
				for(unsigned int k = 0; k < 4; ++k)
					for(unsigned int l = 0; l < 4; ++l)
					{
						this->set_pixel(i*4 + k, j*4+l, 0, 255);
						this->set_pixel(i*4 + k, j*4+l, 1, 255);
						this->set_pixel(i*4 + k, j*4+l, 2, 255);
					}
			}
		}
}

void re_avg(vector<pair<int, int> > &positions)
{
	sort(positions.begin(), positions.end());

	int avg_x = 0;
	int avg_y = 0;
	int avg_dst = 0;
	for(unsigned int g = 0; g < positions.size(); ++g)
	{
		avg_x += positions[g].first;
		avg_y += positions[g].second;
	}
	avg_x /= positions.size();
	avg_y /= positions.size();
	for(unsigned int i = 0; i < 5; ++i)
	{
		avg_dst += (positions[i + 1].first - positions[i].first);
	}
	avg_dst /= 5;

	vector<int> tmp;
    tmp.push_back(avg_x - 3.5 * avg_dst);
	for(unsigned int g = 0; g < positions.size(); ++g)
		tmp.push_back(positions[g].first);

	tmp.push_back(avg_x + 3.5 * avg_dst);


	for(unsigned int g = 0; g < positions.size(); ++g)
	{
		positions[g].first = 0.34*positions[g].first + 0.33*tmp[g] + 0.33*tmp[g+2];
		positions[g].second = avg_y;
	}
}

vector<pair<int, int> > texture::find_positions()
{
	unsigned int h = height;
	unsigned int w = width;

	vector<int> widths;
	vector<pair<int, int> > positions;

 	for(unsigned int i = 0; i < w - 16; ++i)
		widths.push_back(i);

	for(unsigned g = 0; g < 6; ++g)
	{
		vector<pair<int, pair<int, int> > > values;
		for(unsigned int mm = 0; mm < widths.size(); ++mm)
		{
			unsigned int i = widths[mm];
			unsigned int rval = 0;
			for(unsigned int k = 0; k < 16; ++k)
			{
				for(unsigned int l = 0; l < 16; ++l)
				{
					if(pixel(i + k, l, 0) < 128)
					{
						rval++;
					}
				}
			}
			values.push_back(pair<int, pair<int, int> >(rval, pair<int, int>(i, 0)));

			for(unsigned int j = 1; j < h - 16; ++j)
			{
				for(unsigned int k = 0; k < 16; ++k)
				{
					if(pixel(i + k, j, 0) < 128)
					{
						rval--;
					} 
					if(pixel(i + k, j + 16, 0) < 128)
					{
						rval++;
					} 
				}
			values.push_back(pair<int, pair<int, int> >(rval, pair<int, int>(i, j)));
			}
		}
		pair<int, pair<int, int> > val = *max_element(values.begin(), values.end());
		positions.push_back(pair<int, int>(val.second.first, val.second.second));
		vector<int> tmp;
		for(unsigned ttt = 0; ttt < widths.size(); ++ttt)
		{
			if(widths[ttt] < val.second.first - 16 || widths[ttt] > val.second.first + 16)
				tmp.push_back(widths[ttt]);
		}
		widths = tmp;
	}

	re_avg(positions);
	return positions;
}

void texture::fill(char val)
{
	for(unsigned int i = 0; i < this->width; ++i)
		for(unsigned int j = 0; j < this->height; ++j)
		{
			set_pixel(i, j, 0, val);
			set_pixel(i, j, 1, val);
			set_pixel(i, j, 2, val);
		}
}

void texture::smooth()
{
	for(unsigned int i = 0; i < this->width - 1; ++i)
		for(unsigned int j = 0; j < this->height - 1; ++j)
		{
			unsigned int b = pixel(i, j, 0)*0.5 + pixel(i + 1, j, 0)*0.25 + pixel(i, j + 1, 0)*0.25 + pixel(i + 1, j + 1, 0)*0.0;
			unsigned int g = pixel(i, j, 1)*0.5 + pixel(i + 1, j, 1)*0.25 + pixel(i, j + 1, 1)*0.25 + pixel(i + 1, j + 1, 1)*0.0;
			unsigned int r = pixel(i, j, 2)*0.5 + pixel(i + 1, j, 2)*0.25 + pixel(i, j + 1, 2)*0.25 + pixel(i + 1, j + 1, 2)*0.0;
			set_pixel(i, j, 0, b);
			set_pixel(i, j, 1, g);
			set_pixel(i, j, 2, r);
		}
}



texture texture::crop_gray(vector<pair<int, int> > positions)
{
	color c(128, 128, 128);
	unsigned int h = height;
	unsigned int w = width;

	texture output(my_width*6, my_height);
	for(unsigned int g = 0; g < positions.size(); ++g)
	{
		int posx1 = positions[g].first + 8 - my_width/2;
		int posy1 = positions[g].second + 8 - my_height/2;
		for(unsigned int i = 0; i < my_width; ++i)
		{
			for(unsigned int j = 0; j < my_height; ++j)
			{
				color p(pixel(posx1 + i, posy1 + j, 2), pixel(posx1 + i, posy1 + j, 1), pixel(posx1 + i, posy1 + j, 0));
				if(p.color_close(c, 100))
				{
					unsigned int dst = p.color_dst(c);
					output.set_pixel(g * my_width + i, j, 0, dst/2);
					output.set_pixel(g * my_width + i, j, 1, dst/2);
					output.set_pixel(g * my_width + i, j, 2, dst/2);
				}
				else
				{
					output.set_pixel(g * my_width + i, j, 0, 255);
					output.set_pixel(g * my_width + i, j, 1, 255);
					output.set_pixel(g * my_width + i, j, 2, 255);
				}
			}
		}
	}
	return output;
}

texture texture::find_letters(vector<pair<int, int> > positions)
{
	unsigned int h = height;
	unsigned int w = width;

	texture output(my_width*6, my_height);
	for(unsigned int g = 0; g < positions.size(); ++g)
	{
		int posx1 = positions[g].first + 8 - my_width/2;
		int posy1 = positions[g].second + 8 - my_height/2;
		for(unsigned int i = 0; i < my_width; ++i)
		{
			for(unsigned int j = 0; j < my_height; ++j)
			{
				output.set_pixel(g * my_width + i, j, 0, pixel(posx1 + i, posy1 + j, 0));
				output.set_pixel(g * my_width + i, j, 1, pixel(posx1 + i, posy1 + j, 1));
				output.set_pixel(g * my_width + i, j, 2, pixel(posx1 + i, posy1 + j, 2));
			}
		}
	}

	for(unsigned int g = 0; g < positions.size(); ++g)
	{
		int posx1 = positions[g].first + 8 - my_width/2;
		int posy1 = positions[g].second + 8 - my_height/2;
		int posx2 = positions[g].first + 8 + my_width/2;
		int posy2 = positions[g].second + 8 + my_height/2;
		if(posx1 < 0) posx1 = 0;
		if(posy1 < 0) posy1 = 0;
		if(posx2 >= w) posx2 = w -1;
		if(posy2 >= h) posy2 = h - 1;
		for(unsigned int i = posx1; i < posx2; ++i)
		{
			this->set_pixel(i, posy1, 0, 0);
			this->set_pixel(i, posy1, 1, 0);
			this->set_pixel(i, posy1, 2, 255);
			this->set_pixel(i, posy2, 0, 0);
			this->set_pixel(i, posy2, 1, 0);
			this->set_pixel(i, posy2, 2, 255);
		}
		for(unsigned int i = posy1; i < posy2; ++i)
		{
			this->set_pixel(posx1, i, 0, 0);
			this->set_pixel(posx1, i, 1, 0);
			this->set_pixel(posx1, i, 2, 255);
			this->set_pixel(posx2, i, 0, 0);
			this->set_pixel(posx2, i, 1, 0);
			this->set_pixel(posx2, i, 2, 255);
		}
	}
	return output;
}

map<string, texture> memory;
pair<unsigned int, pair<unsigned int, unsigned int> > texture::special_xor(string filename, unsigned int posx, unsigned int posy, bool speed)
{
	texture cbmp;
	if(memory.find(filename) == memory.end())
	{
		if(!cbmp.Load(filename.c_str())) return pair<unsigned int, pair<unsigned int, unsigned int> >(-1, pair<unsigned int, unsigned int>(-1, -1));
		memory[filename] = cbmp;
	}
	else
	{
		cbmp = memory[filename];
	}
	vector<pair<unsigned int, pair<unsigned int, unsigned int> > > values;
	if(speed)
	{
		unsigned int i = (my_width - letter_width)/2;
		unsigned int j = (my_height - letter_height)/2;
		unsigned int rval = 0;
		for(unsigned int k = 0; k < letter_width; ++k)
		{
			for(unsigned int l = 0; l < letter_height; ++l)
			{
				unsigned int dst = abs(cbmp.pixel(k, l, 2) - pixel(posx + i + k, posy + j + l, 2));
				rval += dst;
			}
		}
		pair<unsigned int, unsigned int> pos(i, j);
		return pair<unsigned int, pair<unsigned int, unsigned int> >(rval, pos);
	}
	else
	{
	for(unsigned int i = 0; i <= my_width - letter_width; ++i)
	{
		for(unsigned int j = 0; j <= my_height - letter_height; ++j)
		{
			unsigned int rval = 0;
			for(unsigned int k = 0; k < letter_width; ++k)
			{
				for(unsigned int l = 0; l < letter_height; ++l)
				{
					unsigned int dst = abs(cbmp.pixel(k, l, 2) - pixel(posx + i + k, posy + j + l, 2));
					rval += dst;
				}
			}
			pair<unsigned int, unsigned int> pos(i, j);
			values.push_back(pair<unsigned int, pair<unsigned int, unsigned int> >(rval, pos));
		}
	}
	return *min_element(values.begin(), values.end());
	}
}

void texture::export_letters(string dir, string pattern, string letters_dir, string ret)
{
	unsigned int letters_num = this->width / my_width;
	if(pattern.length() != letters_num) return;
	for(unsigned int l = 0; l < letters_num; ++l)
	{
		vector<string> lst1 = getdir(dir);
		vector<string> lst2 = getdir(letters_dir);
		unsigned int min_n = 0;
		for(unsigned int i = 0; i < lst1.size(); ++i)
			if(pattern[l] == tname_to_char(lst1[i]) && tname_to_int(lst1[i]) >= min_n)
				min_n = tname_to_int(lst1[i]) + 1;
		for(unsigned int i = 0; i < lst2.size(); ++i)
			if(pattern[l] == tname_to_char(lst2[i]) && tname_to_int(lst2[i]) >= min_n)
				min_n = tname_to_int(lst2[i]) + 1;

		texture tmp(letter_width, letter_height);
		for(unsigned int i = 0; i < letter_width; ++i)
		{
			for(unsigned int j = 0; j < letter_height; ++j)
			{
				tmp.set_pixel(i, j, 0, this->pixel(l * my_width + i + (my_width - letter_width)/2, j + (my_height - letter_height)/2, 0));
				tmp.set_pixel(i, j, 1, this->pixel(l * my_width + i + (my_width - letter_width)/2, j + (my_height - letter_height)/2, 1));
				tmp.set_pixel(i, j, 2, this->pixel(l * my_width + i + (my_width - letter_width)/2, j + (my_height - letter_height)/2, 2));
			}
		}
		if(pattern[l] != ret[l])
		{
			char pom[255];
			sprintf(pom, "%02d\0", min_n);
			tmp.write(dir + pattern[l] + pom + ".bmp", 100);
		}
	}
}

void texture::leters_recover(texture &output, vector<pair<pair<unsigned int, pair<unsigned int, unsigned int> >, string> > values, unsigned int let)
{
	if(values.size() < recover_letters) return;
	for(unsigned int m = 0; m < recover_letters; ++m)
	{
		texture cbmp;
#ifdef _WIN32
		string filename = values[m].second;
#else
		string filename = "letters/" + values[m].second;
#endif
		if(memory.find(filename) == memory.end())
		{
			if(!cbmp.Load(filename.c_str())) return;
				memory[filename] = cbmp;
		}
		else
		{
			cbmp = memory[filename];
		}
		unsigned int posx = values[m].first.second.first;
		unsigned int posy = values[m].first.second.second;
		for(unsigned int ip = 0; ip < my_width; ++ip)
		{
			for(unsigned int jp = 0; jp < my_height; ++jp)
			{
				int b = 25;
				int g = 25;
				int r = 25;
 
				if(ip >= posx && ip < (posx + letter_width) && jp >= posy && jp < (posx + letter_height))
				{
					b = (255 - (255 - this->pixel(ip + let*my_width, jp, 0))*(255 - cbmp.pixel(ip - posx, jp - posy, 0))/256)/recover_letters;
					g = (255 - (255 - this->pixel(ip + let*my_width, jp, 1))*(255 - cbmp.pixel(ip - posx, jp - posy, 1))/256)/recover_letters;
					r = (255 - (255 - this->pixel(ip + let*my_width, jp, 2))*(255 - cbmp.pixel(ip - posx, jp - posy, 2))/256)/recover_letters;
				}
				output.set_pixel(ip + let*my_width, jp, 0, output.pixel(ip + let*my_width, jp, 0) + b);
				output.set_pixel(ip + let*my_width, jp, 1, output.pixel(ip + let*my_width, jp, 1) + g);
				output.set_pixel(ip + let*my_width, jp, 2, output.pixel(ip + let*my_width, jp, 2) + r);
			}
		}
	}
}

void texture::special_sort(vector<pair<pair<unsigned int, pair<unsigned int, unsigned int> >, string> > &values)
{
	sort(values.begin(), values.end());
}



////////////////////////////////////////////////////////////////////


bool color::color_close(color &c, unsigned int param)
{
	 if(abs(R - c.R) + abs(G - c.G) + abs(B - c.B) < param) return true;
	 return false;
}

unsigned int color::color_dst(color &c)
{
	 return (abs(R - c.R) + abs(G - c.G) + abs(B - c.B));
}
