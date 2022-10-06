#ifndef UND_CONFIGURATION_H
#define UND_CONFIGURATION_H

#include <iostream>
#include <librdkafka/rdkafkacpp.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <libconfig.h++>

using namespace std;

class Configuration
{

private:
    // Internal Use
    static libconfig::Config cfg;


    // Source
    string  src_brokers;                  // list of Kafka servers (brokers) hosts and ports of format 'host:port[,host:port]*'
    string  src_topic;                    // Kafka topic for input 
    string  src_group_id;                 // Kafka Consumer Group id (share the load)
    string  src_security_protocol;        // PLAINTEXT, SSL, SASL_PLAINTEXT, SASL_SSL
    string  src_ssl_ca_location;          // Cert info for SSL
    string  src_ssl_certificate_location; // Cert info for SSL
    string  src_ssl_key_location;         // Cert info for SSL
    string  src_ssl_key_password;         // Cert info for SSL
    string  src_starting_offset;          // What offset to use
    bool    src_debug;                    // Print out debug info

    // Sink
    bool   snk_supress_output;           // Do not output data to Sink
    string snk_brokers;                  // list of Kafka servers (brokers) hosts and ports of format 'host:port[,host:port]*'
    string snk_topic;                    // Kafka topic to output to
    string snk_linger_ms;                // how long the producer waits for more messages to be produced by the app before sending them off to the broker in one batch of messages.
    string snk_security_protocol;        // PLAINTEXT, SSL, SASL_PLAINTEXT, SASL_SSL
    string snk_ssl_ca_location;          // Cert info for SSL
    string snk_ssl_certificate_location; // Cert info for SSL
    string snk_ssl_key_location;         // Cert info for SSL
    string snk_ssl_key_password;         // Cert info for SSL
    bool   snk_debug;                    // Print out connection messages

public:
    Configuration() {};
    Configuration(string _config_name) {load(_config_name);};
    ~Configuration() {};

    bool load(string filename);
    bool save(string filename);

    // setters and getters
    // Source
    string  getSrcBrokers()                          { return src_brokers; };
    void    setSrcBrokers(string & x)                { src_brokers = x; };

    string  getSrcTopic()                            { return src_topic; };
    void    setSrcTopic(string & x)                  { src_topic = x; };

    string  getSrcGroupId()                          { return src_group_id; };
    void    setSrcGroupId(string & x)                { src_group_id = x; };

    string  getSrcSecurityProtocol()                 { return src_security_protocol; };
    void    setSrcSecurityProtocol(string & x)       { src_security_protocol = x; };

    string  getSrcSslCaLocation()                    { return src_ssl_ca_location; };
    void    setSrcSslCaLocation(string & x)          { src_ssl_ca_location = x; };

    string  getSrcSslCertificateLocation()           { return src_ssl_certificate_location; };
    void    setSrcSslCertificateLocation(string & x) { src_ssl_certificate_location = x; };

    string  getSrcSslKeyLocation()                   { return src_ssl_key_location; };
    void    setSrcSslKeyLocation(string & x)         { src_ssl_key_location = x; };

    string  getSrcSslKeyPassword()                   { return src_ssl_key_password; };
    void    setSrcSslKeyPassword(string & x)         { src_ssl_key_password = x; };

    int64_t getSrcStartingOffset();                  /* see .cpp for implementation */
    void    setSrcStartingOffset(string x)           { src_starting_offset = x; };
    void    setSrcStartingOffset(int64_t x)          { src_starting_offset = std::to_string(x); };

    bool    getSrcDebug()                            { return src_debug; };
    void    setSrcDebug(bool & x)                    { src_debug = x; };

    // Sink
    bool    getSnkSupressOutput()                    { return snk_supress_output; };
    void    setSnkSupressOutput(bool x)              { snk_supress_output = x; };

    string  getSnkBrokers()                          { return snk_brokers; };
    void    setSnkBrokers(string & x)                { snk_brokers = x; };

    string  getSnkTopic()                            { return snk_topic; };
    void    setSnkTopic(string & x)                  { snk_topic = x; };

    string  getSnkLingerMs()                         { return snk_linger_ms; };
    void    setSnkLingerMs(string & x)               { snk_linger_ms = x; };

    string  getSnkSecurityProtocol()                 { return snk_security_protocol; };
    void    setSnkSecurityProtocol(string & x)       { snk_security_protocol = x; };

    string  getSnkSslCaLocation()                    { return snk_ssl_ca_location; };
    void    setSnkSslCaLocation(string & x)          { snk_ssl_ca_location = x; };

    string  getSnkSslCertificateLocation()           { return snk_ssl_certificate_location; };
    void    setSnkSslCertificateLocation(string & x) { snk_ssl_certificate_location = x; };

    string  getSnkSslKeyLocation()                   { return snk_ssl_key_location; };
    void    setSnkSslKeyLocation(string & x)         { snk_ssl_key_location = x; };

    string  getSnkSslKeyPassword()                   { return snk_ssl_key_password; };
    void    setSnkSslKeyPassword(string & x)         { snk_ssl_key_password = x; };

    bool    getSnkDebug()                            { return snk_debug; };
    void    setSnkDebug(bool & x)                    { snk_debug = x; };

};

#endif
