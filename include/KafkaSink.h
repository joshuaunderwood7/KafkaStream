#ifndef KAFKAWRITER_H
#define KAFKAWRITER_H

#include <Configuration.h>

#include <librdkafka/rdkafkacpp.h>

#include <iostream>
#include <string>
#include <memory>
#include <time.h>
#include <deque>

using namespace std;


class KafkaSink : public RdKafka::DeliveryReportCb
{

public:

  KafkaSink(): active(false)
             , brokers("localhost:9092")
             , topic_str("test")
             , changed(false)
             , topic(0x0)
             , producer(0x0)
  {
    conf       = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    topic_conf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    conf->set("dr_cb", this, errstr); // Set delivery report callback
  };

  ~KafkaSink() 
  {
    if (topic)      { delete topic;      topic      = 0x0; }
    if (producer)   { delete producer;   producer   = 0x0; }
  };

  bool initialize(Configuration & configuration);
  void write(string & output_string);
  bool stop() {};

  void setActive           (bool   active_  ){active                   = active_;  changed=true;};
  void setBrokers          (string brokers_ ){brokers                  = brokers_; changed=true;};
  void setTopic            (string topic_   ){topic_str                = topic_;   changed=true;};
  void setDebug            (bool   debug_   ){debug                    = debug_;   changed=true;};
  void setLingerMs         (string lMs_     ){linger_ms                = lMs_;     changed=true;};
  void setSecurityProtocol (string secP_    ){security_protocol        = secP_;    changed=true;};
  void setSslCaLocation    (string sslCA_   ){ssl_ca_location          = sslCA_;   changed=true;};
  void setSslCertificateLocation (string sC_){ssl_certificate_location = sC_;      changed=true;};
  void setSslKeyLocation   (string sslKey_  ){ssl_key_location         = sslKey_;  changed=true;};
  void setSslKeyPassword   (string sslKP_   ){ssl_key_password         = sslKP_;   changed=true;};

  void dr_cb (RdKafka::Message &message);

private:
  // configuration settings
  bool   active;                   // send data to Kafka Queue?
  string brokers;                  // host:ip[,host:ip]* of Kafka Brokers
  string topic_str;                // topic to send data to
  string output_type;              // what type of message to output
  string linger_ms;                // how long the producer waits for more messages to be .._produce()d by the app before sending them off to the broker in one batch of messages.
  string security_protocol;        // PLAINTEXT, SSL, SASL_PLAINTEXT, SASL_SSL
  string ssl_ca_location;          // Cert info for SSL
  string ssl_certificate_location; // Cert info for SSL
  string ssl_key_location;         // Cert info for SSL
  string ssl_key_password;         // Cert info for SSL

  // RdKafka classes and structures
  RdKafka::Conf     * conf;
  RdKafka::Conf     * topic_conf;
  RdKafka::Producer * producer;
  RdKafka::Topic    * topic;
  string              errstr;

  // Gold plating
  bool   changed;
  bool   debug;

  // The actual KafkaSink writer
  void publishString(string & output_string); // write sting to queue

};

#endif
