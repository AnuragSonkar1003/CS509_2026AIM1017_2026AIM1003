# CS509 Assignment 4 (Buddy)

This folder contains the Assignment 4 driver for:
- K-Means Clustering
- FastMap

The project follows the same CLI/menu style as the earlier assignments and is designed to work with the wrapper program in the root `wrapper/` folder.

## Build on Windows PowerShell

Open PowerShell and move to the Assignment 4 directory:

```powershell
cd "D:\DESKTOP FOLDER\Buddy_assignment\buddy_assignment_4"
```

Compile all source files:

```powershell
g++ -O2 -Wall -Wextra -std=c++17 src/main.cpp src/kmeans.cpp src/fastmap.cpp -o assignment4
```

## Build with Make

```bash
cd buddy_assignment_4
make
```

## Run

```bash
./assignment4
```

Run K-Means directly:

```powershell
.\assignment4 km .\tests\km_demo.txt
```

Run FastMap directly:

```powershell
.\assignment4 fm .\tests\fm_demo.txt
```

## Run all Buddy test cases

Run both available Assignment 4 test cases:

```powershell
.\assignment4 km .\tests\km_demo.txt
.\assignment4 fm .\tests\fm_demo.txt
```

Or run them automatically in one PowerShell command:

```powershell
Get-ChildItem .\tests\km_*.txt | ForEach-Object { .\assignment4 km $_.FullName }
Get-ChildItem .\tests\fm_*.txt | ForEach-Object { .\assignment4 fm $_.FullName }
```

The wrapper menu exposes the same algorithms through the top-level project menu.
