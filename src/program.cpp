#include <StreamProcessor.h>

#include <csignal>
#include <cstdio>
#include <iostream>

#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

using namespace std;

bool   SIGNAL_HALT     = false;
void   signalHandler(int signum);

class MyMap : public MapFunction
{
    public:
        shared_ptr<Configuration> configuration;

        int      total_msgs = 0;
        string   base_name  = "program";
        string   init_time  = "";


        MyMap(shared_ptr<Configuration> & config)
        {
            configuration = make_shared<Configuration>(*config);
            // base_name     = configuration->getXXXBaseName();
            init_time = to_string((uintmax_t)time(NULL));
        }

        virtual string map(string input)
        {
            cout << "Processing \"" << input << "\"" << endl;
            total_msgs++;
            return  input;
        }

        virtual bool running() 
        { 
            if (SIGNAL_HALT) 
            {
                cout << "Halt signal detected, stopping " << base_name 
                     << "." << init_time << endl;
                return false;
            }
            return total_msgs < 10;
        }
};

int main(int argc, const char *argv[])
{
    // Initialize from Configuration file;
    if (argc < 2 || argc > 2) 
    {
        cout << "Usage : " << argv[0] << " [Configuration_file_path] " << endl;
        return 1;
    }

    // Register the Normal kill signals to halt program
    signal(SIGINT,  signalHandler);
    signal(SIGTERM, signalHandler);

    cout << "starting." << endl;
    shared_ptr<Configuration> configuration( new Configuration(argv[1]) );

    StreamProcessor   stream_processor(configuration);
    MapFunction     * my_map = new MyMap(configuration);

    if (!stream_processor.initialized)
    {
        cout << "main - StreamProcessor not initialized, exiting now with Input/output error." << endl;
        return 5; // Input/output error
    }

    cout << "running." << endl;
    while(my_map->running())
    {
        stream_processor.applyMap(my_map);
    }

    delete my_map;
    cout << "done." << endl;
    return 0;
};

//What to do when the program is killed with a SINGAL
void signalHandler(int signum)
{
    cout << "SIGNAL " << signum << " detected. ";
    if (SIGNAL_HALT) 
    {
        cout << "Repeated SIGNAL " << signum 
             << " detected. Killing program immediately" << endl;
        exit(signum);
    }

    cout << "Attempting to halt" << endl;
    SIGNAL_HALT = true;
};

