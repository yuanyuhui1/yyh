#-*-coding:utf-8-*-
from openpyxl import Workbook
from openpyxl import load_workbook
import chardet
import os,sys
import datetime
import operator
import shutil

subsystem_path = "\\ARM\\"
source_file = "Source\\art_hmi.c"
def main():
    UOM_PF_USED_FLAG = 0
    RANGEFLT_USED_FLAG = 0
    T_DIAGNOSIS_ALARM_COUNTER_USED_FLAG = 0
    ADDR_USED_FLAG = 0
    CONF_USED_FLAG = 0
    DEV_RNGFLT_USED_FLAG = 0
    TOTDISP_USED_FLAG = 0
    UNITS_USED_FLAG = 0
    print("|---- Process Start -----|")
    parent_dir = os.getcwd()
    parent_dir = os.path.abspath(os.path.join(parent_dir, "../.."))
    with open(parent_dir+"\\"+subsystem_path+source_file, 'r') as f_ah:
        lines_ah = f_ah.readlines()
        for lineIndex in range(len(lines_ah)):
            tempstr = lines_ah[lineIndex]
            if tempstr.find("//") == -1:
                if tempstr.find("#ifdef FRAMEWORK_VERSION_2_3_0") != -1:
                    lines_ah[lineIndex] = tempstr.replace("FRAMEWORK_VERSION_2_3_0", "FRAMEWORK_VERSION_2_3_1")
                elif tempstr.find("t_data_obj_UOM_PF.h") != -1:
                    if UOM_PF_USED_FLAG == 0:
                        lines_ah[lineIndex] = "//"+tempstr+'#include "..\\..\\T_DATA_OBJ\interface\\t_data_obj_uom_pf.h"\n'
                        UOM_PF_USED_FLAG = 1
                    else:
                        lines_ah[lineIndex] = "//"+tempstr
                elif tempstr.find("t_data_obj_RANGEFLT.h") != -1:
                    if RANGEFLT_USED_FLAG == 0:
                        lines_ah[lineIndex] = "//"+tempstr+'#include "..\\..\\T_DATA_OBJ\interface\\t_data_obj_rngflt.h"\n'
                        RANGEFLT_USED_FLAG = 1
                    else:
                        lines_ah[lineIndex] = "//"+tempstr
                elif tempstr.find("t_data_obj_T_DIAGNOSIS_ALARM_COUNTER.h") != -1:
                    lines_ah[lineIndex] = "//"+tempstr
                elif tempstr.find("t_data_obj_ADDR.h") != -1:
                    lines_ah[lineIndex] = "//"+tempstr
                elif tempstr.find("t_data_obj_CONF.h") != -1:
                    lines_ah[lineIndex] = "//"+tempstr
                elif tempstr.find("t_data_obj_DEV_RNGFLT.h") != -1:
                    if DEV_RNGFLT_USED_FLAG == 0:
                        lines_ah[lineIndex] = "//"+tempstr+'#include "..\\..\\T_DATA_OBJ\interface\\t_data_obj_dev_rngflt.h"\n'
                        DEV_RNGFLT_USED_FLAG = 1
                    else:
                        lines_ah[lineIndex] = "//"+tempstr
                elif tempstr.find("t_data_obj_TOTDISP.h") != -1:
                    lines_ah[lineIndex] = "//"+tempstr
                elif tempstr.find("t_data_obj_UNITS.h") != -1:
                    if UNITS_USED_FLAG == 0:
                        lines_ah[lineIndex] = "//"+tempstr+'#include "..\\..\\T_DATA_OBJ\interface\\t_data_obj_units.h"\n#include "..\\..\\T_DATA_OBJ\interface\\t_data_obj_floatunits.h"\n#include "..\\..\\T_DATA_OBJ\interface\\t_data_obj_float_fixedunit.h"\n'
                        UNITS_USED_FLAG = 1
                    else:
                        lines_ah[lineIndex] = "//"+tempstr
    os.remove(parent_dir+"\\"+subsystem_path+source_file)  # delete old file
    with open(parent_dir+"\\"+subsystem_path+source_file, 'w') as wf:
        for tempi in range(len(lines_ah)):
            wf.write(lines_ah[tempi])
    print("|----- Process End ------|")

if __name__ == '__main__':
    main()