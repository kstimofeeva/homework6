#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    int n = 5;
    int m = 8;

    std::vector<std::pair<int, int>> list_of_arcs(m);
    list_of_arcs = { {0,1}, {0,4}, {0,2}, {1,4}, {1,2}, {1,3}, {4,3}, {3,2} };

    std::vector<std::pair<int, int>> sorted_list_of_arcs = list_of_arcs; // упорядоченный список дуг
    sort(sorted_list_of_arcs.begin(), sorted_list_of_arcs.end()); // упоряжоченный список дуг

    std::vector<int> S = { 0, 3, 6, 6, 7 };

    std::vector<std::vector<int>> matrix_representation(n, std::vector<int>(n)); // матрица смежности

    for (int i = 0; i < m; i++) {
        matrix_representation[list_of_arcs[i].first][list_of_arcs[i].second] = 1;
    }

    std::vector<std::vector<int>> incidence_matrix(n, std::vector<int>(m)); // матрица инцидентности


    for (int i = 0; i < m; i++) {
        incidence_matrix[list_of_arcs[i].first][i] = 1;
        incidence_matrix[list_of_arcs[i].second][i] = -1;
    }

    std::vector<std::vector<int>> adjacency_list(n); // список смежности
    for (int i = 0; i < m; i++) {
        adjacency_list[list_of_arcs[i].first].push_back(list_of_arcs[i].second);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << matrix_representation[i][j] << " ";
        }
        std::cout << '\n';
    }

    std::cout << '\n';

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cout << incidence_matrix[i][j] << "\t";
        }
        std::cout << '\n';
    }

    for (int i = 0; i < n; i++) {
        std::cout << '\n'  << i << ": ";
        for (int j : adjacency_list[i])
            std::cout << j << " ";

    }

    std::cout << '\n' << "\n";

    for (auto i : list_of_arcs) {
        std::cout << i.first << " " << i.second << "\n";
    }

    std::cout << '\n';

    for (auto i : sorted_list_of_arcs) {
        std::cout << i.first << " " << i.second << "\n";
    }

    std::cout << '\n';

    for (int i : S) {
        std::cout << i << " ";
    }
}
