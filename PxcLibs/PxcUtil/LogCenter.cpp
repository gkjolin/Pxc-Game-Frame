#include "LogCenter.h"
#include "DateTime.h"

namespace PxcUtil
{

CLogCenter::CLogCenter()
{
	m_pWritingStream = NULL;
}

bool CLogCenter::Init(int iFileID, const char* szFileName)
{
	if (!m_mapFiles.empty())
		return false;

	m_mapFiles[iFileID] = std::ofstream();
	std::ofstream& stream = m_mapFiles[iFileID];
	stream.open(szFileName, std::ios_base::out | std::ios_base::app);
	if (!stream)
	{
		m_mapFiles.erase(iFileID);
		return false;
	}
	return true;
}

void CLogCenter::Release()
{
	std::map<int, std::ofstream>::iterator iter = m_mapFiles.begin();
	for (; iter != m_mapFiles.end(); iter++)
	{
		iter->second.close();
	}
	m_mapFiles.clear();
}

bool CLogCenter::AddFile(int iFileID, const char* szFileName)
{
	if (m_mapFiles.find(iFileID) != m_mapFiles.end())
		return false;

	m_mapFiles[iFileID] = std::ofstream();
	std::ofstream& stream = m_mapFiles[iFileID];
	stream.open(szFileName, std::ios_base::out | std::ios_base::app);
	if (!stream)
	{
		m_mapFiles.erase(iFileID);
		return false;
	}
	return true;
}

void CLogCenter::RemoveFile(int iFileID)
{
	std::map<int, std::ofstream>::iterator iter = m_mapFiles.find(iFileID);
	if (iter != m_mapFiles.end())
	{
		iter->second.close();
		m_mapFiles.erase(iter);
	}
}

CLogCenter& CLogCenter::WriteStream(int iFileID, ELogLevel eLevel)
{
	CRI_SEC(m_lock)
#ifndef _DEBUG
	if (eLevel == ELogLevel_Debug)
	{
		m_pWritingStream = NULL;
		return *this;
	}
#endif
	std::map<int, std::ofstream>::iterator iter = m_mapFiles.find(iFileID);
	if (iter != m_mapFiles.end())
	{
		m_pWritingStream = &iter->second;
		*m_pWritingStream << std::endl;

		DateTimeInfo timeInfo;
		if (DateTime::InformDateTime(DateTime::GetDateTime(), timeInfo))
			*m_pWritingStream << "[" << timeInfo.date << " " << timeInfo.iHour << ":" << timeInfo.iMin << ":" << timeInfo.iSec << "] ";

		switch (eLevel)
		{
		case ELogLevel_Debug:
			*m_pWritingStream << "DEBUG: ";
		case ELogLevel_Warning:
			*m_pWritingStream << "WARNING: ";
		case ELogLevel_Assert:
			*m_pWritingStream << "ASSERT: ";
		case ELogLevel_Error:
			*m_pWritingStream << "ERROR: ";
		}
	}
	else
		m_pWritingStream = NULL;
	return *this;
}

CLogCenter& CLogCenter::WriteAssert(int iFileID, const char* szAsFileName, int iAsLine, const char* szExp, bool bAssert)
{
	CRI_SEC(m_lock)
	WriteStream(iFileID, ELogLevel_Assert) << "(File:" << szAsFileName << " Line:" << iAsLine << " Expression:" << szExp;
	if (m_pWritingStream)
	{
		if (!bAssert)
		{
			*m_pWritingStream << " False) ";
			std::cout << "Log Assert False! File:" << szAsFileName << " Line:" << iAsLine << " Expression:" << szExp << std::endl;
		}
		else
			*m_pWritingStream << " True) ";
	}
	return *this;
}

CLogCenter& CLogCenter::WriteStreamDesc(int iFileID, ELogLevel eLevel, const char* szDesc)
{
	CRI_SEC(m_lock)
	WriteStream(iFileID, eLevel) << szDesc;
	if (m_pWritingStream && (int)eLevel > (int)ELogLevel_Info)
	{
		switch (eLevel)
		{
		case ELogLevel_Warning:
			std::cout << "Log Warning: ";
		case ELogLevel_Assert:
			std::cout << "Log Simple Assert! ";
		case ELogLevel_Error:
			std::cout << "Log Error! ";
		}
		std::cout << szDesc << std::endl;
	}
	return *this;
}

}