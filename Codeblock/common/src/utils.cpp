#include <string>
#include "utils.hpp"

using namespace std;

string GetStdoutFromCommand(string cmd, int buffer_size)
{
    string data;
    FILE * stream;
    char buffer[buffer_size];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream)
    {
        while (!feof(stream))
        if (fgets(buffer, buffer_size, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}
