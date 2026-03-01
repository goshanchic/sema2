#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <iomanip>

using namespace std;


#pragma pack(push, 1)
struct FileHeader {
    char signature[4]; 
    int version;      
    int studentCount;  
};
#pragma pack(pop)

//  для вычисления среднего
double getAvg(const vector<double>& v) {
    return v.empty() ? 0.0 : accumulate(v.begin(), v.end(), 0.0) / v.size();
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "Размер структуры заголовка: " << sizeof(FileHeader) << " байт(а)" << endl;

    string filename = "students_data.bin";
    int N, M;

    cout << "Введите N (студенты) и M (предметы): ";
    cin >> N >> M;

    vector<vector<double>> grades(N, vector<double>(M));
    for(int i = 0; i < N; ++i) {
        cout << "Оценки студента " << i + 1 << ": ";
        for(int j = 0; j < M; ++j) cin >> grades[i][j];
    }

    //  Запись в файл 
    {
        ofstream outFile(filename, ios::binary);
        if (!outFile) return 1;

        FileHeader header = {{'G', 'R', 'D', '\0'}, 1, N};
        
        // Записываем заголовок
        outFile.write(reinterpret_cast<char*>(&header), sizeof(FileHeader));
        // Записываем количество предметов
        outFile.write(reinterpret_cast<char*>(&M), sizeof(int));
        // Записываем все оценки построчно
        for (const auto& row : grades) {
            outFile.write(reinterpret_cast<const char*>(row.data()), M * sizeof(double));
        }
        
        outFile.close();
        cout << "\nДанные успешно сохранены в " << filename << endl;
    }

    //  Чтение из файла 
    {
        ifstream inFile(filename, ios::binary);
        if (!inFile) return 1;

        FileHeader readHeader;
        inFile.read(reinterpret_cast<char*>(&readHeader), sizeof(FileHeader));


        if (string(readHeader.signature) != "GRD") {
            cout << "Ошибка: Неверная сигнатура файла!" << endl;
            return 1;
        }

        int readM;
        inFile.read(reinterpret_cast<char*>(&readM), sizeof(int));

        // Восстановление данных
        vector<vector<double>> loadedGrades(readHeader.studentCount, vector<double>(readM));
        for (int i = 0; i < readHeader.studentCount; ++i) {
            inFile.read(reinterpret_cast<char*>(loadedGrades[i].data()), readM * sizeof(double));
        }

        cout << "\n--- Данные успешно загружены ---" << endl;
        cout << "Студентов: " << readHeader.studentCount << ", Предметов: " << readM << endl;
        
        for (int i = 0; i < readHeader.studentCount; ++i) {
            cout << "Студент #" << i + 1 << " средний балл: " 
                 << fixed << setprecision(2) << getAvg(loadedGrades[i]) << endl;
        }
    }

    return 0;
}