#include <StreamProcessor.h>

StreamProcessor::StreamProcessor(string config_file)
{
    configuration = shared_ptr<Configuration>(new Configuration(config_file) );
    source        = shared_ptr<KafkaSource>  (new KafkaSource()              );
    sink          = shared_ptr<KafkaSink>    (new KafkaSink()                );
    initialized   = true; // Changed to false on initialize failure

    if (!source->initialize(configuration))
    {
        cout << "StreamProcessor::initialize : Source failed to initialize" << endl;
        initialized = false;
    }
    if (!sink->initialize(configuration))
    {
        cout << "StreamProcessor::initialize : Sink failed to initialize" << endl;
        initialized = false;
    }
}

void StreamProcessor::applyMap(MapFunction * map_function)
{ 
    sink->write(map_function->map(source->next())); 
}

