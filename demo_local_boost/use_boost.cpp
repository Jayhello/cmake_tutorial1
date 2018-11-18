//
// Created by root on 8/4/18.
//

#include <boost/version.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

void print_version(){
    std::cout << "using Boost version: "
              << BOOST_VERSION / 100000
              << "."
              << BOOST_VERSION / 100 % 1000
              << "."
              << BOOST_VERSION % 100
              << std::endl;
}

void print_date(){
    using namespace boost::gregorian;

    std::string s("2001-10-9"); //2001-October-09
    date d(from_simple_string(s));
    std::cout << to_simple_string(d) << std::endl;
}

void print_time(){
    using namespace boost::posix_time;
    using namespace boost::gregorian;

    //get the current time from the clock -- one second resolution
    ptime now = second_clock::local_time();
    //Get the date part out of the time
    date today = now.date();
    date tommorrow = today + days(1);
    ptime tommorrow_start(tommorrow); //midnight

    //iterator adds by one hour
    time_iterator titr(now, hours(1));
    for (; titr < tommorrow_start; ++titr) {
        std::cout << to_simple_string(*titr) << std::endl;
    }

    time_duration remaining = tommorrow_start - now;
    std::cout << "Time left till midnight: "
              << to_simple_string(remaining) << std::endl;
}


int main(){
    print_version();
    print_time();

    return 0;
}