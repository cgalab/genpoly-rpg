/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "elapsed.h"

#include <stdio.h>

/*                                                                           */
/* Note: CPUTIME_IN_MILLISECONDS can be defined in the Imakefile by using    */
/*       flag "-DCPUTIME_IN_MILLISECONDS".                                   */
/*                                                                           */
#ifdef CPUTIME_IN_MILLISECONDS
#include <sys/resource.h>
#ifndef RUSAGE_CHILDREN
#undef CPUTIME_IN_MILLISECONDS
#endif
#endif



#ifdef CPUTIME_IN_MILLISECONDS 

/*                                                                           */
/* This procedure computes the CPU time used by the calling process since    */
/* the last call. (The sum of both system and user time is reported.)        */
/* Call it before and after the execution of some instructions of your       */
/* program, whose CPU consumption you would like to know. The first value    */
/* returned can be ignored, and the second value gives the actual CPU        */
/* consumption in the form milliseconds.microseconds. (I.e., 12345 microsecs */
/* are returned as 12.345 millisecs.)                                        */
/*                                                                           */
double elapsed()
{
   double cpu_time;  
   unsigned long new_secs, secs, new_usecs, usecs, new_add_secs;  
   static unsigned long total_secs = 0, total_usecs = 0;  
   int getrusage(int who, struct rusage *rusage);  
   struct rusage rusage_self, rusage_children;  

   getrusage(RUSAGE_SELF, &rusage_self);  
   getrusage(RUSAGE_CHILDREN, &rusage_children);  

   new_secs  = rusage_self.ru_utime.tv_sec +
               rusage_self.ru_stime.tv_sec +
               rusage_children.ru_utime.tv_sec +
               rusage_children.ru_stime.tv_sec;  
   new_usecs = rusage_self.ru_utime.tv_usec +
               rusage_self.ru_stime.tv_usec +
               rusage_children.ru_utime.tv_usec +
               rusage_children.ru_stime.tv_usec;  

   if (new_usecs >= 1000000) {
      new_add_secs = new_usecs / 1000000;  
      new_usecs -= new_add_secs * 1000000;  
      new_secs  += new_add_secs;  
   }

   if (total_usecs > new_usecs) {
      secs  = new_secs - total_secs - 1;  
      usecs = new_usecs - total_usecs + 1000000;  
   }
   else {
      secs  = new_secs - total_secs;  
      usecs = new_usecs - total_usecs;  
   }
   
   total_secs  = new_secs;  
   total_usecs = new_usecs;  

   cpu_time = 1.0e3 * (double) secs + 1.0e-3 * (double) usecs;  

   return cpu_time;  
}




#else /* ifndef CPUTIME_IN_MILLISECONDS */

#ifdef CPUTIME_VIA_CLOCK

#include <time.h>
#include "martin.h"


static clock_t total_cpu_time;  
#define CLOCK_UNITS 1000.0        /* this could be figured out automatically */

/*                                                                           */
/* This procedure computes the CPU time used by the calling process since    */
/* the last call. (The sum of both system and user time is reported.)        */
/* Call it before and after the execution of some instructions of your       */
/* program, whose CPU consumption you would like to know. The first value    */
/* returned can be ignored, and the second value gives the actual CPU        */
/* consumption in the form milliseconds.microseconds. (I.e., 12345 microsecs */
/* are returned as 12.345 millisecs.)                                        */
/*                                                                           */
double elapsed()
{
   double cpu_time;  
   clock_t finish;  
   static boolean initialized = false;  

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC sysconf(_SC_CLK_TCK)
#endif

   if (!initialized) {
     total_cpu_time = clock();  
     initialized = true;  
   }
   finish = clock();  

   cpu_time = ((double) (finish - total_cpu_time)) * 
      (CLOCK_UNITS / CLOCKS_PER_SEC);  
   total_cpu_time = finish;  

   return cpu_time;  
}
 




#else /* ifndef CPUTIME_IN_MILLISECONDS  ||  CPUTIME_VIA_CLOCK */





#include <sys/types.h>
#include <sys/times.h>
#include <unistd.h>

/*                                                                           */
/* This procedure computes the CPU time used by the calling process since    */
/* the last call. (The sum of both system and user time is reported.)        */
/* Call it before and after the execution of some instructions of your       */
/* program, whose CPU consumption you would like to know. The first value    */
/* returned can be ignored, and the second value gives the actual CPU        */
/* consumption (in milliseconds).                                            */
/*                                                                           */
double elapsed()
{
   static long total = 0;  
   long dummy;  
   double cpu_time;  
   struct tms buffer;  
   static int HZ = 0;  

   if (HZ == 0)  HZ = sysconf(_SC_CLK_TCK);  
 
   times(&buffer);  
   dummy    = buffer.tms_utime  + buffer.tms_stime + 
              buffer.tms_cutime + buffer.tms_cstime;  
   cpu_time = (double) ((dummy - total) * 1000.0) / HZ;  
   total    = dummy;  

   return cpu_time;  
}





#endif
#endif

