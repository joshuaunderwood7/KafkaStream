#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>
#include <librdkafka/rdkafkacpp.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <tinyxml.h>

using namespace std;

class Configuration
{

private:
    // Internal Use
    string config_name;

    // Source
    string src_hostname;                 // Host name of Stream Processor (MLMG)
    string src_port;                     // port number of Stream Processor (MLMG)
    string src_brokers;                  // list of Kafka servers (brokers) hosts and ports of format 'host:port[,host:port]*'
    string src_topic;                    // Kafka topic to output to
    string src_group_id;                 // Kafka Consumer Group id (share the load)
    string src_security_protocol;        // PLAINTEXT, SSL, SASL_PLAINTEXT, SASL_SSL
    string src_ssl_ca_location;          // Cert info for SSL
    string src_ssl_certificate_location; // Cert info for SSL
    string src_ssl_key_location;         // Cert info for SSL
    string src_ssl_key_password;         // Cert info for SSL
    string src_starting_offset;          // What offset to use
    string src_partition;                // What partition
    string src_debug;                    // Print out debug info

    // Sink
    string snk_active;                   // 0=off 1=output MTTs to Kafka Server
    string snk_brokers;                  // list of Kafka servers (brokers) hosts and ports of format 'host:port[,host:port]*'
    string snk_topic;                    // Kafka topic to output to
    string snk_linger_ms;                // how long the producer waits for more messages to be produced by the app before sending them off to the broker in one batch of messages.
    string snk_security_protocol;        // PLAINTEXT, SSL, SASL_PLAINTEXT, SASL_SSL
    string snk_ssl_ca_location;          // Cert info for SSL
    string snk_ssl_certificate_location; // Cert info for SSL
    string snk_ssl_key_location;         // Cert info for SSL
    string snk_ssl_key_password;         // Cert info for SSL
    string snk_debug;                    // Print out connection messages

public:
    Configuration(string _config_name) {};
    ~Configuration() {};

    bool load(string filename);
    void save(string filename);

    // setters and getters
    string         getSrcHostname()                         { return src_hostname; };
    unsigned short getSrcPort()                             { return stoi(src_port); };
    string         getSrcBrokers()                          { return src_brokers; };
    string         getSrcTopic()                            { return src_topic; };
    string         getSrcGroupId()                          { return src_group_id; };
    string         getSrcSecurityProtocol()                 { return src_security_protocol; };
    string         getSrcSslCaLocation()                    { return src_ssl_ca_location; };
    string         getSrcSslCertificateLocation()           { return src_ssl_certificate_location; };
    string         getSrcSslKeyLocation()                   { return src_ssl_key_location; };
    string         getSrcSslKeyPassword()                   { return src_ssl_key_password; };
    int64_t        getSrcStartingOffset();                  /* see .cpp for implementation */
    int32_t        getSrcPartition();                       /* see .cpp for implementation */
    bool           getSrcDebug()                            { return src_debug == "1"; };

    void           setSrcHostname(string & x)               { src_hostname = x; };
    void           setSrcPort(unsigned short x)             { src_port = std::to_string(x); };
    void           setSrcBrokers(string & x)                { src_brokers = x; };
    void           setSrcTopic(string & x)                  { src_topic = x; };
    void           setSrcGroupId(string & x)                { src_group_id = x; };
    void           setSrcSecurityProtocol(string & x)       { src_security_protocol = x; };
    void           setSrcSslCaLocation(string & x)          { src_ssl_ca_location = x; };
    void           setSrcSslCertificateLocation(string & x) { src_ssl_certificate_location = x; };
    void           setSrcSslKeyLocation(string & x)         { src_ssl_key_location = x; };
    void           setSrcSslKeyPassword(string & x)         { src_ssl_key_password = x; };
    void           setSrcStartingOffset(string x)           { src_starting_offset = x; };
    void           setSrcStartingOffset(int64_t x)          { src_starting_offset = std::to_string(x); };
    void           setSrcPartition(string x)                { src_partition = x; };
    void           setSrcPartition(int32_t x)               { src_partition = std::to_string(x); };
    bool           getSrcDebug(bool & x)                    { src_debug = (x) ? "1" : "0"; };

    bool           getSnkActive()                           { return snk_active == "1"; };
    string         getSnkBrokers()                          { return snk_brokers; };
    string         getSnkTopic()                            { return snk_topic; };
    string         getSnkLingerMs()                         { return snk_linger_ms; };
    string         getSnkSecurityProtocol()                 { return snk_security_protocol; };
    string         getSnkSslCaLocation()                    { return snk_ssl_ca_location; };
    string         getSnkSslCertificateLocation()           { return snk_ssl_certificate_location; };
    string         getSnkSslKeyLocation()                   { return snk_ssl_key_location; };
    string         getSnkSslKeyPassword()                   { return snk_ssl_key_password; };
    bool           getSnkDebug()                            { return snk_debug == "1"; };

    bool           setSnkActive(bool & x)                   { snk_active = (x) ? "1" : "0"; };
    string         setSnkBrokers(string & x)                { snk_brokers = x; };
    string         setSnkTopic(string & x)                  { snk_topic = x; };
    string         setSnkLingerMs(string & x)               { snk_linger_ms = x; };
    string         setSnkSecurityProtocol(string & x)       { snk_security_protocol = x; };
    string         setSnkSslCaLocation(string & x)          { snk_ssl_ca_location = x; };
    string         setSnkSslCertificateLocation(string & x) { snk_ssl_certificate_location = x; };
    string         setSnkSslKeyLocation(string & x)         { snk_ssl_key_location = x; };
    string         setSnkSslKeyPassword(string & x)         { snk_ssl_key_password = x; };
    bool           setSnkDebug(bool & x)                    { snk_debug = (x) ? "1" : "0"; };


};

#endif
