#define BOOST_TEST_MODULE handlerTests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "handlers.h"
#include "data.h"

BOOST_AUTO_TEST_CASE(testDateManagement){
    Handlers::Date *d=new Handlers::Date(7,5,1992);
    //check that can create objects of Handlers::Date
    BOOST_TEST(d!=nullptr);

    //check that days like 30/2/YYYY never are allowed to exist
    delete d;
    d=nullptr;
    try {
        d=new Handlers::Date(30,2,2009);
    } catch (std::exception &ex) {std::cerr<<"ERROR: "<<ex.what()<<"\n";}
    BOOST_TEST(d==nullptr);
    //test that 31/4/YYYY will never happen
    try {
        d=new Handlers::Date(31,4,2009);
    } catch (std::exception &ex) {std::cerr<<"ERROR: "<<ex.what()<<"\n";}
    BOOST_TEST(d==nullptr);

    //test if two objects can equal the same
    d=new Handlers::Date;
    auto d2=new Handlers::Date;
    auto ans=*d==*d2;
    BOOST_TEST(ans==true);
    delete d2; delete d;

    //next test for saving to a DB
    Handlers::ShDate d3(new Handlers::Date(7,5,1992));
    //make sure not yet saved
    BOOST_TEST(d3->getPID()<=-1);
    d3->save();
    //now make sure it's above 0 or 0
    BOOST_TEST(d3->getPID()>=0);

    //now change the date of d3 to something else
    d3->setYear(2009);
    //get id
    int id=d3->getPID();
    //save update
    d3->save();
    //check that the id's still match
    BOOST_TEST(id==d3->getPID());


    //now delete the object from database
    d3->unSave();
    //id should now equal -1 since object deleted
    BOOST_TEST(d3->getPID()==-1);
}

BOOST_AUTO_TEST_CASE(testingVideoTypes){
    Handlers::ShVideoType v1(new Handlers::VideoType("mkv"));
    Handlers::ShVideoType v2(new Handlers::VideoType());
    Handlers::ShVideoType v3(new Handlers::VideoType("mp4"));
    Handlers::ShVideoType v4(new Handlers::VideoType("mp4"));

    bool result=false;
    //make sure they don't match
    result=*v4==*v1;
    BOOST_TEST(result==false);

    result=*v4.get()==*v3.get();
    //test that v4 and v3 are actually equal
    BOOST_TEST(result);

    //save v4 and make sure it's pid > 0
    BOOST_TEST(v4->getPID()<0);
    v4->save();
    BOOST_TEST(v4->getPID()>=0);

    //and then go ahead and remove element
    v4->unSave();
    //pid should now be less than 0
    BOOST_TEST(v4->getPID()<0);
}

BOOST_AUTO_TEST_CASE(testingGettingFiles){
   Handlers::SharedFilesVector files=  Handlers::getFilesInDir();
   //test no null object passed
   BOOST_TEST(files!=nullptr);
   //test that size of files in directory is greater than 1
   BOOST_TEST(files->size()>0);
}

BOOST_AUTO_TEST_CASE(testAddingListToPlayerHandler){
    Handlers::PlayerHandler player;
    BOOST_TEST(player.getSupportedVideoTypes().size()==3);

    //add item
    player.addVideoType("mkv");
    BOOST_TEST(player.getSupportedVideoTypes().size()==4);
}
