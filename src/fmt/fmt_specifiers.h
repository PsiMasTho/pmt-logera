#ifndef INCLUDED_FMT_SPECIFIERS_H
#define INCLUDED_FMT_SPECIFIERS_H

enum FormatSpecifier : int
{			  // notations
	NUMBER,
    SETSREPS, // xsyr OR r, r, r, ...
    WEIGHT,	  // xx kg OR xx lb
    WORD,
    NONE,
    TIME,	  // ONLY 24h
    DURATION, // ONLY dd:hh:mm:ss.xxx
    DISTANCE, // xxx meter OR  xxx kilometer OR xxx mile OR xxx yard
    ENERGY	  // kcal OR kJ
};

#endif