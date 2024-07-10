// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>

using namespace std;

vector<vector<int>> graph;
void printShortestPath(vector<int> parent, int dst)
{
    int vertex = dst;
    vector<int> path;
    while (vertex != -1)
    {
        path.push_back(vertex);
        vertex = parent[vertex];
    }
    for (auto it = path.rbegin(); it != path.rend(); ++it)
    {
        if (it != path.rbegin())
            cout << "-";
        cout << *it;
    }
    cout << endl;
}

bool isValid(int src, int dst, int numOfVertex)
{
    return src >= 1 && src <= numOfVertex && dst >= 1 && dst <= numOfVertex;
}

void BFS(int src, int dst, int numOfVertex)
{
    if (graph.empty())
    {
        cerr << "Error: Empty graph." << endl;
        return;
    }

    queue<int> bfsQueue;
    vector<bool> visited(numOfVertex + 1, false);
    vector<int> parent(numOfVertex + 1, -1);
    bfsQueue.push(src);
    visited[src] = true;

    if (isValid(src, dst, numOfVertex))
    {
        if (src == dst)
        {
            cout << src << endl;
            return;
        }
        while (!bfsQueue.empty())
        {
            int currNode = bfsQueue.front();
            bfsQueue.pop();
            if (currNode == dst)
            {
                break;
            }

            vector<int> neighbours = graph[currNode];
            for (int neighbour : neighbours)
            {
                if (!visited[neighbour])
                {
                    visited[neighbour] = true;
                    bfsQueue.push(neighbour);
                    parent[neighbour] = currNode;
                }
            }
        }

        if (parent[dst] == -1)
        {
            cerr << "Error: No path exists." << endl;
        }
        else
        {
            printShortestPath(parent, dst);
        }
    }
    else
    {
        cerr << "Error: Invalid Input." << endl;
    }
}

void buildGraph(istringstream &input, int v)
{
    if (graph.empty() && v>0)
    {
        for (int i = 0; i <= v; i++)
        {
            graph.emplace_back();
        }
        char bracket;
        input >> bracket;
        if (bracket == '{')
        {
            char open;
            while (input >> open && open == '<')
            {
                int src, dst;
                char comma, close;
                input >> src >> comma >> dst >> close;
                if (isValid(src, dst, v))
                {
                    graph[src].push_back(dst);
                    graph[dst].push_back(src);
                }
                else
                {
                    graph.clear();
                    cerr << "Error: Invalid Input" << endl;
                    break;
                }
                input >> comma;
            }
        }
    }
}

void print2DVector()
{
    for (const auto &innerVec : graph)
    {
        if (innerVec.empty())
        {
            continue;
        }
        for (int num : innerVec)
        {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    string action;
    string prevAction;
    int v = -1;
    while (!cin.eof())
    {
        try
        {
            string cmdLine;
            getline(cin, cmdLine);
            istringstream input(cmdLine);
            if (cmdLine.empty())
            {
                continue;
            }
            if (input.eof())
            {
                break;
            }
            if (!action.empty())
            {
                prevAction = action;
            }
            input >> action;
            if (action == "V")
            {
                graph.clear();
                cout<<cmdLine<<endl;
                input >> v;
                if (v <= 0)
                {
                    cerr << "Error: Number of vertices must be greater than 0." << endl;
                }
            }

            else if (prevAction == "V" && action == "E")
            {
                cout<< cmdLine<<endl;
                buildGraph(input, v);
                // print2DVector();
            }

            else if (action == "s")
            {
                int src, dst;
                input >> src;
                input >> dst;
                BFS(src, dst, v);
            }
        }
        catch (...)
        {
            cerr << "Error: Unexpected error occurred." << endl;
        }
    }
}