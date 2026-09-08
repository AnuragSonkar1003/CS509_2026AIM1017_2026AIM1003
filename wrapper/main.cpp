#include <iostream>
#include <cstdlib>
#include <chrono>
#include <string>

using namespace std;


// =====================================================
// RUN ASSIGNMENT 1
// =====================================================

void runAssignment1(int algorithm)
{
    string command;

    if (algorithm == 1)
    {
        // SSSP
        command = "../buddy_assignment_1/driver sssp ../buddy_assignment_1/sssp_10.txt";
    }
    else if (algorithm == 2)
    {
        // DFS
        command = "../buddy_assignment_1/driver dfs ../buddy_assignment_1/dfs_10.txt";
    }
    else if (algorithm == 3)
    {
        // BFS
        command = "../buddy_assignment_1/driver bfs ../buddy_assignment_1/bfs_10.txt";
    }
    else
    {
        cout << "Invalid algorithm choice!\n";
        return;
    }

    cout << "\n====================================\n";
    cout << "Running Assignment 1...\n";
    cout << "====================================\n";

    auto start = chrono::high_resolution_clock::now();

    int result = system(command.c_str());

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> time = end - start;

    if (result != 0)
    {
        cout << "\nError: Program execution failed.\n";
    }
    else
    {
        cout << "\nExecution Time: "
             << time.count()
             << " ms\n";
    }
}


// =====================================================
// RUN ASSIGNMENT 2
// =====================================================

void runAssignment2(int algorithm)
{
    string command;

    if (algorithm == 1)
    {
        command = "../buddy_assignment_2/assignment2 tc ../buddy_assignment_2/tests/tc_10.txt";
    }
    else if (algorithm == 2)
    {
        command = "../buddy_assignment_2/assignment2 bc ../buddy_assignment_2/tests/bc_10.txt";
    }
    else if (algorithm == 3)
    {
        command = "../buddy_assignment_2/assignment2 cc ../buddy_assignment_2/tests/cc_10.txt";
    }
    else
    {
        cout << "Invalid algorithm choice!\n";
        return;
    }

    cout << "\n====================================\n";
    cout << "Running Assignment 2...\n";
    cout << "====================================\n";

    auto start = chrono::high_resolution_clock::now();
    int result = system(command.c_str());
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time = end - start;

    if (result != 0)
    {
        cout << "\nError: Program execution failed.\n";
    }
    else
    {
        cout << "\nExecution Time: " << time.count() << " ms\n";
    }
}

// =====================================================
// RUN ASSIGNMENT 3
// =====================================================

void runAssignment3(int algorithm)
{
    string command;

    if (algorithm == 1)
    {
        command = "g++ -O2 -Wall -Wextra -std=c++17 \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\src\\main.cpp\" \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\src\\csr.cpp\" \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\src\\gd_io.cpp\" \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\src\\gd.cpp\" \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\src\\maxflow_io.cpp\" \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\src\\maxflow.cpp\" -o \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\assignment3\" && \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\assignment3\" gd \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\tests\\gd\\gd_01.txt\"";
    }
    else if (algorithm == 2)
    {
        command = "g++ -O2 -Wall -Wextra -std=c++17 \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\src\\main.cpp\" \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\src\\csr.cpp\" \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\src\\gd_io.cpp\" \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\src\\gd.cpp\" \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\src\\maxflow_io.cpp\" \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\src\\maxflow.cpp\" -o \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\assignment3\" && \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\assignment3\" maxflow \"D:\\DESKTOP FOLDER\\Buddy_assignment\\buddy_assignment_3\\tests\\maxflow\\demo_maxflow.txt\"";
    }
    else
    {
        cout << "Invalid algorithm choice!\n";
        return;
    }

    cout << "\n====================================\n";
    cout << "Running Assignment 3...\n";
    cout << "====================================\n";

    auto start = chrono::high_resolution_clock::now();
    int result = system(command.c_str());
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time = end - start;

    if (result != 0)
    {
        cout << "\nError: Program execution failed.\n";
    }
    else
    {
        cout << "\nExecution Time: " << time.count() << " ms\n";
    }
}

