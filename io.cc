// #include "io.hh"
// #include <iostream>
// 
// io::io(){};
// 
// void io::SetNoPks(int noPks){
//   numberPeaksToFit = noPks;
//   std::cout<<"Set to:"<<numberPeaksToFit<<std::endl;
// }
// 
// /*int io::GetNoPks(){
//   return noPks;
// }*/
// void io::GetNoPks(int noPks){
//   noPks = numberPeaksToFit;
//   std::cout<<"Getting: "<<noPks<<std::endl;
// }

#include "io.hh"
#include <iostream>

//int noPks = 5;

void io::SetNoPks(int nP){
   int noPks = nP;
   std::cout<<"Set to:"<<noPks<<std::endl;
 }

