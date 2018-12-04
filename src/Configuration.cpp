#include <Configuration.h>


void safeAttribute(TiXmlElement * pElem, string & target, const string & key)
{ if (pElem && pElem->Attribute(key.c_str())) target = pElem->Attribute(key.c_str()); }


bool Configuration::load(string filename)
{
    TiXmlDocument doc(filename.c_str());  
    if (!doc.LoadFile()) return false; // could not load file

    TiXmlHandle    hDoc(&doc);
    TiXmlElement * pElem;
    TiXmlHandle    hRoot(0);

    pElem = hDoc.FirstChildElement().Element();
    if (!pElem) return false; // No root element

    hRoot = TiXmlHandle(pElem);

    // Connection is a mandatory Configuration
    pElem = hRoot.FirstChild("Source").Element();
    if (!pElem) return false; //Missing source Configuration
    safeAttribute(pElem, src_hostname                , "hostname");
    safeAttribute(pElem, src_port                    , "port");
    safeAttribute(pElem, src_brokers                 , "brokers");
    safeAttribute(pElem, src_topic                   , "topic");
    safeAttribute(pElem, src_group_id                , "group-id");
    safeAttribute(pElem, src_security_protocol       , "security-protocol");
    safeAttribute(pElem, src_ssl_ca_location         , "ssl-ca-location");
    safeAttribute(pElem, src_ssl_certificate_location, "ssl-certificate-location");
    safeAttribute(pElem, src_ssl_key_location        , "ssl-key-location");
    safeAttribute(pElem, src_ssl_key_password        , "ssl-key-password");
    safeAttribute(pElem, src_starting_offset         , "starting-offset");
    safeAttribute(pElem, src_partition               , "partition");

    pElem = hRoot.FirstChild("Sink").Element();
    if (!pElem) return false; //Missing sink Configuration
    safeAttribute(pElem, snk_active                  , "active");
    safeAttribute(pElem, snk_brokers                 , "brokers");
    safeAttribute(pElem, snk_topic                   , "topic");
    safeAttribute(pElem, snk_linger_ms               , "linger-ms");
    safeAttribute(pElem, snk_security_protocol       , "security-protocol");
    safeAttribute(pElem, snk_ssl_ca_location         , "ssl-ca-location");
    safeAttribute(pElem, snk_ssl_certificate_location, "ssl-certificate-location");
    safeAttribute(pElem, snk_ssl_key_location        , "ssl-key-location");
    safeAttribute(pElem, snk_ssl_key_password        , "ssl-key-password");
    safeAttribute(pElem, snk_debug                   , "debug");

    return true;
};


void Configuration::save(string filename)
{
    TiXmlDocument doc;  
    TiXmlComment* comment;
    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
    doc.LinkEndChild( decl ); 

    TiXmlElement * root = new TiXmlElement(config_name.c_str());  
    doc.LinkEndChild( root );  

    comment  = new TiXmlComment();
    string s = " Settings for " + config_name + " ";
    comment->SetValue(s.c_str());  
    root->LinkEndChild( comment );  


    TiXmlElement * src = new TiXmlElement( "Source" );  
    root->LinkEndChild( src );  
    src->SetAttribute("hostname", src_hostname.c_str());
    src->SetAttribute("port", src_port.c_str()); 
    src->SetAttribute("brokers", src_brokers.c_str());
    src->SetAttribute("topic", src_topic.c_str());
    src->SetAttribute("group-id", src_group_id.c_str());
    src->SetAttribute("security-protocol", src_security_protocol.c_str());
    src->SetAttribute("ssl-ca-location", src_ssl_ca_location.c_str());
    src->SetAttribute("ssl-certificate-location", src_ssl_certificate_location.c_str());
    src->SetAttribute("ssl-key-location", src_ssl_key_location.c_str());
    src->SetAttribute("ssl-key-password", src_ssl_key_password.c_str());
    src->SetAttribute("starting-offset", src_starting_offset.c_str());
    src->SetAttribute("partition", src_partition.c_str());


    TiXmlElement * snk = new TiXmlElement( "Sink" );  
    root->LinkEndChild( snk );  
    snk->SetAttribute("active", snk_active.c_str());
    snk->SetAttribute("brokers", snk_brokers.c_str());
    snk->SetAttribute("topic", snk_topic.c_str());
    snk->SetAttribute("linger-ms", snk_linger_ms.c_str());
    snk->SetAttribute("security-protocol", snk_security_protocol.c_str());
    snk->SetAttribute("ssl-ca-location", snk_ssl_ca_location.c_str());
    snk->SetAttribute("ssl-certificate-location", snk_ssl_certificate_location.c_str());
    snk->SetAttribute("ssl-key-location", snk_ssl_key_location.c_str());
    snk->SetAttribute("ssl-key-password", snk_ssl_key_password.c_str());
    snk->SetAttribute("debug", snk_debug.c_str());


    doc.SaveFile(filename.c_str());  

    delete comment;
    delete decl;
    delete src;
    delete snk;
};

int64_t Configuration::getSrcStartingOffset  () { 
    if (src_starting_offset == "from-beginning") return RdKafka::Topic::OFFSET_BEGINNING; //-2
    if (src_starting_offset == "from-end")       return RdKafka::Topic::OFFSET_END; //-1
    if (src_starting_offset == "from-stored")    return RdKafka::Topic::OFFSET_STORED; //-1000
    try {                                    return stoll(src_starting_offset); 
    } catch ( const invalid_argument & e ) {
        cout << "Invalid Configuration: Connection::starting-offset" << endl;
        cout << "-intepreting as \"from-end\"" << endl;
        return RdKafka::Topic::OFFSET_END; //-1
    }
};


int32_t Configuration::getSrcPartition  () { 
    try { return stoll(src_partition); 
    } catch ( const invalid_argument & e ) {
        cout << "Invalid partition: Connection::partition" << endl;
        cout << "-intepreting as \"0\"" << endl;
        return 0;
    }
};
