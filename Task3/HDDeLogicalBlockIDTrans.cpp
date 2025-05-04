/* CS 4310.04, CPP
    Professor Damavandi
    Mario Mariotta
    Spring 2025
    Title: Project 2, Task 3, Harddrive Logical Block ID Translator
 */

#include <cstdint>
#include <iostream>


 int main () {
    int32_t blockN = -1;
    do {
        std::string blockNum;
        std::cout << "Enter a logical block number between 0 and positive infinity (inclusive): ";
        std::cin >> blockNum;
        std::cout << std::endl;
        blockN = stoi(blockNum);
    } while (blockN < 0);

    std::string numCylinders;
    std::cout << "Enter HD's number of cylinders: ";
    std::cin >> numCylinders;
    std::cout << std::endl;
    uint64_t  cylindersN = static_cast<uint64_t>(stoi(numCylinders));

    std::string numtracks;
    std::cout << "Enter HD's number of tracks: ";
    std::cin >> numtracks;
    std::cout << std::endl;
    uint64_t  tracksN = static_cast<uint64_t>(stoi(numtracks));

    std::string numSectors;
    std::cout << "Enter HD's number of sectors: ";
    std::cin >> numSectors;
    std::cout << std::endl;
    uint64_t  sectorsN = static_cast<uint64_t>(stoi(numSectors));

    if (static_cast<uint64_t>(blockN) > (sectorsN * cylindersN * tracksN) - 1) {
        std::cout << "The address you have given does not fit in the space provided by the number of cylinders, tracks, and sectors specified. Please try again." << std::endl;
        return 1;
    }

    // cylinder size (in terms of # sectors) == # tracks per cylinder * # sectors per track
    uint64_t cylinderSize = tracksN * sectorsN;

    // floor divide blockN by cylinder size to get cylinder address
    uint64_t cylinderAddr = static_cast<uint64_t>(blockN) / cylinderSize;

    // cylinder remainder is the number of sectors in the last cylinder, which is incompletely filled
    uint64_t cylinderRemainder = static_cast<uint64_t>(blockN) % cylinderSize;

    // floor divide cylinder remainder by number of sectors per track to get track address
    uint64_t trackAddr = cylinderRemainder/sectorsN;

    // take the modulus of the cylinder remainderr and number of sectors per track to get the sector address
    uint64_t sectorAddr = cylinderRemainder % (sectorsN);

    std::cout << "The logical block number is located at < " << cylinderAddr << " , " << trackAddr << " , " << sectorAddr << ">\n";
    
    return 0;
 }