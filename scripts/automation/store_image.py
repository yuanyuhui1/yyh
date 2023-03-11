
#  <pre>
#  Copyright              Copyright ABB, 2018.
#                         All rights reserved. Reproduction, modification,
#                         use or disclosure to third parties without express
#                         authority is forbidden.
#  System
#  Module
#  Description
#  Remarks
#  </pre>

import os
import shutil
import time
import zlib
import struct

def get_version_num():
    f = open("ElettronicServices/Source/versioncode.h", 'r')
    for line in f:
        if -1 != line.find("SW_REV0"):
            sw_rev0 = line.split()[2]
        elif -1 != line.find("SW_REV1"):
            sw_rev1 = line.split()[2]
        elif -1 != line.find("SW_REV2"):
            sw_rev2 = line.split()[2]
        else:
            pass
    f.close()

    return sw_rev0 + "." + sw_rev1 + "." + sw_rev2

def get_boot_version_num():
    f = open("cb_bootloader/Interface/command.h", 'r')
    for line in f:
        if -1 != line.find("SW_REV0"):
            sw_rev0 = line.split()[2].replace('(0x0','').replace(')','')
        elif -1 != line.find("SW_REV1"):
            sw_rev1 = line.split()[2].replace('(0x0','').replace(')','')
        elif -1 != line.find("SW_REV2"):
            sw_rev2 = line.split()[2].replace('(0x0','').replace(')','')
        else:
            pass
    f.close()

    return sw_rev0 + "." + sw_rev1 + "." + sw_rev2

def get_crc32(filename):
    with open(filename, 'rb') as f:
        return hex(zlib.crc32(f.read())).upper().replace("0X", "")

def combine_boot_app(boot_file_path, app_file_path):
    boot_offset = 0x00010000                      # Start adress of bootloader binary file content in the combined file 
    app_offset = 0x00080000                       # Start adress of CB App binary file content in the combined file 
    boot_entry_offset = 0x000FFFFC
    hardware_ver_offset = 0x0000E000              # Hardware verison adress of CB 
    hardware_version_0 = struct.pack('B', 0x01)   # CB Hardware version
    hardware_version_1 = struct.pack('B', 0x03)   
    hardware_version_2 = struct.pack('B', 0x00)    

    combine_file_size = 0x000FFFFF                 # Combined binary file size.
    default_value = struct.pack('B', 0xFF)         # Default value of combined binary file.Default value is 0xFF
    
    # get bootloader binary file
    boot_file = open(boot_file_path, 'rb')

    # get app binary file
    app_file = open(app_file_path, 'rb')
    # app_file[boot_entry_offset].replace(,struct.pack('B', 0x01)) 

    # create combined binary file
    combined_file_path = "Release/Exe/VT5CB_BOOT_COMBINED.bin"

    if True == os.path.exists(combined_file_path):
        os.remove(combined_file_path)

    combined_file = open(combined_file_path, 'ab')
    # Combine app and bootloader
    offset = 0x00000000
    while offset < combine_file_size:
        if offset == hardware_ver_offset:
            combined_file.write(hardware_version_0)     # write CB hardware version into combined file
            combined_file.write(hardware_version_1)
            combined_file.write(hardware_version_2)

        elif offset == boot_offset:
            bootloader_content = boot_file.read()       # merge bootloader binary file
            combined_file.write(bootloader_content)
            boot_file.close()

        elif offset == app_offset:
            app_content = app_file.read()
            combined_file.write(app_content)
            app_file.close()

        else:
            combined_file.write(default_value)        # fill other Rom by 0xFF

        offset = combined_file.tell()                 # Get current position in the combined file
    combined_file = open(combined_file_path, 'rb')
    combined_file_data = combined_file.read()

    combined_file_release = open(combined_file_path, 'wb')
    combined_file_release.write(combined_file_data)
    combined_file_release.seek(boot_entry_offset)
    combined_file_release.write(b'\x00\x00\x01\xFF')

    combined_file_release.close()

    

    return combined_file_path

def store_image(dest):
    boot_file_path = "cb_bootloader/Release/Exe/VT5_CB_Bootloader.bin"
    app_file_path = "Release/Exe/VT5_SOFTWARE.bin"
    combined_file_path = ""

    # Get combined file path
    combined_file_path = combine_boot_app(boot_file_path, app_file_path)

    if True != os.path.exists(dest):
        os.makedirs(dest)

    if None != os.environ.get("BUILD_RELEASE"):
        shutil.copyfile(app_file_path, dest + "/3KXF065133U0113_" + get_version_num() + "_" + time.strftime("%Y.%m.%d",time.localtime(time.time()))[2:] + "_" + get_crc32(app_file_path) + ".bin")     
        shutil.copyfile(boot_file_path, dest + "/3KXF065134U0113_" + get_boot_version_num() + "_" + time.strftime("%Y.%m.%d",time.localtime(time.time()))[2:] + "_" + get_crc32(boot_file_path) + ".bin")
        shutil.copyfile(combined_file_path, dest + "/3KXF065135U0113_" + get_version_num() + "_" + time.strftime("%Y.%m.%d",time.localtime(time.time()))[2:] + "_" + get_crc32(combined_file_path) + ".bin")
    else:
        build_target = os.environ.get("BUILD_TARGET")
        if "Debug" == build_target:
            shutil.copyfile("Debug/Exe/VT5_SOFTWARE.bin", dest + "/" + os.environ.get("JOB_NAME").split("/")[0] + "_" + os.environ.get("JOB_NAME").split("/")[2] + "_" + build_target + "_" + get_version_num() + "_" + get_crc32("Debug/Exe/VT5_SOFTWARE.bin") +".bin")
        elif "Release" == build_target:
            shutil.copyfile("Release/Exe/VT5_SOFTWARE.bin", dest + "/" + os.environ.get("JOB_NAME").split("/")[0] + "_" + os.environ.get("JOB_NAME").split("/")[2] + "_" + build_target + "_" + get_version_num() + "_"  + get_crc32("Release/Exe/VT5_SOFTWARE.bin") + ".bin")
        else:
            print("Incorrect build target")

if __name__ == "__main__":
    store_image(os.environ.get("IMAGE_STORAGE_PATH") + "\\" +
                os.environ.get("JOB_NAME") + "\\" + "Build_" + "%05d" % int(os.environ.get("BUILD_NUMBER")) + "_" + time.strftime("%Y%m%d%H%M%S",time.localtime(time.time())))
    
