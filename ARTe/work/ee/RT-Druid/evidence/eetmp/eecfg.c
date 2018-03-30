#include "ee.h"





/***************************************************************************
 *
 * Kernel ( CPU 0 )
 *
 **************************************************************************/
    /* Definition of task's body */
    DeclareTask(loop4);
    DeclareTask(loop1);
    DeclareTask(loop2);
    DeclareTask(loop3);

    const EE_THREAD_PTR EE_hal_thread_body[EE_MAX_TASK] = {
        &Funcloop4,		/* thread loop4 */
        &Funcloop1,		/* thread loop1 */
        &Funcloop2,		/* thread loop2 */
        &Funcloop3 		/* thread loop3 */

    };

    /* ready priority */
    const EE_TYPEPRIO EE_th_ready_prio[EE_MAX_TASK] = {
        0x8U,		/* thread loop4 */
        0x4U,		/* thread loop1 */
        0x2U,		/* thread loop2 */
        0x1U 		/* thread loop3 */
    };

    /* dispatch priority */
    const EE_TYPEPRIO EE_th_dispatch_prio[EE_MAX_TASK] = {
        0x8U,		/* thread loop4 */
        0x4U,		/* thread loop1 */
        0x2U,		/* thread loop2 */
        0x1U 		/* thread loop3 */
    };

    /* thread status */
    #if defined(__MULTI__) || defined(__WITH_STATUS__)
        EE_TYPESTATUS EE_th_status[EE_MAX_TASK] = {
            EE_READY,
            EE_READY,
            EE_READY,
            EE_READY
        };
    #endif

    /* next thread */
    EE_TID EE_th_next[EE_MAX_TASK] = {
        EE_NIL,
        EE_NIL,
        EE_NIL,
        EE_NIL
    };

    EE_TYPEPRIO EE_th_nact[EE_MAX_TASK];
    /* The first stacked task */
    EE_TID EE_stkfirst = EE_NIL;

    /* The first task into the ready queue */
    EE_TID EE_rqfirst  = EE_NIL;

    /* system ceiling */
    EE_TYPEPRIO EE_sys_ceiling= 0x0000U;



/***************************************************************************
 *
 * Counters
 *
 **************************************************************************/
    EE_counter_RAM_type       EE_counter_RAM[EE_MAX_COUNTER] = {
        {0, -1}         /* TaskCounter */
    };



/***************************************************************************
 *
 * Alarms
 *
 **************************************************************************/
    const EE_alarm_ROM_type   EE_alarm_ROM[EE_ALARM_ROM_SIZE] = {
        {0, EE_ALARM_ACTION_TASK    , loop4, NULL},
        {0, EE_ALARM_ACTION_TASK    , loop1, NULL},
        {0, EE_ALARM_ACTION_TASK    , loop2, NULL},
        {0, EE_ALARM_ACTION_TASK    , loop3, NULL}
    };

    EE_alarm_RAM_type         EE_alarm_RAM[EE_MAX_ALARM];

