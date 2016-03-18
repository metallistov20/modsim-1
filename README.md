# SW and HW parts for simulation of 3G-/LTE-modems to test SoHO hadware


Both interfaces to be supperted : USB 1.1, USB 2.0.

The USB 1.1 to be supported on 'floatless' MC68EZ328-based uCsimm/uClinux,

damit the voltage levels are pure TTL, we can't get more out of bare bus

of MC68EZ328.


The USB 2.0 to be supported on AT91SAM7X-based 'baremetal' platform, and

the TTLs to be formed out from both controllers - PWMC and ADC - of TDMI

core.


Known issues: present desing of HW and SW implies a heavy rework. Firstly,

the proper upgrade of HW sould be finalized so we get real USB2.0 voltage

leevels. Second, the main routine of 'modsim.c' should expose a possibilty

of being integrated into real long-time testing framework. Idea: to split

the testing (of entire set of devices [PLATF]/apps/private/handle_card/modeswitch_conf/support_3g_list )

into as much parallel processes as hardware part of project allows.


STATUS: DISCONTINUED