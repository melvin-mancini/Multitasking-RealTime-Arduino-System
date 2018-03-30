#ifndef RTDH_EECFG_H
#define RTDH_EECFG_H


#define RTDRUID_CONFIGURATOR_NUMBER 1278



/***************************************************************************
 *
 * Common defines ( CPU 0 )
 *
 **************************************************************************/

    /* TASK definition */
    #define EE_MAX_TASK 4
    #define loop4 0
    #define loop1 1
    #define loop2 2
    #define loop3 3

    /* MUTEX definition */
    #define EE_MAX_RESOURCE 0U

    /* ALARM definition */
    #define EE_MAX_ALARM 4U
    #define Alarmloop4 0U
    #define Alarmloop1 1U
    #define Alarmloop2 2U
    #define Alarmloop3 3U

    /* SCHEDULING TABLE definition */
    #define EE_MAX_SCHEDULETABLE 0U

    /* COUNTER OBJECTS definition */
    #define EE_MAX_COUNTER_OBJECTS (EE_MAX_ALARM + EE_MAX_SCHEDULETABLE)

    /* COUNTER definition */
    #define EE_MAX_COUNTER 1U
    #define TaskCounter 0U

    /* APPMODE definition */
    #define EE_MAX_APPMODE 0U

    /* CPUs */
    #define EE_MAX_CPU 1
    #define EE_CURRENTCPU 0

    /* Number of isr 2 */
    #define EE_MAX_ISR2   0
    #define EE_MAX_ISR_ID 0

#ifndef __DISABLE_EEOPT_DEFINES__


/***************************************************************************
 *
 * User options
 *
 **************************************************************************/
#define __ASSERT__
#define __USE_SYSTICK__
#define __ADD_LIBS__
#define __USE_ATMEL_CMSIS__
#define __USE_ATMEL_SAM_DEV__
#define __USE_ATMEL_SAM_DEV_ALL__


/***************************************************************************
 *
 * Automatic options
 *
 **************************************************************************/
#define __RTD_CYGWIN__
#define __ATMEL__
#define __SAM3__
#define __SAM3X8E__
#define __CORTEX_MX__
#define __CORTEX_M3__
#define __GNU__
#define __FP__
#define __MONO__
#define __ALARMS__
#define __FP_NO_RESOURCE__
#define __ALLOW_NESTED_IRQ__

#endif



/***************************************************************************
 *
 * Vector size defines
 *
 **************************************************************************/
    #define EE_ALARM_ROM_SIZE 4


#endif

