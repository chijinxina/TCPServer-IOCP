#include "stdafx.h"
#include "TCPServerBootstrap.h"

int main(int argc, char *argv[])
{
	TCPServerBootstrap* server = TCPServer::getInstance();
	server->StartListen(20000, "0.0.0.0");
	int cmd = 0;
	while (cin >> cmd)
	{
		if (cmd == 1)
		{
			//输入1  关闭TCP服务器
			server->CloseServer();
			break;
		}
		else
		{
			//输入任意数字，打印所有在线客户端信息
			server->getConnectionClient();
		}
	}
	Sleep(5000);
	return 0;
}
