#include "utils.h"

unsigned int tname_len = 7;


vector<string> getdir (string dir)
{
	vector<string> files;

	DIR *dp;
	struct dirent *dirp;
	if((dp = opendir(dir.c_str())) == NULL) {
		cerr << "Error(" << errno << ") opening " << dir << endl;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);

	return files;
}



char tname_to_char(string tname)
{
	return tname[tname.size() - tname_len];
}

unsigned int tname_to_int(string tname)
{
	unsigned int ret = 0;
	for(unsigned int i = tname.size() - tname_len + 1; i < tname.size() - 4; ++i)
	{
		ret *= 10;
		ret += (tname[i] - '0');
	}
	return ret;
}
