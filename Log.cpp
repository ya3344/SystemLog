#include "pch.h"
#include "Log.h"
#include <string>


TCHAR* Log::GetTimeStamp(const TCHAR* title)
{
	time_t timer;
	struct tm timeInfo;

	timer = time(NULL);
	localtime_s(&timeInfo, &timer);
	
	StringCchPrintf(mTimeData, sizeof(mTimeData),
		TEXT("%s_%04d%02d%02d"), title, timeInfo.tm_year + 1900,
		timeInfo.tm_mon + 1, timeInfo.tm_mday);

	return mTimeData;
}

void Log::WriteLog(const TCHAR* title, const BYTE logLevel,const TCHAR* funcName, const TCHAR* str, ...)
{
	va_list vaList;
	HRESULT hResult;
	time_t timer;
	struct tm timeInfo;
	TCHAR levelBuffer[15];
	TCHAR strBuffer[512];
	static DWORD count = 0;
	unsigned int fileDataLength = 0;

	timer = time(NULL);
	localtime_s(&timeInfo, &timer);

	StringCchPrintf(mTimeData, sizeof(mTimeData),
		TEXT("%s_%04d%02d%02d.txt"), title, timeInfo.tm_year + 1900,
		timeInfo.tm_mon + 1, timeInfo.tm_mday);

	_wfopen_s(&mFp, mTimeData, L"a");
	if (mFp == nullptr)
	{
		wprintf(L"mFp nullptr!\n");
		return;
	}

	switch (logLevel)
	{
	case LOG_LEVEL_DEBUG:
		StringCchCopy(levelBuffer, sizeof(levelBuffer), TEXT("DEBUG"));
		break;
	case LOG_LEVEL_ERROR:
		StringCchCopy(levelBuffer, sizeof(levelBuffer), TEXT("ERROR"));
		break;
	case LOG_LEVEL_DISPLAY:
		StringCchCopy(levelBuffer, sizeof(levelBuffer), TEXT("DISPLAY"));
		break;
	}

	va_start(vaList, str);
	hResult = StringCchVPrintf(strBuffer, sizeof(strBuffer), str, vaList);
	va_end(vaList);

	StringCchPrintf(mFileData, sizeof(mFileData),
		TEXT("[%s][%04d-%02d-%02d %02d:%02d:%02d][%s][%04d][%s][%s]\n"),
			title, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_wday,
			timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, levelBuffer, ++count, funcName, strBuffer);
	
	StringCchLength(mFileData, sizeof(mFileData), &fileDataLength);

	fwrite(mFileData, fileDataLength * 2, 1, mFp);



	wprintf(L"%s\n", mFileData);

	fclose(mFp);
}
