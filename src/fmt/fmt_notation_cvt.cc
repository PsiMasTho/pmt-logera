#include "fmt_notations.h"

using namespace std;

    // declarations of helpers (implementations below)
namespace
{
        // SetsReps
string _listToxsyr(string const& list);
string _xsyrTolist(string const& xsyr);

        // Weight
string _lbTokg(string const& lb);
string _kgTolb(string const& kg);

        // Distance
string _mileTokm(string const& mile);
string _kmTomile(string const& km);

        // Energy
string _kcalTokj(string const& kcal);
string _kjTokcal(string const& kj);

}// end anonymous namespace


namespace FormatSpecifierNotations
{

string convert(string const& str, SetsReps to)
{
    SetsReps from;
    identify(from, str);

    string ret;

    switch (from)
    {
        case SetsReps::LIST:
            switch (to)
            {
                case SetsReps::LIST:
                    ret = str;
                break;
                case SetsReps::XSYR:
                    ret = _listToxsyr(str);
                break;
            }
        break;
        case SetsReps::XSYR:
            switch (to)
            {
                case SetsReps::LIST:
                    ret = _xsyrTolist(str);
                break;
                case SetsReps::XSYR:
                    ret = str;
                break;
            }
        break;
    }
    return ret;
}

string convert(string const& str, Weight to)
{
    Weight from;
    identify(from, str);

    string ret;

    switch (from)
    {
        case Weight::KG:
            switch (to)
            {
                case Weight::KG:
                    ret = str;
                break;
                case Weight::LB:
                    ret = _kgTolb(str);
                break;
            }
        break;
        case Weight::LB:
            switch (to)
            {
                case Weight::KG:
                    ret = _lbTokg(str);
                break;
                case Weight::LB:
                    ret = str;
                break;
            }
        break;
    }
    return ret;
}

string convert(string const& str, Distance to)
{
    Distance from;
    identify(from, str);

    string ret;

    switch (from)
    {
        case Distance::KM:
            switch (to)
            {
                case Distance::KM:
                    ret = str;
                break;
                case Distance::MILE:
                    ret = _kmTomile(str);
                break;
            }
        break;
        case Distance::MILE:
            switch (to)
            {
                case Distance::KM:
                    ret = _mileTokm(str);
                break;
                case Distance::MILE:
                    ret = str;
                break;
            }
        break;
    }
    return ret;
}

string convert(string const& str, Energy to)
{
    Energy from;
    identify(from, str);

    string ret;

    switch (from)
    {
        case Energy::KCAL:
            switch (to)
            {
                case Energy::KCAL:
                    ret = str;
                break;
                case Energy::KJ:
                    ret = _kcalTokj(str);
                break;
            }
        break;
        case Energy::KJ:
            switch (to)
            {
                case Energy::KCAL:
                    ret = _kjTokcal(str);
                break;
                case Energy::KJ:
                    ret = str;
                break;
            }
        break;
    }
    return ret;
}

}// end namespace FormatSpecifierNotations

    // implementation of helpers
namespace
{
        // SetsReps
string _listToxsyr(string const& list)
{

}
string _xsyrTolist(string const& xsyr)
{

}

        // Weight
string _lbTokg(string const& lb)
{

}

string _kgTolb(string const& kg)
{

}

        // Distance
string _mileTokm(string const& mile)
{

}

string _kmTomile(string const& km)
{

}

        // Energy
string _kcalTokj(string const& kcal)
{

}

string _kjTokcal(string const& kj)
{

}

}