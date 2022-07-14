#include <KafkaSink.h>

bool KafkaSink::initialize(shared_ptr<Configuration> configuration)
{
    if (!conf) 
    { 
        conf       = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
        conf->set("dr_cb", this, errstr); // Set delivery report callback
    }
    if (!topic_conf) { topic_conf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC); }

    if (topic)      { delete topic;      topic      = 0x0; }
    if (producer)   { delete producer;   producer   = 0x0; }

    if (configuration->getSnkSupressOutput())
    {
        cout << "Kafka output supressed." << endl;
        supress_output = true;
        return true;
    } else supress_output = false;

    setBrokers(configuration->getSnkBrokers());
    setTopic(configuration->getSnkTopic());
    setDebug(configuration->getSnkDebug());
    setLingerMs(configuration->getSnkLingerMs());
    setSecurityProtocol(configuration->getSnkSecurityProtocol());
    setSslCaLocation(configuration->getSnkSslCaLocation());
    setSslCertificateLocation(configuration->getSnkSslCertificateLocation());
    setSslKeyLocation(configuration->getSnkSslKeyLocation());
    setSslKeyPassword(configuration->getSnkSslKeyPassword());

    conf->set("linger.ms"               , linger_ms                , errstr); if (debug) cout << errstr << endl;
    conf->set("security.protocol"       , security_protocol        , errstr); if (debug) cout << errstr << endl;
    if (security_protocol == "SSL")
    {
        conf->set("ssl.ca.location"         , ssl_ca_location          , errstr); if (debug) cout << errstr << endl;
        conf->set("ssl.certificate.location", ssl_certificate_location , errstr); if (debug) cout << errstr << endl;
        conf->set("ssl.key.location"        , ssl_key_location         , errstr); if (debug) cout << errstr << endl;
        conf->set("ssl.key.password"        , ssl_key_password         , errstr); if (debug) cout << errstr << endl;
    }

    // set brokers
    conf->set("metadata.broker.list", brokers, errstr); if (debug) cout << errstr << endl;

    // Create producer using global configuration.
    producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        cout << "Failed to create Kafka Writer: " << errstr << endl;
        return false;
    } cout << "Created producer " << producer->name() << endl;

    // Create topic handle.
    topic = RdKafka::Topic::create(producer, topic_str, topic_conf, errstr);
    if (!topic) {
        cout << "Failed to create topic: " << errstr << endl;
        delete producer;
        return false;
    } cout << "Connected to topic : " << topic_str << endl;

    return true;
};


void KafkaSink::write(string output_string)
{
    if (supress_output) return;

    // Here the output_string is published
    RdKafka::ErrorCode resp =
        producer->produce( topic
                , RdKafka::Topic::PARTITION_UA
                , RdKafka::Producer::RK_MSG_COPY /* Copy payload */
                , const_cast<char *>(output_string.c_str()), output_string.size()
                , NULL
                , NULL
                );

    if (resp != RdKafka::ERR_NO_ERROR)
        cout << "Produce failed: " << RdKafka::err2str(resp) << endl;
    else if (debug)
        cout << "Produced message (" << output_string.size() << " bytes)" << endl;
    producer->poll(0);
};


void KafkaSink::dr_cb (RdKafka::Message &message) {
    if (!debug) return;
    cout << "Message delivery for (" << message.len() << " bytes): " <<
        message.errstr() << endl;
    if (message.key())
        cout << "Key: " << *(message.key()) << ";" << endl;
};


