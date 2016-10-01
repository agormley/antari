#define LOG(a,...) { if (f) {fprintf(f, "[%s,%s,%d,%lu] " a "\n",__FILE__,__FUNCTION__,__LINE__,(unsigned long)time(NULL), __VA_ARGS__);}}
