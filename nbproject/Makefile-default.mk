#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/QuadController.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/QuadController.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=app/src/os_app_hooks.c app/src/button_reader.cpp app/src/app.cpp app/src/app_accel_gyro.cpp BSP/src/bsp.c BSP/src/bsp_a.S BSP/src/crt0.S BSP/src/bsp_utils.c BSP/src/bsp_accel_gyro.cpp CPU/src/PIC32INTlib.c drivers/src/driver_i2c.cpp drivers/src/driver_uart.c uC-CPU/src/cpu_a.s uC-CPU/src/cpu_c.c uC-CPU/src/cpu_core.c uC-LIB/src/lib_ascii.c uC-LIB/src/lib_math.c uC-LIB/src/lib_mem.c uC-LIB/src/lib_str.c uCOS-III/src/os_cfg_app.c uCOS-III/src/os_core.c uCOS-III/src/os_cpu_a.S uCOS-III/src/os_cpu_c.c uCOS-III/src/os_dbg.c uCOS-III/src/os_flag.c uCOS-III/src/os_int.c uCOS-III/src/os_mem.c uCOS-III/src/os_msg.c uCOS-III/src/os_mutex.c uCOS-III/src/os_pend_multi.c uCOS-III/src/os_prio.c uCOS-III/src/os_q.c uCOS-III/src/os_sem.c uCOS-III/src/os_stat.c uCOS-III/src/os_task.c uCOS-III/src/os_tick.c uCOS-III/src/os_time.c uCOS-III/src/os_tmr.c uCOS-III/src/os_var.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/app/src/os_app_hooks.o ${OBJECTDIR}/app/src/button_reader.o ${OBJECTDIR}/app/src/app.o ${OBJECTDIR}/app/src/app_accel_gyro.o ${OBJECTDIR}/BSP/src/bsp.o ${OBJECTDIR}/BSP/src/bsp_a.o ${OBJECTDIR}/BSP/src/crt0.o ${OBJECTDIR}/BSP/src/bsp_utils.o ${OBJECTDIR}/BSP/src/bsp_accel_gyro.o ${OBJECTDIR}/CPU/src/PIC32INTlib.o ${OBJECTDIR}/drivers/src/driver_i2c.o ${OBJECTDIR}/drivers/src/driver_uart.o ${OBJECTDIR}/uC-CPU/src/cpu_a.o ${OBJECTDIR}/uC-CPU/src/cpu_c.o ${OBJECTDIR}/uC-CPU/src/cpu_core.o ${OBJECTDIR}/uC-LIB/src/lib_ascii.o ${OBJECTDIR}/uC-LIB/src/lib_math.o ${OBJECTDIR}/uC-LIB/src/lib_mem.o ${OBJECTDIR}/uC-LIB/src/lib_str.o ${OBJECTDIR}/uCOS-III/src/os_cfg_app.o ${OBJECTDIR}/uCOS-III/src/os_core.o ${OBJECTDIR}/uCOS-III/src/os_cpu_a.o ${OBJECTDIR}/uCOS-III/src/os_cpu_c.o ${OBJECTDIR}/uCOS-III/src/os_dbg.o ${OBJECTDIR}/uCOS-III/src/os_flag.o ${OBJECTDIR}/uCOS-III/src/os_int.o ${OBJECTDIR}/uCOS-III/src/os_mem.o ${OBJECTDIR}/uCOS-III/src/os_msg.o ${OBJECTDIR}/uCOS-III/src/os_mutex.o ${OBJECTDIR}/uCOS-III/src/os_pend_multi.o ${OBJECTDIR}/uCOS-III/src/os_prio.o ${OBJECTDIR}/uCOS-III/src/os_q.o ${OBJECTDIR}/uCOS-III/src/os_sem.o ${OBJECTDIR}/uCOS-III/src/os_stat.o ${OBJECTDIR}/uCOS-III/src/os_task.o ${OBJECTDIR}/uCOS-III/src/os_tick.o ${OBJECTDIR}/uCOS-III/src/os_time.o ${OBJECTDIR}/uCOS-III/src/os_tmr.o ${OBJECTDIR}/uCOS-III/src/os_var.o
POSSIBLE_DEPFILES=${OBJECTDIR}/app/src/os_app_hooks.o.d ${OBJECTDIR}/app/src/button_reader.o.d ${OBJECTDIR}/app/src/app.o.d ${OBJECTDIR}/app/src/app_accel_gyro.o.d ${OBJECTDIR}/BSP/src/bsp.o.d ${OBJECTDIR}/BSP/src/bsp_a.o.d ${OBJECTDIR}/BSP/src/crt0.o.d ${OBJECTDIR}/BSP/src/bsp_utils.o.d ${OBJECTDIR}/BSP/src/bsp_accel_gyro.o.d ${OBJECTDIR}/CPU/src/PIC32INTlib.o.d ${OBJECTDIR}/drivers/src/driver_i2c.o.d ${OBJECTDIR}/drivers/src/driver_uart.o.d ${OBJECTDIR}/uC-CPU/src/cpu_a.o.d ${OBJECTDIR}/uC-CPU/src/cpu_c.o.d ${OBJECTDIR}/uC-CPU/src/cpu_core.o.d ${OBJECTDIR}/uC-LIB/src/lib_ascii.o.d ${OBJECTDIR}/uC-LIB/src/lib_math.o.d ${OBJECTDIR}/uC-LIB/src/lib_mem.o.d ${OBJECTDIR}/uC-LIB/src/lib_str.o.d ${OBJECTDIR}/uCOS-III/src/os_cfg_app.o.d ${OBJECTDIR}/uCOS-III/src/os_core.o.d ${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.d ${OBJECTDIR}/uCOS-III/src/os_cpu_c.o.d ${OBJECTDIR}/uCOS-III/src/os_dbg.o.d ${OBJECTDIR}/uCOS-III/src/os_flag.o.d ${OBJECTDIR}/uCOS-III/src/os_int.o.d ${OBJECTDIR}/uCOS-III/src/os_mem.o.d ${OBJECTDIR}/uCOS-III/src/os_msg.o.d ${OBJECTDIR}/uCOS-III/src/os_mutex.o.d ${OBJECTDIR}/uCOS-III/src/os_pend_multi.o.d ${OBJECTDIR}/uCOS-III/src/os_prio.o.d ${OBJECTDIR}/uCOS-III/src/os_q.o.d ${OBJECTDIR}/uCOS-III/src/os_sem.o.d ${OBJECTDIR}/uCOS-III/src/os_stat.o.d ${OBJECTDIR}/uCOS-III/src/os_task.o.d ${OBJECTDIR}/uCOS-III/src/os_tick.o.d ${OBJECTDIR}/uCOS-III/src/os_time.o.d ${OBJECTDIR}/uCOS-III/src/os_tmr.o.d ${OBJECTDIR}/uCOS-III/src/os_var.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/app/src/os_app_hooks.o ${OBJECTDIR}/app/src/button_reader.o ${OBJECTDIR}/app/src/app.o ${OBJECTDIR}/app/src/app_accel_gyro.o ${OBJECTDIR}/BSP/src/bsp.o ${OBJECTDIR}/BSP/src/bsp_a.o ${OBJECTDIR}/BSP/src/crt0.o ${OBJECTDIR}/BSP/src/bsp_utils.o ${OBJECTDIR}/BSP/src/bsp_accel_gyro.o ${OBJECTDIR}/CPU/src/PIC32INTlib.o ${OBJECTDIR}/drivers/src/driver_i2c.o ${OBJECTDIR}/drivers/src/driver_uart.o ${OBJECTDIR}/uC-CPU/src/cpu_a.o ${OBJECTDIR}/uC-CPU/src/cpu_c.o ${OBJECTDIR}/uC-CPU/src/cpu_core.o ${OBJECTDIR}/uC-LIB/src/lib_ascii.o ${OBJECTDIR}/uC-LIB/src/lib_math.o ${OBJECTDIR}/uC-LIB/src/lib_mem.o ${OBJECTDIR}/uC-LIB/src/lib_str.o ${OBJECTDIR}/uCOS-III/src/os_cfg_app.o ${OBJECTDIR}/uCOS-III/src/os_core.o ${OBJECTDIR}/uCOS-III/src/os_cpu_a.o ${OBJECTDIR}/uCOS-III/src/os_cpu_c.o ${OBJECTDIR}/uCOS-III/src/os_dbg.o ${OBJECTDIR}/uCOS-III/src/os_flag.o ${OBJECTDIR}/uCOS-III/src/os_int.o ${OBJECTDIR}/uCOS-III/src/os_mem.o ${OBJECTDIR}/uCOS-III/src/os_msg.o ${OBJECTDIR}/uCOS-III/src/os_mutex.o ${OBJECTDIR}/uCOS-III/src/os_pend_multi.o ${OBJECTDIR}/uCOS-III/src/os_prio.o ${OBJECTDIR}/uCOS-III/src/os_q.o ${OBJECTDIR}/uCOS-III/src/os_sem.o ${OBJECTDIR}/uCOS-III/src/os_stat.o ${OBJECTDIR}/uCOS-III/src/os_task.o ${OBJECTDIR}/uCOS-III/src/os_tick.o ${OBJECTDIR}/uCOS-III/src/os_time.o ${OBJECTDIR}/uCOS-III/src/os_tmr.o ${OBJECTDIR}/uCOS-III/src/os_var.o

# Source Files
SOURCEFILES=app/src/os_app_hooks.c app/src/button_reader.cpp app/src/app.cpp app/src/app_accel_gyro.cpp BSP/src/bsp.c BSP/src/bsp_a.S BSP/src/crt0.S BSP/src/bsp_utils.c BSP/src/bsp_accel_gyro.cpp CPU/src/PIC32INTlib.c drivers/src/driver_i2c.cpp drivers/src/driver_uart.c uC-CPU/src/cpu_a.s uC-CPU/src/cpu_c.c uC-CPU/src/cpu_core.c uC-LIB/src/lib_ascii.c uC-LIB/src/lib_math.c uC-LIB/src/lib_mem.c uC-LIB/src/lib_str.c uCOS-III/src/os_cfg_app.c uCOS-III/src/os_core.c uCOS-III/src/os_cpu_a.S uCOS-III/src/os_cpu_c.c uCOS-III/src/os_dbg.c uCOS-III/src/os_flag.c uCOS-III/src/os_int.c uCOS-III/src/os_mem.c uCOS-III/src/os_msg.c uCOS-III/src/os_mutex.c uCOS-III/src/os_pend_multi.c uCOS-III/src/os_prio.c uCOS-III/src/os_q.c uCOS-III/src/os_sem.c uCOS-III/src/os_stat.c uCOS-III/src/os_task.c uCOS-III/src/os_tick.c uCOS-III/src/os_time.c uCOS-III/src/os_tmr.c uCOS-III/src/os_var.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/QuadController.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/uC-CPU/src/cpu_a.o: uC-CPU/src/cpu_a.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-CPU/src 
	@${FIXDEPS} "${OBJECTDIR}/uC-CPU/src/cpu_a.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -D__MPLAB_DEBUGGER_PIC32MXSK=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${OBJECTDIR}/uC-CPU/src/cpu_a.o uC-CPU/src/cpu_a.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PIC32MXSK=1,--gdwarf-2,-MD="${OBJECTDIR}/uC-CPU/src/cpu_a.o.d"
else
${OBJECTDIR}/uC-CPU/src/cpu_a.o: uC-CPU/src/cpu_a.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-CPU/src 
	@${FIXDEPS} "${OBJECTDIR}/uC-CPU/src/cpu_a.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${OBJECTDIR}/uC-CPU/src/cpu_a.o uC-CPU/src/cpu_a.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--gdwarf-2,-MD="${OBJECTDIR}/uC-CPU/src/cpu_a.o.d"
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/BSP/src/bsp_a.o: BSP/src/bsp_a.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/BSP/src 
	@${RM} ${OBJECTDIR}/BSP/src/bsp_a.o.d 
	@${RM} ${OBJECTDIR}/BSP/src/bsp_a.o.ok ${OBJECTDIR}/BSP/src/bsp_a.o.err 
	@${FIXDEPS} "${OBJECTDIR}/BSP/src/bsp_a.o.d" "${OBJECTDIR}/BSP/src/bsp_a.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/BSP/src/bsp_a.o.d"  -o ${OBJECTDIR}/BSP/src/bsp_a.o BSP/src/bsp_a.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/BSP/src/bsp_a.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PIC32MXSK=1
	
${OBJECTDIR}/BSP/src/crt0.o: BSP/src/crt0.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/BSP/src 
	@${RM} ${OBJECTDIR}/BSP/src/crt0.o.d 
	@${RM} ${OBJECTDIR}/BSP/src/crt0.o.ok ${OBJECTDIR}/BSP/src/crt0.o.err 
	@${FIXDEPS} "${OBJECTDIR}/BSP/src/crt0.o.d" "${OBJECTDIR}/BSP/src/crt0.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/BSP/src/crt0.o.d"  -o ${OBJECTDIR}/BSP/src/crt0.o BSP/src/crt0.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/BSP/src/crt0.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PIC32MXSK=1
	
${OBJECTDIR}/uCOS-III/src/os_cpu_a.o: uCOS-III/src/os_cpu_a.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.d 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.ok ${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.err 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.d" "${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.d"  -o ${OBJECTDIR}/uCOS-III/src/os_cpu_a.o uCOS-III/src/os_cpu_a.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PIC32MXSK=1
	
else
${OBJECTDIR}/BSP/src/bsp_a.o: BSP/src/bsp_a.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/BSP/src 
	@${RM} ${OBJECTDIR}/BSP/src/bsp_a.o.d 
	@${RM} ${OBJECTDIR}/BSP/src/bsp_a.o.ok ${OBJECTDIR}/BSP/src/bsp_a.o.err 
	@${FIXDEPS} "${OBJECTDIR}/BSP/src/bsp_a.o.d" "${OBJECTDIR}/BSP/src/bsp_a.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/BSP/src/bsp_a.o.d"  -o ${OBJECTDIR}/BSP/src/bsp_a.o BSP/src/bsp_a.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/BSP/src/bsp_a.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/BSP/src/crt0.o: BSP/src/crt0.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/BSP/src 
	@${RM} ${OBJECTDIR}/BSP/src/crt0.o.d 
	@${RM} ${OBJECTDIR}/BSP/src/crt0.o.ok ${OBJECTDIR}/BSP/src/crt0.o.err 
	@${FIXDEPS} "${OBJECTDIR}/BSP/src/crt0.o.d" "${OBJECTDIR}/BSP/src/crt0.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/BSP/src/crt0.o.d"  -o ${OBJECTDIR}/BSP/src/crt0.o BSP/src/crt0.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/BSP/src/crt0.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/uCOS-III/src/os_cpu_a.o: uCOS-III/src/os_cpu_a.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.d 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.ok ${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.err 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.d" "${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.d"  -o ${OBJECTDIR}/uCOS-III/src/os_cpu_a.o uCOS-III/src/os_cpu_a.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/uCOS-III/src/os_cpu_a.o.asm.d",--gdwarf-2
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/app/src/os_app_hooks.o: app/src/os_app_hooks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app/src 
	@${RM} ${OBJECTDIR}/app/src/os_app_hooks.o.d 
	@${FIXDEPS} "${OBJECTDIR}/app/src/os_app_hooks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/app/src/os_app_hooks.o.d" -o ${OBJECTDIR}/app/src/os_app_hooks.o app/src/os_app_hooks.c   
	
${OBJECTDIR}/BSP/src/bsp.o: BSP/src/bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/BSP/src 
	@${RM} ${OBJECTDIR}/BSP/src/bsp.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BSP/src/bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/BSP/src/bsp.o.d" -o ${OBJECTDIR}/BSP/src/bsp.o BSP/src/bsp.c   
	
${OBJECTDIR}/BSP/src/bsp_utils.o: BSP/src/bsp_utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/BSP/src 
	@${RM} ${OBJECTDIR}/BSP/src/bsp_utils.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BSP/src/bsp_utils.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/BSP/src/bsp_utils.o.d" -o ${OBJECTDIR}/BSP/src/bsp_utils.o BSP/src/bsp_utils.c   
	
${OBJECTDIR}/CPU/src/PIC32INTlib.o: CPU/src/PIC32INTlib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/CPU/src 
	@${RM} ${OBJECTDIR}/CPU/src/PIC32INTlib.o.d 
	@${FIXDEPS} "${OBJECTDIR}/CPU/src/PIC32INTlib.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/CPU/src/PIC32INTlib.o.d" -o ${OBJECTDIR}/CPU/src/PIC32INTlib.o CPU/src/PIC32INTlib.c   
	
${OBJECTDIR}/drivers/src/driver_uart.o: drivers/src/driver_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/driver_uart.o.d 
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/driver_uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/drivers/src/driver_uart.o.d" -o ${OBJECTDIR}/drivers/src/driver_uart.o drivers/src/driver_uart.c   
	
${OBJECTDIR}/uC-CPU/src/cpu_c.o: uC-CPU/src/cpu_c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-CPU/src 
	@${RM} ${OBJECTDIR}/uC-CPU/src/cpu_c.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uC-CPU/src/cpu_c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uC-CPU/src/cpu_c.o.d" -o ${OBJECTDIR}/uC-CPU/src/cpu_c.o uC-CPU/src/cpu_c.c   
	
${OBJECTDIR}/uC-CPU/src/cpu_core.o: uC-CPU/src/cpu_core.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-CPU/src 
	@${RM} ${OBJECTDIR}/uC-CPU/src/cpu_core.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uC-CPU/src/cpu_core.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uC-CPU/src/cpu_core.o.d" -o ${OBJECTDIR}/uC-CPU/src/cpu_core.o uC-CPU/src/cpu_core.c   
	
${OBJECTDIR}/uC-LIB/src/lib_ascii.o: uC-LIB/src/lib_ascii.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-LIB/src 
	@${RM} ${OBJECTDIR}/uC-LIB/src/lib_ascii.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uC-LIB/src/lib_ascii.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uC-LIB/src/lib_ascii.o.d" -o ${OBJECTDIR}/uC-LIB/src/lib_ascii.o uC-LIB/src/lib_ascii.c   
	
${OBJECTDIR}/uC-LIB/src/lib_math.o: uC-LIB/src/lib_math.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-LIB/src 
	@${RM} ${OBJECTDIR}/uC-LIB/src/lib_math.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uC-LIB/src/lib_math.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uC-LIB/src/lib_math.o.d" -o ${OBJECTDIR}/uC-LIB/src/lib_math.o uC-LIB/src/lib_math.c   
	
${OBJECTDIR}/uC-LIB/src/lib_mem.o: uC-LIB/src/lib_mem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-LIB/src 
	@${RM} ${OBJECTDIR}/uC-LIB/src/lib_mem.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uC-LIB/src/lib_mem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uC-LIB/src/lib_mem.o.d" -o ${OBJECTDIR}/uC-LIB/src/lib_mem.o uC-LIB/src/lib_mem.c   
	
${OBJECTDIR}/uC-LIB/src/lib_str.o: uC-LIB/src/lib_str.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-LIB/src 
	@${RM} ${OBJECTDIR}/uC-LIB/src/lib_str.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uC-LIB/src/lib_str.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uC-LIB/src/lib_str.o.d" -o ${OBJECTDIR}/uC-LIB/src/lib_str.o uC-LIB/src/lib_str.c   
	
${OBJECTDIR}/uCOS-III/src/os_cfg_app.o: uCOS-III/src/os_cfg_app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_cfg_app.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_cfg_app.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_cfg_app.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_cfg_app.o uCOS-III/src/os_cfg_app.c   
	
${OBJECTDIR}/uCOS-III/src/os_core.o: uCOS-III/src/os_core.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_core.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_core.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_core.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_core.o uCOS-III/src/os_core.c   
	
${OBJECTDIR}/uCOS-III/src/os_cpu_c.o: uCOS-III/src/os_cpu_c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_cpu_c.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_cpu_c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_cpu_c.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_cpu_c.o uCOS-III/src/os_cpu_c.c   
	
${OBJECTDIR}/uCOS-III/src/os_dbg.o: uCOS-III/src/os_dbg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_dbg.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_dbg.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_dbg.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_dbg.o uCOS-III/src/os_dbg.c   
	
${OBJECTDIR}/uCOS-III/src/os_flag.o: uCOS-III/src/os_flag.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_flag.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_flag.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_flag.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_flag.o uCOS-III/src/os_flag.c   
	
${OBJECTDIR}/uCOS-III/src/os_int.o: uCOS-III/src/os_int.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_int.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_int.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_int.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_int.o uCOS-III/src/os_int.c   
	
${OBJECTDIR}/uCOS-III/src/os_mem.o: uCOS-III/src/os_mem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_mem.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_mem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_mem.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_mem.o uCOS-III/src/os_mem.c   
	
${OBJECTDIR}/uCOS-III/src/os_msg.o: uCOS-III/src/os_msg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_msg.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_msg.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_msg.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_msg.o uCOS-III/src/os_msg.c   
	
${OBJECTDIR}/uCOS-III/src/os_mutex.o: uCOS-III/src/os_mutex.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_mutex.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_mutex.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_mutex.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_mutex.o uCOS-III/src/os_mutex.c   
	
${OBJECTDIR}/uCOS-III/src/os_pend_multi.o: uCOS-III/src/os_pend_multi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_pend_multi.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_pend_multi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_pend_multi.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_pend_multi.o uCOS-III/src/os_pend_multi.c   
	
${OBJECTDIR}/uCOS-III/src/os_prio.o: uCOS-III/src/os_prio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_prio.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_prio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_prio.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_prio.o uCOS-III/src/os_prio.c   
	
${OBJECTDIR}/uCOS-III/src/os_q.o: uCOS-III/src/os_q.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_q.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_q.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_q.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_q.o uCOS-III/src/os_q.c   
	
${OBJECTDIR}/uCOS-III/src/os_sem.o: uCOS-III/src/os_sem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_sem.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_sem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_sem.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_sem.o uCOS-III/src/os_sem.c   
	
${OBJECTDIR}/uCOS-III/src/os_stat.o: uCOS-III/src/os_stat.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_stat.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_stat.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_stat.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_stat.o uCOS-III/src/os_stat.c   
	
${OBJECTDIR}/uCOS-III/src/os_task.o: uCOS-III/src/os_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_task.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_task.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_task.o uCOS-III/src/os_task.c   
	
${OBJECTDIR}/uCOS-III/src/os_tick.o: uCOS-III/src/os_tick.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_tick.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_tick.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_tick.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_tick.o uCOS-III/src/os_tick.c   
	
${OBJECTDIR}/uCOS-III/src/os_time.o: uCOS-III/src/os_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_time.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_time.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_time.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_time.o uCOS-III/src/os_time.c   
	
${OBJECTDIR}/uCOS-III/src/os_tmr.o: uCOS-III/src/os_tmr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_tmr.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_tmr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_tmr.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_tmr.o uCOS-III/src/os_tmr.c   
	
${OBJECTDIR}/uCOS-III/src/os_var.o: uCOS-III/src/os_var.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_var.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_var.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_var.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_var.o uCOS-III/src/os_var.c   
	
else
${OBJECTDIR}/app/src/os_app_hooks.o: app/src/os_app_hooks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app/src 
	@${RM} ${OBJECTDIR}/app/src/os_app_hooks.o.d 
	@${FIXDEPS} "${OBJECTDIR}/app/src/os_app_hooks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/app/src/os_app_hooks.o.d" -o ${OBJECTDIR}/app/src/os_app_hooks.o app/src/os_app_hooks.c   
	
${OBJECTDIR}/BSP/src/bsp.o: BSP/src/bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/BSP/src 
	@${RM} ${OBJECTDIR}/BSP/src/bsp.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BSP/src/bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/BSP/src/bsp.o.d" -o ${OBJECTDIR}/BSP/src/bsp.o BSP/src/bsp.c   
	
${OBJECTDIR}/BSP/src/bsp_utils.o: BSP/src/bsp_utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/BSP/src 
	@${RM} ${OBJECTDIR}/BSP/src/bsp_utils.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BSP/src/bsp_utils.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/BSP/src/bsp_utils.o.d" -o ${OBJECTDIR}/BSP/src/bsp_utils.o BSP/src/bsp_utils.c   
	
${OBJECTDIR}/CPU/src/PIC32INTlib.o: CPU/src/PIC32INTlib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/CPU/src 
	@${RM} ${OBJECTDIR}/CPU/src/PIC32INTlib.o.d 
	@${FIXDEPS} "${OBJECTDIR}/CPU/src/PIC32INTlib.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/CPU/src/PIC32INTlib.o.d" -o ${OBJECTDIR}/CPU/src/PIC32INTlib.o CPU/src/PIC32INTlib.c   
	
${OBJECTDIR}/drivers/src/driver_uart.o: drivers/src/driver_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/driver_uart.o.d 
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/driver_uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/drivers/src/driver_uart.o.d" -o ${OBJECTDIR}/drivers/src/driver_uart.o drivers/src/driver_uart.c   
	
${OBJECTDIR}/uC-CPU/src/cpu_c.o: uC-CPU/src/cpu_c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-CPU/src 
	@${RM} ${OBJECTDIR}/uC-CPU/src/cpu_c.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uC-CPU/src/cpu_c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uC-CPU/src/cpu_c.o.d" -o ${OBJECTDIR}/uC-CPU/src/cpu_c.o uC-CPU/src/cpu_c.c   
	
${OBJECTDIR}/uC-CPU/src/cpu_core.o: uC-CPU/src/cpu_core.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-CPU/src 
	@${RM} ${OBJECTDIR}/uC-CPU/src/cpu_core.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uC-CPU/src/cpu_core.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uC-CPU/src/cpu_core.o.d" -o ${OBJECTDIR}/uC-CPU/src/cpu_core.o uC-CPU/src/cpu_core.c   
	
${OBJECTDIR}/uC-LIB/src/lib_ascii.o: uC-LIB/src/lib_ascii.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-LIB/src 
	@${RM} ${OBJECTDIR}/uC-LIB/src/lib_ascii.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uC-LIB/src/lib_ascii.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uC-LIB/src/lib_ascii.o.d" -o ${OBJECTDIR}/uC-LIB/src/lib_ascii.o uC-LIB/src/lib_ascii.c   
	
${OBJECTDIR}/uC-LIB/src/lib_math.o: uC-LIB/src/lib_math.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-LIB/src 
	@${RM} ${OBJECTDIR}/uC-LIB/src/lib_math.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uC-LIB/src/lib_math.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uC-LIB/src/lib_math.o.d" -o ${OBJECTDIR}/uC-LIB/src/lib_math.o uC-LIB/src/lib_math.c   
	
${OBJECTDIR}/uC-LIB/src/lib_mem.o: uC-LIB/src/lib_mem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-LIB/src 
	@${RM} ${OBJECTDIR}/uC-LIB/src/lib_mem.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uC-LIB/src/lib_mem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uC-LIB/src/lib_mem.o.d" -o ${OBJECTDIR}/uC-LIB/src/lib_mem.o uC-LIB/src/lib_mem.c   
	
${OBJECTDIR}/uC-LIB/src/lib_str.o: uC-LIB/src/lib_str.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uC-LIB/src 
	@${RM} ${OBJECTDIR}/uC-LIB/src/lib_str.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uC-LIB/src/lib_str.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uC-LIB/src/lib_str.o.d" -o ${OBJECTDIR}/uC-LIB/src/lib_str.o uC-LIB/src/lib_str.c   
	
${OBJECTDIR}/uCOS-III/src/os_cfg_app.o: uCOS-III/src/os_cfg_app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_cfg_app.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_cfg_app.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_cfg_app.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_cfg_app.o uCOS-III/src/os_cfg_app.c   
	
${OBJECTDIR}/uCOS-III/src/os_core.o: uCOS-III/src/os_core.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_core.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_core.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_core.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_core.o uCOS-III/src/os_core.c   
	
${OBJECTDIR}/uCOS-III/src/os_cpu_c.o: uCOS-III/src/os_cpu_c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_cpu_c.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_cpu_c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_cpu_c.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_cpu_c.o uCOS-III/src/os_cpu_c.c   
	
${OBJECTDIR}/uCOS-III/src/os_dbg.o: uCOS-III/src/os_dbg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_dbg.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_dbg.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_dbg.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_dbg.o uCOS-III/src/os_dbg.c   
	
${OBJECTDIR}/uCOS-III/src/os_flag.o: uCOS-III/src/os_flag.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_flag.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_flag.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_flag.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_flag.o uCOS-III/src/os_flag.c   
	
${OBJECTDIR}/uCOS-III/src/os_int.o: uCOS-III/src/os_int.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_int.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_int.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_int.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_int.o uCOS-III/src/os_int.c   
	
${OBJECTDIR}/uCOS-III/src/os_mem.o: uCOS-III/src/os_mem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_mem.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_mem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_mem.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_mem.o uCOS-III/src/os_mem.c   
	
${OBJECTDIR}/uCOS-III/src/os_msg.o: uCOS-III/src/os_msg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_msg.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_msg.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_msg.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_msg.o uCOS-III/src/os_msg.c   
	
${OBJECTDIR}/uCOS-III/src/os_mutex.o: uCOS-III/src/os_mutex.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_mutex.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_mutex.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_mutex.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_mutex.o uCOS-III/src/os_mutex.c   
	
${OBJECTDIR}/uCOS-III/src/os_pend_multi.o: uCOS-III/src/os_pend_multi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_pend_multi.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_pend_multi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_pend_multi.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_pend_multi.o uCOS-III/src/os_pend_multi.c   
	
${OBJECTDIR}/uCOS-III/src/os_prio.o: uCOS-III/src/os_prio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_prio.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_prio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_prio.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_prio.o uCOS-III/src/os_prio.c   
	
${OBJECTDIR}/uCOS-III/src/os_q.o: uCOS-III/src/os_q.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_q.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_q.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_q.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_q.o uCOS-III/src/os_q.c   
	
${OBJECTDIR}/uCOS-III/src/os_sem.o: uCOS-III/src/os_sem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_sem.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_sem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_sem.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_sem.o uCOS-III/src/os_sem.c   
	
${OBJECTDIR}/uCOS-III/src/os_stat.o: uCOS-III/src/os_stat.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_stat.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_stat.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_stat.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_stat.o uCOS-III/src/os_stat.c   
	
${OBJECTDIR}/uCOS-III/src/os_task.o: uCOS-III/src/os_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_task.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_task.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_task.o uCOS-III/src/os_task.c   
	
${OBJECTDIR}/uCOS-III/src/os_tick.o: uCOS-III/src/os_tick.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_tick.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_tick.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_tick.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_tick.o uCOS-III/src/os_tick.c   
	
${OBJECTDIR}/uCOS-III/src/os_time.o: uCOS-III/src/os_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_time.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_time.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_time.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_time.o uCOS-III/src/os_time.c   
	
${OBJECTDIR}/uCOS-III/src/os_tmr.o: uCOS-III/src/os_tmr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_tmr.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_tmr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_tmr.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_tmr.o uCOS-III/src/os_tmr.c   
	
${OBJECTDIR}/uCOS-III/src/os_var.o: uCOS-III/src/os_var.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uCOS-III/src 
	@${RM} ${OBJECTDIR}/uCOS-III/src/os_var.o.d 
	@${FIXDEPS} "${OBJECTDIR}/uCOS-III/src/os_var.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/uCOS-III/src/os_var.o.d" -o ${OBJECTDIR}/uCOS-III/src/os_var.o uCOS-III/src/os_var.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/app/src/button_reader.o: app/src/button_reader.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app/src 
	@${RM} ${OBJECTDIR}/app/src/button_reader.o.d 
	@${FIXDEPS} "${OBJECTDIR}/app/src/button_reader.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/app/src/button_reader.o.d" -o ${OBJECTDIR}/app/src/button_reader.o app/src/button_reader.cpp  
	
${OBJECTDIR}/app/src/app.o: app/src/app.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app/src 
	@${RM} ${OBJECTDIR}/app/src/app.o.d 
	@${FIXDEPS} "${OBJECTDIR}/app/src/app.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/app/src/app.o.d" -o ${OBJECTDIR}/app/src/app.o app/src/app.cpp  
	
${OBJECTDIR}/app/src/app_accel_gyro.o: app/src/app_accel_gyro.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app/src 
	@${RM} ${OBJECTDIR}/app/src/app_accel_gyro.o.d 
	@${FIXDEPS} "${OBJECTDIR}/app/src/app_accel_gyro.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/app/src/app_accel_gyro.o.d" -o ${OBJECTDIR}/app/src/app_accel_gyro.o app/src/app_accel_gyro.cpp  
	
${OBJECTDIR}/BSP/src/bsp_accel_gyro.o: BSP/src/bsp_accel_gyro.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/BSP/src 
	@${RM} ${OBJECTDIR}/BSP/src/bsp_accel_gyro.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BSP/src/bsp_accel_gyro.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/BSP/src/bsp_accel_gyro.o.d" -o ${OBJECTDIR}/BSP/src/bsp_accel_gyro.o BSP/src/bsp_accel_gyro.cpp  
	
${OBJECTDIR}/drivers/src/driver_i2c.o: drivers/src/driver_i2c.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/driver_i2c.o.d 
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/driver_i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/drivers/src/driver_i2c.o.d" -o ${OBJECTDIR}/drivers/src/driver_i2c.o drivers/src/driver_i2c.cpp  
	
else
${OBJECTDIR}/app/src/button_reader.o: app/src/button_reader.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app/src 
	@${RM} ${OBJECTDIR}/app/src/button_reader.o.d 
	@${FIXDEPS} "${OBJECTDIR}/app/src/button_reader.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/app/src/button_reader.o.d" -o ${OBJECTDIR}/app/src/button_reader.o app/src/button_reader.cpp  
	
${OBJECTDIR}/app/src/app.o: app/src/app.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app/src 
	@${RM} ${OBJECTDIR}/app/src/app.o.d 
	@${FIXDEPS} "${OBJECTDIR}/app/src/app.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/app/src/app.o.d" -o ${OBJECTDIR}/app/src/app.o app/src/app.cpp  
	
${OBJECTDIR}/app/src/app_accel_gyro.o: app/src/app_accel_gyro.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app/src 
	@${RM} ${OBJECTDIR}/app/src/app_accel_gyro.o.d 
	@${FIXDEPS} "${OBJECTDIR}/app/src/app_accel_gyro.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/app/src/app_accel_gyro.o.d" -o ${OBJECTDIR}/app/src/app_accel_gyro.o app/src/app_accel_gyro.cpp  
	
${OBJECTDIR}/BSP/src/bsp_accel_gyro.o: BSP/src/bsp_accel_gyro.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/BSP/src 
	@${RM} ${OBJECTDIR}/BSP/src/bsp_accel_gyro.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BSP/src/bsp_accel_gyro.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/BSP/src/bsp_accel_gyro.o.d" -o ${OBJECTDIR}/BSP/src/bsp_accel_gyro.o BSP/src/bsp_accel_gyro.cpp  
	
${OBJECTDIR}/drivers/src/driver_i2c.o: drivers/src/driver_i2c.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/driver_i2c.o.d 
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/driver_i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"app/inc" -I"BSP/inc" -I"CPU/inc" -I"drivers/inc" -I"uC-CPU/inc" -I"uC-LIB/inc" -I"uCOS-III/inc" -MMD -MF "${OBJECTDIR}/drivers/src/driver_i2c.o.d" -o ${OBJECTDIR}/drivers/src/driver_i2c.o drivers/src/driver_i2c.cpp  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/QuadController.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CPPC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PIC32MXSK=1 -mprocessor=$(MP_PROCESSOR_OPTION) -nostartfiles -o dist/${CND_CONF}/${IMAGE_TYPE}/QuadController.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}              -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PIC32MXSK=1,--defsym=_min_heap_size=128,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/QuadController.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CPPC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -nostartfiles -o dist/${CND_CONF}/${IMAGE_TYPE}/QuadController.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=128,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/QuadController.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
