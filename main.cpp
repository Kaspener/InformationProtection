#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <sstream>
#include "crypto_lib.hpp"

class RSA{
public:
    static RSA *getInstance();

    RSA(RSA& other) = delete;
    RSA(RSA&& other) = delete;
    void operator=(const RSA &) = delete;
    void operator=(RSA&&) = delete;
    long long N() {return m_N;}
    long long d() {return m_d;}
    void regenerate();

private:
    RSA();
    static RSA* m_rsa;
    long long m_fi;
    long long m_c;
    long long m_N;
    long long m_d;
};

RSA::RSA()
{
    regenerate();
}

RSA *RSA::getInstance()
{
    if(m_rsa==nullptr){
        m_rsa = new RSA();
    }
    return m_rsa;
}

RSA* RSA::m_rsa= nullptr;

void RSA::regenerate()
{
    long long P = genPrime(1000, 50000);
    long long Q = genPrime(1000, 50000);
    m_N = P*Q;
    m_fi = (P-1)*(Q-1);
    m_d = genMutuallyPrime(m_fi);
    m_c = inversion(m_d, m_fi);
}

class Graph{
public:
    Graph(int vertices) noexcept;

    void addEdge(int a, int b) noexcept;
    void print() const noexcept;
    void print2() const noexcept;
    std::vector<int> genPermutation();
    std::vector<int> genUnPermutation(const std::vector<int>& permutations);
    Graph makeIsomorph(const std::vector<int>& verices) const noexcept;
    void randConcatenation();
    Graph rsaEncode() const;
    std::vector<long long> hamCycle();
    void clearRandom();
    int size() const noexcept {return V;}
    long long at(int row, int col) const {return adjacency[row][col];}

    bool operator==(const Graph& other);

private:
    RSA *rsa;
    int V;
    std::vector<std::vector<long long>> adjacency;
    void setAt(int row, int column, long long value);
    bool isSafe(int v, const std::vector<long long>& path, int pos);
    bool hamCycleUtil(std::vector<long long>& path, int pos);
};

Graph::Graph(int verices) noexcept
{
    V = verices;
    adjacency.resize(verices);
    for(auto& it : adjacency) it.resize(verices, 0);
    rsa = RSA::getInstance();
}

void Graph::addEdge(int a, int b) noexcept
{
    adjacency[a][b] = 1;
    adjacency[b][a] = 1;
}

void Graph::print() const noexcept
{
    std::cout << "\033[33m";
    for(int i = 0; i < V; ++i){
        std::cout << "\t" << i+1;
    }
    for(int i = 0; i < V; ++i){
        std::cout << std::endl << "\033[33m" << i+1 << "\033[0m";
        for(int j = 0; j < V; ++j){
            std::cout << "\t" << adjacency[i][j];
        }
    }
    std::cout << std::endl << std::endl;
}

void Graph::print2() const noexcept
{
    for(int i = 0; i < V; ++i){
        for(int j = 0; j < V; ++j){
            std::cout << adjacency[i][j] << ",";
        }
        std::cout << std::endl;
    }
}

std::vector<int> Graph::genPermutation()
{
    std::vector<int> verices(V);
    for(int i = 0; i < V; ++i){
        verices[i] = i;
    }
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(verices), std::end(verices), rng);
    return verices;
}

std::vector<int> Graph::genUnPermutation(const std::vector<int> &permutations)
{
    std::vector<int> unpermutatins(V);
    for(int i = 0; i < V; ++i){
        unpermutatins[permutations[i]] = i;
    }
    return unpermutatins;
}

Graph Graph::makeIsomorph(const std::vector<int>& verices) const noexcept
{
    Graph newGraph(V);
    for(int i = 0; i < V; ++i){
        for(int j = i; j < V; ++j){
            newGraph.setAt(i, j, adjacency[verices[i]][verices[j]]);
            newGraph.setAt(j, i, adjacency[verices[i]][verices[j]]);
        }
    }
    return newGraph;
}

void Graph::randConcatenation()
{
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::uniform_int_distribution<int> distribution(1, V);
    std::ostringstream oss;
    for(auto& row : adjacency){
        for(auto& number : row){
            oss << distribution(rng) << number;
            std::istringstream(oss.str()) >> number;
            oss.str("");
        }
    }
}

Graph Graph::rsaEncode() const
{
    Graph newGraph(V);
    for(int i = 0; i < V; ++i){
        for(int j = 0; j < V; ++j){
            newGraph.setAt(i, j, modulPow(adjacency[i][j], rsa->d(), rsa->N()));
        }
    }
    return newGraph;
}

void Graph::setAt(int row, int column, long long value)
{
    adjacency[row][column] = value;
}

Graph getGraphFromFile(const std::string& filePath){
    std::ifstream in(filePath);
    int n, m;
    in >> n >> m;

    int a,b;
    Graph graph(n);
    while(m--){
        in >> a >> b;
        graph.addEdge(a-1,b-1);
    }
    return graph;
}

bool Graph::isSafe(int v, const std::vector<long long>& path, int pos)
{
    if (adjacency[path[pos - 1]][v] == 0) 
        return false; 

    for (int i = 0; i < pos; i++) 
        if (path[i] == v) 
            return false; 

    return true; 
} 

