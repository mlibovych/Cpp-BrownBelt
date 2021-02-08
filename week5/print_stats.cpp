#include <iostream>
#include <vector>
#include <algorithm>

enum class Gender {
  FEMALE,
  MALE
};

struct Person {
    int age;  // возраст
    Gender gender;  // пол
    bool is_employed;  // имеет ли работу
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    std::vector<typename InputIt::value_type> range_copy(range_begin, range_end);
    auto middle = begin(range_copy) + range_copy.size() / 2;
    nth_element(
        begin(range_copy), middle, end(range_copy),
        [](const Person& lhs, const Person& rhs) {
            return lhs.age < rhs.age;
        }
    );
    return middle->age;
}

void PrintStats(std::vector<Person> persons) {
    int age = ComputeMedianAge(persons.begin(), persons.end());

    std::cout << "Median age = " << age << std::endl;
    //--
    auto gender_it = std::partition(persons.begin(), persons.end(), [](const Person &person) {
        return person.gender == Gender::MALE;
    });
    int female_age = ComputeMedianAge(gender_it, persons.end());
    std::cout << "Median age for females = " << female_age << std::endl;
    int male_age = ComputeMedianAge(persons.begin(), gender_it);
    std::cout << "Median age for males = " << male_age << std::endl;
    //--
    auto females_employed_it = std::partition(gender_it, persons.end(), [](const Person &person) {
        return person.is_employed;
    });
    int female_employed_age = ComputeMedianAge(gender_it, females_employed_it);
    int female_unemployed_age = ComputeMedianAge(females_employed_it, persons.end());
    std::cout << "Median age for employed females = " << female_employed_age << std::endl;
    std::cout << "Median age for unemployed females = " << female_unemployed_age << std::endl;
    //--
    auto males_employed_it = std::partition(persons.begin(), gender_it, [](const Person &person) {
        return person.is_employed;
    });
    int male_employed_age = ComputeMedianAge(persons.begin(), males_employed_it);
    int male_unemployed_age = ComputeMedianAge(males_employed_it, gender_it);
    std::cout << "Median age for employed males = " << male_employed_age << std::endl;
    std::cout << "Median age for unemployed males = " << male_unemployed_age << std::endl;
}

int main() {
    std::vector<Person> persons = {
        {31, Gender::MALE, false},
        {40, Gender::FEMALE, true},
        {24, Gender::MALE, true},
        {20, Gender::FEMALE, true},
        {80, Gender::FEMALE, false},
        {78, Gender::MALE, false},
        {10, Gender::FEMALE, false},
        {55, Gender::MALE, true},
    };
    PrintStats(persons);
    return 0;
}