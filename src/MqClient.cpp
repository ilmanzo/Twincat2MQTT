#include "MqClient.h"
#include <iostream>

MqClient::MqClient(const char *id, const char *host, int port) : mosquittopp(id)
{
    mosqpp::lib_init();			// Initialize libmosquitto

    int keepalive = 120; // seconds
    connect(host, port, keepalive);		// Connect to MQTT Broker
}


//il distruttore dealloca risorse
MqClient::~MqClient()
{
    std::cout << "1" << std::endl;
    if (loop_stop() != MOSQ_ERR_SUCCESS)
    {
        std::cout << "loop_stop failed" << std::endl;
    }

    disconnect();
    std::cout << "2" << std::endl;
    mosqpp::lib_cleanup();
    std::cout << "3" << std::endl;

}


void MqClient::on_connect(int rc)
{

}

void MqClient::on_message(const struct mosquitto_message *message)
{

}

void MqClient::on_subcribe(int mid, int qos_count, const int *granted_qos)
{

}

void MqClient::strpublish(std::string topic, std::string message)
{
    int ret = publish(NULL, topic.c_str(), message.size(), message.c_str(), 1, false);
    if (ret != MOSQ_ERR_SUCCESS)
    {
        std::cout << "Sending failed." << std::endl;
    }
}
