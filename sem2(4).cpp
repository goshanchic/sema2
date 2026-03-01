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

int main() {
    setlocale(LC_ALL, "Russian");

    int N, M;
    cout << "Введите количество студентов: "; cin >> N;
    cout << "Введите количество предметов: "; cin >> M;

    if (N <= 0 || M <= 0) return 1;

    vector<vector<double>> grades(N, vector<double>(M));
    vector<pair<int, double>> studentStats;

    for (int i = 0; i < N; ++i) {
        cout << "Студент #" << i + 1 << ":" << endl;
        for (int j = 0; j < M; ++j) {
            double val;
            while (!(cin >> val) || val < 0 || val > 5) {
                cout << "Ошибка (0-5): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            grades[i][j] = val;
        }
        studentStats.push_back({ i + 1, getStudentAverage(grades[i]) });
    }


    double minThreshold;
    cout << "\nВведите минимально допустимый средний балл для рейтинга: ";
    cin >> minThreshold;

    studentStats.erase(
        remove_if(studentStats.begin(), studentStats.end(), 
            [minThreshold](const pair<int, double>& s) {
                return s.second < minThreshold; 
            }), 
        studentStats.end()
    );

    
    sort(studentStats.begin(), studentStats.end(), 
        [](const pair<int, double>& a, const pair<int, double>& b) {
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        }
    );

    
    int excellenceCount = count_if(studentStats.begin(), studentStats.end(), 
        [](const pair<int, double>& s) { return s.second >= 4.5; });

    int atRiskCount = count_if(studentStats.begin(), studentStats.end(), 
        [](const pair<int, double>& s) { return s.second < 3.0; });


    cout << fixed << setprecision(2);
    cout << "---ОБНОВЛЕННЫЙ РЕЙТИНГ (после фильтрации)---" << endl;

    if (studentStats.empty()) {
        cout << "Список пуст. Никто не прошел порог." << endl;
    } else {
        for (const auto& stat : studentStats) {
            cout << "Студент #" << stat.first << " | Балл: " << stat.second << endl;
        }
    }

    cout << "\n--- Статистика группы ---" << endl;
    cout << "Отличники (>= 4.5): " << excellenceCount << endl;
    cout << "Близки к отчислению (< 3.0): " << atRiskCount << endl;

    return 0;
}