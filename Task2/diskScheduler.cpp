#include <iostream> // cout/cin
#include <fstream> // file opening
#include <string> // store inputs as string
#include <vector> // store inputs into dynamic array
#include <random> // generate random disk requests
#include <ctime> // seed random num gen
#include <cmath>
#include <map>
#include <algorithm>

const int maxCylinder = 4999;

void scheduleDiskRequests(const std::vector<int> &diskRequests, int diskMovementStatistics[4][2]);
bool compareIntSigns(int futureDir, int currentDir);

int findMinOffset(const std::map<int, int> &offsets);
int adjustOffsets(int currentOffset, int adjustment);
void printMap(std::map<int, int> mapa);
void deleteFromMapUsingIt(std::map<int, int> &mapa, int key);

void FCFS(const std::vector<int> &diskRequests, int diskMovementStatistics[][2]);
void SSTF(const std::vector<int> &diskRequests, int diskMovementStatistics[][2]);
void SCAN(std::vector<int> diskRequests, int diskMovementStatistics[][2]);
void CSCAN(std::vector<int> diskRequests, int diskMovementStatistics[][2]);

void printDiskStats(int diskMovementStatistics[][2]);

int main(int argc, char* argv[]) {
    
    if (argc < 2 || argc > 2) {
        std::cout << "Incorrect number of parameters, usage: [program name], [initial disk head position (int)]" << std::endl;
        return 1;
    }

    int initialPos;
    if (argc == 2) {
        initialPos = std::stoi(argv[1]);
    }

    std::string filePath;
    std::cout << "Enter the file path for input.txt: ";
    std::cin >> filePath;
    std::cout << std::endl;

    std::cout << "whtt";

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "File could not be opened" << std::endl;
        return 1;
    }

    std::vector<int> inputDiskRequests;
    inputDiskRequests.push_back(initialPos);
    
    int request;
    while (file >> request) {
        inputDiskRequests.push_back(request);
    }
    std::cout << "is it borked?";

    // each row is reserved for a disk scheduling algorithm
    // column 0 is head's travel distance, column 1 is number of head direction changes
    int diskMovementStatistics[4][2] = {0};

    scheduleDiskRequests(inputDiskRequests, diskMovementStatistics);

    printDiskStats(diskMovementStatistics);

/*     // reset the statistics matrix for the radom disk scheduling trial
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            diskMovementStatistics[i][j] = 0;
        }
    }
    
    srand(time(NULL));
    std::vector<int> randomDiskRequests;
    for (int i = 0; i < 1000; i++) {
        randomDiskRequests.push_back(rand() % maxCylinder + 1);
    }

    scheduleDiskRequests(randomDiskRequests, diskMovementStatistics);

    printDiskStats(diskMovementStatistics); */


    return 0;
}


/*
* FCFS
* SSTF
* SCAN
* C-SCAN
*/
void scheduleDiskRequests(const std::vector<int> &diskRequests, int diskMovementStatistics[4][2]) {
    
    FCFS(diskRequests, diskMovementStatistics);
    std::cout << "Test!!";
    SSTF(diskRequests, diskMovementStatistics);
    
    SCAN(diskRequests, diskMovementStatistics);
/*    CSCAN(diskRequests, diskMovementStatistics); */

}

void FCFS(const std::vector<int> &diskRequests, int diskMovementStatistics[][2]) {
    // determine initial head direction
    int headDirection = 0;
    headDirection = diskRequests[1] - diskRequests[0];

    int movement = 0;
    for (int i = 1; i < diskRequests.size(); i++) {
        movement = diskRequests[i] - diskRequests[i - 1];
        if (movement == 0) {
            continue;
        }
        diskMovementStatistics[0][0] += abs(movement);
        if (compareIntSigns(movement, headDirection)) {
            diskMovementStatistics[0][1]++;
            headDirection *= -1;
        }
    }        

}
void SSTF(const std::vector<int> &diskRequests, int diskMovementStatistics[][2]) {
    
    // determine initial offset values from first head positon
    std::map<int, int> requestsOffsetsFromInit;
    for (int i = 1; i < diskRequests.size(); i++) {
        requestsOffsetsFromInit.insert(std::make_pair(diskRequests[i], diskRequests[i] - diskRequests[0]));
        //std::cout << diskRequests[i] << ": " << diskRequests[i] - diskRequests[0] << std::endl; //debug
    }

    // iterate through the offset values, find the sector with minimal abs. val. offset, 
    // add that sector's offset to total head travel distance
    // then adjust all of the remaining offsets by the selected smallest offset
    int currentDiskSector;
    int sectorOffset = 0;
    int numDiskReads = diskRequests.size() - 1;
    int headDirection = 0;
    for (int i = 0; i < numDiskReads; i++) {
        printMap(requestsOffsetsFromInit);
        currentDiskSector = findMinOffset(requestsOffsetsFromInit);
        //std::cout << "current selected sector: " <<  currentDiskSector; //debug
        sectorOffset = requestsOffsetsFromInit[currentDiskSector];
        //std::cout << " current offset: " << sectorOffset << std::endl; 
        diskMovementStatistics[1][0] += abs(sectorOffset);
        //std::cout << "total movement: " << diskMovementStatistics[1][0] << std::endl;
        if (headDirection == 0) {
            headDirection = sectorOffset;
        }
        // check if head direction changes
        if (compareIntSigns(sectorOffset, headDirection)) {
            diskMovementStatistics[1][1]++;
            headDirection *= -1;
        }
        deleteFromMapUsingIt(requestsOffsetsFromInit, currentDiskSector);
        for (auto it = requestsOffsetsFromInit.begin(); it != requestsOffsetsFromInit.end(); ++it) {
            it->second = adjustOffsets(it->second, sectorOffset);         
        }
        //std::cout << "map post erase and adjustment: "; printMap(requestsOffsetsFromInit); std::cout << std::endl;// DEBUG
    }
    std::cout << "SSTF exit";
}

