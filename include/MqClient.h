#pragma once

#include <mosquittopp.h>
#include <string>


class MqClient : public mosqpp::mosquittopp
{
public:
    MqClient(const char *id, const char *host, int port=1883);
    virtual ~MqClient();

    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subcribe(int mid, int qos_count, const int *granted_qos);

    void strpublish(std::string topic, std::string message);

};

