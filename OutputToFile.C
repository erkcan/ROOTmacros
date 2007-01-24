// Simple functions to divert the standard output to a file.
// It is not very useful when called within CINT, since you
// might not recover from it. The best use is within CINT
// macros. Example use:
// OutputToFile("fitresults.txt");
// histo->Fit("f1","R");
// OutputToConsole();

// There are absolutely no guarantees OutputToConsole() will
// work. Only tested on under Cygwin with ROOT 5.12.

void OutputToFile(char* filename, bool append=1) {
  if (append) { freopen(filename,"a",stdout); return; }
  freopen(filename,"w",stdout); }

void OutputToConsole() { freopen("/dev/tty","w",stdout); }
