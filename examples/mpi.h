
#ifndef EMPHIRA_MPI
#define EMPHIRA_MPI

typedef struct {
  int source;
  int tag;
  int error;
  int len;
  int type;
} MPI_Status;

typedef int MPI_Comm;
typedef int MPI_Datatype;

#define MPI_COMM_WORLD     0
#define MPI_INT           18
#define MPI_STATUS_IGNORE (MPI_Status *)0

int MPI_Init(int *argc, char ***argv);

int MPI_Finalize();

int MPI_Comm_rank(MPI_Comm comm, int *rank);

int MPI_Comm_size(MPI_Comm comm, int *size);

int MPI_Send(void* buf, int count, MPI_Datatype datatype, int dest,
             int tag, MPI_Comm comm);

int MPI_Recv(void* buf, int count, MPI_Datatype datatype, int source,
             int tag, MPI_Comm comm, MPI_Status *status);

#endif // EMPHIRA_MPI
