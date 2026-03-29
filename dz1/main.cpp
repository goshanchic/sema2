#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <boost/thread.hpp>
#include <mutex>
#include <condition_variable>
#include <clocale>
#include <future>
#include <atomic>
#include <fstream>

using namespace std;

const int ROWS = 1000;
const int COLS = 1000;
const int BLOCK_SIZE = 250; 

atomic<int> completed_blocks(0);

void fill_matrix(vector<vector<int>>& matrix) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 10);
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            matrix[i][j] = dis(gen);
        }
    }
}

double process_block(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, 
                     int r_start, int r_end, int c_start, int c_end) {
    double block_sum = 0;
    for (int i = r_start; i < r_end; ++i) {
        for (int j = c_start; j < c_end; ++j) {
            C[i][j] = A[i][j] + B[i][j];
            block_sum += C[i][j];
        }
    }
    completed_blocks++;
    return block_sum;
}

int main() {
    setlocale(LC_ALL, "Russian");

    vector<vector<int>> A(ROWS, vector<int>(COLS));
    vector<vector<int>> B(ROWS, vector<int>(COLS));
    vector<vector<int>> C(ROWS, vector<int>(COLS));

    fill_matrix(A);
    fill_matrix(B);

    cout << "Запуск обработки блоков " << BLOCK_SIZE << "x" << BLOCK_SIZE << "..." << endl;

    auto start_time = chrono::high_resolution_clock::now();

    vector<future<double>> futures;

    for (int i = 0; i < ROWS; i += BLOCK_SIZE) {
        for (int j = 0; j < COLS; j += BLOCK_SIZE) {
            int r_end = min(i + BLOCK_SIZE, ROWS);
            int c_end = min(j + BLOCK_SIZE, COLS);
            
            futures.push_back(async(launch::async, process_block, ref(A), ref(B), ref(C), i, r_end, j, c_end));
        }
    }

    double total_sum = 0;
    for (auto& fut : futures) {
        total_sum += fut.get();
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end_time - start_time;

    cout << "-----------------------------------" << endl;
    cout << "Итоговая сумма элементов матрицы C: " << total_sum << endl;
    cout << "Обработано блоков: " << completed_blocks.load() << endl;
    cout << "Общее время выполнения: " << duration.count() << " мс" << endl;

    ofstream out("result_matrix.txt");
    if (out.is_open()) {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                out << C[i][j] << " ";
            }
            out << "\n";
        }
        out.close();
        cout << "Результат сложения записан в result_matrix.txt" << endl;
    }
    cout << "-----------------------------------" << endl;

    return 0;
}
