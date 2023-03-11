#!/usr/bin/bash

set -aeou
shopt -s nocaseglob

[ "" = "${SHELLFLAGS-}" ] || set $SHELLFLAGS

platform="$(uname)"
if [[ $platform =~ WIN ]]
then
    MKTEMP="mktemp.exe"
else
    MKTEMP="mktemp"
fi

RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m'

declare -a special_headfiles=(t_data_obj_UNITS.h)
#declare -a files2handle=()

error()
{
    #info="${1//\!/\\\!}"
    echo -e "${RED}${1}${NC}" 

}

warning()
{
    echo -e "${YELLOW}${1}${NC}" 

}

clean()
{
    rm $@
}

find_and_replace()
{
    file2handle=$1
    file=${file2handle##*/}
    dir=${file2handle%/*}
    declare -a headfiles=()
    
    (
    cd ../../$dir
    if ls _tt.* >/dev/null;then
        rm _tt.*
    fi
    tempfile1="$($MKTEMP _tt.XXXX)" #..\..\xx\xx\headfile
    tempfile2="$($MKTEMP _tt.XXXX)" #..\..\|headfile
    tempfile3="$($MKTEMP _tt.XXXX)" #remove same record in tempfile2
    cat $file | grep -i t_data_obj_ | grep -i -v 'T_DATA_OBJ\\' | cut -d '"' -f2 >>$tempfile1
	if [[ ! -s "$tempfile1" ]];then
		echo "Nothing needs to do for $file2handle!"
		exit 0
	fi
    sed -i 's,\\,/,g' $tempfile1

    while read line;
    do
        tmpstr=${line##*../}
        let offset=${#line}-${#tmpstr}
        topdir=${line:0:$offset}
        headfile=${line##*/}
        echo ${topdir}\|${headfile} >>$tempfile2
    done < $tempfile1

    cat $tempfile2 | sort -u >>$tempfile3
    readarray -t headfiles <<< "$(cat $tempfile3 | cut -d '|' -f 2)"
    readarray -t topdirs <<< "$(cat $tempfile3 | cut -d '|' -f 1)"
    for ((i=0;i<${#headfiles[@]};i++))
    do
        if ! ls ${topdirs[$i]}/T_DATA_OBJ/interface/${headfiles[$i]} >/dev/null
        then
            error "The headfile ${headfiles[$i]} can not be found under T_DATA_OBJ/interface! we will remove it!"
            echo "The original directory info is:"
            grep -i ${headfiles[$i]} $tempfile1
            #clean $tempfile1 $tempfile2 $tempfile3
            sed -i "/${headfiles[$i]}/d" $tempfile3 

            if [[ "${headfiles[$i],,}" == "t_data_obj_rangeflt.h" ]];then
				if ls ${topdirs[$i]}/T_DATA_OBJ/interface/t_data_obj_rngflt.h 1>&2>/dev/null
				then
					warning "we consider it is t_data_obj_rngflt.h" 
					echo "${topdirs[$i]}|t_data_obj_rngflt.h" >>$tempfile3
				fi
            fi
        fi
    done
    echo file to read is $file
	#The first pattern matches the lines containing 't_data_obj_',  the second pattern deletes lines unless they match 'T_DATA_OBJ'
    sed -i -e '/t_data_obj_/{/T_DATA_OBJ/!d;}' $file

    while read line;
    do
        topdir="${line%%/|*}"
        topdir="${topdir//\//\\\\}"
        headfile="${line##*|}"
		
		if grep -i "T_DATA_OBJ\\\\.*${headfile}" $file >/dev/null
		then
			echo T_DATA_OBJ...$headfile already exists! Ignore it!
			continue
		fi
        echo "inserting $headfile"
        #two times evaluation here. Firstly bash expands the variables when variables are involved by "", and then sed program evaluates the expression again
        sed -i -r -e "/subsystem-specific include files added here/a\#include \"$topdir\\\\T_DATA_OBJ\\\\interface\\\\$headfile\"" $file 
        if [[ "${headfile,,}" == "t_data_obj_units.h" ]]
        then
            sed -i -r -e "/$headfile/a\#include \"$topdir\\\\T_DATA_OBJ\\\\interface\\\\t_data_obj_float_fixedunit.h\"" $file 
            sed -i -r -e "/$headfile/a\#include \"$topdir\\\\T_DATA_OBJ\\\\interface\\\\t_data_obj_floatunits.h\"" $file 
        fi
    done < $tempfile3
    )
}

main()
{
	sed -r -i '/^\s*$/d' HF_To_Correct_CFG.txt #remove empty lines to avoid uncontrolled newline intert by below action
	echo >> HF_To_Correct_CFG.txt #Windows doesn't insert the newline character auotomatically unless user enter 'Enter'. So we need to aovid this.
	
	tr -d '\r'  < HF_To_Correct_CFG.txt | while read files2handle;
	do
		[ ! -z $files2handle ] && find_and_replace $files2handle
	done
}

main
exit 0

#TODO:
#1 use configuration file to support all likewise source files
#2 mapping the wrong head file names to correct ones
