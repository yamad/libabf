/////////////////////////////////////////////////////////////////////////////
// CComDateTime class helpers
#ifndef INC_COMDATETIME_HPP
#define INC_COMDATETIME_HPP

#define COM_DATETIME_ERROR (-1)

#define COM_DATETIME_HALFSECOND (1.0 / (2.0 * (60.0 * 60.0 * 24.0)))

/////////////////////////////////////////////////////////////////////////////
// CComDateTime class
#include <time.h>

class CComDateTime
{
public:
	enum DateTimeStatus
	{
		valid = 0,
		invalid = 1,    // Invalid date (out of range, etc.)
		null = 2,       // Literally has no value
	};

	DATE           m_dt;
	DateTimeStatus m_status;

// Implementation
protected:
	void CheckRange();

// Constructors
public:
	static CComDateTime GetCurrentTime();

	CComDateTime();

	CComDateTime(const CComDateTime& dateSrc);
	CComDateTime(const VARIANT& varSrc);
	CComDateTime(DATE dtSrc);

	CComDateTime(time_t timeSrc);
	CComDateTime(const SYSTEMTIME& systimeSrc);
	CComDateTime(const FILETIME& filetimeSrc);

	CComDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);
	CComDateTime(WORD wDosDate, WORD wDosTime);

// Attributes
public:
	void SetStatus(DateTimeStatus status) { m_status = status; }
	DateTimeStatus GetStatus() const      { return m_status; }

	BOOL GetAsSystemTime(SYSTEMTIME& sysTime) const;

	int GetYear() const;
	int GetMonth() const;       // month of year (1 = Jan)
	int GetDay() const;         // day of month (0-31)
	int GetHour() const;        // hour in day (0-23)
	int GetMinute() const;      // minute in hour (0-59)
	int GetSecond() const;      // second in minute (0-59)
	int GetDayOfWeek() const;   // 1=Sun, 2=Mon, ..., 7=Sat
	int GetDayOfYear() const;   // days since start of year, Jan 1 = 1

// Operations
public:
	const CComDateTime& operator=(const CComDateTime& dateSrc);
	const CComDateTime& operator=(const VARIANT& varSrc);
	const CComDateTime& operator=(DATE dtSrc);

	const CComDateTime& operator=(const time_t& timeSrc);
	const CComDateTime& operator=(const SYSTEMTIME& systimeSrc);
	const CComDateTime& operator=(const FILETIME& filetimeSrc);

	BOOL operator==(const CComDateTime& date) const;
	BOOL operator!=(const CComDateTime& date) const;
	BOOL operator<(const CComDateTime& date) const;
	BOOL operator>(const CComDateTime& date) const;
	BOOL operator<=(const CComDateTime& date) const;
	BOOL operator>=(const CComDateTime& date) const;

	operator DATE() const { return m_dt; }

	int SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);
	int SetDate(int nYear, int nMonth, int nDay);
	int SetTime(int nHour, int nMin, int nSec);
	BOOL ParseDateTime(LPCTSTR lpszDate, DWORD dwFlags = 0, LCID lcid = LANG_USER_DEFAULT);

	// formatting
	LPCSTR Format(DWORD dwFlags = 0, LCID lcid = LANG_USER_DEFAULT) const;
	LPCSTR Format(LPCTSTR lpszFormat) const;
};

// CComDateTime
inline CComDateTime::CComDateTime()
{ m_dt = 0; SetStatus(valid); }

inline CComDateTime::CComDateTime(const CComDateTime& dateSrc)
{ m_dt = dateSrc.m_dt; m_status = dateSrc.m_status; }
inline CComDateTime::CComDateTime(const VARIANT& varSrc)
{ *this = varSrc; }

inline CComDateTime::CComDateTime(DATE dtSrc)
{ m_dt = dtSrc; SetStatus(valid); }

inline CComDateTime::CComDateTime(time_t timeSrc)
{ *this = timeSrc; }

inline CComDateTime::CComDateTime(const SYSTEMTIME& systimeSrc)
{ *this = systimeSrc; }

inline CComDateTime::CComDateTime(const FILETIME& filetimeSrc)
{ *this = filetimeSrc; }

inline CComDateTime::CComDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{ SetDateTime(nYear, nMonth, nDay, nHour, nMin, nSec); }

inline CComDateTime::CComDateTime(WORD wDosDate, WORD wDosTime)
{ m_status = DosDateTimeToVariantTime(wDosDate, wDosTime, &m_dt) ? valid : invalid; }

inline const CComDateTime& CComDateTime::operator=(const CComDateTime& dateSrc)
{
   m_dt = dateSrc.m_dt; 
   m_status = dateSrc.m_status; 
   return *this;
}

inline BOOL CComDateTime::operator==(const CComDateTime& date) const
{ return (m_status == date.m_status && m_dt == date.m_dt); }

inline BOOL CComDateTime::operator!=(const CComDateTime& date) const
{ return (m_status != date.m_status || m_dt != date.m_dt); }

inline int CComDateTime::SetDate(int nYear, int nMonth, int nDay)
{ return SetDateTime(nYear, nMonth, nDay, 0, 0, 0); }

inline int CComDateTime::SetTime(int nHour, int nMin, int nSec)
// Set date to zero date - 12/30/1899
{ return SetDateTime(1899, 12, 30, nHour, nMin, nSec); }

#endif   // INC_COMDATETIME_HPP
