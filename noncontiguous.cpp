#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include "process.hpp"


// PROCESS REMOVAL FOR NON-CONTIGUOUS===============================================================================
void process_removal_nc(std::vector<char> &mem_pool, std::vector<std::vector<std::size_t>> &pages, Process &proc, int t) {

	std::size_t i = 0;

	for (; i < pages.size(); i++)
		if (proc.getPID() == mem_pool[(std::size_t) pages[i][0]])
			break;
	for (std::size_t j = 0; j < pages[i].size(); j++)
		mem_pool[pages[i][j]] = '.';
	
	pages.erase(pages.begin() + i);

	std::cout << "time " << t << "ms: Process " << proc.getPID() << " removed:\n";
	display_mem_pool(mem_pool);
	display_pages(mem_pool, pages);
}


// NEXT FIT ======================================================================================
bool first_fit(std::vector<char> &mem_pool, std::vector< std::vector< std::size_t > > &pages, std::vector<Process> &processes, Process &proc, int &t, int &start_frame) {
	    
    int frames_needed = proc.getNumFrames();
    char pid = proc.getPID();

    std::cout << "time " << t << "ms: Process " << pid << " arrived (requires " << frames_needed;
    if (frames_needed > 1)  std::cout << " frames)\n";
    else                    std::cout << " frame)\n";

    int total = 0;


    // search for free space from the the first to the end
    for (std::size_t i = 0; i < MEM_POOL_SIZE; i++) {
        // count free frames
        if (mem_pool[i] == '.')
           total++;
    }

	if (total >= frames_needed) {
		total = 0;
		std::vector<std::size_t> page_temp;
		for (std::size_t i = 0; i < MEM_POOL_SIZE; i++) {
			// count free frames
			if (mem_pool[i] == '.') {
				mem_pool[i] = pid;
				page_temp.push_back(i);
				total++;
			}
			if (total == frames_needed)
				break;
		}
		pages.push_back(page_temp);
	}
	else {
		std::cout << "time " << t << "ms: Cannot place process " << pid << " -- skipped!\n";
		return false;
	}

    std::cout << "time " << t << "ms: Placed process " << pid << ":\n";
    display_mem_pool(mem_pool);
	display_pages(mem_pool, pages);
    return true;
}

// SIMULATOR =====================================================================================
void simulator_nc(std::vector<Process> processes, std::string algorithm) {

    int t = 0;
    std::size_t remaining_processes = processes.size();

    std::vector<char> mem_pool(MEM_POOL_SIZE, '.');
	std::vector<std::vector<std::size_t>> pages;

    // temporary buffers for when multiple processes are arriving/leaving at the same time
    std::vector<Process> arriving;
    std::vector<Process> leaving;

    std::cout << "time " << t << "ms: Simulator started (Non-contiguous)\n";

    while (remaining_processes > 0) {

        // check if any processes need to be removed (removal is the same for all algorithms)
        for (auto &proc : processes) {

            if (!proc.finished()  &&  proc.getEndTime() == t) {
                process_removal_nc(mem_pool, pages, proc, t);
                proc.removed();

                if (proc.finished())
                    remaining_processes--;
            }

        }


        // check for arrival of any processes
        for (auto &proc : processes) {

            // if a process is arriving at the current time
            if (!proc.finished()  &&  proc.getArrTime((std::size_t) proc.getCurrentBurst() ) == t) {

                bool success;
                int start_frame;

                // attempt to place it; start_frame is guaranteed to be set if success == true
                // t may be changed if defragmentation occurred
				if (algorithm == "First-Fit")
					success = first_fit(mem_pool, pages, processes, proc, t, start_frame);
				else
					return;


                if (success) {
                    proc.placed(t, start_frame);

                } else {
                    proc.skipped();
                    if (proc.finished())
                        remaining_processes--;
                }
            }
        }





        t++;
    }

    std::cout << "time " << (t-1) << "ms: Simulator ended (Non-contiguous)\n";
}




// NONCONTIGUOUS MEMORY ALLOCATION ==================================================================
void noncontiguous_memory_allocation(std::vector<Process> &processes) {
	simulator_nc(processes, "First-Fit");
}