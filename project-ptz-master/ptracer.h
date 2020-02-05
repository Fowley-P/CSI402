//prototypes for all functions

void help ();

void regToString(const pid_t, const unsigned long long, char*, unsigned int);

void tabFormat(char*, char*, int);

int statToLong(unsigned long long, pid_t, size_t, void*);

void getSyscall(FILE*, struct user_regs_struct, pid_t, int*);

void sigPrint(FILE*, siginfo_t);
