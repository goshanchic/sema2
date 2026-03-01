#include <iostream>
#include <vector>
#include <string>
#include <memory>    
#include <numeric>
#include <iomanip>

using namespace std;


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

//  Наследование Student от Person 
class Student : public Person {
private:
    vector<double> grades;

public:
    explicit Student(string n) : Person(n) {}

    void addGrade(double g) {
        if (g >= 0 && g <= 5) grades.push_back(g);
    }

    double getAverage() const {
        if (grades.empty()) return 0.0;
        return accumulate(grades.begin(), grades.end(), 0.0) / grades.size();
    }

    
    void print() const override {
        Person::print();
        cout << " | Роль: Студент | Ср. балл: " 
             << fixed << setprecision(2) << getAverage() << endl;
    }
};

// --- 4. Класс Teacher ---
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

    // Добавляем разных личностей
    university.push_back(make_shared<Teacher>("Иван Петрович", "Высшая математика"));
    
    auto s1 = make_shared<Student>("Алексей");
    s1->addGrade(4.5);
    s1->addGrade(5.0);
    university.push_back(s1);

    auto s2 = make_shared<Student>("Мария");
    s2->addGrade(3.8);
    university.push_back(s2);

    university.push_back(make_shared<Teacher>("Елена Сергеевна", "Программирование"));

    cout << "--- Список участников учебного процесса ---" << endl;
    for (const auto& person : university) {
        
    }

    return 0;
}