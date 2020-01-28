/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "ipeWrite.h"
#include "basicIo.h"
#include "basicDefs.h"

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
FILE *IWopenFile(char *fileName)
{
   FILE *ipeFile;  
   
   if ((ipeFile = FopenWrite(fileName)) != NULL)  {
      fprintf(ipeFile, "<?xml version=\"1.0\"?>\n<!DOCTYPE ipe SYSTEM \"ipe.dtd\">\n<ipe version=\"70206\" creator=\"RPG\">\n<info bbox=\"cropbox\"/>\n<ipestyle name=\"RPGstyle\">\n<symbol name=\"arrow/arc(spx)\">\n<path stroke=\"sym-stroke\" fill=\"sym-stroke\" pen=\"sym-pen\">\n0 0 m\n-1 0.333 l\n-1 -0.333 l\nh\n</path>\n</symbol>\n<symbol name=\"arrow/farc(spx)\">\n<path stroke=\"sym-stroke\" fill=\"white\" pen=\"sym-pen\">\n0 0 m\n-1 0.333 l\n-1 -0.333 l\nh\n</path>\n</symbol>\n<symbol name=\"arrow/ptarc(spx)\">\n<path stroke=\"sym-stroke\" fill=\"sym-stroke\" pen=\"sym-pen\">\n0 0 m\n-1 0.333 l\n-0.8 0 l\n-1 -0.333 l\nh\n</path>\n</symbol>\n<symbol name=\"arrow/fptarc(spx)\">\n<path stroke=\"sym-stroke\" fill=\"white\" pen=\"sym-pen\">\n0 0 m\n-1 0.333 l\n-0.8 0 l\n-1 -0.333 l\nh\n</path>\n</symbol>\n<symbol name=\"mark/circle(sx)\" transformations=\"translations\">\n<path fill=\"sym-stroke\">\n0.6 0 0 0.6 0 0 e\n0.4 0 0 0.4 0 0 e\n</path>\n</symbol>\n<symbol name=\"mark/disk(sx)\" transformations=\"translations\">\n<path fill=\"sym-stroke\">\n0.6 0 0 0.6 0 0 e\n</path>\n</symbol>\n<symbol name=\"mark/fdisk(sfx)\" transformations=\"translations\">\n<group>\n<path fill=\"sym-fill\">\n0.5 0 0 0.5 0 0 e\n</path>\n<path fill=\"sym-stroke\" fillrule=\"eofill\">\n0.6 0 0 0.6 0 0 e\n0.4 0 0 0.4 0 0 e\n</path>\n</group>\n</symbol>\n<symbol name=\"mark/box(sx)\" transformations=\"translations\">\n<path fill=\"sym-stroke\" fillrule=\"eofill\">\n-0.6 -0.6 m\n0.6 -0.6 l\n0.6 0.6 l\n-0.6 0.6 l\nh\n-0.4 -0.4 m\n0.4 -0.4 l\n0.4 0.4 l\n-0.4 0.4 l\nh\n</path>\n</symbol>\n<symbol name=\"mark/square(sx)\" transformations=\"translations\">\n<path fill=\"sym-stroke\">\n-0.6 -0.6 m\n0.6 -0.6 l\n0.6 0.6 l\n-0.6 0.6 l\nh\n</path>\n</symbol>\n<symbol name=\"mark/fsquare(sfx)\" transformations=\"translations\">\n<group>\n<path fill=\"sym-fill\">\n-0.5 -0.5 m\n0.5 -0.5 l\n0.5 0.5 l\n-0.5 0.5 l\nh\n</path>\n<path fill=\"sym-stroke\" fillrule=\"eofill\">\n-0.6 -0.6 m\n0.6 -0.6 l\n0.6 0.6 l\n-0.6 0.6 l\nh\n-0.4 -0.4 m\n0.4 -0.4 l\n0.4 0.4 l\n-0.4 0.4 l\nh\n</path>\n</group>\n</symbol>\n<symbol name=\"mark/cross(sx)\" transformations=\"translations\">\n<group>\n<path fill=\"sym-stroke\">\n-0.43 -0.57 m\n0.57 0.43 l\n0.43 0.57 l\n-0.57 -0.43 l\nh\n</path>\n<path fill=\"sym-stroke\">\n-0.43 0.57 m\n0.57 -0.43 l\n0.43 -0.57 l\n-0.57 0.43 l\nh\n</path>\n</group>\n</symbol>\n<symbol name=\"arrow/fnormal(spx)\">\n<path stroke=\"sym-stroke\" fill=\"white\" pen=\"sym-pen\">\n0 0 m\n-1 0.333 l\n-1 -0.333 l\nh\n</path>\n</symbol>\n<symbol name=\"arrow/pointed(spx)\">\n<path stroke=\"sym-stroke\" fill=\"sym-stroke\" pen=\"sym-pen\">\n0 0 m\n-1 0.333 l\n-0.8 0 l\n-1 -0.333 l\nh\n</path>\n</symbol>\n<symbol name=\"arrow/fpointed(spx)\">\n<path stroke=\"sym-stroke\" fill=\"white\" pen=\"sym-pen\">\n0 0 m\n-1 0.333 l\n-0.8 0 l\n-1 -0.333 l\nh\n</path>\n</symbol>\n<symbol name=\"arrow/linear(spx)\">\n<path stroke=\"sym-stroke\" pen=\"sym-pen\">\n-1 0.333 m\n0 0 l\n-1 -0.333 l\n</path>\n</symbol>\n<symbol name=\"arrow/fdouble(spx)\">\n<path stroke=\"sym-stroke\" fill=\"white\" pen=\"sym-pen\">\n0 0 m\n-1 0.333 l\n-1 -0.333 l\nh\n-1 0 m\n-2 0.333 l\n-2 -0.333 l\nh\n</path>\n</symbol>\n<symbol name=\"arrow/double(spx)\">\n<path stroke=\"sym-stroke\" fill=\"sym-stroke\" pen=\"sym-pen\">\n0 0 m\n-1 0.333 l\n-1 -0.333 l\nh\n-1 0 m\n-2 0.333 l\n-2 -0.333 l\nh\n</path>\n</symbol>\n<pen name=\"heavier\" value=\"0.8\"/>\n<pen name=\"fat\" value=\"1.2\"/>\n<pen name=\"ultrafat\" value=\"2\"/>\n<symbolsize name=\"large\" value=\"5\"/>\n<symbolsize name=\"small\" value=\"2\"/>\n<symbolsize name=\"tiny\" value=\"1.1\"/>\n<arrowsize name=\"large\" value=\"10\"/>\n<arrowsize name=\"small\" value=\"5\"/>\n<arrowsize name=\"tiny\" value=\"3\"/>\n<color name=\"red\" value=\"1 0 0\"/>\n<color name=\"green\" value=\"0 1 0\"/>\n<color name=\"blue\" value=\"0 0 1\"/>\n<color name=\"yellow\" value=\"1 1 0\"/>\n<color name=\"orange\" value=\"1 0.647 0\"/>\n<color name=\"gold\" value=\"1 0.843 0\"/>\n<color name=\"purple\" value=\"0.627 0.125 0.941\"/>\n<color name=\"gray\" value=\"0.745\"/>\n<color name=\"brown\" value=\"0.647 0.165 0.165\"/>\n<color name=\"navy\" value=\"0 0 0.502\"/>\n<color name=\"pink\" value=\"1 0.753 0.796\"/>\n<color name=\"seagreen\" value=\"0.18 0.545 0.341\"/>\n<color name=\"turquoise\" value=\"0.251 0.878 0.816\"/>\n<color name=\"violet\" value=\"0.933 0.51 0.933\"/>\n<color name=\"darkblue\" value=\"0 0 0.545\"/>\n<color name=\"darkcyan\" value=\"0 0.545 0.545\"/>\n<color name=\"darkgray\" value=\"0.663\"/>\n<color name=\"darkgreen\" value=\"0 0.392 0\"/>\n<color name=\"darkmagenta\" value=\"0.545 0 0.545\"/>\n<color name=\"darkorange\" value=\"1 0.549 0\"/>\n<color name=\"darkred\" value=\"0.545 0 0\"/>\n<color name=\"lightblue\" value=\"0.678 0.847 0.902\"/>\n<color name=\"lightcyan\" value=\"0.878 1 1\"/>\n<color name=\"lightgray\" value=\"0.827\"/>\n<color name=\"lightgreen\" value=\"0.565 0.933 0.565\"/>\n<color name=\"lightyellow\" value=\"1 1 0.878\"/>\n<dashstyle name=\"dashed\" value=\"[4] 0\"/>\n<dashstyle name=\"dotted\" value=\"[1 3] 0\"/>\n<dashstyle name=\"dash dotted\" value=\"[4 2 1 2] 0\"/>\n<dashstyle name=\"dash dot dotted\" value=\"[4 2 1 2 1 2] 0\"/>\n<textsize name=\"large\" value=\"\\large\"/>\n<textsize name=\"Large\" value=\"\\Large\"/>\n<textsize name=\"LARGE\" value=\"\\LARGE\"/>\n<textsize name=\"huge\" value=\"\\huge\"/>\n<textsize name=\"Huge\" value=\"\\Huge\"/>\n<textsize name=\"small\" value=\"\\small\"/>\n<textsize name=\"footnote\" value=\"\\footnotesize\"/>\n<textsize name=\"tiny\" value=\"\\tiny\"/>\n<textstyle name=\"center\" begin=\"\\begin{center}\" end=\"\\end{center}\"/>\n<textstyle name=\"itemize\" begin=\"\\begin{itemize}\" end=\"\\end{itemize}\"/>\n<textstyle name=\"item\" begin=\"\\begin{itemize}\\item{}\" end=\"\\end{itemize}\"/>\n<gridsize name=\"4 pts\" value=\"4\"/>\n<gridsize name=\"8 pts (~3 mm)\" value=\"8\"/>\n<gridsize name=\"16 pts (~6 mm)\" value=\"16\"/>\n<gridsize name=\"32 pts (~12 mm)\" value=\"32\"/>\n<gridsize name=\"10 pts (~3.5 mm)\" value=\"10\"/>\n<gridsize name=\"20 pts (~7 mm)\" value=\"20\"/>\n<gridsize name=\"14 pts (~5 mm)\" value=\"14\"/>\n<gridsize name=\"28 pts (~10 mm)\" value=\"28\"/>\n<gridsize name=\"56 pts (~20 mm)\" value=\"56\"/>\n<anglesize name=\"90 deg\" value=\"90\"/>\n<anglesize name=\"60 deg\" value=\"60\"/>\n<anglesize name=\"45 deg\" value=\"45\"/>\n<anglesize name=\"30 deg\" value=\"30\"/>\n<anglesize name=\"22.5 deg\" value=\"22.5\"/>\n<opacity name=\"10%%\" value=\"0.1\"/>\n<opacity name=\"30%%\" value=\"0.3\"/>\n<opacity name=\"50%%\" value=\"0.5\"/>\n<opacity name=\"75%%\" value=\"0.75\"/>\n<tiling name=\"falling\" angle=\"-60\" step=\"4\" width=\"1\"/>\n<tiling name=\"rising\" angle=\"30\" step=\"4\" width=\"1\"/>\n</ipestyle>\n\n");  
      fprintf(ipeFile, "<page>\n");  
      fprintf(ipeFile, "<layer name=\"polygon\"/>\n");  
      fprintf(ipeFile, "<layer name=\"vertices\"/>\n");  
      fprintf(ipeFile, "<view layers=\"polygon vertices\" active=\"polygon\"/>\n");  
   }
   
   return(ipeFile);  
}

