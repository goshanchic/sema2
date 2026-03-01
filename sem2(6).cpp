#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <iomanip>

using namespace std;

class Student {
private:

    string name;
    vector<double> grades;

public:
    Student() : name("Unknown") {}
    explicit Student(string studentName) : name(studentName) {}

    void addGrade(double grade) {
        if (grade >= 0 && grade <= 5) {
            grades.push_back(grade);
        } else {
            cout << "Ошибка: Некорректный балл для студента " << name << endl;
        }
    }

    double getAverage() const {
        if (grades.empty()) return 0.0;
        return accumulate(grades.begin(), grades.end(), 0.0) / grades.size();
    }


    void printInfo() const {
        cout << left << setw(15) << name 
             << " | Ср. балл: " << fixed << setprecision(2) << getAverage() 
             << " | Оценок: " << grades.size() << endl;
    }

};

int main() {
    setlocale(LC_ALL, "Russian");
    
    int N, M;
    cout << "Введите количество студентов: ";
    cin >> N;
    cout << "Введите количество предметов: ";
    cin >> M;

    vector<Student> classroom;

    for (int i = 0; i < N; ++i) {
        string sName;
        cout << "\nВведите имя студента #" << i + 1 << ": ";
        cin >> sName;

        // Создаем объект студента
        Student tempStudent(sName);

        for (int j = 0; j < M; ++j) {
            double g;
            cout << "  Оценка за предмет " << j + 1 << ": ";
            cin >> g;
            tempStudent.addGrade(g);
        }

        // Добав объект в общий список
        classroom.push_back(tempStudent);
    }

   
    cout << "\n--- Ведомость группы ---" << endl;
    for (const auto& student : classroom) {
        student.printInfo();
    }

    return 0;
}