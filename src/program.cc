#include "program.h"

#include "../external/argparse/argparse.hpp"
#include "../external/termcolor/termcolor.hpp"

#include "attributes.h"
#include "csv_generator.h"
#include "date.h"                               
#include "header_parser.h"
#include "log_parser.h"

#include <stdlib.h>
#include <algorithm>
#include <exception>
#include <iostream>
#include <string>

using namespace std;

namespace
{

argparse::ArgumentParser get_arg_parser()
{
    argparse::ArgumentParser ret("Logera", "1.0", argparse::default_arguments::help);

    ret.set_assign_chars("=");

    ret.add_argument("-d", "--directory")
        .nargs(1)
        .help("directory containing log files");

    ret.add_argument("-m", "--manual")
        .nargs(argparse::nargs_pattern::at_least_one)
        .help("manually select files (e.g. glob)");

    ret.add_argument("-v", "--verbose").help("verbose output").implicit_value(true);

    ret.add_argument("-o", "--output").nargs(1).help("output file. stdout if not specified");

    ret.add_argument("-c", "--color")
        .help("colored errors (on/off)")
        .default_value(string{"on"})
        .nargs(1);
    
    return ret;
}

vector<string> get_header_line(header_data const& header)
{
    vector<string> ret{"date", "var"};
    for(size_t idx = 0; idx < header.get_attributes().get_count(); ++idx)
        ret.push_back(header.get_attributes().get_name(idx));

    return ret;
}

void print_error(header_parse_error const& error, bool color)
{
    if (color)
        cerr << termcolor::red;
    cerr << "\tError parsing header file\n";
    if (color)
        cerr << termcolor::bright_yellow;
    cerr << "\t\tFilename:      " << error.filename << '\n';
    cerr << "\t\tError message: " << error.msg << '\n';
    cerr << "\t\tLine:          " << error.line_nr << "\n\n";
    if (color)
        cerr << termcolor::reset;
}

void print_error(log_parse_error const& error, bool color)
{
    if (color)
        cerr << termcolor::red;
    cerr << "\tError parsing log file\n";
    if (color)
        cerr << termcolor::bright_yellow;
    cerr << "\t\tFilename:      " << error.filename << '\n';
    cerr << "\t\tError message: " << error.msg << '\n';
    cerr << "\t\tLine:          " << error.line_nr << "\n\n";
    if (color)
        cerr << termcolor::reset;
}

void print_error(arg_parse_error const& error, bool color)
{
    if (color)
        cerr << termcolor::red;
    cerr << "\tError parsing command line arguments\n";
    if (color)
        cerr << termcolor::bright_yellow;
    cerr << "\t\tError message: " << error.msg << "\n\n";
    if (color)
        cerr << termcolor::reset;
}

} // namespace

program::program(int argc, char** argv)
: m_cfg{}
, m_errors{}
, m_headerData{}
, m_log_data{}
{
    argparse::ArgumentParser cmdl = get_arg_parser();
    try
    {
        cmdl.parse_args(argc, argv); // may throw
    }
    catch(const std::exception& e)
    {
        m_errors.push(arg_parse_error{e.what()});
        return;
    }
    m_cfg = config(cmdl);
}

int program::run()
{
    if (!m_errors.empty())
    {
        print_errors();
        return EXIT_FAILURE;
    }

    if(m_cfg.verbose)
        print_config(m_cfg, cout);

    parse_header(m_cfg.header_file);

    if (!m_errors.empty())
    {
        print_errors();
        return EXIT_FAILURE;
    }

    parse_log_files(m_cfg.log_files);

    if (!m_errors.empty())
    {
        print_errors();
        return EXIT_FAILURE;
    }

    generate_csv();

    return EXIT_SUCCESS;
}

void program::parse_header(filesystem::path const& path)
{
    header_parser headerParser(path);
    m_headerData = headerParser.gen();
    if (m_headerData == nullptr)
        m_errors.push(headerParser.getErrorInfo());
}

void program::parse_log_files(vector<filesystem::path> const& paths)
{
    for (auto const& pth : paths)
    {
        log_parser parser(pth, *m_headerData);
        unique_ptr<log_data> logData = parser.gen();

        // error encountered
        if (logData == nullptr)
        {
            m_errors.push(parser.getErrorInfo());
            continue;
        }

        if (m_errors.empty())
            m_log_data.push_back(std::move(logData));
    }

    if (m_errors.empty())
    {
            // sort the output by date
        stable_sort(begin(m_log_data), end(m_log_data), [](auto& lhs, auto& rhs) {
        return lhs->get_date() < rhs->get_date();
        });
    }
}

void program::print_errors()
{
    bool const color = m_cfg.color;
    if (color)
        cerr << termcolor::red;
    cerr << m_errors.size() << " errors encountered:\n";
    if (color)
        cerr << termcolor::reset;
    while (!m_errors.empty())
    {
        visit([color](auto const& error) { print_error(error, color); }, m_errors.top());
        m_errors.pop();
    }
}

void program::generate_csv()
{
    // write header line
    csv_generator gen(*m_cfg.output_stream);
    gen.write(get_header_line(*m_headerData));

    // write all other lines
    for(auto const& log_data_ptr : m_log_data)
        for(auto const& log_line : log_data_ptr->get_lines())
            gen.write(log_data_ptr->get_date(), log_line);
}