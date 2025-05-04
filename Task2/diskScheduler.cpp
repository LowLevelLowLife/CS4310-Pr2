#include <cstddef>
#include <cstdint>
#include <iostream> // cout/cin
#include <fstream> // file opening
#include <string> // store inputs as string
#include <sys/types.h>
#include <vector> // store inputs into dynamic array

#include <ctime> // seed random num gen
#include <cmath>
#include <map>
#include <algorithm>

const int32_t maxCylinder = 4999;

void scheduleDiskRequests(const std::vector<int32_t> &diskRequests, int32_t diskMovementStatistics[4][2]);
bool compareIntSigns(int32_t futureDir, int32_t currentDir);

int32_t findMinOffset(const std::map<int32_t, int32_t> &offsets);
int32_t adjustOffsets(int32_t currentOffset, int32_t adjustment);
void printMap(std::map<int32_t, int32_t> mapa);
void deleteFromMapUsingIt(std::map<int32_t, int32_t> &mapa, int32_t key);

void FCFS(const std::vector<int32_t> &diskRequests, int32_t diskMovementStatistics[][2]);
void SSTF(const std::vector<int32_t> &diskRequests, int32_t diskMovementStatistics[][2]);
void SCAN(const std::vector<int32_t> &diskRequests, int32_t diskMovementStatistics[][2]);
void CSCAN(const std::vector<int32_t> &diskRequests, int32_t diskMovementStatistics[][2]);

void printDiskStats(int32_t diskMovementStatistics[][2]);

int main(int argc, char* argv[]) {   
    if (argc < 2 || argc > 2) {
        std::cout << "Incorrect number of parameters, usage: [program name], [initial disk head position (int32_t)]" << std::endl;
        return 1;
    }

    int32_t initialPos;
    if (argc == 2) {
        initialPos = static_cast<int32_t>(std::stoi(argv[1]));
    }

    std::ifstream file("input.txt");
    if (!file) {
        std::cerr << "File could not be opened" << std::endl;
        return 1;
    }

    std::vector<int32_t> inputDiskRequests;
    inputDiskRequests.push_back(initialPos);
    
    int32_t request;
    while (file >> request) {
        inputDiskRequests.push_back(request);
    }

    // each row is reserved for a disk scheduling algorithm
    // column 0 is head's travel distance, column 1 is number of head direction changes
    int32_t diskMovementStatistics[4][2] = {0};

    scheduleDiskRequests(inputDiskRequests, diskMovementStatistics);

    printDiskStats(diskMovementStatistics);

    // reset the statistics matrix for the radom disk scheduling trial
    for (int32_t i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            diskMovementStatistics[i][j] = 0;
        }
    }
    
    srand(static_cast<uint>(time(NULL)));
    std::vector<int> randomDiskRequests;
    for (int32_t i = 0; i < 1000; i++) {
        randomDiskRequests.push_back(rand() % maxCylinder + 1);
    }

    scheduleDiskRequests(randomDiskRequests, diskMovementStatistics);

    printDiskStats(diskMovementStatistics);


    return 0;
}


/*
* FCFS
* SSTF
* SCAN
* C-SCAN
*/
void scheduleDiskRequests(const std::vector<int32_t> &diskRequests, int32_t diskMovementStatistics[4][2]) {
    
    FCFS(diskRequests, diskMovementStatistics);
    SSTF(diskRequests, diskMovementStatistics);
    SCAN(diskRequests, diskMovementStatistics);
    CSCAN(diskRequests, diskMovementStatistics); 

}

