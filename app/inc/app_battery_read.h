#include <os.h>

#ifdef	__cplusplus
extern "C" {
#endif
OS_TCB* app_battery_get_tcb( void );
void app_battery_get_stack( CPU_STK* stk );
#ifdef	__cplusplus
}
#endif

void battery_read_init( void );
extern CPU_STK battery_stack[1024];
