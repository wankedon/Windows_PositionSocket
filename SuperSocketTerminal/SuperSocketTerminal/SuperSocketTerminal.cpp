#include "stdafx.h"
#include <vector>
#include <string>
#include "czmq.h"
using namespace std;
static bool connectfag = false;

int main()
{
	zsock_t* serversock = zsock_new(ZMQ_STREAM);
	zsock_bind(serversock, "tcp://127.1.1.1:5050");
	printf("监听于:%s\n", "127.1.1.1:5050");

	int num = 0;
	while (true)
	{
		zmsg_t* request = zmsg_recv(serversock);
		auto msg_len = zmsg_size(request);
		if (request!=nullptr)
		{
			assert(zmsg_size(request) == 2);
			zframe_t* id_frame = zmsg_pop(request);
			size_t size = zframe_size(id_frame);
			vector<byte> id(size);
			memcpy(id.data(), zframe_data(id_frame), size);
			zframe_t* body = zmsg_pop(request);
			int framesize= zframe_size(body);

			if (framesize)
			{
				num++;
				printf("第%d次接受:", num);
				printf("%s\n", (char*)zframe_data(body));
				zmsg_t* reply = zmsg_new();
				zmsg_addmem(reply, id.data(), id.size());
				char* str = "hello client";
				zframe_t* hello = zframe_new(str, strlen(str) + 1);
				zmsg_append(reply, &hello);
				zmsg_send(&reply, serversock);
			}
			else
			{
				if (connectfag == false)
				{
					printf("%s\n", "客户端连接....");
					connectfag = true;
				}
				else
				{
					printf("%s\n", "客户端断开连接....");
					connectfag = false;
				}
			}

			zframe_destroy(&id_frame);
			zframe_destroy(&body);
			zmsg_destroy(&request);
		}
	}
	
	return 0;
}

