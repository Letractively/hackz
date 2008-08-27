#include "utils.h"


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
