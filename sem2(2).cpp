#include <iostream>
#include <vector>
#include <numeric>   
#include <algorithm> 
#include <iomanip>   
#include <limits>

using namespace std;


//  ср балл каждого студента
double getStudentAverage(const vector<double>& grades) {
    if (grades.empty()) return 0.0;
    double sum = accumulate(grades.begin(), grades.end(), 0.0);
    return sum / grades.size();
}

//  ср балла по конкретному предмету 
double getSubjectAverage(const vector<vector<double>>& matrix, int colIdx) {
    if (matrix.empty()) return 0.0;
    double sum = 0;
    for (const auto& studentRow : matrix) {
        sum += studentRow[colIdx];
    }
    return sum / matrix.size();
}

// Поиск индекса студента с макс ср баллом
int findBestStudentIdx(const vector<vector<double>>& matrix) {
    if (matrix.empty()) return -1;
    
    int bestIdx = 0;
    double maxAvg = -1.0;
    
    for (int i = 0; i < matrix.size(); ++i) {
        double currentAvg = getStudentAverage(matrix[i]);
        if (currentAvg > maxAvg) {
            maxAvg = currentAvg;
            bestIdx = i;
        }
    }
    return bestIdx;
}

double inputValidGrade(int s, int p) {
    double val;
    while (true) {
        cout << "  Студент " << s + 1 << ", Предмет " << p + 1 << " (0-5): ";
        if (cin >> val && val >= 0 && val <= 5) {
            return val;
        }
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

    if (N <= 0 || M <= 0) {
        cout << "Ошибка: Размеры должны быть больше нуля!" << endl;
        return 1;
    }

    vector<vector<double>> grades(N, vector<double>(M));


    cout << "\n Заполнение базы оценок" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            grades[i][j] = inputValidGrade(i, j);
        }
    }


    cout << fixed << setprecision(2);
    cout << "СТАТИСТИКА" << endl;


    cout << "\n[Средний балл студентов]:" << endl;
    for (int i = 0; i < N; ++i) {
        cout << " Студент #" << i + 1 << ": " << getStudentAverage(grades[i]) << endl;
    }

    cout << "\n[Средний балл по предметам]:" << endl;
    for (int j = 0; j < M; ++j) {
        cout << " Предмет #" << j + 1 << ": " << getSubjectAverage(grades, j) << endl;
    }

    int bestIdx = findBestStudentIdx(grades);
    cout << "\n>>> Лучший результат: Студент #" << bestIdx + 1 
         << " (балл: " << getStudentAverage(grades[bestIdx]) << ")" << endl;
    return 0;
}