# SW and HW parts for simulation of 3G-/LTE-modems to test SoHO hadware

# Both interfaces to be supperted : USB 1.1, USB 2.0.

# The USB 1.1 to be supported on 'floatless' MC68EZ328-based uCsimm/uClinux, damit the 
# voltage levels are pure TTL, we can't get more out of bare bus of MC68EZ328.

# The USB 2.0 to be supported on AT91SAM7X-based 'baremetal' platform, and the TTLs
# to be formed out from both controllers - PWMC and ADC - of TDMI core.
