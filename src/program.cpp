#include <StreamProcessor.h>

#include <iostream>

using namespace std;

class MyMap : public MapFunction
{
    public:
        int total_msgs = 0;

        virtual string map(string input)
        {
            cout << "Processing \"" << input << "\"" << endl;
            total_msgs++;
            return  input;
        }

        virtual bool running() { return total_msgs < 10; }
};

int main(int argc, const char *argv[])
{
    // Initialize from Configuration file;
    if (argc < 2 || argc > 2) 
    {
        cout << "Usage : " << argv[0] << " [Configuration_file_path] " << endl;
        return 1;
    }

    cout << "starting." << endl;
    string            config_file(argv[1]);
    StreamProcessor   stream_processor(config_file);
    MapFunction     * my_map = new MyMap();

    cout << "running." << endl;
    while(my_map->running())
    {
        stream_processor.applyMap(my_map);
    }

    cout << "done." << endl;
    return 0;
}
