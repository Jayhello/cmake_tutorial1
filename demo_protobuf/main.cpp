//
// Created by root on 7/27/18.
//

#include <iostream>
#include <fstream>
#include "proto/message.pb.h"
#include <functional>
#include <google/protobuf/text_format.h>

using namespace std;
using namespace google;

using namespace message;

void test_example(std::function<void(void)> func){
    cout<<"----------now test ";
    func();
    cout<<endl;
}


void test_basic();

void test_serialize();

void test_repeat_field();

void test_refelect();

// for caffe test
void test_blob();

int main(int argc, char const *argv[])
{
    test_example(test_basic);

    test_example(test_serialize);

    test_example(test_repeat_field);

    return 0;
}

void test_repeat_field(){

    cout << " function : " << __func__ << endl;

    Family family;
    FamilyMember* member;

    // 添加一个家庭成员，John
    member = family.add_member();
    member->set_age(25);
    member->set_name("John");

    // 添加一个家庭成员，Lucy
    member = family.add_member();
    member->set_age(23);
    member->set_name("Lucy");

    // 添加一个家庭成员，Tony
    member = family.add_member();
    member->set_age(2);
    member->set_name("Tony");

    // 显示所有家庭成员
    int size = family.member_size();

    cout << "这个家庭有 " << size << " 个成员，如下：" << endl;

    for(int i=0; i<size; i++)
    {
        FamilyMember mb = family.member(i);
        cout << i+1 << ". " << mb.name() << ", 年龄 " << mb.age() << endl;
    }

    std::string str;
    cout<<member->DebugString()<<endl;
    //TODO bug
//    google::protobuf::TextFormat::PrintToString(family, &str);

}

void test_refelect(){

    cout << " function : " << __func__ << endl;

    message::Message m;
    m.add_id(5);
    m.set_id(0, 3);

    std::vector<int> vecInt{1, 5, 8};
    google::protobuf::RepeatedField<int> data(vecInt.begin(), vecInt.end());
    m.mutable_id()->Swap(&data);

    //other method
    //*m.mutable_id() = {vecInt.begin(), vecInt.end()};

    const protobuf::Descriptor *desc       = m.GetDescriptor();
    const protobuf::Reflection *refl       = m.GetReflection();

    int fieldCount= desc->field_count();

    fprintf(stderr, "The fullname of the message is %s \n", desc->full_name().c_str());
    for(int i=0;i<fieldCount;i++)
    {
        const protobuf::FieldDescriptor *field = desc->field(i);
        fprintf(stderr, "The name of the %i th element is %s and the type is  %s \n",i,field->name().c_str(),field->type_name());
    }

}

void test_basic(){
    cout << " function : " << __func__ << endl;

    message::SearchRequest sr;
    sr.set_query("jay");
    sr.set_page_number(2);
    sr.set_result_per_page(5);

    printf("set query: %s, pg: %d, per: %d",
           sr.query().c_str(), sr.page_number(), sr.result_per_page());

    string encode_str;
    sr.SerializeToString(&encode_str);
//    cout<<encode_str<<std::endl;

    for (auto s:encode_str)cout<<s;
    cout<<encode_str.size()<<endl;

    message::SearchRequest sr_back;

    sr_back.ParseFromString(encode_str);

    printf("parse from string query: %s, pg: %d, per: %d \n",
           sr_back.query().c_str(), sr_back.page_number(), sr_back.result_per_page());

}

void test_serialize(){
    cout << " function : " << __func__ << endl;

    Person person;  //类型对象
    person.set_id(101);
    person.set_name("_name小熊_");
    person.set_email("email@qq.com");

    cout << "person is "<<person.id() << person.name() << person.email() << endl;

    string encode_str;
    person.SerializeToString(&encode_str);  //序列化到字符串
    cout << "the encode string is "<<encode_str << encode_str.size()<<endl;

    //输出到文件
    string file_name("prototest.txt");
    fstream output(file_name, ios::out | ios::trunc | ios::binary);
    if (!person.SerializeToOstream(&output)) {
        cerr << "Failed to write person." << endl;
    }
    output.close();


    //从字符串来反序列化
    Person person2;
    person2.ParseFromString(encode_str);
    cout << "person2 is "<<person2.id() << ends << person2.name() << ends << person2.email() << endl;


    //从文件流来反序列化
    fstream input(file_name, ios::in | ios::binary);

    Person person3;
    if (!person3.ParseFromIstream(&input)) {
        cerr << "Failed to read address book." << endl;
    }
    cout << "person3 is " << person3.id() << ends << person3.name() << ends << person3.email() << endl;

    Person no_exist;
//    cout << person3.SpaceUsed()<<endl;
    cout << no_exist.SpaceUsed()<<endl;

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

}

void test_blob(){
    BlobProto proto;
    vector<int> shape{1, 2, 3, 4};
    for (int i = 0; i < shape.size(); ++i) {
        proto.mutable_shape()->add_dim(shape[i]);
    }

}