bool Graph::hamCycleUtil(std::vector<long long>& path, int pos) 
{ 
    if (pos == V) 
    { 
        if (adjacency[path[pos - 1]][path[0]] == 1) 
            return true; 
        else
            return false; 
    } 

    for (int v = 1; v < V; v++) 
    { 
        if (isSafe(v, path, pos)) 
        { 
            path[pos] = v; 

            if (hamCycleUtil(path, pos + 1) == true) 
                return true; 

            path[pos] = -1; 
        } 
    } 

    return false; 
} 

std::vector<long long> Graph::hamCycle() 
{ 
    std::vector<long long> path(V, -1);

    path[0] = 0;

    if (hamCycleUtil(path, 1) == false ) 
    { 
        std::cout << "\nSolution does not exist"; 
        return std::vector<long long>(); 
    } 
    path.push_back(0);
    return path; 
}

void Graph::clearRandom()
{
    for(auto& row : adjacency){
        for(auto& it : row){
            it&=1;
        }
    }
}

bool Graph::operator==(const Graph &other)
{
    for(int i = 0; i < V; ++i){
        for(int j = 0; j < V; ++j){
            if (this->adjacency[i][j] != other.adjacency[i][j])
                return false;
        }
    }
    return true;
}

int main()
{
    RSA *rsa = nullptr;
    Graph G = getGraphFromFile("file1.txt");
    std::cout << "START GRAPH:" << std::endl;
    G.print();
    std::cout << "----------------------------ALICE------------------------------" << std::endl;
    std::vector<int> permutation = G.genPermutation();
    std::cout << "PERMUTATIONS:" << std::endl;
    for(auto& it : permutation) std::cout << it+1 << " ";
    std::cout << std::endl;
    Graph H = G.makeIsomorph(permutation);
    std::cout << "ISOMORPH GRAPH:" << std::endl;
    H.print();
    std::cout << "HIS CYCLE:" << std::endl;
    std::vector<long long> cycle = H.hamCycle();
    for(auto& it : cycle) std::cout << it+1 << " ";
    std::cout << std::endl;
    H.randConcatenation();
    std::cout << "GRAPH AFTER RAND:" << std::endl;
    H.print();
    Graph F = H.rsaEncode();
    std::cout << "ENCODED RSA GRAPH FROM ALICE:" << std::endl;
    F.print();
    std::cout << "-----------------------------BOB-------------------------------" << std::endl;
    std::cout << "1. What is the Hamiltonian cycle for graph H?" << std::endl;
    std::cout << "2. Is the graph H really isomorphic to G?" << std::endl;
    int result;
    std::cin >> result;
    if (result == 1){
        std::vector<std::tuple<int, int, long long>> coords(F.size());
        int row, column;
        long long number;
        int pos = 0;
        std::cout << "H CYCLE:" << std::endl;
        for(int i = 0; i < F.size(); ++i){
            coords[i] = {pos, cycle[i+1], H.at(pos, cycle[i+1])};
            pos = cycle[i+1];
        }
        for(int i = 0; i < F.size(); ++i){
            std::tie(row, column, number) = coords[i];
            std::cout << "{" << row+1 << ", " << column+1 << ", " << number << "}" << std::endl;
        }
        bool flag = true;
        rsa = RSA::getInstance();
        for(int i = 0; i < F.size(); ++i){
            std::tie(row, column, number) = coords[i];
            if (modulPow(number, rsa->d(), rsa->N()) != F.at(row, column)){
                flag = false;
                break;
            }   
        }
        if (flag){
            std::cout << "Edges in the matrix are equal" << std::endl;
            std::vector<bool> checkCycle(F.size(), false);
            for(int i = 0; i < coords.size(); ++i){
                std::tie(row, column, number) = coords[i];
                if (checkCycle[row]){
                    flag = false;
                    break;
                }
                checkCycle[row] = true;
            }
            if (flag){
                std::cout << "Hamiltonian cycle exists" << std::endl;
            }
            else{
                std::cout << "Hamiltonian cycle not exists" << std::endl;
            }
        }
        else{
            std::cout << "Edges in the matrix not equal" << std::endl;
            
        }
    }
    else if (result == 2){
        Graph bobGraph = H.rsaEncode();
        if (bobGraph == F){
            std::cout << "Graph of Bob = F graph" << std::endl;
            H.clearRandom();
            std::cout << "Clear random numbers from graph:" << std::endl;
            H.print();
            std::vector<int> unpermutations = H.genUnPermutation(permutation);
            std::cout << "UNPERMUTATIONS:" << std::endl;
            for(auto& it : unpermutations) std::cout << it+1 << " ";
            std::cout << std::endl;
            std::cout << "NEW G GRAPH:" << std::endl;
            Graph newG = H.makeIsomorph(unpermutations);
            newG.print();
            if (newG == G){
                std::cout << "New G = G" << std::endl;
            }
            else{
                std::cout << "New G != G" << std::endl;
            }
        }
        else {
            std::cout << "Graph of Bob != F graph" << std::endl;
        }
    }
    delete rsa;
    return 0;
}
