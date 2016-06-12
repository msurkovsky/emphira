#include <mpi.h>
#include <stdlib.h>

#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <stack>

enum status_t { DATA, SIZE, DISTRIBUTION, FINISH };
enum actor_t { MASTER = 0, WORKER = 1 };

struct Interval {
  int start;
  int end;
};

bool is_prime(int n) {
  if (n == 2) {
    return true;
  }

  if (n < 2 || n % 2 == 0) {
    return false;
  }

  int boundary = sqrt(n);
  for (int i = 3; i <= boundary; i += 2) {
    if (n % i == 0) {
      return false;
    }
  }

  return true;
}

void assign_job (const int worker, Interval interval) {
  // std::cout << "[" << worker << "] assigned: " << interval.start << ", " << interval.end << std::endl;
  MPI_Send(&interval, 2, MPI_INT, worker, DISTRIBUTION, MPI_COMM_WORLD);
}

std::vector<int> compute(int start, int end) {
  std::vector<int> primes;
  for (int i = start; i <= end; i++) {
    if (is_prime(i)) {
      primes.push_back(i);
    }
  }
  return primes;
}

int main(int argc, char **argv) {

  if (argc != 3) {
    std::cout << argv[0] << " <block_size> <limit>" << std::endl;
    return 1;
  }

  int limit, block_size;

  block_size = std::stoi(argv[1]);
  limit = std::stoi(argv[2]);

  if (limit % block_size != 0) {
    // limit has to be dividable by block_size
    return 0;
  }

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Status status;
  std::vector<int> result;

  if (rank == MASTER) {
    // std::cout << "limit: " << limit << std::endl;
    // std::cout << "block size: " << block_size << std::endl;
    // std::cout << "limit / block size: " << (limit / block_size) << std::endl;

    // prepare intervals
    std::vector<Interval> intervals;
    intervals.resize(limit / block_size);
    for (int i = 0; i < limit / block_size; i++) {
      intervals[i].start = block_size * i + 1;
      intervals[i].end = block_size * (i + 1);
    }

    // prepare workers info
    int workers = size - 1;
    std::stack<int> free_workers;
    for (int w = WORKER; w <= workers; w++) {
      free_workers.push(w);
    }

    // distribute
    // std::cout << "intervals: " << intervals.size() << std::endl;
    for (int i = 0; i < intervals.size(); i++) {
      if (free_workers.size() <= 0) {
        int nprimes;
        MPI_Recv(&nprimes, 1, MPI_INT, MPI_ANY_SOURCE, SIZE, MPI_COMM_WORLD, &status);
        free_workers.push(status.MPI_SOURCE);

        std::vector<int> primes;
        primes.resize(nprimes);
        MPI_Recv(primes.data(), nprimes, MPI_INT, status.MPI_SOURCE, DATA, MPI_COMM_WORLD, &status);

        // for (int j = 0; j < nprimes; j++) {
        //   std::cout << primes[j] << ", ";
        // }
        // std::cout << std::endl;
        result.insert(result.end(), primes.begin(), primes.end());
      }

      assign_job(free_workers.top(), intervals[i]);
      free_workers.pop();
    }

    Interval dummy; dummy.start = -1; dummy.end = -1;
    for (int w = WORKER; w <= workers; w++) {
      MPI_Send(&dummy, 2, MPI_INT, w, FINISH, MPI_COMM_WORLD);
    }
  } else { // workers
    Interval interval;
    while(true) {
      MPI_Recv(&interval, 2, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      if (status.MPI_TAG == FINISH) {
        break;
      }

      std::vector<int> primes = compute(interval.start, interval.end);
      int nprimes = primes.size();
      MPI_Send(&nprimes, 1, MPI_INT, MASTER, SIZE, MPI_COMM_WORLD);
      MPI_Send(primes.data(), nprimes, MPI_INT, MASTER, DATA, MPI_COMM_WORLD);
    }
  }

  if (rank == MASTER) {
    std::sort(result.begin(), result.end());
    for(unsigned int i = 0; i < result.size(); i++) {
      std::cout << result[i] << ", ";
    }
    std::cout << std::endl;
  }

  MPI_Finalize();
  return 0;
}
