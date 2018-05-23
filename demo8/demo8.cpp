//
// Created by root on 5/20/18.
//

//#include "demo8.h"
#include <curl/curl.h>
#include <iostream>


int use_curl(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;

}


int main(){
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
//        curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, use_curl);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
//        res = curl_easy_perform(curl);
//        curl_easy_cleanup(curl);

//        std::cout << readBuffer << std::endl;
    }
}