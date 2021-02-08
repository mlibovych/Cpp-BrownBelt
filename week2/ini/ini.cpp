#include "ini.h"

namespace Ini {
    Section& Document::AddSection(string name) {
        return sections[name];
    }

    const Section& Document::GetSection(const string& name) const {
        return sections.at(name);
    }

    size_t Document::SectionCount() const {
        return sections.size();
    }

    Document Load(istream& input) {
        unordered_map<string, Section> sections;
        string section_name;
        
        for (string line; getline(input, line); ) {
            if (line[0] == '[' && line[line.size() - 1] == ']') {
                section_name = line.substr(1, line.size() - 2);
                sections[section_name];
            }
            else if (!line.empty()) {
                size_t pos = line.find('=');
                string key = line.substr(0, pos);
                string value = line.substr(pos + 1);

                // std::cout << key << "   " << value << std::endl;
                sections[section_name][key] = value;
            }
        }
        return Document(move(sections));
    }

    void processNewLine(string &line,  unordered_map<string, Section> &sections) {

    }

    Document::Document(unordered_map<string, Section> &&new_sections) : 
                        sections(new_sections)
    {
        //
    }
}

