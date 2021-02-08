#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
using namespace std;

namespace Ini {

using Section = unordered_map<string, string>;

class Document {
public:
    Document() = default;
    Document(unordered_map<string, Section> &&new_sections);

    Section& AddSection(string name);
    const Section& GetSection(const string& name) const;
    size_t SectionCount() const;

private:
    unordered_map<string, Section> sections;
};


Document Load(istream& input);

}