/* Based on code from:
	https://cboard.cprogramming.com/c-programming/71924-good-practice-writing-debug-print-statements.html
	https://www.tutorialspoint.com/c_standard_library/c_function_vprintf.htm
*/

#include <stdio.h> /* vprintf() */
#include <stdarg.h> /* va_end(), va_list, va_start() */
#include "log.h"

void dbprintf(const char *fmt, ...) {
#ifdef PRINT_DEBUG
  va_list args;

  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
#endif
}
