#include <KafkaSource.h>
#include <KafkaSink.h>
#include <Configuration.h>

#include <iostream>

using namespace std;

int main(int argc, const char *argv[])
{
    // Initialize from Configuration file;
    if (argc < 2 || argc > 2) 
    {
        cout << "Usage : " << argv[0] << " [Configuration_file_path] " << endl;
        return 1;
    }

    string config_file = string(argv[1]);
    Configuration config(config_file);

    cout << "Hello world." << endl;
    return 0;
}