void FCFS(const std::vector<int32_t> &diskRequests, int32_t diskMovementStatistics[][2]) {
    // determine initial head direction
    int32_t headDirection = 0;
    headDirection = diskRequests[1] - diskRequests[0];

    int32_t movement = 0;
    for (size_t i = 1; i < diskRequests.size(); i++) {
        movement = static_cast<int32_t>(diskRequests[i]) - static_cast<int32_t>(diskRequests[i- 1]);
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


void SSTF(const std::vector<int32_t> &diskRequests, int32_t diskMovementStatistics[][2]) {
    
    // determine initial offset values from first head positon
    std::map<int32_t, int32_t> requestsOffsetsFromInit;
    for (size_t i = 1; i < diskRequests.size(); i++) {
        requestsOffsetsFromInit.insert(std::make_pair(diskRequests[i], diskRequests[i] - diskRequests[0]));
        //std::cout << diskRequests[static_cast<int32_t>(i)/] << ": " << diskRequests[static_cast<int32_t>(i)] - diskRequests[0] << std::endl; //debug
    }

    // iterate through the offset values, find the sector with minimal abs. val. offset, 
    // add that sector's offset to total head travel distance
    // then adjust all of the remaining offsets by the selected smallest offset
    int32_t currentDiskSector;
    int32_t sectorOffset = 0;
    int32_t numDiskReads = static_cast<int32_t>(diskRequests.size()) - 1;
    int32_t headDirection = 0;
    for (size_t i = 0; i < static_cast<size_t>(numDiskReads); i++) {
        // printMap(requestsOffsetsFromInit);
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
    //std::cout << "SSTF exit"; DEBUG
}

// TODO implement this with a binary search tree or heap that would sort the nodes so you could find things faster

void SCAN(const std::vector<int32_t> &diskRequests, int32_t diskMovementStatistics[][2]) {

    // determine direction of head
    int32_t headDirection = diskRequests[1] - diskRequests[0];
    diskMovementStatistics[2][0] = abs(headDirection);
    std::map<int32_t, bool>visited;
    uint64_t numVisited = 2;
    uint64_t numRequests = diskRequests.size();
    visited[diskRequests[0]] = true;
    visited[diskRequests[1]] = true;
    int32_t current = diskRequests[1];
    std::vector<int32_t>visitList;

    // go in the direction already going. 
    // "visit" (as in, service/run the HDD head over) cylinders that are greater than or less than current
    // TODO: implement this with a better data structure so you don't have to go over the whole thing each time
    //            and then filter out the ones that are lower than the current. A binary tree has to work here.
    while (numVisited < numRequests) {
        // going up!
        if (headDirection > 0) {
            // enqueue the cylinders to visit
            for (size_t i = 0; i < numRequests; i++) {
                if (!visited[diskRequests[i]] && diskRequests[i] > current) {
                    visitList.push_back(diskRequests[i]);
                }
            }
            // sort the visited cylinders in ascending order
            std::sort(visitList.begin(), visitList.end());
            // record the distance traveled for each visited cylinder
            for (size_t i = 0; i < visitList.size(); i++) {
                diskMovementStatistics[2][0] += visitList[i] - current;
                visited[visitList[i]] = true;
                numVisited++;
                current = visitList[i];
            }
            // if there are fewer visited cylinders than total requests, that means there are more to visit,
            // but their number is lower than the initial cylinder, so we have to go up to maximum
            // and get those other cylinders on the way down after changing direction
            if (numVisited < numRequests) {
            diskMovementStatistics[2][0] += maxCylinder - current;
            current = maxCylinder;
            visitList.clear();
            headDirection *= -1;
            diskMovementStatistics[2][1]++;
            }
        }
        // going downnnnn
        else {
            // enqueue cylinders to visit
            for (size_t i = numRequests - 1; i > 0; i--) {
                if (!visited[diskRequests[i]] && diskRequests[i] < current) {
                    visitList.push_back(diskRequests[i]);
                }
            }
            // sort the enqueued cylinders in descending order
            if (visitList.size() > 1) {
                std::sort(visitList.begin(), visitList.end(), std::greater<size_t>());
            }
            // record distance for every cylinder visited
            for (size_t i = 0; i < visitList.size(); i++) {
                diskMovementStatistics[2][0] += current - visitList[i];
                visited[visitList[i]] = true;
                numVisited++;
                current = visitList[i];
            }
            // check to see if this is the last cylinder or if there's more to go, in which case, we'll have to turn around after hitting bottom cylinder (0)
            if (numVisited < numRequests) {
                diskMovementStatistics[2][0] += current;
                current = 0;
                visitList.clear();
                headDirection *= -1;
                diskMovementStatistics[2][1]++;
            } 
        }
    }
}

void CSCAN(const std::vector<int32_t> &diskRequests, int32_t diskMovementStatistics[][2]) {

    std::map<int32_t, bool>visited;
    uint64_t numVisited = 1;
    uint64_t numRequests = diskRequests.size();
    visited[diskRequests[0]] = true;
    int32_t current = diskRequests[0];
    std::vector<int32_t>visitList;

    // go in the direction already going. 
    // "visit" (as in, service/run the HDD head over) cylinders that are greater than or less than current
    // TODO: implement this with a better data structure so you don't have to go over the whole thing each time
    //            and then filter out the ones that are lower than the current. A binary tree has to work here.
    while (numVisited < numRequests) {
        // going up!
        for (size_t i = 0; i < numRequests; i++) {
            if (!visited[diskRequests[i]] && diskRequests[i] > current) {
                visitList.push_back(diskRequests[i]);
            }
            if (diskRequests[i] == current) {
                continue;
            }
        }
        // sort the enqueued cylinders in ascending order
        std::sort(visitList.begin(), visitList.end());
        // record the distance traveled for each visited cylinder
        for (size_t i = 0; i < visitList.size(); i++) {
            diskMovementStatistics[3][0] += visitList[i] - current;
            visited[visitList[i]] = true;
            numVisited++;
            current = visitList[i];
        }
        // if there are fewer visited cylinders than total requests, that means there are more to visit,
        // but their number is lower than the initial cylinder, so we have to go up to maximum
        // and get those other cylinders on the way down after changing direction
        if (numVisited < numRequests) {
            // head has to go up to max from current and then sweep down to bottom, traversing whole disk
            diskMovementStatistics[3][0] += maxCylinder + maxCylinder - current;
            current = 0;
            visitList.clear();
            diskMovementStatistics[3][1] += 2;
        }
    }
}

bool compareIntSigns(int32_t a, int32_t b) {
    return (a * b < 0);
}

void printDiskStats(int32_t diskMovementStatistics[][2]) {
    for (int32_t i = 0; i < 4; i++) {
        std::string alg;
        switch (i) {
            case 0:
                alg = "FCFS";
                break;
            case 1:
                alg = "SSTF";
                break;
            case 2:
                alg = "SCAN";
                break;
            case 3:
                alg = "CSCAN";
                break;
            default:
                alg = "NOT DEFINED";
        }
        std::cout << alg << " total distance traveled: " << diskMovementStatistics[static_cast<int32_t>(i)][0] << ", total head direction changes: " << diskMovementStatistics[static_cast<int32_t>(i)][1] << std::endl;
    }
}

// helper function to help find the nearest cylinder for SSTF
int32_t findMinOffset(const std::map<int32_t, int32_t> &offsets) {
    auto it = std::begin(offsets);
    it = std::min_element(std::begin(offsets), std::end(offsets), 
    [](const auto& l, const auto& r) { return abs(l.second) < abs(r.second); });
    return it->first;
}

// adjust the offset from current cylinder for all of the cylinders waiting for service
// TODO: eliminate some of the redundant branches in this code, some cases are repetitions, but I wanted to be safe
int32_t adjustOffsets(int32_t currentOffset, int32_t adjustment) {
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

void printMap(std::map<int32_t, int32_t> mapa) {
    for (const auto& [key, value] : mapa)
        std::cout << '[' << key << "] = " << value <<  "; ";
}

// Because I thought that vector.erase([i]) was't working, I used the iterator to delete.
// I think the problem ended up being that one of my functions, maybe this one, called by value instead of reference, so the erasure didn't affect the rest of the program. 
void deleteFromMapUsingIt(std::map<int32_t, int32_t> &mapa, int32_t key) {
    auto targetIndex = mapa.find(key);
    mapa.erase(targetIndex);
}