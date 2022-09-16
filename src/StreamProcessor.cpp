#include <StreamProcessor.h>

StreamProcessor::StreamProcessor(shared_ptr<Configuration> & config_sptr)
{
    configuration = make_shared<Configuration>(    *config_sptr  );
    source        = shared_ptr<KafkaSource>   ( new KafkaSource());
    sink          = shared_ptr<KafkaSink>     ( new KafkaSink()  );
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
};

void StreamProcessor::applyMap(MapFunction * map_function)
{ 
    string source_next = source->next();
    if (source_next == "XX__CONNECTION__TIMED__OUT__XX") return;
    sink->write(map_function->map(source_next)); 
};

