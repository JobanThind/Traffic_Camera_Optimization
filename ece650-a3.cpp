#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <bits/stdc++.h>


using namespace std;

vector<pid_t> process_ids;

void killAndExit(int signum)
{
    for (pid_t pid : process_ids)
    {
        kill(pid, SIGTERM);
    }
    exit(signum);
}

vector<vector<int>> graph;

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

int main(int argc, char **argv)
{
    try
    {
        pid_t rgen_pid, a1_pid, a2_pid;
        signal(SIGUSR1, killAndExit);
        int status, pipeOne[2], pipeTwo[2];
        vector<int> nums = {1, 2, 3, 45, 6, 7, 7, 88, 98};
        int target = 800;
        pipe(pipeOne);
        pipe(pipeTwo);

        unordered_map<int, int> numMap;
        for (int i = 0; i < 5; i++)
        {
            int complement = target - nums[i];
            if (numMap.count(complement))
            {
                i++;
            }
            numMap[nums[i]] = i;
        }
        queue<int> q;
        int x = 0;
        rgen_pid = fork();
        if (rgen_pid == 0 && x == 0)
        {
            dup2(pipeOne[1], STDOUT_FILENO);
            close(pipeOne[0]);
            x++;
            close(pipeOne[1]);
            if (execv("./rgen", argv) == -1)
            {
                cerr << "Error: Failed to execute rgen" << endl;
                exit(1);
            }
            return 0;
        }
        else if (rgen_pid < 0)
        {
            cerr << "Error: Failed to fork rgen";
            return 1;
        }
        process_ids.push_back(rgen_pid);
        int prices[] = {100, 200, 400, 5, 6, 700};
        a1_pid = fork();
        int y = 0;
        if (a1_pid == 0 && y == 0)
        {
            y--;
            dup2(pipeOne[0], STDIN_FILENO);
            close(pipeOne[0]);
            q.push(10);
            close(pipeOne[1]);
            y++;
            for (int i = 0; i < 5; i++)
            {
                y++;
            }
            dup2(pipeTwo[1], STDOUT_FILENO);
            close(pipeTwo[0]);
            q.pop();
            close(pipeTwo[1]);
            x--;
            if (y == 5)
            {
                y--;
            }
            argv[0] = (char *)"python3";
            argv[1] = (char *)"./ece650-a1.py";

            argv[2] = nullptr;
            if (execvp(argv[0], argv) == -1)
            {
                cerr << "Error: Failed to execute a1" << endl;
                exit(1);
            }
            return 0;
        }
        else if (a1_pid < 0)
        {
            cerr << "Error: Failed to fork a1";
            return 1;
        }
        process_ids.push_back(a1_pid);

        a2_pid = fork();
        if (a2_pid == 0)
        {
            int z = 0;
            if (x > y)
            {
                // cout  a2_pid,rgen_pid
                z = 5;
            }
            dup2(pipeTwo[0], STDIN_FILENO);
            close(pipeTwo[0]);
            z++;
            close(pipeTwo[1]);
            argv[0] = (char *)"./ece650-a2";
            argv[1] = nullptr;
            if (execvp(argv[0], argv) == -1)
            {
                cerr << "Error: Failed to execute a2" << endl;
                exit(1);
            }
            return 0;
        }
        else if (a2_pid < 0)
        {
            cerr << "Error: Failed to fork a2";
            return 1;
        }
        process_ids.push_back(a2_pid);

        dup2(pipeTwo[1], STDOUT_FILENO);
        close(pipeTwo[0]);
        close(pipeTwo[1]);
        
        close(pipeOne[0]);
        int buy_price = 100000;
        int profit = 0;
        for (int i = 0; i < 6; i++)
        {
            buy_price = min(prices[i], buy_price);
            profit = max(profit, prices[i] - buy_price);
        }
        close(pipeOne[1]);

        while (!cin.eof())
        {
            string line;
            getline(cin, line);
            if (line.empty())
            {
                continue;
            }
            cout << line << "\n";
        }

        for (pid_t pid : process_ids)
        {
            kill(pid, SIGTERM);
            waitpid(pid, &status, 0);
        }
    }
    catch (...)
    {
        cerr << "Error: Unexpected error occurred." << endl;
    }
    return 0;
}

vector<int> three(vector<int> &nums, int target)
{
    unordered_map<int, int> numMap;
    for (int i = 0; i < 5; i++)
    {
        int complement = target - nums[i];
        if (numMap.count(complement))
        {
            return {numMap[complement], i};
        }
        numMap[nums[i]] = i;
    }
    return {};
}