#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <utility> 

using namespace std;


double getStudentAverage(const vector<double>& grades) {
    if (grades.empty()) return 0.0;
    return accumulate(grades.begin(), grades.end(), 0.0) / grades.size();
}

double inputValidGrade(int s, int p) {
    double val;
    while (true) {
        cout << "  Студент " << s + 1 << ", Предмет " << p + 1 << " (0-5): ";
        if (cin >> val && val >= 0 && val <= 5) return val;
        cout << "  Ошибка! Введите число от 0 до 5." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    int N, M;
    cout << "Введите количество студентов (N): ";
    cin >> N;
    cout << "Введите количество предметов (M): ";
    cin >> M;

    if (N <= 0 || M <= 0) return 1;

    vector<vector<double>> grades(N, vector<double>(M));

    for (int i = 0; i < N; ++i) {
        cout << "\nВведите оценки для студента #" << i + 1 << ":" << endl;
        for (int j = 0; j < M; ++j) {
            grades[i][j] = inputValidGrade(i, j);
        }
    }

    //  контейнер пар {индекс, средний балл} 
    vector<pair<int, double>> studentStats;
    for (int i = 0; i < N; ++i) {
        studentStats.push_back({ i + 1, getStudentAverage(grades[i]) });
    }
    
    string sortMsg = "Выполняется сортировка...";
    
    sort(studentStats.begin(), studentStats.end(), 
        [&sortMsg](const pair<int, double>& a, const pair<int, double>& b) {
            if (a.second != b.second) {
                return a.second > b.second; 
            }
            return a.first < b.first;
        }
    );

    cout << fixed << setprecision(2);
    cout << "\n=====================================" << endl;
    cout << "РЕЙТИНГ СТУДЕНТОВ (ПО УБЫВАНИЮ БАЛЛА)" << endl;
    cout << "=====================================" << endl;

    for (const auto& stat : studentStats) {
        cout << "Место в рейтинге | Студент #" << stat.first 
             << " | Средний балл: " << stat.second << endl;
    }
    double threshold = 4.0;
    int count = 0;

    for_each(studentStats.begin(), studentStats.end(), [threshold, &count](const pair<int, double>& s) {
        if (s.second >= threshold) {
            count++;
        }
    });

    cout << "\nСправочно (захват переменных):" << endl;
    cout << "Студентов с баллом >= " << threshold << ": " << count << endl;

    return 0;
}