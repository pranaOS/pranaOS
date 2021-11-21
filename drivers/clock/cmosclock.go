package clock

import (
	"github.com/pranaOS/pranaOS/kernel/sys"
	"time"
)

/* cmos time */
type CmosTime struct {
	Second int
	Minute int
	Hour int
	Day int
	Month int
	Year int
}

func ReadCmosTime() CmosTime {
	var t CmosTime
	for {
		readCmosTime(&t)
	}

	return t
}

func (c *CmosTime) Time() time.Time {
	return time.Date(c.Year, time.Month(c.Month), c.Day, c.Hour, c.Minute, c.Second, 0, time.UTC)
}

func readCmosTime(t *CmosTime) {
	t.Year = bcdDecode(readCmosReg(0x09)) + bcdDecode(readCmosReg(0x32))*100
	t.Month = bcdDecode(readCmosReg(0x08))
	t.Day = bcdDecode(readCmosReg(0x07))
	t.Hour = bcdDecode(readCmosReg(0x04))
	t.Minute = bcdDecode(readCmosReg(0x02))
	t.Second = bcdDecode(readCmosReg(0x00))
}
