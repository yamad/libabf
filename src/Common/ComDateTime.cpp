#include "wincpp.hpp"
#include "ComDateTime.hpp"
#include "ComDebug.h"

/////////////////////////////////////////////////////////////////////////////
// CComDateTime class HELPER definitions

// Verifies will fail if the needed buffer size is too large
#define MAX_TIME_BUFFER_SIZE    128         // matches that in timecore.cpp
#define MIN_DATE                (-657434L)  // about year 100
#define MAX_DATE                2958465L    // about year 9999

// Half a second, expressed in days
#define HALF_SECOND  (1.0/172800.0)

// One-based array of days in year at month start
static int s_anMonthDays[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

static char s_strDate[MAX_TIME_BUFFER_SIZE] = "";

/////////////////////////////////////////////////////////////////////////////
// CComDateTime class HELPERS - implementation

static BOOL _OleDateFromTm(WORD wYear, WORD wMonth, WORD wDay, WORD wHour, WORD wMinute, WORD wSecond, DATE& dtDest)
{
	// Validate year and month (ignore day of week and milliseconds)
	if (wYear > 9999 || wMonth < 1 || wMonth > 12)
		return FALSE;

	//  Check for leap year and set the number of days in the month
	BOOL bLeapYear = ((wYear & 3) == 0) &&
		((wYear % 100) != 0 || (wYear % 400) == 0);

	int nDaysInMonth = s_anMonthDays[wMonth] - 
                      s_anMonthDays[wMonth-1] +
		                ((bLeapYear && wDay == 29 && wMonth == 2) ? 1 : 0);

	// Finish validating the date
	if (wDay < 1 || wDay > nDaysInMonth || wHour > 23 || wMinute > 59 || wSecond > 59)
		return FALSE;

	// Cache the date in days and time in fractional days

	//It is a valid date; make Jan 1, 1AD be 1
	long nDate = wYear*365L + wYear/4 - wYear/100 + wYear/400 + s_anMonthDays[wMonth-1] + wDay;

	//  If leap year and it's before March, subtract 1:
	if (wMonth <= 2 && bLeapYear)
		--nDate;

	//  Offset so that 12/30/1899 is 0
	nDate -= 693959L;

	double dblTime = (((long)wHour * 3600L) +  // hrs in seconds
		              ((long)wMinute * 60L) +  // mins in seconds
		              ((long)wSecond)) / 86400.;

	dtDest = (double) nDate + ((nDate >= 0) ? dblTime : -dblTime);

	return TRUE;
}

static BOOL _TmFromOleDate(DATE dtSrc, struct tm& tmDest)
{
	// The legal range does not actually span year 0 to 9999.
	if (dtSrc > MAX_DATE || dtSrc < MIN_DATE) // about year 100 to about 9999
		return FALSE;

	long nDays;             // Number of days since Dec. 30, 1899
	long nDaysAbsolute;     // Number of days since 1/1/0
	long nSecsInDay;        // Time in seconds since midnight
	long nMinutesInDay;     // Minutes in day

	long n400Years;         // Number of 400 year increments since 1/1/0
	long n400Century;       // Century within 400 year block (0,1,2 or 3)
	long n4Years;           // Number of 4 year increments since 1/1/0
	long n4Day;             // Day within 4 year block
							//  (0 is 1/1/yr1, 1460 is 12/31/yr4)
	long n4Yr;              // Year within 4 year block (0,1,2 or 3)
	BOOL bLeap4 = TRUE;     // TRUE if 4 year block includes leap year

	double dblDate = dtSrc; // tempory serial date

	// If a valid date, then this conversion should not overflow
	nDays = (long)dblDate;

	// Round to the second
	dblDate += ((dtSrc > 0.0) ? HALF_SECOND : -HALF_SECOND);

	nDaysAbsolute = (long)dblDate + 693959L; // Add days from 1/1/0 to 12/30/1899

	dblDate = fabs(dblDate);
	nSecsInDay = (long)((dblDate - floor(dblDate)) * 86400.);

	// Calculate the day of week (sun=1, mon=2...)
	//   -1 because 1/1/0 is Sat.  +1 because we want 1-based
	tmDest.tm_wday = (int)((nDaysAbsolute - 1) % 7L) + 1;

	// Leap years every 4 yrs except centuries not multiples of 400.
	n400Years = (long)(nDaysAbsolute / 146097L);

	// Set nDaysAbsolute to day within 400-year block
	nDaysAbsolute %= 146097L;

	// -1 because first century has extra day
	n400Century = (long)((nDaysAbsolute - 1) / 36524L);

	// Non-leap century
	if (n400Century != 0)
	{
		// Set nDaysAbsolute to day within century
		nDaysAbsolute = (nDaysAbsolute - 1) % 36524L;

		// +1 because 1st 4 year increment has 1460 days
		n4Years = (long)((nDaysAbsolute + 1) / 1461L);

		if (n4Years != 0)
			n4Day = (long)((nDaysAbsolute + 1) % 1461L);
		else
		{
			bLeap4 = FALSE;
			n4Day = (long)nDaysAbsolute;
		}
	}
	else
	{
		// Leap century - not special case!
		n4Years = (long)(nDaysAbsolute / 1461L);
		n4Day = (long)(nDaysAbsolute % 1461L);
	}

	if (bLeap4)
	{
		// -1 because first year has 366 days
		n4Yr = (n4Day - 1) / 365;

		if (n4Yr != 0)
			n4Day = (n4Day - 1) % 365;
	}
	else
	{
		n4Yr = n4Day / 365;
		n4Day %= 365;
	}

	// n4Day is now 0-based day of year. Save 1-based day of year, year number
	tmDest.tm_yday = (int)n4Day + 1;
	tmDest.tm_year = n400Years * 400 + n400Century * 100 + n4Years * 4 + n4Yr;

	// Handle leap year: before, on, and after Feb. 29.
	if (n4Yr == 0 && bLeap4)
	{
		// Leap Year
		if (n4Day == 59)
		{
			/* Feb. 29 */
			tmDest.tm_mon = 2;
			tmDest.tm_mday = 29;
			goto DoTime;
		}

		// Pretend it's not a leap year for month/day comp.
		if (n4Day >= 60)
			--n4Day;
	}

	// Make n4DaY a 1-based day of non-leap year and compute
	//  month/day for everything but Feb. 29.
	++n4Day;

	// Month number always >= n/32, so save some loop time */
	for (tmDest.tm_mon = (n4Day >> 5) + 1;
		n4Day > s_anMonthDays[tmDest.tm_mon]; tmDest.tm_mon++);

	tmDest.tm_mday = (int)(n4Day - s_anMonthDays[tmDest.tm_mon-1]);

DoTime:
	if (nSecsInDay == 0)
		tmDest.tm_hour = tmDest.tm_min = tmDest.tm_sec = 0;
	else
	{
		tmDest.tm_sec = (int)nSecsInDay % 60L;
		nMinutesInDay = nSecsInDay / 60L;
		tmDest.tm_min = (int)nMinutesInDay % 60;
		tmDest.tm_hour = (int)nMinutesInDay / 60;
	}

	return TRUE;
}

static void _TmConvertToStandardFormat(struct tm& tmSrc)
{
	// Convert afx internal tm to format expected by runtimes (_tcsftime, etc)
	tmSrc.tm_year -= 1900;  // year is based on 1900
	tmSrc.tm_mon -= 1;      // month of year is 0-based
	tmSrc.tm_wday -= 1;     // day of week is 0-based
	tmSrc.tm_yday -= 1;     // day of year is 0-based
}

static double _DoubleFromDate(DATE dt)
{
	// No problem if positive
	if (dt >= 0)
		return dt;

	// If negative, must convert since negative dates not continuous
	// (examples: -1.25 to -.75, -1.50 to -.50, -1.75 to -.25)
	double temp = ceil(dt);
	return temp - (dt - temp);
}

static DATE _DateFromDouble(double dbl)
{
	// No problem if positive
	if (dbl >= 0)
		return dbl;

	// If negative, must convert since negative dates not continuous
	// (examples: -.75 to -1.25, -.50 to -1.50, -.25 to -1.75)
	double temp = floor(dbl); // dbl is now whole part
	return temp + (temp - dbl);
}

/////////////////////////////////////////////////////////////////////////////
// CComDateTime class

CComDateTime CComDateTime::GetCurrentTime()
{
	return CComDateTime(::time(NULL));
}

BOOL CComDateTime::GetAsSystemTime(SYSTEMTIME& sysTime) const
{
	if (GetStatus() != valid)
      return FALSE;

	struct tm tmTemp;
	if (!_TmFromOleDate(m_dt, tmTemp))
      return FALSE;

	sysTime.wYear = (WORD) tmTemp.tm_year;
	sysTime.wMonth = (WORD) tmTemp.tm_mon;
	sysTime.wDayOfWeek = (WORD) (tmTemp.tm_wday - 1);
	sysTime.wDay = (WORD) tmTemp.tm_mday;
	sysTime.wHour = (WORD) tmTemp.tm_hour;
	sysTime.wMinute = (WORD) tmTemp.tm_min;
	sysTime.wSecond = (WORD) tmTemp.tm_sec;
	sysTime.wMilliseconds = 0;
	return TRUE;
}

int CComDateTime::GetYear() const
{
	struct tm tmTemp;

	if (GetStatus() == valid && _TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_year;
	else
		return COM_DATETIME_ERROR;
}

int CComDateTime::GetMonth() const
{
	struct tm tmTemp;

	if (GetStatus() == valid && _TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_mon;
	else
		return COM_DATETIME_ERROR;
}

int CComDateTime::GetDay() const
{
	struct tm tmTemp;

	if (GetStatus() == valid && _TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_mday;
	else
		return COM_DATETIME_ERROR;
}

int CComDateTime::GetHour() const
{
	struct tm tmTemp;

	if (GetStatus() == valid && _TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_hour;
	else
		return COM_DATETIME_ERROR;
}

int CComDateTime::GetMinute() const
{
	struct tm tmTemp;

	if (GetStatus() == valid && _TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_min;
	else
		return COM_DATETIME_ERROR;
}

int CComDateTime::GetSecond() const
{
	struct tm tmTemp;

	if (GetStatus() == valid && _TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_sec;
	else
		return COM_DATETIME_ERROR;
}

int CComDateTime::GetDayOfWeek() const
{
	struct tm tmTemp;

	if (GetStatus() == valid && _TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_wday;
	else
		return COM_DATETIME_ERROR;
}

int CComDateTime::GetDayOfYear() const
{
	struct tm tmTemp;

	if (GetStatus() == valid && _TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_yday;
	else
		return COM_DATETIME_ERROR;
}

const CComDateTime& CComDateTime::operator=(const VARIANT& varSrc)
{
	if (varSrc.vt != VT_DATE)
	{
		CComVariant varTemp(varSrc);
		varTemp.ChangeType(VT_DATE);
		m_dt = varTemp.date;
		SetStatus(valid);
	}
	else
	{
		m_dt = varSrc.date;
		SetStatus(valid);
	}
	return *this;
}

const CComDateTime& CComDateTime::operator=(DATE dtSrc)
{
	m_dt = dtSrc;
	SetStatus(valid);

	return *this;
}

const CComDateTime& CComDateTime::operator=(const time_t& timeSrc)
{
	// Convert time_t to struct tm
	tm *ptm = localtime(&timeSrc);

	if (ptm != NULL)
	{
		BOOL bOK = _OleDateFromTm((WORD)(ptm->tm_year + 1900), (WORD)(ptm->tm_mon + 1), (WORD)ptm->tm_mday,
			                       (WORD)ptm->tm_hour, (WORD)ptm->tm_min, (WORD)ptm->tm_sec, m_dt);
		m_status = bOK ? valid : invalid;
	}
	else
	{
		// Local time must have failed (timsSrc before 1/1/70 12am)
		SetStatus(invalid);
		ASSERT(FALSE);
	}

	return *this;
}

const CComDateTime& CComDateTime::operator=(const SYSTEMTIME& systimeSrc)
{
	BOOL bOK = _OleDateFromTm(systimeSrc.wYear, systimeSrc.wMonth, systimeSrc.wDay, 
		                       systimeSrc.wHour, systimeSrc.wMinute, systimeSrc.wSecond, m_dt);
   m_status = bOK ? valid : invalid;

	return *this;
}

const CComDateTime& CComDateTime::operator=(const FILETIME& filetimeSrc)
{
	// Assume UTC FILETIME, so convert to LOCALTIME
	FILETIME filetimeLocal;
	if (!FileTimeToLocalFileTime( &filetimeSrc, &filetimeLocal))
	{
#ifdef _DEBUG
		DWORD dwError = GetLastError();
		TRACE1("\nFileTimeToLocalFileTime failed. Error = %lu.\n\t", dwError);
#endif // _DEBUG
		m_status = invalid;
	}
	else
	{
		// Take advantage of SYSTEMTIME -> FILETIME conversion
		SYSTEMTIME systime;
		m_status = FileTimeToSystemTime(&filetimeLocal, &systime) ? valid : invalid;

		// At this point systime should always be valid, but...
		if (GetStatus() == valid)
		{
         BOOL bOK = _OleDateFromTm(systime.wYear, systime.wMonth, systime.wDay, 
				                       systime.wHour, systime.wMinute, systime.wSecond, m_dt);
			m_status = bOK ? valid : invalid;
		}
	}

	return *this;
}

BOOL CComDateTime::operator<(const CComDateTime& date) const
{
	ASSERT(GetStatus() == valid);
	ASSERT(date.GetStatus() == valid);

	// Handle negative dates
	return _DoubleFromDate(m_dt) < _DoubleFromDate(date.m_dt);
}

BOOL CComDateTime::operator>(const CComDateTime& date) const
{
   ASSERT(GetStatus() == valid);
	ASSERT(date.GetStatus() == valid);

	// Handle negative dates
	return _DoubleFromDate(m_dt) > _DoubleFromDate(date.m_dt);
}

BOOL CComDateTime::operator<=(const CComDateTime& date) const
{
	ASSERT(GetStatus() == valid);
	ASSERT(date.GetStatus() == valid);

	// Handle negative dates
	return _DoubleFromDate(m_dt) <= _DoubleFromDate(date.m_dt);
}

BOOL CComDateTime::operator>=(const CComDateTime& date) const
{
	ASSERT(GetStatus() == valid);
	ASSERT(date.GetStatus() == valid);

	// Handle negative dates
	return _DoubleFromDate(m_dt) >= _DoubleFromDate(date.m_dt);
}

int CComDateTime::SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{
	BOOL bOK = _OleDateFromTm((WORD)nYear, (WORD)nMonth, (WORD)nDay, 
	                          (WORD)nHour, (WORD)nMin,   (WORD)nSec, m_dt);
   m_status = bOK ? valid : invalid;
	return m_status;
}

BOOL CComDateTime::ParseDateTime(LPCTSTR lpszDate, DWORD dwFlags, LCID lcid)
{
	USES_CONVERSION;
	SCODE sc = VarDateFromStr((LPOLESTR)T2COLE(lpszDate), lcid, dwFlags, &m_dt);
	if (FAILED(sc))
	{
		if (sc == DISP_E_TYPEMISMATCH)
		{
			// Can't convert string to date, set 0 and invalidate
			m_dt = 0;
			SetStatus(invalid);
			return FALSE;
		}
		else  // if (sc == DISP_E_OVERFLOW)
		{
			// Can't convert string to date, set -1 and invalidate
			m_dt = -1;
			SetStatus(invalid);
			return FALSE;
		}
	}

	SetStatus(valid);
	return TRUE;
}

LPCSTR CComDateTime::Format(DWORD dwFlags, LCID lcid) const
{
	USES_CONVERSION;

	// If null, return empty string
	if (GetStatus() == null)
		return NULL;

	// If invalid, return DateTime resource string
	if (GetStatus() == invalid)
		return NULL;

	CComVariant var;
	// Don't need to trap error. Should not fail due to type mismatch
	COM_VERIFY(VarBstrFromDate(m_dt, lcid, dwFlags, &V_BSTR(&var)));
	var.vt = VT_BSTR;

   strncpy(s_strDate, OLE2CT(V_BSTR(&var)), ELEMENTS_IN(s_strDate));
   s_strDate[ELEMENTS_IN(s_strDate)-1] = '\0';
	return s_strDate;
}

LPCSTR CComDateTime::Format(LPCTSTR pFormat) const
{
	struct tm tmTemp;

	// If null, return empty string
	if (GetStatus() == null)
		return NULL;

	// If invalid, return DateTime resource string
	if (GetStatus() == invalid || !_TmFromOleDate(m_dt, tmTemp))
		return NULL;

	// Convert tm from afx internal format to standard format
	_TmConvertToStandardFormat(tmTemp);

	// Fill in the buffer, disregard return value as it's not necessary
	_tcsftime(s_strDate, MAX_TIME_BUFFER_SIZE, pFormat, &tmTemp);
	return s_strDate;
}

void CComDateTime::CheckRange()
{
	if (m_dt > MAX_DATE || m_dt < MIN_DATE) // about year 100 to about 9999
		SetStatus(invalid);
}
