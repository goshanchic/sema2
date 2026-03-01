#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include <iomanip>
#include <algorithm>

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
        return static_cast<double>(accumulate(grades.begin(), grades.end(), 0)) / grades.size();
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

    string getName() const { return name; }
};

class Student : public Person {
private:
    RecordBook recordBook;

public:
    Student(string n, string bNumber) : Person(n), recordBook(bNumber) {}

    void addGrade(int g) {
        recordBook.addGrade(g);
    }

    double getAverageScore() const {
        return recordBook.calculateAverage();
    }

    void print() const override {
        Person::print();
        cout << " | Зачетка: " << recordBook.getNumber()
             << " | Средний балл: " << fixed << setprecision(2) 
             << getAverageScore();
    }
};

class Group {
private:
    string groupName;
    vector<shared_ptr<Student>> students;

public:
    explicit Group(string name) : groupName(name) {}

    void addStudent(shared_ptr<Student> student) {
        if (student) {
            students.push_back(student);
        }
    }

    void removeStudent(const string& studentName) {
        students.erase(
            remove_if(students.begin(), students.end(),
                [&studentName](const shared_ptr<Student>& s) {
                    return s->getName() == studentName;
                }),
            students.end()
        );
    }

    double calculateGroupAverage() const {
        if (students.empty()) return 0.0;
        double totalSum = 0;
        for (const auto& s : students) {
            totalSum += s->getAverageScore();
        }
        return totalSum / students.size();
    }

    void printGroupInfo() const {
        cout << "\nГруппа: " << groupName << endl;
        cout << "-----------------------------------" << endl;
        for (const auto& s : students) {
            s->print();
            cout << endl;
        }
        cout << "-----------------------------------" << endl;
        cout << "Общий средний балл группы: " << calculateGroupAverage() << endl;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    auto s1 = make_shared<Student>("Алексей", "23-ЭВМ-101");
    auto s2 = make_shared<Student>("Мария", "23-ЭВМ-105");
    auto s3 = make_shared<Student>("Игорь", "23-ЭВМ-110");

    s1->addGrade(5); s1->addGrade(4);
    s2->addGrade(3); s2->addGrade(3);
    s3->addGrade(5); s3->addGrade(5);

    Group myGroup("ИВТ-23");
    
    myGroup.addStudent(s1);
    myGroup.addStudent(s2);
    myGroup.addStudent(s3);

    myGroup.printGroupInfo();

    cout << "\nУдаление студента Мария..." << endl;
    myGroup.removeStudent("Мария");

    myGroup.printGroupInfo();

    return 0;
}