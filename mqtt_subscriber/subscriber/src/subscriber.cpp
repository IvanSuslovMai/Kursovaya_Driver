#include <iostream>
#include <string_view>
#include <fcntl.h>
#include <unistd.h>
 
#include "general.h"
#include "subscriber.h"

using namespace std::literals;

constexpr auto Topic = "update"sv;

Subscriber::Subscriber(const char *id, const char *host, int port)
    : mosquittopp(id)
{
    std::cout << app::AppTag << "Connecting to MQTT Broker with address "
              << host << " and port " << port << std::endl;

    const int keepAlive = 60;

    connect(host, port, keepAlive);
}

void Subscriber::on_connect(int rc)
{
    std::cout << app::AppTag << "Subscriber connected with code "
              << rc << std::endl;

    if (rc == 0)
    {
        subscribe(NULL, Topic.data());
    }
}

void Subscriber::on_message(const struct mosquitto_message *message)
{
    if (Topic == message->topic)
    {
        const std::string_view payload{reinterpret_cast<char*>(message->payload),
                                       static_cast<size_t>(message->payloadlen)};
        std::cout << app::AppTag << "Got message with topic: " << message->topic
                  << ", payload: " << payload 
                  << std::endl;
       	int fd = -1;
       	
       	if ((fd = open ("/update.img", O_RDWR | O_CREAT)) < 0)
	{
		fprintf(stderr, "Error\n");
	}	
       	write(fd, message->payload, message->payloadlen);
       	close(fd);
       	fprintf (stderr, "file safe");
       	
     }
}
void Subscriber::on_subscribe(__rtl_unused int        mid,
                              __rtl_unused int        qos_count,
                              __rtl_unused const int *granted_qos)
{
    std::cout << app::AppTag << "Subscription succeeded." << std::endl;
}
