/*
 *
 * File: prefetcher.h
 * Author: Sat Garcia (sat@cs)
 * Description: Header file for prefetcher implementation
 *
 */

#ifndef PREFETCHER_H
#define PREFETCHER_H

#include <sys/types.h>
#include "mem-sim.h"

const int _b = 4;
const int _p = 7;
const int _num_delta = 5;
const int _dhb_capacity = 32;
const int _dpt_capacity = (1<<_p);
const int _dpt_depth = 3;
const int _degree = 5;
const int _opt_capacity = (1<<_p);
const int _prefetch_capacity = 20;

class DHB {
  public:
	struct Entry {
		int32_t page;
		int32_t addr;
		int32_t last_predictor;
		int32_t used;
		int32_t num_preds;
		bool MRU;

		int32_t delta[_num_delta];
		int32_t offset[_num_delta];
	};
	
	DHB();

	// Returns -1 on miss, +i on hit, how bout 0 bitch ? 
	int has(int32_t addr);
	void add(int32_t ind, int32_t page, int32_t offset);
	void set(int32_t ind, int32_t page, int32_t offset);
	DHB::Entry* get(int32_t ind);
	void refresh_MRU();
	bool is_hit(int32_t ind, int32_t addr);
	// Either evicts or returns a non used index
	int evict();
  private:
	Entry _buffer[_dhb_capacity];
};

class DPT {
  public:
	struct Entry {
		int32_t pred;
		bool acc[2];
		bool MRU;

		int32_t delta[_num_delta];
	};

	DPT();

	// Returns -1 on miss, +i on hit, how bout 0 bitch ? 
	int has(int32_t delta[_num_delta], int32_t depth);
	DPT::Entry* get(int32_t ind);
	void refresh_MRU();
	// Either evicts or returns a non used index
	int evict();
  private:
	bool _delta_equal(int32_t a[_num_delta], int32_t b[_num_delta], const int32_t &depth);
	Entry _buffer[_dpt_capacity];
};

class OPT {
  public:
	struct Entry {
		int32_t pred;
		bool acc;
		bool used;
	};

	OPT();
	OPT::Entry* get(int32_t addr);
  private:
	Entry _buffer[_opt_capacity];
};

class Prefetcher {
  private:
	bool _ready;
	Request _nextReq;

	DHB _dhb;
	OPT _opt;
	DPT _dpt[_dpt_depth];

	int32_t _capacity;

	int32_t _front;
	int32_t _rear;
	int32_t _size;

	int32_t _prefetch[_prefetch_capacity];

	void _add(int32_t dhb_ind, int32_t addr, bool should_add);
	void _update_dpt(int32_t dhb_ind);
	void _update_opt(int32_t dhb_ind);
	void _check_prediction(int32_t dhb_ind, bool load, bool success);
  public:
	Prefetcher();

	// should return true if a request is ready for this cycle
	bool hasRequest(int32_t cycle);

	// request a desired address be brought in
	Request getRequest(int32_t cycle);

	// this function is called whenever the last prefetcher request was successfully sent to the L2
	void completeRequest(int32_t cycle);

	/*
	 * This function is called whenever the CPU references memory.
	 * Note that only the addr, pc, load, issuedAt, and HitL1 should be considered valid data
	 */
	void cpuRequest(Request req); 
};

#endif
