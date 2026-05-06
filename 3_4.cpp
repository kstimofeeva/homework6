#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <variant>
#include <stdexcept>

struct AdjacencyMatrix { 
    std::vector<std::vector<int>> mat; 
    int n; 
};
struct IncidenceMatrix { 
    std::vector<std::vector<int>> mat; 
    int n, m; 
};
struct AdjacencyList { 
    std::vector<std::vector<int>> list; 
    int n; 
};
struct ListOfArcs { 
    std::vector<std::pair<int, int>> arcs;
    int n, m; 
};
struct SortedListOfArcs { 
    std::vector<std::pair<int, int>> arcs; 
    std::vector<int> S; 
    int n, m;
};

using GraphVariant = std::variant<AdjacencyMatrix, IncidenceMatrix, AdjacencyList, ListOfArcs, SortedListOfArcs>;

std::vector<std::pair<int, int>> get_outgoing_arcs(const GraphVariant& graph, const std::string& rep_name, int v) {
    std::vector<std::pair<int, int>> result;

    if (rep_name == "AdjacencyMatrix") {
        const auto& g = std::get<AdjacencyMatrix>(graph);
        for (int j = 0; j < g.n; j++) {
            if (g.mat[v][j] == 1) 
                result.push_back({ v, j });
        }
    }
    else if (rep_name == "IncidenceMatrix") {
        const auto& g = std::get<IncidenceMatrix>(graph);
        for (int e = 0; e < g.m; e++) {
            if (g.mat[v][e] == 1) { 
                for (int j = 0; j < g.n; j++) {
                    if (g.mat[j][e] == -1) { 
                        result.push_back({ v, j });
                        break;
                    }
                }
            }
        }
    }
    else if (rep_name == "AdjacencyList") {
        const auto& g = std::get<AdjacencyList>(graph);
        for (int j : g.list[v]) {
            result.push_back({ v, j });
        }
    }
    else if (rep_name == "ListOfArcs") {
        const auto& g = std::get<ListOfArcs>(graph);
        for (const auto& arc : g.arcs) {
            if (arc.first == v)
                result.push_back({ v, arc.second });
        }
    }
    else if (rep_name == "SortedListOfArcs") {
        const auto& g = std::get<SortedListOfArcs>(graph);
        for (const auto& arc : g.arcs) {
            if (arc.first == v)
                result.push_back({ v, arc.second });
        }
    }
    else {
        throw std::invalid_argument("Неизвестное представление графа.");
    }

    return result;
}


ListOfArcs to_arc_list(const GraphVariant& graph, const std::string& rep_name) {
    ListOfArcs res;
    if (rep_name == "AdjacencyMatrix") {
        const auto& g = std::get<AdjacencyMatrix>(graph);
        res.n = g.n; 
        res.m = 0;
        for (int i = 0; i < g.n; i++) {
            for (int j = 0; j < g.n; j++) {
                if (g.mat[i][j] == 1) { 
                    res.arcs.push_back({ i, j });
                    res.m++; 
                }
            }
        }
    }
    else if (rep_name == "IncidenceMatrix") {
        const auto& g = std::get<IncidenceMatrix>(graph);
        res.n = g.n; 
        res.m = g.m;
        for (int e = 0; e < g.m; e++) {
            int u = -1, v = -1;
            for (int i = 0; i < g.n; i++) {
                if (g.mat[i][e] == 1) 
                    u = i;
                if (g.mat[i][e] == -1) 
                    v = i;
            }
            if (u != -1 && v != -1)
                res.arcs.push_back({ u, v });
        }
    }
    else if (rep_name == "AdjacencyList") {
        const auto& g = std::get<AdjacencyList>(graph);
        res.n = g.n;
        res.m = 0;
        for (int i = 0; i < g.n; i++) {
            for (int j : g.list[i]) { 
                res.arcs.push_back({ i, j });
                res.m++;
            }
        }
    }
    else if (rep_name == "ListOfArcs") {
        return std::get<ListOfArcs>(graph);
    }
    else if (rep_name == "SortedListOfArcs") {
        const auto& g = std::get<SortedListOfArcs>(graph);
        res.arcs = g.arcs;
        res.n = g.n; 
        res.m = g.m;
    }
    return res;
}

GraphVariant convert_graph(const GraphVariant& graph, const std::string& from_rep, const std::string& to_rep) {
    if (from_rep == to_rep) 
        return graph;

    ListOfArcs inter = to_arc_list(graph, from_rep);
    int n = inter.n;
    int m = inter.m;

    if (to_rep == "AdjacencyMatrix") {
        AdjacencyMatrix out{ std::vector<std::vector<int>>(n, std::vector<int>(n, 0)), n };
        for (const auto& arc : inter.arcs)
            out.mat[arc.first][arc.second] = 1;
        return out;
    }
    else if (to_rep == "IncidenceMatrix") {
        IncidenceMatrix out{ std::vector<std::vector<int>>(n, std::vector<int>(m, 0)), n, m };
        for (int e = 0; e < m; e++) {
            out.mat[inter.arcs[e].first][e] = 1;
            out.mat[inter.arcs[e].second][e] = -1;
        }
        return out;
    }
    else if (to_rep == "AdjacencyList") {
        AdjacencyList out{ std::vector<std::vector<int>>(n), n };
        for (const auto& arc : inter.arcs)
            out.list[arc.first].push_back(arc.second);
        return out;
    }
    else if (to_rep == "ListOfArcs") {
        return inter;
    }

    throw std::invalid_argument("Неизвестное целевое представление.");
}

int main() {
    int n = 5;
    int m = 8;

    std::vector<std::pair<int, int>> arcs = { {0,1}, {0,4}, {0,2}, {1,4}, {1,2}, {1,3}, {4,3}, {3,2} };
    std::vector<std::pair<int, int>> sorted_arcs = arcs;
    std::sort(sorted_arcs.begin(), sorted_arcs.end());
    std::vector<int> S = { 0, 3, 6, 6, 7 }; 

    ListOfArcs arc_list_rep{ arcs, n, m };
    GraphVariant graph = arc_list_rep; 

    GraphVariant adj_list_variant = convert_graph(graph, "ListOfArcs", "AdjacencyList");

    const auto& adj_list_data = std::get<AdjacencyList>(adj_list_variant);

    for (int i = 0; i < n; i++) {
        std::cout << i << ": ";
        for (int j : adj_list_data.list[i]) std::cout << j << " ";
        std::cout << '\n';
    }

    int vertex_to_check = 1;

    auto out_arcs = get_outgoing_arcs(adj_list_variant, "AdjacencyList", vertex_to_check);

    for (const auto& edge : out_arcs) {
        std::cout << "(" << edge.first << ", " << edge.second << ")\n";
    }

    return 0;
}
