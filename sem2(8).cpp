#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include <iomanip>

using namespace std;

class RecordBook {
private:
    string bookNumber;
    vector<int> grades;

public:
    RecordBook() : bookNumber("000000") {}
    
    explicit RecordBook(string number) : bookNumber(number) {}

    void addGrade(int grade) {
        if (grade >= 2 && grade <= 5) {
            grades.push_back(grade);
        }
    }

    double calculateAverage() const {
        if (grades.empty()) return 0.0;
        double sum = accumulate(grades.begin(), grades.end(), 0.0);
        return sum / grades.size();
    }

    string getNumber() const {
        return bookNumber;
    }
};

class Person {
protected:
    string name;

public:
    explicit Person(string n) : name(n) {}
    virtual ~Person() = default;

    virtual void print() const {
        cout << "Имя: " << name;
    }
};

class Student : public Person {
private:
    RecordBook recordBook;

public:
    Student(string n, string bNumber) : Person(n), recordBook(bNumber) {}

    void addGrade(int g) {
        recordBook.addGrade(g);
    }

    void print() const override {
        Person::print();
        cout << " | Роль: Студент"
             << " | Зачетка: " << recordBook.getNumber()
             << " | Средний балл: " << fixed << setprecision(2) 
             << recordBook.calculateAverage() << endl;
    }
};

class Teacher : public Person {
private:
    string subject;

public:
    Teacher(string n, string sub) : Person(n), subject(sub) {}

    void print() const override {
        Person::print();
        cout << " | Роль: Преподаватель | Предмет: " << subject << endl;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    vector<shared_ptr<Person>> university;

    auto s1 = make_shared<Student>("Алексей", "23-ЭВМ-101");
    s1->addGrade(5);
    s1->addGrade(4);
    s1->addGrade(5);

    auto s2 = make_shared<Student>("Мария", "23-ЭВМ-105");
    s2->addGrade(3);
    s2->addGrade(4);

    university.push_back(make_shared<Teacher>("Иван Петрович", "ООП"));
    university.push_back(s1);
    university.push_back(s2);

    for (const auto& person : university) {
        person->print();
    }

    return 0;
}