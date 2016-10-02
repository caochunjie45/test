
#ifndef UTC_DATE_H
#define UTC_DATE_H

void MJD2YearMonthDate(__u32 mjd, __u32* pYear, __u32* pMonth, __u32* pDate);

void GetWeekDay(__u32 year, __u32 month, __u32 date, __u32* pWeekDay);

__u32 YearMonthDate2MJD(__u32 year, __u32 month, __u32 date);

#endif