void IWwriteCoord(FILE *ipeFile, t_point aPoint)
{
  double scale;  
  double x, y;  

  scale = MIN((X_MAX-X_MIN), (Y_MAX-Y_MIN));  

  x = IPE_MIN_X + (aPoint.x - X_MIN)*(IPE_MAX_X-IPE_MIN_X)/scale;  
  y = IPE_MIN_Y + (aPoint.y - Y_MIN)*(IPE_MAX_Y-IPE_MIN_Y)/scale;  

  fprintf(ipeFile, "%f %f", x, y);  
}


void IWwritePoint(FILE *ipeFile, t_point aPoint)
{
   static int first = 0;  
   double scale;  
   double x, y;  
   
   scale = MIN((X_MAX-X_MIN), (Y_MAX-Y_MIN));  
   
   x = IPE_MIN_X + (aPoint.x - X_MIN)*(IPE_MAX_X-IPE_MIN_X)/scale;  
   y = IPE_MIN_Y + (aPoint.y - Y_MIN)*(IPE_MAX_Y-IPE_MIN_Y)/scale;  
   if (first == 0) 
      fprintf(ipeFile, "<use layer=\"vertices\" name=\"mark/disk(sx)\" pos=\"%f %f\" size=\"normal\" stroke=\"black\"/>\n", x, y);  
   else 
      fprintf(ipeFile, "<use name=\"mark/disk(sx)\" pos=\"%f %f\" size=\"normal\" stroke=\"black\"/>\n", x, y);  
   first = 1;  
}

