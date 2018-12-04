#include <KafkaSource.h>

bool KafkaSource::initialize(Configuration & conf)
{
    if (connected) close();

    setKafkaBrokers(conf.getSrcBrokers());           
    setKafkaTopic(conf.getSrcTopic());
    setGroupId(conf.getSrcGroupId());
    setSecurityProtocol(conf.getSrcSecurityProtocol());
    setSslCaLocation(conf.getSrcSslCaLocation());
    setSslCertificateLocation(conf.getSrcSslCertificateLocation());
    setSslKeyLocation(conf.getSrcSslKeyLocation());
    setSslKeyPassword(conf.getSrcSslKeyPassword());
    setDebug(conf.getSrcDebug());                   

    // connect to server
    create_consumer();
    return connected;
};


string KafkaSource::next()
{   // fetch next OUMRecord instance
    while (!load_raw()) {};
    return raw_buffer;
};

bool KafkaSource::close()
{   // close the connection
    if (!connected) return false;

    connected = false;
    the_consumer->close();
    delete the_consumer;

    return true;
};


bool KafkaSource::create_consumer() 
{
    string             the_errstr;
    RdKafka::Conf     *the_conf       = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    the_conf->set("security.protocol", security_protocol, the_errstr); if (debug) cout << the_errstr << endl;
    if ( security_protocol == "SSL")
    {
        the_conf->set("ssl.ca.location"         , ssl_ca_location          , the_errstr); if (debug) cout << the_errstr << endl;
        the_conf->set("ssl.certificate.location", ssl_certificate_location , the_errstr); if (debug) cout << the_errstr << endl;
        the_conf->set("ssl.key.location"        , ssl_key_location         , the_errstr); if (debug) cout << the_errstr << endl;
        the_conf->set("ssl.key.password"        , ssl_key_password         , the_errstr); if (debug) cout << the_errstr << endl;
    }

    // set brokers
    the_conf->set("metadata.broker.list", kafka_brokers, the_errstr); if (debug) cout << the_errstr << endl;
    the_conf->set("group.id", group_id, the_errstr); if (debug) cout << the_errstr << endl;

    // set auto.commit
    the_conf->set("enable.auto.commit"     , "true", the_errstr); if (debug) cout << the_errstr << endl;
    the_conf->set("auto.commit.interval.ms", "1000", the_errstr); if (debug) cout << the_errstr << endl;

    // Create producer using global configuration.
    the_consumer = RdKafka::KafkaConsumer::create(the_conf, the_errstr); if (debug) cout << the_errstr << endl;
    if (!the_consumer) 
    {
        cout << "Failed to create Kafka Consumer: " << the_errstr << endl;
        return false;
    } 
    cout << "Created Consumer " << the_consumer->name() << endl;
    //  topic handle.
    std::vector<std::string> topics;
    topics.push_back( kafka_topic );
    the_consumer->subscribe( topics );

    cout << "Connected to topic : " << kafka_topic << endl;
    cout << "Consumer Group : " << group_id << endl;
    std::vector<RdKafka::TopicPartition*> partitions;
    the_consumer->assignment( partitions );
    cout << "Partitions : " << partitions.size() << endl;
    for ( unsigned i = 0; i < partitions.size(); i ++ )
    {
        RdKafka::TopicPartition* partition = partitions[ i ];
        int id = partition->partition();
        cout << "  Partition : id " << id << " offset "  << partition->offset() << endl;
    }

    connected = true;
    return true;
}


bool KafkaSource::load_raw()
{
    unique_ptr<RdKafka::Message> message(the_consumer->consume( 1000 ));
    if ( message == 0 ) return false;
    if ( message->err() == RdKafka::ERR_NO_ERROR  )
    {
        raw_buffer = string((char*)message->payload());
        if (raw_buffer.size() < 1) return false;
        return true;
    }
    if (  message->err() == -185 /* RdKafka::RD_KAFKA_RESP_ERR__TIMED_OUT */
       || message->err() == -191 /* RdKafka::RD_KAFKA_RESP_ERR__PARTITION_EOF */
       )
    {
        /* We don't care about timeouts or "No more message" errors */
        //if (debug) cout << RdKafka::err2str(message->err()) << endl;
        return false;
    }
    if (debug) cout << RdKafka::err2str(message->err()) << " --- " 
                    << "Error number : " << message->err() << endl;
    return false;
}


