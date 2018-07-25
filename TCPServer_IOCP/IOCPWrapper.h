#pragma once
#ifndef IOCPWRAPPER_H
#define IOCPWRAPPER_H
#include "stdafx.h"

class CIOCP
{
public:
	//CIOCP构造函数
	CIOCP(int nMaxConcurrent = -1)
	{
		m_hIOCP = NULL;
		if (nMaxConcurrent != -1)
		{
			Create(nMaxConcurrent);
		}
	}
	//CIOCP析构函数
	~CIOCP()
	{
		if (m_hIOCP != NULL)
		{
			CloseHandle(m_hIOCP);
		}
	}

	//关闭IOCP
	bool Close()
	{
		bool bResult = CloseHandle(m_hIOCP);
		m_hIOCP = NULL;
		return bResult;
	}

	//创建IOCP， nMaxConcurrency指定最大线程并发数量， 0 默认为CPU核数
	bool Create(int nMaxConcurrency = 0)
	{
		m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, nMaxConcurrency);  
		//ASSERT(m_hIOCP != NULL);
		return (m_hIOCP != NULL);
	}

	//为设备（文件，socket,邮件槽，管道等）关联一个IOCP
	bool AssociateDevice(HANDLE hDevice, ULONG_PTR Compkey)
	{
		bool fOk = (CreateIoCompletionPort(hDevice, m_hIOCP, Compkey, 0) == m_hIOCP);
		return fOk;
	}

	//为Socket关联一个IOCP
	bool AssociateSocket(SOCKET hSocket, ULONG_PTR CompKey)
	{
		return AssociateDevice((HANDLE)hSocket, CompKey);
	}

	//为IOCP传递事件通知
	bool PostStatus(ULONG_PTR CompKey, DWORD dwNumBytes = 0, OVERLAPPED* po = NULL)
	{
		bool fOk = PostQueuedCompletionStatus(m_hIOCP, dwNumBytes, CompKey, po);
		return fOk;
	}
	//从IO完成端口队列中获取事件通知， IO完成队列中没有事件时，该函数阻塞
	bool GetStatus(ULONG_PTR* pCompKey, PDWORD pdwNumBytes, OVERLAPPED** ppo, DWORD dwMillseconds = INFINITE)
	{
		return GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, pCompKey, ppo, dwMillseconds);
	}
	//获取IOCP对象
	const HANDLE GetIOCP()
	{
		return m_hIOCP;
	}

private:
	HANDLE m_hIOCP;   //IOCP句柄
};
#endif // !IOCPWRAPPER_H

