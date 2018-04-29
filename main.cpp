#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>


#include "process.hpp"

// DISPLAY MEM POOL ==============================================================================
void display_mem_pool(std::vector<char> &mem_pool) {
    int rows = 8;
    int elements_per_row = MEM_POOL_SIZE / rows;

    // display delineator
    for (int i = 0; i < elements_per_row; i++)
        std::cout << '=';

    // display memory pool
    int i = 0;
    for (auto &pid : mem_pool) {
        if (i % elements_per_row == 0)
            std::cout << '\n';

        std::cout << pid;
        i++;
    }
    std::cout << '\n';

    // display delineator
    for (int i = 0; i < elements_per_row; i++)
        std::cout << '=';

    std::cout << std::endl;
}


// ALPHABETICAL SORT ==============================================================================
bool AB_sort(page_info &a, page_info &b) {
	return (a.ID < b.ID);
}

// DISPLAY PAGES ================================================================================
void display_pages(std::vector<char> &mem_pool, std::vector<std::vector<std::size_t>> &pages) {
	int elements_per_row = 10;
	// display notice
	std::cout <<  "PAGE TABLE [page,frame]:\n";
	
	// sort in alphabetical order
	std::list<page_info> page_infos;
	for (std::size_t i = 0; i < pages.size(); i++) {
		page_info temp;
		temp.ID = mem_pool[pages[i][0]];
		temp.index = i;
		page_infos.push_back(temp);
	}
	page_infos.sort(AB_sort);

	// display pages
	std::list<page_info>::iterator it = page_infos.begin();
	std::size_t j = 0;
	while (it != page_infos.end()) {
		std::size_t idx = it->index;
		std::size_t pg_size = pages[idx].size();
		for (j = 0; j < pg_size; j++) {
			if (j == 0)
				std::cout << it->ID << ": ";
			else if (j % elements_per_row == 0)
				std::cout << '\n';

			std::cout << '[' << j << ',' << pages[idx][j] << ']';
			

			if ((j+1) % elements_per_row != 0 && j<pg_size-1)
				std::cout << ' ';
		}
		//if (j % elements_per_row != 0)
			std::cout << '\n';
		it++;
	}
}

// PARSE INPUT ===================================================================================
void parse_input(std::ifstream &InputStream, std::vector<Process> &processes) {

    char pid;
    int mem_frames;
    int arr_time;
    int run_time;
    char delimiter;
    std::vector<int> arrival_times;
    std::vector<int> run_times;
    std::string line;

    // pull a whole line from the input file
    while (std::getline(InputStream, line)) {

        arrival_times.clear();
        run_times.clear();

        // Ignore commented lines
        if (line[0] == '#') {
            continue;
        }

        std::stringstream ss(line);

        // first two values are the process ID and the memory frames needed
        ss >> pid >> mem_frames;

        // Then read an indeterminate number of arrival_time/run_time
        while (ss >> arr_time) {
            ss >> delimiter
               >> run_time;

            arrival_times.push_back(arr_time);
            run_times.push_back(run_time);
        }

        processes.push_back(Process(pid, mem_frames, arrival_times.size(), arrival_times, run_times));
    }

    #ifdef DEBUG
        std::cerr << "Read from input file:\n";

        for (auto &proc : processes) {
            std::cerr << proc.getPID() << " " << proc.getNumFrames();

            for (int i = 0; i < proc.getNumBursts(); i++) {
                std::cerr << " " << proc.getArrTime(i) << "/" << proc.getRunTime(i);
            }
            std::cerr << std::endl;
        }
        std::cerr << "\n\n";
    #endif // DEBUG
}


// MAIN ==========================================================================================
int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "ERROR: Invalid arguments\n"
                  << "USAGE: " << argv[0] << "<sim-input-file>\n";

        return EXIT_FAILURE;
    }

    std::ifstream InputStream(argv[1]);
    if (!InputStream.good()){
        std::cerr << "ERROR: Invalid input file format\n";
        return EXIT_FAILURE;
    }

    std::vector<Process> processes;
    parse_input(InputStream, processes);

    contiguous_memory_allocation(processes);

	noncontiguous_memory_allocation(processes);


    return EXIT_SUCCESS;
}
