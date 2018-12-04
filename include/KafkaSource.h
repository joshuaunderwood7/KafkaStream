#ifndef UND_KAFKASOURCE_H
#define UND_KAFKASOURCE_H

// KafkaSource.h
#include <Configuration.h>

#include <librdkafka/rdkafka.h>
#include <librdkafka/rdkafkacpp.h>

#include <stdlib.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class KafkaSource {

    public:

        KafkaSource() {};
        KafkaSource(shared_ptr<Configuration> conf) { initialize(conf); };
        ~KafkaSource() { close(); };

        // The interface, in call order.
        bool   initialize(shared_ptr<Configuration> conf); // Connect
        string next();                                     // fetch next OUMRecord instance
        bool   close();                                    // close the connection

        bool   isConnected() { return connected; }; // check connection connected

        // Setters for configuration
        void   setKafkaBrokers(string val)           { kafka_brokers = val;             }
        void   setKafkaTopic(string val)             { kafka_topic = val;               }
        void   setGroupId(string val)                { group_id = val;                  }
        void   setSecurityProtocol(string val)       { security_protocol = val;         }
        void   setSslCaLocation(string val)          { ssl_ca_location = val;           }
        void   setSslCertificateLocation(string val) { ssl_certificate_location = val;  }
        void   setSslKeyLocation(string val)         { ssl_key_location = val;          }
        void   setSslKeyPassword(string val)         { ssl_key_password = val;          }
        void   setDebug(bool val)                    { debug = val;                     }

    private:

        // Kafka configurations
        string kafka_brokers = "localhost:9092";  // list of Kafka servers (brokers) hosts and ports of format 'host:port[,host:port]*'
        string kafka_topic = "test";              // Kafka topic to output to
        string group_id = "kafka-source";         // Reported node_id in MTs
        string security_protocol = "PLAINTEXT";   // PLAINTEXT, SSL
        string ssl_ca_location;                   // Cert info for SSL
        string ssl_certificate_location;          // Cert info for SSL
        string ssl_key_location;                  // Cert info for SSL
        string ssl_key_password;                  // Cert info for SSL
        bool debug = true;   

        // Internal flags
        bool connected = false;   

        // message handlers
        bool create_consumer();
        bool load_raw();

        // The KafkaConsumer object
        RdKafka::KafkaConsumer * the_consumer;

        // Data and header containers
        string           raw_buffer;
    
};

#endif

