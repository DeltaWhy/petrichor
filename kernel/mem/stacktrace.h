#ifndef STACKTRACE_H
#define STACKTRACE_H
void init_stacktrace(void);
void stacktrace(void);
void stacktrace_from(void *eip, void **ebp);
#endif
