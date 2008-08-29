#pragma once

#include <string>
#include <vector>
using namespace std;


class texture
{
public:
	unsigned char	*imageData;			// Image Data (Up To 32 Bits)
	unsigned int	bpp;				// Image Color Depth In Bits Per Pixel.
	unsigned int	width;				// Image Width
	unsigned int	height;				// Image Height

public:
	texture(string filename);
	texture(unsigned int width, unsigned int height);
	texture(){};
	~texture(void);

	unsigned char pixel(unsigned int pos_x, unsigned int pos_y, unsigned int rgb);
	void set_pixel(unsigned int pos_x, unsigned int pos_y, unsigned int rgb, unsigned char value);
	unsigned int ssq(texture &t);
	void write(string filename, unsigned int quality);
	void clear();

	unsigned int getWidth(){return width;};
	unsigned int getHeight(){return height;};
	unsigned char *getPointer(){return imageData;};
#ifdef _WIN32
	System::Drawing::Bitmap* toBitmap();
#endif
	void special_reduce();
	void noise_reduction();
	texture find_letters(vector<pair<int, int> > positions);
	pair<unsigned int, pair<unsigned int, unsigned int> > special_xor(string filename, unsigned int posx, unsigned int posy, bool speed);
	bool Load(const char *filename);
	void export_letters(string dir, string pattern, string letters_dir, string ret);
	vector<pair<int, int> > find_positions();
	texture crop_gray(vector<pair<int, int> > positions);
	void add(texture &t, int val = 0, double factor = 1.0);
	void leters_recover(texture &output, vector<pair<pair<unsigned int, pair<unsigned int, unsigned int> >, string> > values, unsigned int let);
	void fill(char val);
	void smooth();
	void special_sort(vector<pair<pair<unsigned int, pair<unsigned int, unsigned int> >, string> > &values);
	void estimate_letter(unsigned int posx1 = 0, unsigned int posy1 = 0, unsigned int posx2 = 0, unsigned int posy2 = 0);
};



class color
{
public:
	unsigned char	R;
	unsigned char	G;
	unsigned char	B;
	color(unsigned char	red, unsigned char green, unsigned char blue){R = red; G = green; B = blue;};
	bool color_close(color &c, unsigned int param = 220);
	unsigned int color_dst(color &c);
};
