#include <sys/stat.h>
#include "Utils.hpp"

using namespace std;

bool writeFile(string filename, Download * download) {
    deleteFile(filename);

    FILE * file = fopen(filename.c_str(), "w");
    if (!file) {
        return false;
    }

    size_t result = fwrite(download->data, sizeof(char), download->size, file);
    fclose(file);

    return (result == download->size);
}

bool deleteFile(string filename) {
    if (fileExists(filename)) {
        return remove(filename.c_str()) == 0;
    }

    return false;
}

bool fileExists(string filename) {
    FILE * file = fopen(filename.c_str(), "r");

    if (file) {
        fclose(file);
        return true;
    }

    return false;
}

bool CreateSubfolder(char* cpath)
{
	string path(cpath);
	return mkpath(path);
}

// http://stackoverflow.com/a/11366985
bool mkpath(string path)
{
    bool bSuccess = false;
    int nRC = ::mkdir(path.c_str(), 0775);
    if(nRC == -1)
    {
        switch(errno)
        {
            case ENOENT:
                //parent didn't exist, try to create it
                if( mkpath(path.substr(0, path.find_last_of('/'))))
                    //Now, try to create again.
                    bSuccess = 0 == ::mkdir(path.c_str(), 0775);
                else
                    bSuccess = false;
                break;
            case EEXIST:
                //Done!
                bSuccess = true;
                break;std::string getHost();
            default:
                bSuccess = false;
                break;
        }
    }
    else
        bSuccess = true;
    
    return bSuccess;
}
