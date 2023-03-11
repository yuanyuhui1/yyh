SET INDENT_OPTIONS=-bad -bap -bli0 -i4 -l99 -ncs -npcs -nut -npsl -nfca -lc99 -nfc1 -c1 -ss

cls

indent %INDENT_OPTIONS% interface/t_data_obj_alarm.h
indent %INDENT_OPTIONS% source/t_data_obj_alarm.c

indent %INDENT_OPTIONS% interface/t_data_obj_alarm_counter.h
indent %INDENT_OPTIONS% source/t_data_obj_alarm_counter.c

indent %INDENT_OPTIONS% interface/diagnosis.h
indent %INDENT_OPTIONS% interface/diagnosis_idx.h
indent %INDENT_OPTIONS% interface/DiagnosisCompileTimeDefinitions.h

indent %INDENT_OPTIONS% source/diagnosis_execute.c