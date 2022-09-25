#ifndef INCLUDED_FMT_NOTATIONS_H
#define INCLUDED_FMT_NOTATIONS_H

#include <string>

namespace FormatSpecifierNotations
{

enum SetsReps : int
{
	LIST,
	XSYR,
};

enum Weight : int
{
	KG,
	LB
};

enum Distance : int
{
	KM,
	MILE
};

enum Energy : int
{
	KCAL,
	KJ
};

	// identifies the notation of the specifier, returns true on success and false if failed
bool identify(SetsReps& out, std::string const& fmt);
bool identify(Weight& out, std::string const& fmt);
bool identify(Distance& out, std::string const& fmt);
bool identify(Energy& out, std::string const& fmt);

	// converts an assumed valid notation in str to another
[[nodiscard]] std::string convert(std::string const& str, SetsReps to);
[[nodiscard]] std::string convert(std::string const& str, Weight to);
[[nodiscard]] std::string convert(std::string const& str, Distance to);
[[nodiscard]] std::string convert(std::string const& str, Energy to);

}

#endif