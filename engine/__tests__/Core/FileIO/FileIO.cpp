#include <gtest/gtest.h>
#include "FileIO.hpp"
#include "Resource/Resource.hpp"
using namespace ez;

class Actor 
{
public:
    int i;
    std::string _name;
};

TEST(FILEIO, InputChar) {
    std::cout <<" ENTERN"<<std::endl;
    // FileIO rp;
    // Actor a ;
    // a._name=  "ula";
    //rp.saveBinaryResource("var.dat", &actor);
   
    //std::string path("var.dat");
    // std::cout <<" ENTddddERN"<<std::endl;

    // rp.saveResource(path,&a);
    // std::cout <<" ENTddERN"<<std::endl;

    // void *b = rp.loadResource(path);
    // std::cout << "b" << std::endl;
    //std::cout << "b:" << b << std::endl;
    //std::cout << "b.name" << b->_name << std::endl;
	//EXPECT_EQ(b->_name,"ula");
}