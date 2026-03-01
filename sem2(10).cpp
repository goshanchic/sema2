#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include <iomanip>
#include <algorithm>

using namespace std;

// --- Опережающие объявления (Forward Declarations) ---
class Student;

// ==================================================
// КЛАСС RecordBook (Композиция в Student)
// ==================================================
class RecordBook {
private:
    string bookNumber;
    vector<int> grades;

public:
    RecordBook() : bookNumber("000000") {}
    explicit RecordBook(string number) : bookNumber(number) {}

    void addGrade(int grade) {
        if (grade >= 2 && grade <= 5) grades.push_back(grade);
    }

    double calculateAverage() const {
        if (grades.empty()) return 0.0;
        return static_cast<double>(accumulate(grades.begin(), grades.end(), 0)) / grades.size();
    }

    // Инлайн-метод (реализован в теле класса)
    inline string getNumber() const { return bookNumber; }
};

// ==================================================
// БАЗОВЫЙ КЛАСС Person
// ==================================================
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

// ==================================================
// КЛАСС Student (Наследование от Person)
// ==================================================
class Student : public Person {
private:
    RecordBook recordBook; // Композиция

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

// ==================================================
// КЛАСС Group (Агрегация студентов)
// ==================================================
class Group {
private:
    string groupName;
    vector<shared_ptr<Student>> students; // Агрегация (не владеет временем жизни)

public:
    explicit Group(string name) : groupName(name) {}

    void addStudent(shared_ptr<Student> student) {
        if (student) students.push_back(student);
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
        cout << "\nГруппа: " << groupName << "\n--------------------------" << endl;
        for (const auto& s : students) {
            s->print();
            cout << endl;
        }
        cout << "--------------------------" << endl;
        cout << "Средний балл группы: " << fixed << setprecision(2) 
             << calculateGroupAverage() << endl;
    }
};

// ==================================================
// ОСНОВНАЯ ПРОГРАММА
// ==================================================
int main() {
    setlocale(LC_ALL, "Russian");

    // Создаем студентов (владение через shared_ptr в main)
    auto s1 = make_shared<Student>("Алексей", "23-ЭВМ-101");
    auto s2 = make_shared<Student>("Мария", "23-ЭВМ-105");

    s1->addGrade(5); s1->addGrade(4);
    s2->addGrade(3); s2->addGrade(4);

    // Создаем группу и добавляем студентов (агрегация)
    Group myGroup("ИВТ-26");
    myGroup.addStudent(s1);
    myGroup.addStudent(s2);

    myGroup.printGroupInfo();

    // Студенты s1 и s2 продолжат существовать здесь, даже если myGroup будет уничтожена
    return 0;
}