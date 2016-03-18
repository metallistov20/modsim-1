# Script to generate toolchain for ARM7TDMI-based evaluation boards


During run the toolchain is installed to:

    /opt/xGCC-XXX-baremetal ,

where 'XXX' may be 'arm-tplink-eabi'. Thereby before usage (from

regular Linux shell) execute:

    PATH=/opt/xGCC-arm-tplink-eabi-baremetal/arm-tplink-eabi/bin:/opt/xGCC-arm-tplink-eabi-baremetal/bin:$PATH

so the cross compiler ( along with rest of cross toolchain ) appears

on the path, and can be invoked by 'Makefile' with no absolue pathwork.



STATUS: USABLE FOR ARM7 BAREMETALL PROJECTS