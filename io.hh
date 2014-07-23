#ifndef IO_HH
#define IO_HH
// 
// #include <iostream>
// 
// class io 
// {
//   protected:
//   	int numberPeaksToFit;
//   public:
//     io();
//     virtual ~io() { std::cout<<"destructing"<<std::endl; };
//   	void SetNoPks(int noPks);
//   	//int GetNoPks();
//   	void GetNoPks(int noPks);
// };

extern int noPks;

class io
{
  public:
	void SetNoPks(int nP);
};
#endif
