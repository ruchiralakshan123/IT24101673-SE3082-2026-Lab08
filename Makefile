CXX=mpicxx
CC=mpicc
CXXFLAGS=-O2 -std=c++17
CFLAGS=-O2
BIN=bin

all: dirs $(BIN)/hello_mpi $(BIN)/message1 $(BIN)/message2 $(BIN)/parallel_sum $(BIN)/monte_carlo_pi $(BIN)/mismatch $(BIN)/message2_bsend $(BIN)/pi_any_source $(BIN)/pi_any_source_bsend

dirs:
	mkdir -p $(BIN)

$(BIN)/hello_mpi: exercise0/HelloMPI.c
	$(CXX) $(CFLAGS) $< -o $@

$(BIN)/message1: exercise1/message1.cc
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN)/message2: exercise1/message2.cc
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN)/parallel_sum: exercise2/parallel_sum.cc
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN)/monte_carlo_pi: exercise3/monte_carlo_pi.cc
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN)/mismatch: exercise5/source_destination_mismatch.cc
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN)/message2_bsend: exercise5/message2_bsend.cc
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN)/pi_any_source: exercise6/pi_any_source.cc
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN)/pi_any_source_bsend: exercise7/pi_any_source_bsend.cc
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BIN)