void IWwritePoints(FILE *ipeFile, t_pointArray *pArray)
{
   int count;  
   
   for (count=1;  count <= PAnrOfPoints(pArray);  count++)  {
      IWwritePoint(ipeFile, PAgetPoint(pArray, count));  
   }
}


void IWwritePoly(FILE *ipeFile, t_polygon *aPoly, t_pointArray *pArray)
{
   int first;  
   int count;  
   
   first = TRUE;  
   
   if (BPsizeOf(aPoly) > 0)  {
      first = TRUE;  
      
      /* begin the line */
      fprintf(ipeFile, "<path layer=\"polygon\" stroke=\"seagreen\">\n");  

      /* write all the points */
      for (count=1;  count <= BPsizeOf(aPoly);  count++) 	{
         IWwriteCoord(ipeFile, PAgetPoint(pArray, BPgetPIndex(aPoly, count)));  
         if (first)	    {
            fprintf(ipeFile, " m\n");  
            first = FALSE;  
         }
         else
            fprintf(ipeFile, " l\n");  
      }
      
      /* close the line */
      fprintf(ipeFile, "h\n");  
      fprintf(ipeFile, "</path>\n");  
   }
}

void IWcloseFile(FILE *ipeFile)
{
  fprintf(ipeFile, "</page>\n");  
  fprintf(ipeFile, "</ipe>\n");  
  
  Fclose(ipeFile);  
}




