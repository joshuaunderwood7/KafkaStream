#ifndef UND_KAFKASINK_H
#define UND_KAFKASINK_H

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

  KafkaSink() {};
  KafkaSink(shared_ptr<Configuration> conf) { initialize(conf); };

  ~KafkaSink() 
  {
    if (producer)   { producer->flush(3000); } /* flush the write queue */

    if (topic)      { delete topic;      topic      = 0x0; }
    if (producer)   { delete producer;   producer   = 0x0; }
  };

  bool initialize(shared_ptr<Configuration> configuration);
  void write(string output_string);
  bool stop() { return true; };

  void addToBuffer(unique_ptr<string> output_string);
  void writeBuffer();

  void setBrokers          (string brokers_ ){brokers                  = brokers_; };
  void setTopic            (string topic_   ){topic_str                = topic_;   };
  void setDebug            (bool   debug_   ){debug                    = debug_;   };
  void setLingerMs         (string lMs_     ){linger_ms                = lMs_;     };
  void setSecurityProtocol (string secP_    ){security_protocol        = secP_;    };
  void setSslCaLocation    (string sslCA_   ){ssl_ca_location          = sslCA_;   };
  void setSslCertificateLocation (string sC_){ssl_certificate_location = sC_;      };
  void setSslKeyLocation   (string sslKey_  ){ssl_key_location         = sslKey_;  };
  void setSslKeyPassword   (string sslKP_   ){ssl_key_password         = sslKP_;   };

  void dr_cb (RdKafka::Message &message);

private:

  // configuration settings
  string brokers;                  // host:ip[,host:ip]* of Kafka Brokers
  string topic_str;                // topic to send data to
  string linger_ms;                // how long the producer waits for more messages to be .._produce()d by the app before sending them off to the broker in one batch of messages.
  string security_protocol;        // PLAINTEXT, SSL, SASL_PLAINTEXT, SASL_SSL
  string ssl_ca_location;          // Cert info for SSL
  string ssl_certificate_location; // Cert info for SSL
  string ssl_key_location;         // Cert info for SSL
  string ssl_key_password;         // Cert info for SSL
  bool   debug;
  bool   supress_output = false;

  // RdKafka classes and structures
  RdKafka::Conf               * conf = 0x0;
  RdKafka::Conf               * topic_conf = 0x0;
  RdKafka::Producer           * producer = 0x0;
  RdKafka::Topic              * topic = 0x0;
  string                        errstr;
  deque< unique_ptr<string> >   output_buffer;

};

#endif
