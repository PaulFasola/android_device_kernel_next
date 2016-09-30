####################################################################################################
#
####################################################################################################
USER_CCFLAGS :=

####################################################################################################
#Directories for library files 
####################################################################################################
vob_lib_dirs :=

####################################################################################################
#library files
####################################################################################################
vob_lib_files :=

####################################################################################################
#Directories for include files
####################################################################################################
include $(BALONG_SRC_PATH)/modem/oam/build/gu/MODEM_CORE/msp_code_gu_modem_core.inc

####################################################################################################
#Directories for source files
####################################################################################################
vob_src_dirs :=

MSP_MCORE_DIR			:=$(BALONG_SRC_PATH)/modem/oam/comm/comm
MSP_CCORE_DIR  			:=$(BALONG_SRC_PATH)/modem/oam/comm/ccore
PRJ_CONFIG_DIR          :=$(BALONG_SRC_PATH)/modem/config
PRJ_INCLUDE_DIR         :=$(BALONG_SRC_PATH)/modem/include
PRJ_PLATFORM_DIR        :=$(BALONG_SRC_PATH)/modem/platform
####################################################################################################
#Source files
####################################################################################################
vob_src_files :=    $(MSP_CCORE_DIR)/nvim/nvim_gcffunction.c\


			

ifneq ($(CFG_RAT_MODE),RAT_GU)
vob_src_files += \
					$(MSP_MCORE_DIR)/nvim/Nvm_fw_api.c \
					$(MSP_MCORE_DIR)/nvim/Nvm_fw_proc.c
endif
   

####################################################################################################
#general make rules
####################################################################################################
# ifeq ($(PRODUCT_CFG_PRODUCT_VERSION),VERSION_V3R2)
# include $(BALONG_SRC_PATH)\build\Build\MODEM_CORE\Rule\rules.msp.mk
# endif

####################################################################################################
#
####################################################################################################


####################################################################################################
#
####################################################################################################