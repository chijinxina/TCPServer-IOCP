#pragma once
#include "stdafx.h"
using namespace std;

class TCPServerBootstrap{
public:
	//构造函数
	TCPServerBootstrap() ;
	//虚构函数
	~TCPServerBootstrap();
	//开始监听
	bool StartListen(unsigned short port, string ip);
	void getConnectionClient();
	/*
	释放3个部分步骤：
	1. 清空IOCP线程队列，退出线程
	2. 清空等待accept套接字m_vecAcps
	3. 清空已连接套接字m_vecContInfo并清空缓存
	*/
	void CloseServer();
	//私有成员函数
private:
	//启动CPU*2个线程，返回已启动的线程个数
	int StartWorkThreadPool();
	//获取AcceptEx_和 GetAcceptExSockaddrs 函数指针
	bool GetLPFNAcceptExAndGetAcceptSockaddrs();
	//利用AcceptEx_监听accept请求
	bool PostAccept(COverlappedIOInfo* info);
	//处理accept请求， NumberOfBytes=0表示没有收到第一帧数据， >0时表示收到了第一帧数据
	bool DoAccept(COverlappedIOInfo* info, DWORD NumberOfBytes = 0);
	//投递recv请求
	bool PostRecv(COverlappedIOInfo* info);
	//处理recv请求
	bool DoRecv(COverlappedIOInfo* info);
	//从已连接的socket列表中移除socket及释放空间
	bool DelectLink(SOCKET s);

	//私有成员变量
private:
	//winsock版本类型
	WSAData m_wsaData;
	//端口监听套接字
	SOCKET m_sListen;
	//等待accept的套接字，这些套接字都是没有使用过的，数量为ACCEPT_SOCKET_NUM。同时会有10个套接字等待accept
	vector<SOCKET> m_vecAcps;
	//已建立连接的信息，每个结构含有一个套接字、发送缓冲区、接收缓冲区、对端地址
	vector<COverlappedIOInfo*> m_vecContInfo;
	//操作vector的互斥访问锁
	mutex m_mu;
	//CIOCP封装类
	CIOCP m_iocp;
	//AcceptEx_函数指针
	LPFN_ACCEPTEX m_lpfnAcceptEx;
	//GetAcceptSockAddrs函数指针
	LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptSockAddrs;

	//工作线程池
	std::vector<std::thread> workThreadPool_;
	//工作线程池大小
	size_t size_;
};

typedef Singleton<TCPServerBootstrap> TCPServer;