#include <os.h>

#ifdef	__cplusplus
extern "C" {
#endif
OS_TCB* app_test_get_tcb( void );
void app_test_get_stack( CPU_STK* stk );
#ifdef	__cplusplus
}
#endif
void start_test( void );
extern CPU_STK test_stack[1024];