// TOOD implement this with a binary search tree or heap that would sort the nodes so you could find things faster

void SCAN(std::vector<int> diskRequests, int diskMovementStatistics[][2]) {
    std::cout << "test";
    int headDirection = diskRequests[1] - diskRequests[0];
    diskMovementStatistics[2][0] = abs(headDirection);
    std::map<int, bool>visited;
    visited[diskRequests[0]] = true;
    visited[diskRequests[1]] = true;
    int current = diskRequests[1];
    std::vector<int>visitList;

    while (visited.size() < diskRequests.size()) {
        std::cout << "test";
        if (headDirection > 0) {
            for (int i = 0; i < diskRequests.size(); i++) {
                if (!visited[diskRequests[i]] && diskRequests[i] > current) {
                    visitList.push_back(diskRequests[i]);
                }
            }
            std::sort(visitList.begin(), visitList.end());
            for (int i = 0; i < visitList.size(); i++) {
                diskMovementStatistics[2][0] += visitList[i] - current;
                visited[visitList[i]] = true;
                current = visitList[i];
            }
            if (visited.size() < diskRequests.size()) {
            diskMovementStatistics[2][0] += maxCylinder - current;
            current = maxCylinder;
            visitList.clear();
            headDirection *= -1;
            diskMovementStatistics[2][1]++;
            }
        }
        else {
            for (int i = diskRequests.size() ; i >= 0; i--) {
                if (!visited[diskRequests[i]] && diskRequests[i] < current) {
                    visitList.push_back(diskRequests[i]);
                }
            }
            std::sort(visitList.end(), visitList.begin(), std::greater<int>());
            for (int i = 0; i < visitList.size(); i++) {
                diskMovementStatistics[2][0] += current - visitList[i];
                visited[visitList[i]] = true;
                current = visitList[i];
            }
            if (visited.size() < diskRequests.size()) {
                diskMovementStatistics[2][0] += current;
                current = 0;
                visitList.clear();
                headDirection *= -1;
                diskMovementStatistics[2][1]++;
            } 
        }
    }
}
void CSCAN(int initialPos, std::vector<int> diskRequests, int diskMovementStatistics[][2]) {}

bool compareIntSigns(int a, int b) {
    return (a * b < 0);
}

void printDiskStats(int diskMovementStatistics[][2]) {
    for (int i = 0; i < 4; i++) {
        std::string alg;
        switch (i){
            case 0:
                alg = "FCFS";
                break;
            case 1:
                alg = "SSTF";
                break;
            case 2:
                alg = "SCAN";
                break;
            default:
                alg = "NOT DEFINED";
        }
        std::cout << alg << " total distance traveled: " << diskMovementStatistics[i][0] << ", total head direction changes: " << diskMovementStatistics[i][1] << std::endl;
    }
}

int findMinOffset(const std::map<int, int> &offsets) {
    auto it = std::begin(offsets);
    it = std::min_element(std::begin(offsets), std::end(offsets), 
    [](const auto& l, const auto& r) { return abs(l.second) < abs(r.second); });
    return it->first;
}

int adjustOffsets(int currentOffset, int adjustment) {
    if (currentOffset > 0) {
        if (adjustment > 0) {
            if (currentOffset > adjustment)
                return currentOffset - adjustment;
            else
                return currentOffset + adjustment;
        }
        else 
            return currentOffset - adjustment;
    }
    else {
        if (adjustment > 0) 
            return currentOffset - adjustment;
        else {
            if (adjustment > currentOffset)
                return currentOffset - adjustment;
            else
                return currentOffset - adjustment;
            
        }
    }
}

void printMap(std::map<int, int> mapa) {
    for (const auto& [key, value] : mapa)
        std::cout << '[' << key << "] = " << value <<  "; ";
}

void deleteFromMapUsingIt(std::map<int, int> &mapa, int key) {
    auto targetIndex = mapa.find(key);
    mapa.erase(targetIndex);
}