
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
import sys

def build(project, build_target, log_file):
    build_result = os.system('"' + os.environ.get("IAR_EW_M16C_LEGACY_PATH") + '\IarBuild.exe" ' + project + ' -build ' + build_target + ' >> ' + log_file)
    build_result = 0
    if build_result is 0:
        return True
    else:
        return False

if __name__ == "__main__":
    if True == os.path.exists("./scripts/automation/tmp"):
        shutil.rmtree("./scripts/automation/tmp")
    os.mkdir("./scripts/automation/tmp")

    if None != os.environ.get("BUILD_RELEASE"):
        build_result = build("cb_bootloader/Bootloader.ewp", "Release", "./scripts/automation/tmp/build_log.txt")
        build_result += build('VT5_SOFTWARE.ewp', 'Release', "./scripts/automation/tmp/build_log.txt")
    else:
        build_result = build('VT5_SOFTWARE.ewp', os.environ.get("BUILD_TARGET"), "./scripts/automation/tmp/build_log.txt")
        build_result += build('VT5_SOFTWARE.ewp', 'Release', "./scripts/automation/tmp/build_log.txt")

    if build_result:
        sys.exit(0)
    else:
        sys.exit(1)
