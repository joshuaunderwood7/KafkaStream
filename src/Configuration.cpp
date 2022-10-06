#include <Configuration.h>

libconfig::Config Configuration::cfg;

template <typename T>
bool safeLookupValue(const libconfig::Setting & setting, string key, T & dest)
{
    try { setting.lookupValue(key, dest); }
    catch(const libconfig::SettingNotFoundException &nfex)
    {
        cout << "Configuration::load : Source settings not found with excetion " << nfex.what() << endl;
        //cerr << "Configuration::load : Source settings not found with excetion " << nfex.what() << endl;
        return false;
    }
    return true;
};

bool Configuration::load(string filename)
{
    Configuration::cfg.setOptions( libconfig::Config::OptionAutoConvert
                                 | libconfig::Config::OptionColonAssignmentForNonGroups
                                 | libconfig::Config::OptionAllowOverrides
                                 | libconfig::Config::OptionFsync
                                 );

    try
    {
        Configuration::cfg.readFile(filename);
    }
    catch(const libconfig::FileIOException &fioex)
    {
        cout << "Configuration::load : I/O error while reading file with excetion : " << fioex.what() << endl;
        cerr << "Configuration::load : I/O error while reading file.with excetion : " << fioex.what() << endl;
        return false;
    }
    catch(const libconfig::ParseException &pex)
    {
        cout << "Configuration::load : Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError() << endl;
        //cerr << "Configuration::load : Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError() << endl;
        return false;
    }

    const libconfig::Setting& root = Configuration::cfg.getRoot();
    try
    {
        const libconfig::Setting &source = root["kafka-stream"]["Source"];
        safeLookupValue<string>(source, "brokers"                  , src_brokers                  );
        safeLookupValue<string>(source, "topic"                    , src_topic                    );
        safeLookupValue<string>(source, "group-id"                 , src_group_id                 );
        safeLookupValue<bool>  (source, "debug"                    , src_debug                    );
        safeLookupValue<string>(source, "security-protocol"        , src_security_protocol        );
        safeLookupValue<string>(source, "starting-offset"          , src_starting_offset          );
        safeLookupValue<string>(source, "ssl-ca-location"          , src_ssl_ca_location          );
        safeLookupValue<string>(source, "ssl-certificate_location" , src_ssl_certificate_location );
        safeLookupValue<string>(source, "ssl-key-location"         , src_ssl_key_location         );
        safeLookupValue<string>(source, "ssl-key-password"         , src_ssl_key_password         );

    }
    catch(const libconfig::SettingNotFoundException &nfex)
    {
        cout << "Configuration::load : Source settings not found with excetion " << nfex.what() << endl;
        //cerr << "Configuration::load : Source settings not found with excetion " << nfex.what() << endl;
        return false;
    }

    try
    {
        const libconfig::Setting &sink = root["kafka-stream"]["Sink"];
        safeLookupValue<string>(sink, "brokers"                  , snk_brokers                  );
        safeLookupValue<string>(sink, "topic"                    , snk_topic                    );
        safeLookupValue<string>(sink, "linger-ms"                , snk_linger_ms                );
        safeLookupValue<bool>  (sink, "supress-output"           , snk_supress_output           );
        safeLookupValue<bool>  (sink, "debug"                    , snk_debug                    );
        safeLookupValue<string>(sink, "security-protocol"        , snk_security_protocol        );
        safeLookupValue<string>(sink, "ssl-ca-location"          , snk_ssl_ca_location          );
        safeLookupValue<string>(sink, "ssl-certificate-location" , snk_ssl_certificate_location );
        safeLookupValue<string>(sink, "ssl-key-location"         , snk_ssl_key_location         );
        safeLookupValue<string>(sink, "ssl-key-password"         , snk_ssl_key_password         );

    }
    catch(const libconfig::SettingNotFoundException &nfex)
    {
        cout << "Configuration::load : Sink settings not found with excetion " << nfex.what() << endl;
        //cerr << "Configuration::load : Sink settings not found with excetion " << nfex.what() << endl;
    }

    return true;
};


bool Configuration::save(string filename)
{
  try
  {
    Configuration::cfg.writeFile(filename);
    cout << "Configuration::save - Updated configuration successfully written to: " << filename << endl;
    //cerr << "Configuration::save - Updated configuration successfully written to: " << filename << endl;
  }
  catch(const libconfig::FileIOException &fioex)
  {
    cout << "Configuration::save - I/O error while writing file: " << filename << endl;
    //cerr << "Configuration::save - I/O error while writing file: " << filename << endl;
    return false;
  }
  return true;
};

int64_t Configuration::getSrcStartingOffset  () { 
    if (src_starting_offset == "from-beginning") return RdKafka::Topic::OFFSET_BEGINNING; //-2
    if (src_starting_offset == "from-end")       return RdKafka::Topic::OFFSET_END; //-1
    if (src_starting_offset == "from-stored")    return RdKafka::Topic::OFFSET_STORED; //-1000
    try { /* If a number is input as offset */   return stoll(src_starting_offset); 
    } catch ( const invalid_argument & e ) {
        cout << "Invalid or missing configuration for Source's \"starting-offset\" "
             << "-intepreting as \"from-end\"" << endl;
        return RdKafka::Topic::OFFSET_END; //-1
    }
};


