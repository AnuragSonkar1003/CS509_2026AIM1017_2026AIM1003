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
            command = "../buddy_assignment_2/driver betweenness_centrality ...";
        }
        else if (algorithm == 2)
        {
            command = "../buddy_assignment_2/driver connected_components ...";
        }
        else if (algorithm == 3)
        {
            command = "../buddy_assignment_2/driver triangle_counting ...";
        }
        else if (algorithm == 4)
        {
            command = "../buddy_assignment_2/driver undirected_csr ...";
        }
        else
        {
            cout << "Invalid algorithm choice!\n";
            return;
        }   
    cout << "\nAssignment 2 selected.\n";
    cout << "Assignment 2 algorithms will be added here.\n";
}


// =====================================================
// MAIN
// =====================================================

int main()
{
    int assignmentChoice;
    int algorithmChoice;


    // =================================================
    // MAIN MENU
    // =================================================

    cout << "\n";
    cout << "====================================\n";
    cout << "            MAIN MENU\n";
    cout << "====================================\n";
    cout << "1. Assignment 1\n";
    cout << "2. Assignment 2\n";
    cout << "====================================\n";
    cout << "Enter your choice: ";

    cin >> assignmentChoice;


    // =================================================
    // CHECK ASSIGNMENT CHOICE
    // =================================================

    if (assignmentChoice != 1 && assignmentChoice != 2)
    {
        cout << "\nInvalid choice!\n";
        return 0;
    }


    // =================================================
    // ASSIGNMENT 1 MENU
    // =================================================

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


    // =================================================
    // ASSIGNMENT 2 MENU
    // =================================================

    else if (assignmentChoice == 2)
    {
        cout << "\n";
        cout << "====================================\n";
        cout << "          ASSIGNMENT 2\n";
        cout << "====================================\n";

        cout << "1. betweenness centrality\n";
        cout << "2. connected components\n";
        cout << "3. triangle counting\n";
        cout << "4. undirected csr\n";

        cout << "====================================\n";
        cout << "Enter your choice: ";

        cin >> algorithmChoice;


        if (algorithmChoice < 1 || algorithmChoice > 4)
        {
            cout << "\nInvalid choice!\n";
            return 0;
        }

        runAssignment2(algorithmChoice);
    }


    return 0;
}


