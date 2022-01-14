#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "InputHandler.h"
#include "Log.h"

std::string ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(' ');
    return (start == std::string::npos) ? "" : s.substr(start);
}

void replaceAllMatches(std::string& data, const std::string& to_replace, const std::string& replace_with)
{
    size_t start_pos = data.find(to_replace);
    while (start_pos != std::string::npos)
    {
        data.replace(start_pos, to_replace.size(), replace_with);
        start_pos = data.find(to_replace);
    }
}

std::string splitStringByChar(const std::string& to_split, const char& split_by)
{
    std::string segment;
    bool already_split = false;

    for (std::string::const_iterator i = to_split.begin(); i < to_split.end(); ++i)
    {
        if(already_split)
            segment += *i;
        else if (*i == split_by && !already_split)
        {
            already_split = true;
            continue;
        }
    }

    return segment;

}

unsigned int extractNumber(std::string& data)
{
    std::string number_s = "0";
    for (std::string::iterator i = data.begin(); i < data.end(); ++i)
    {
        switch (*i)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '-':
            if (*i == '-' && number_s.size() == 1)
            {
                number_s[0] = *i;
                number_s += "0";
                break;
            }
            number_s += *i;
            break;
        default:
            return std::stoi(number_s);
        }
    }

    return 0;
}

int main(int argc, char** argv)
{
    InputHandler ip(argc, argv);

    if (ip.size() < 3 || !ip.isNumber(ip[2].c_str()))
    {
        std::cout << "usage: rotatePDF.exe <filename> <degrees[clockwise]>\n";
        return 0;
    }

    int user_degrees = ip.number(ip[2].c_str());
    if (user_degrees % 90 != 0)
    {
        pfo::LogR("rotatePDF<error>: Degrees value must be divisable by 90\n", PFO_ERROR);
        return 0;
    }
    std::string input_file_path = ip.at(1);
    std::string output_file_path = input_file_path + "-rotated.pdf";

    size_t start = ip.lower(input_file_path).find(".pdf");
    if (start != std::string::npos)
        output_file_path = input_file_path.substr(0, start) + "-rotated.pdf";

    
    std::ifstream in(input_file_path, std::ios::in | std::ios::binary);
    if (!in)
    {
        pfo::useLogColorFor(3, PFO_ERROR);
        pfo::LogR("rotatePDF<error>: unable to open the input file<", PFO_ERROR);
        pfo::LogR(input_file_path.c_str(), PFO_ERROR);
        pfo::LogR(">\n", PFO_ERROR);
        in.close();
        return 0;
    }

    std::ofstream out(output_file_path, std::ios::out | std::ios::binary);
    if (!out)
    {
        pfo::useLogColorFor(3, PFO_ERROR);
        pfo::LogR("rotatePDF<error>: unable to open the output file<", PFO_ERROR);
        pfo::LogR(output_file_path.c_str(), PFO_ERROR);
        pfo::LogR(">\n", PFO_ERROR);
        out.close();
        return 0;
    }

    // Convert file content to string
    std::stringstream ss;
    ss << in.rdbuf();
    std::string file_data = ss.str();
    in.close();

    // Check if theres already a /Rotate statement
    size_t start_pos = file_data.find("/Rotate");
    if (start_pos != std::string::npos)
    {
        while (start_pos != std::string::npos)
        {
            std::string substring = splitStringByChar(file_data.substr(start_pos), ' ');
            substring = ltrim(substring);

            int rotation_number = extractNumber(substring);
            replaceAllMatches(file_data, "/Rotate " + std::to_string(rotation_number), "/[({PDF_ROTATE_PDF_ROTATION_PLACEHOLDER})] " + std::to_string(user_degrees + rotation_number));
            start_pos = file_data.find("/Rotate");
        }
        replaceAllMatches(file_data, "/[({PDF_ROTATE_PDF_ROTATION_PLACEHOLDER})]", "/Rotate");
        out << file_data;
        out.close();
        return 0;
    }
    replaceAllMatches(file_data, "/Contents", "/Rotate " + std::to_string(user_degrees) + "/[({PDF_ROTATE_PDF_CONTENTS_PLACEHOLDER})]");
    replaceAllMatches(file_data, "/[({PDF_ROTATE_PDF_CONTENTS_PLACEHOLDER})]", "/Contents");
    out << file_data;
    out.close();
}

/*
    // if statement in while loop
    start_pos = line.find("/Rotate");
    if (start_pos != std::string::npos)
    {
        std::string substring = splitStringByChar(line.substr(start_pos), ' ');
        substring = ltrim(substring);

        int rotation_number = extractNumber(substring);
        replaceAllMatches(line, "/Rotate " + std::to_string(rotation_number), "/Rotate " + std::to_string(user_degrees + rotation_number));
        out << line << std::endl;
        continue;
    }

    in.clear();
    in.seekg(0, in.beg);

    // Add a new /Rotate statement if theres not already one
    std::string line;
    while (std::getline(in, line))
    {
        start_pos = line.find("/Contents");
        if (start_pos != std::string::npos)
        {
            size_t start_pos = line.find("/Contents");
            line.replace(start_pos, 9, "/Rotate " + std::to_string(user_degrees) + "/Contents");
        }

        out << line << std::endl;
    }

    in.close();
    out.close();
*/