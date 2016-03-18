# AT91SAM7x256-EK platform bootup code in baremetal mode


Platform on bootup enters modsim_main(), after which is assumed

to call ../modsim.c:main() . Due range of reasons the code is left

'as is', i.e. leds DS1-DS4 are left blinking to demonstrate we're

capable of posting 4-bit digital signal to DAC, as soon as the one

is soldered onto AT91SAM7x256-EK physically.



No effort to implement RawData download (via TCP/IP, or serial ln)

is undertaken, as well as no effort to implement timing functions

and rest of necessary (GLIBC/USLIBC) functions was done.



Known issues: whole set of functions defined in 'subst.c' ( such

as: 'malloc(), free(), callo(), ... , sscanf(), gettimeofday()' )

to be implemented prior to following usage of the project on this 

platform. This, btw, seems to be time consuming and the effort 

does not seem to justify the presumed outcome given branch of

project. Will be re-designed and implemented in further versions

os project.



STATUS: DISCONTNIED