#ifndef UND_STREAMPROCESSOR_H
#define UND_STREAMPROCESSOR_H

#include <Configuration.h>
#include <KafkaSource.h>
#include <KafkaSink.h>

#include <iostream>
#include <memory>
#include <string>

using namespace std;

class MapFunction
{
    public:
        virtual ~MapFunction() = default;

        virtual string map(string) = 0;
        virtual bool   running()   = 0;   
};

class StreamProcessor
{
    public:
        StreamProcessor(shared_ptr<Configuration> & config_sptr);

        void applyMap(MapFunction * map_function);
        
        // Public facing values
        bool initialized = false;

    protected:
        shared_ptr<KafkaSource>   source;
        shared_ptr<KafkaSink>     sink;
        shared_ptr<Configuration> configuration;
};

#endif
