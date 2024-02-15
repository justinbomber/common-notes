#ifndef FHIR_RESOURCES_H
#define FHIR_RESOURCES_H
#include <iostream>
#include <vector>
#include <chrono>

namespace fhir
{
namespace resources
{
enum Code
{
    usual,
    official,
    temp,
    nickname,
    anonymous,
    old,
    maiden,
    home,
    work,
    mobile,
    phone,
    male,
    female,
    other,
    unknown
};

struct Period
{
    std::string *start;
    std::string *end;
};

struct Coding
{
    std::string *system;
    std::string *version;
    std::string *code;
    std::string *display;
    bool userSelected;
};

struct CodeableConcept
{
    Coding *coding;
    std::string *text;
};

struct ContactPoint
{
    std::string *system;
    std::string *_value;
    int _use;
};

struct Identifier
{
    int _use;
    CodeableConcept *_type;
    std::string *_family;
    std::string *_system;
    std::string *_value;
    Period *_period;
};

struct Address
{
    int _use;
    int _type;
    std::string * _text;
    std::vector<std::string> *_line;
    std::string *_city;
    std::string *_district;
    std::string *_state;
    std::string *_postalCode;
    std::string *_counrty;
    Period *_period;
};

struct HumanName
{
    int _use;
    std::string *_text;
    std::string *_family;
    std::vector<std::string> *_given;
    std::vector<std::string> *_prefix;
    std::vector<std::string> *_suffix;
    Period *_period;
};

}
}
#endif // FHIR_RESOURCES_H