// =====================================================
// RUN ASSIGNMENT 4
// =====================================================

void runAssignment4(int algorithm)
{
    string command;

    if (algorithm == 1)
    {
        command = "../buddy_assignment_4/assignment4 km ../buddy_assignment_4/tests/km_demo.txt";
    }
    else if (algorithm == 2)
    {
        command = "../buddy_assignment_4/assignment4 fm ../buddy_assignment_4/tests/fm_demo.txt";
    }
    else
    {
        cout << "Invalid algorithm choice!\n";
        return;
    }

    cout << "\n====================================\n";
    cout << "Running Assignment 4...\n";
    cout << "====================================\n";

    auto start = chrono::high_resolution_clock::now();
    int result = system(command.c_str());
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time = end - start;

    if (result != 0)
    {
        cout << "\nError: Program execution failed.\n";
    }
    else
    {
        cout << "\nExecution Time: " << time.count() << " ms\n";
    }
}

// =====================================================
// MAIN
// =====================================================

int main()
{
    int assignmentChoice;
    int algorithmChoice;

    cout << "\n";
    cout << "====================================\n";
    cout << "            MAIN MENU\n";
    cout << "====================================\n";
    cout << "1. Assignment 1\n";
    cout << "2. Assignment 2\n";
    cout << "3. Assignment 3\n";
    cout << "4. Assignment 4\n";
    cout << "====================================\n";
    cout << "Enter your choice: ";

    cin >> assignmentChoice;

    if (assignmentChoice != 1 && assignmentChoice != 2 && assignmentChoice != 3 && assignmentChoice != 4)
    {
        cout << "\nInvalid choice!\n";
        return 0;
    }

    if (assignmentChoice == 1)
    {
        cout << "\n";
        cout << "====================================\n";
        cout << "          ASSIGNMENT 1\n";
        cout << "====================================\n";
        cout << "1. SSSP\n";
        cout << "2. DFS\n";
        cout << "3. BFS\n";
        cout << "====================================\n";
        cout << "Enter your choice: ";

        cin >> algorithmChoice;

        if (algorithmChoice < 1 || algorithmChoice > 3)
        {
            cout << "\nInvalid choice!\n";
            return 0;
        }

        runAssignment1(algorithmChoice);
    }
    else if (assignmentChoice == 2)
    {
        cout << "\n";
        cout << "====================================\n";
        cout << "          ASSIGNMENT 2\n";
        cout << "====================================\n";

        cout << "1. betweenness centrality\n";
        cout << "2. connected components\n";
        cout << "3. triangle counting\n";
        cout << "====================================\n";
        cout << "Enter your choice: ";

        cin >> algorithmChoice;

        if (algorithmChoice < 1 || algorithmChoice > 3)
        {
            cout << "\nInvalid choice!\n";
            return 0;
        }

        runAssignment2(algorithmChoice);
    }
    else if (assignmentChoice == 3)
    {
        cout << "\n";
        cout << "====================================\n";
        cout << "          ASSIGNMENT 3\n";
        cout << "====================================\n";

        cout << "1. Gradient Descent\n";
        cout << "2. Maxflow\n";
        cout << "====================================\n";
        cout << "Enter your choice: ";

        cin >> algorithmChoice;

        if (algorithmChoice < 1 || algorithmChoice > 2)
        {
            cout << "\nInvalid choice!\n";
            return 0;
        }

        runAssignment3(algorithmChoice);
    }
    else if (assignmentChoice == 4)
    {
        cout << "\n";
        cout << "====================================\n";
        cout << "          ASSIGNMENT 4\n";
        cout << "====================================\n";

        cout << "1. K-Means Clustering\n";
        cout << "2. FastMap\n";
        cout << "====================================\n";
        cout << "Enter your choice: ";

        cin >> algorithmChoice;

        if (algorithmChoice < 1 || algorithmChoice > 2)
        {
            cout << "\nInvalid choice!\n";
            return 0;
        }

        runAssignment4(algorithmChoice);
    }

    return 0;
}


