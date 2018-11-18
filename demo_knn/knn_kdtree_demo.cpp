//
// Created by root on 7/2/18.
//

#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <iterator>
#include <chrono>
#include "opencv2/core/core_c.h"


using namespace std;
using namespace cv;
using namespace cv::ml;

using cv::Ptr;
using cv::ml::StatModel;
using cv::ml::TrainData;
using cv::ml::NormalBayesClassifier;
using cv::ml::SVM;
using cv::ml::KNearest;
using cv::ml::ParamGrid;
using cv::ml::ANN_MLP;
using cv::ml::DTrees;
using cv::ml::Boost;
using cv::ml::RTrees;


void defaultDistribs( Mat& means, vector<Mat>& covs, int type=CV_32FC1 )
{
//    CV_TRACE_FUNCTION();
    float mp0[] = {0.0f, 0.0f}, cp0[] = {0.67f, 0.0f, 0.0f, 0.67f};
    float mp1[] = {5.0f, 0.0f}, cp1[] = {1.0f, 0.0f, 0.0f, 1.0f};
    float mp2[] = {1.0f, 5.0f}, cp2[] = {1.0f, 0.0f, 0.0f, 1.0f};
    means.create(3, 2, type);
    Mat m0( 1, 2, CV_32FC1, mp0 ), c0( 2, 2, CV_32FC1, cp0 );
    Mat m1( 1, 2, CV_32FC1, mp1 ), c1( 2, 2, CV_32FC1, cp1 );
    Mat m2( 1, 2, CV_32FC1, mp2 ), c2( 2, 2, CV_32FC1, cp2 );
    means.resize(3), covs.resize(3);

    Mat mr0 = means.row(0);
    m0.convertTo(mr0, type);
    c0.convertTo(covs[0], type);

    Mat mr1 = means.row(1);
    m1.convertTo(mr1, type);
    c1.convertTo(covs[1], type);

    Mat mr2 = means.row(2);
    m2.convertTo(mr2, type);
    c2.convertTo(covs[2], type);
}

// generate points sets by normal distributions
void generateData( Mat& data, Mat& labels, const vector<int>& sizes,
                   const Mat& _means, const vector<Mat>& covs, int dataType, int labelType )
{
//    CV_TRACE_FUNCTION();
    vector<int>::const_iterator sit = sizes.begin();
    int total = 0;
    for( ; sit != sizes.end(); ++sit )
        total += *sit;
    CV_Assert( _means.rows == (int)sizes.size() && covs.size() == sizes.size() );
    CV_Assert( !data.empty() && data.rows == total );
    CV_Assert( data.type() == dataType );

    labels.create( data.rows, 1, labelType );

    randn( data, Scalar::all(-1.0), Scalar::all(1.0) );
    vector<Mat> means(sizes.size());
    for(int i = 0; i < _means.rows; i++)
        means[i] = _means.row(i);
    vector<Mat>::const_iterator mit = means.begin(), cit = covs.begin();
    int bi, ei = 0;
    sit = sizes.begin();
    for( int p = 0, l = 0; sit != sizes.end(); ++sit, ++mit, ++cit, l++ )
    {
        bi = ei;
        ei = bi + *sit;
        assert( mit->rows == 1 && mit->cols == data.cols );
        assert( cit->rows == data.cols && cit->cols == data.cols );
        for( int i = bi; i < ei; i++, p++ )
        {
            Mat r = data.row(i);
            r =  r * (*cit) + *mit;
            if( labelType == CV_32FC1 )
                labels.at<float>(p, 0) = (float)l;
            else if( labelType == CV_32SC1 )
                labels.at<int>(p, 0) = l;
            else
            {
                CV_DbgAssert(0);
            }
        }
    }
}

int maxIdx( const vector<int>& count )
{
    int idx = -1;
    int maxVal = -1;
    vector<int>::const_iterator it = count.begin();
    for( int i = 0; it != count.end(); ++it, i++ )
    {
        if( *it > maxVal)
        {
            maxVal = *it;
            idx = i;
        }
    }
    assert( idx >= 0);
    return idx;
}

bool getLabelsMap( const Mat& labels, const vector<int>& sizes, vector<int>& labelsMap,
                   bool checkClusterUniq=true )
{
    size_t total = 0, nclusters = sizes.size();
    for(size_t i = 0; i < sizes.size(); i++)
        total += sizes[i];

    assert( !labels.empty() );
    assert( labels.total() == total && (labels.cols == 1 || labels.rows == 1));
    assert( labels.type() == CV_32SC1 || labels.type() == CV_32FC1 );

    bool isFlt = labels.type() == CV_32FC1;

    labelsMap.resize(nclusters);

    vector<bool> buzy(nclusters, false);
    int startIndex = 0;
    for( size_t clusterIndex = 0; clusterIndex < sizes.size(); clusterIndex++ )
    {
        vector<int> count( nclusters, 0 );
        for( int i = startIndex; i < startIndex + sizes[clusterIndex]; i++)
        {
            int lbl = isFlt ? (int)labels.at<float>(i) : labels.at<int>(i);
            CV_Assert(lbl < (int)nclusters);
            count[lbl]++;
            CV_Assert(count[lbl] < (int)total);
        }
        startIndex += sizes[clusterIndex];

        int cls = maxIdx( count );
        CV_Assert( !checkClusterUniq || !buzy[cls] );

        labelsMap[clusterIndex] = cls;

        buzy[cls] = true;
    }

    if(checkClusterUniq)
    {
        for(size_t i = 0; i < buzy.size(); i++)
            if(!buzy[i])
                return false;
    }

    return true;
}

bool calcErr( const Mat& labels, const Mat& origLabels, const vector<int>& sizes,
              float& err, bool labelsEquivalent = true, bool checkClusterUniq=true )
{
    err = 0;
    CV_Assert( !labels.empty() && !origLabels.empty() );
    CV_Assert( labels.rows == 1 || labels.cols == 1 );
    CV_Assert( origLabels.rows == 1 || origLabels.cols == 1 );
    CV_Assert( labels.total() == origLabels.total() );
    CV_Assert( labels.type() == CV_32SC1 || labels.type() == CV_32FC1 );
    CV_Assert( origLabels.type() == labels.type() );

    vector<int> labelsMap;
    bool isFlt = labels.type() == CV_32FC1;
    if( !labelsEquivalent )
    {
        if( !getLabelsMap( labels, sizes, labelsMap, checkClusterUniq ) )
            return false;

        for( int i = 0; i < labels.rows; i++ )
            if( isFlt )
                err += labels.at<float>(i) != labelsMap[(int)origLabels.at<float>(i)] ? 1.f : 0.f;
            else
                err += labels.at<int>(i) != labelsMap[origLabels.at<int>(i)] ? 1.f : 0.f;
    }
    else
    {
        for( int i = 0; i < labels.rows; i++ )
            if( isFlt )
                err += labels.at<float>(i) != origLabels.at<float>(i) ? 1.f : 0.f;
            else
                err += labels.at<int>(i) != origLabels.at<int>(i) ? 1.f : 0.f;
    }
    err /= (float)labels.rows;
    return true;
}


/////////////////////////////////////////// K-Nearest Neighbor(KNN) //////////////////////////////////////
int test_opencv_knn_predict()
{
    const int K{ 3 };
    cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create();
    knn->setDefaultK(K);
    knn->setIsClassifier(true);
    knn->setAlgorithmType(cv::ml::KNearest::BRUTE_FORCE);

    const std::string image_path{"E:/GitCode/NN_Test/data/images/digit/handwriting_0_and_1/"};
    cv::Mat tmp = imread(image_path + "0_1.jpg", 0);

    const int train_samples_number{ 40 }, predict_samples_number{ 40 };
    const int every_class_number{ 10 };

    cv::Mat train_data(train_samples_number, tmp.rows * tmp.cols, CV_32FC1);
    cv::Mat train_labels(train_samples_number, 1, CV_32FC1);
    float* p = (float*)train_labels.data;
    for (int i = 0; i < 4; ++i) {
        std::for_each(p + i * every_class_number, p + (i + 1)*every_class_number, [i](float& v){v = (float)i; });
    }

    // train data
    for (int i = 0; i < 4; ++i) {
        static const std::vector<std::string> digit{ "0_", "1_", "2_", "3_" };
        static const std::string suffix{ ".jpg" };

        for (int j = 1; j <= every_class_number; ++j) {
            std::string image_name = image_path + digit[i] + std::to_string(j) + suffix;
            cv::Mat image = cv::imread(image_name, 0);
//            CHECK(!image.empty() && image.isContinuous());
            image.convertTo(image, CV_32FC1);

            image = image.reshape(0, 1);
            tmp = train_data.rowRange(i * every_class_number + j - 1, i * every_class_number + j);
            image.copyTo(tmp);
        }
    }

    knn->train(train_data, cv::ml::ROW_SAMPLE, train_labels);

    // predict datta
    cv::Mat predict_data(predict_samples_number, tmp.rows * tmp.cols, CV_32FC1);
    for (int i = 0; i < 4; ++i) {
        static const std::vector<std::string> digit{ "0_", "1_", "2_", "3_" };
        static const std::string suffix{ ".jpg" };

        for (int j = 11; j <= every_class_number+10; ++j) {
            std::string image_name = image_path + digit[i] + std::to_string(j) + suffix;
            cv::Mat image = cv::imread(image_name, 0);
//            CHECK(!image.empty() && image.isContinuous());
            image.convertTo(image, CV_32FC1);

            image = image.reshape(0, 1);
            tmp = predict_data.rowRange(i * every_class_number + j - 10 - 1, i * every_class_number + j - 10);
            image.copyTo(tmp);
        }
    }

    cv::Mat result;
    knn->findNearest(predict_data, K, result);
//    CHECK(result.rows == predict_samples_number);

    cv::Mat predict_labels(predict_samples_number, 1, CV_32FC1);
    p = (float*)predict_labels.data;
    for (int i = 0; i < 4; ++i) {
        std::for_each(p + i * every_class_number, p + (i + 1)*every_class_number, [i](float& v){v = (float)i; });
    }

    int count{ 0 };
    for (int i = 0; i < predict_samples_number; ++i) {
        float value1 = ((float*)predict_labels.data)[i];
        float value2 = ((float*)result.data)[i];
        fprintf(stdout, "expected value: %f, actual value: %f\n", value1, value2);

        if (int(value1) == int(value2)) ++count;
    }
    fprintf(stdout, "when K = %d, accuracy: %f\n", K, count * 1.f / predict_samples_number);

    return 0;
}


inline uint32_t hash_str_uint32(const std::string& str) {

    uint32_t hash = 0x811c9dc5;
    uint32_t prime = 0x1000193;

    for(int i = 0; i < str.size(); ++i) {
        uint8_t value = str[i];
        hash = hash ^ value;
        hash *= prime;
    }

    return hash;
}

inline int hash_str_int(const std::string& str) {

    uint32_t hash = 0x811c9dc5;
    uint32_t prime = 0x1000193;

    for(int i = 0; i < str.size(); ++i) {
        uint8_t value = str[i];
        hash = hash ^ value;
        hash *= prime;
    }

    if(hash & 0x80000000){
        return hash - 0x80000000;
    } else{
        return static_cast<int>(hash);
    }

}


//ostringstream 用于执行C风格字符串的输出操作
void ostringstream_test()
{
    //ostringstream 只支持 << 操作符
    std::ostringstream oss;
    oss << "this is test" << 123456;

    std::cout << oss.str() << "\r\n" << std::endl;  // this is test123456

    oss.str("");//清空之前的内容
    //oss.clear();//并不能清空内存

    //浮点数转换限制
    float tmp = 0.1234567e-2;
    oss.precision(6);
    oss.setf(std::ios::fixed);//将浮点数的位数限定为小数点之后的位数
    oss << tmp;

    std::cout << oss.str() << "\r\n" << std::endl;  // 0.001235
}

//istringstream 用于执行C风格字符串的输入操作
void istringstream_test() {
    //istringstream 只支持 >> 操作符
    std::string str = "welcome to china";
    std::istringstream iss(str);

    //把字符串中以空格隔开的内容提取出来
    std::string out;
    while (iss >> out) {
        std::cout << out << std::endl;
    }
    std::cout << "\r\n" << std::endl;

}


//stringstream 同时支持C风格字符串的输入输出操作
void stringstream_test()
{
    //输入
    std::stringstream ss;
    ss << "hello this is kandy " << 123;
    std::cout << ss.str() << "\r\n" << std::endl;
    // hello this is kandy 123

    //输出
    std::string out;
    while(ss >> out)
    {
        std::cout << out.c_str() << std::endl;
    }
//    hello
//    this
//    is
//    kandy
//    123
    std::cout << "\r\n" << std::endl;
}

//void test_knn(const std::string& path, std::vector<>){
//
//}

class LabelInfo{
    private:
        // id 即是label(opencv 中knn只支持整数型label)
        int id;
        uint16_t cls;
        string name;

    public:
        enum CLS_TYPE{
            NORMAL_TYPE = 0,             //正常
            POLITICIAN_TYPE = 1,         //政治家
            POLITICAL_CRIMINAL_TYPE = 2  //中国大陆摄政任务
        };

        LabelInfo(int id, uint16_t cls, const string &label) : id(id), cls(cls), name(label) {}

        friend std::ostream& operator <<(std::ostream& os, const LabelInfo& li){
            os<<"id: "<<li.id<<", cls: "<<li.cls<<", name: "<<li.name<<std::endl;
            return os;
        }

        int getId() const {
                return id;
        }

        void setId(int id) {
            LabelInfo::id = id;
        }

        uint16_t getCls() const {
            return cls;
        }

        void setCls(uint16_t cls) {
            LabelInfo::cls = cls;
        }

        const string &getName() const {
            return name;
        }

        void setName(const string &name) {
            LabelInfo::name = name;
        }

        bool b_political() const{
            return cls != CLS_TYPE::NORMAL_TYPE;
        }

};


std::unordered_map<int, std::string> mapIdName;
std::unordered_map<int, LabelInfo> mapLabelInfo;

//
const size_t FEATURE_DIM = 512;
const float DIST_THRESHOLD = 1.1;


void init_id_label(const std::string& path, Mat& trainLabels){

    ifstream fin(path);
    if(!fin.is_open()) {
        std::cerr << "fatal error, label txt not exists" << std::endl;
        exit(-1);
    }

    string sLine;

    while (getline(fin, sLine)){

        istringstream iss(sLine);
        int id;
        uint16_t cls;
        string label, nouse_name;

        iss >> id >> label >> nouse_name >> cls;

        mapLabelInfo.emplace(id, LabelInfo(id, cls, label));
        trainLabels.push_back(id);
    }

//    for (std::pair<int, string> ele: mapLabelInfo)
//    for (const auto& ele: mapLabelInfo)
//        if(ele.second.b_political()){
//            cout<< ele.first << " :: "<< ele.second<<endl;
//        }
}

void init_features(const std::string& path, Mat& trainFeatures){
    std::ifstream fin(path);

    if(!fin.is_open()){
        std::cerr<< "fatal error, feature txt not exists"<<std::endl;
        exit(-1);
    }

    std::string sLine;
    float f_arr[FEATURE_DIM];
    float f_tmp;

    while(getline(fin, sLine)){
        istringstream iss(sLine);
        int i = 0;
        while(iss >> f_tmp)
            f_arr[i++] = f_tmp;

        trainFeatures.push_back(Mat_<float>(1, FEATURE_DIM, f_arr));
    }
}

void test_knn(){

    Ptr<KNearest> knearest = KNearest::create();
    Mat trainFeatures, trainLabels;

    std::string pathFeature = "/home/xy/face_det_code/features_512/features_512_dim.txt";
    init_features(pathFeature, trainFeatures);

    std::string pathLabel = "/home/xy/face_det_code/features_512/id_label_class.txt";
    init_id_label(pathLabel, trainLabels);

    Mat testData;
    std::string testPath = "/home/xy/face_det_code/features_512/feature_tmp.txt";
    init_features(testPath, testData);

    cout << "trainFeatures (python)  = " << endl << format(trainFeatures.rowRange(0, 3), Formatter::FMT_PYTHON) << endl << endl;

    cout << "trainLabels (python)  = " << endl << format(trainLabels.rowRange(0, 3), Formatter::FMT_PYTHON) << endl << endl;

    knearest->train(trainFeatures, ml::ROW_SAMPLE, trainLabels);

    Mat retBestLabels, retLabels;
    Mat retDist;

    std::chrono::steady_clock::time_point beg = std::chrono::steady_clock::now();

//    for(int i = 0;i < 1000;++i)
        knearest->findNearest(testData.rowRange(0, 1), 1, retBestLabels, retLabels, retDist);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout <<"findNearest time consume= " << std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count() <<std::endl;

    cout << "===============retBestLabels===================="<< endl;
    cout << "retBestLabels (python)  = " << endl << format(retBestLabels, Formatter::FMT_PYTHON)<< endl << endl;

    cout << "===============retLabels===================="<< endl;
    cout << "retLabels (python)  = " << endl << format(retLabels, Formatter::FMT_PYTHON)<< endl << endl;

    cout << "===============retDist===================="<< endl;
    cout << "retDist (python)  = " << endl << format(retDist, Formatter::FMT_PYTHON)<< endl << endl;

}

void write_txt1(){
    std::vector<float> vecf{1.1, 1.2};

    std::ofstream fout("vector.txt");
    fout.precision(10);

    for(auto const& x : vecf)
        fout << x << '\n';
}

void ostream_test(){
    vector<double> dVec{1.2, 1.3, 4.7};
    std::ostream_iterator<double> out_it (std::cout, ", ");
    std::copy(dVec.begin(), dVec.end(), out_it);
    // 1.2, 1.3, 4.7,

    std::ofstream fout("vector.txt");
    std::copy(dVec.begin(), dVec.end(),
              std::ostream_iterator<double>(fout, "\n"));
}

void istream_test(){
    std::istringstream iss("welcome to china");

    std::istream_iterator<string> in_it(iss);
    std::istream_iterator<string> in_it_eos; // end-of-stream iterator

    std::ostream_iterator<string> out_it (std::cout, ", ");

    std::copy(in_it, in_it_eos, out_it);
    // welcome, to, china,
}


int main(){
    size_t st = 555;
    printf("%s", std::to_string(st).c_str());


//    test_knn();

//    istream_test();
//    ostream_test();

//    test_knn();

//    std::hash<std::string> hashStr;
//    std::cout<<hashStr("hello world")<<std::endl;

//    int a = 3582672807 ^ 0x811c9dc5;
//    int a = 0x811c9dc5 ^ 0x80000000;
//    std::cout<<a<<std::endl;
//    ostringstream_test();

//    istringstream_test();
//
//    stringstream_test();

//    std::cout<<hash_str_uint32("hello world")<<std::endl;  // 3582672807
//
//    std::cout<<hash_str_uint32("hello world")<<std::endl;  // 3582672807
//    std::cout<<hash_str_uint32("hello world1")<<std::endl; // 3224705314
//    std::cout<<hash_str_uint32("hello world1hello world1hello world1")<<std::endl; // 1442491638
//
//    std::cout<<hash_str_uint32("12345")<<std::endl; // 1136836824
//
//
//    std::cout<<hash_str_int("hello world")<<std::endl;  // 3582672807
//    std::cout<<hash_str_int("hello world")<<std::endl;  // 3582672807
//    std::cout<<hash_str_int("hello world1")<<std::endl; // 3224705314
//    std::cout<<hash_str_int("hello world1hello world1hello world1")<<std::endl; // 1442491638
//
//    std::cout<<hash_str_int("12345")<<std::endl; // 1136836824

    return 0;
/*
    int sizesArr[] = { 500, 700, 800 };
    int pointsCount = sizesArr[0]+ sizesArr[1] + sizesArr[2];
    // train data
    Mat trainData( pointsCount, 2, CV_32FC1 ), trainLabels;

    vector<int> sizes( sizesArr, sizesArr + sizeof(sizesArr) / sizeof(sizesArr[0]) );
    // sizes -> {500, 700, 800}

    Mat means;
    vector<Mat> covs;
    defaultDistribs( means, covs );
    generateData( trainData, trainLabels, sizes, means, covs, CV_32FC1, CV_32FC1 );

    cout << "trainData row: 0~5 (python)  = " << endl << format(trainData.rowRange(0, 5), Formatter::FMT_PYTHON) << endl << endl;
//    cout << "trainData row: 0~5 = "<< endl << " "  << trainData.rowRange(0, 5) << endl << endl;

    // test data
    Mat testData( pointsCount, 2, CV_32FC1 ), testLabels, bestLabels;
    generateData( testData, testLabels, sizes, means, covs, CV_32FC1, CV_32FC1 );

    cout << "testLabels row: 0~5 (python)  = " << endl << format(testLabels.rowRange(0, 5), Formatter::FMT_PYTHON) << endl << endl;
//    cout << "testLabels row: 0~5 = "<< endl << " "  << testLabels.rowRange(0, 5) << endl << endl;

    // KNearest default implementation
    Ptr<KNearest> knearest = KNearest::create();
    knearest->train(trainData, ml::ROW_SAMPLE, trainLabels);
    knearest->findNearest(testData, 4, bestLabels);

    cout << "bestLabels row: 0~5 = "<< endl << " "  << bestLabels.rowRange(0, 5) << endl << endl;

    float err;
    if( !calcErr( bestLabels, testLabels, sizes, err, true ) )
    {
        std::cout<<"Bad output labels.\n"<<std::endl;
    }
    else if( err > 0.01f )
    {
        std::cout<<"Bad accuracy (%f) on test data.\n"<<std::endl;
    }

    // KNearest KDTree implementation
    Ptr<KNearest> knearestKdt = KNearest::create();
//    knearestKdt->setAlgorithmType(KNearest::KDTREE);
    knearestKdt->train(trainData, ml::ROW_SAMPLE, trainLabels);
    knearestKdt->findNearest(testData, 4, bestLabels);

    cout << "bestLabels row: 0~5 = "<< endl << " "  << bestLabels.rowRange(0, 5) << endl << endl;

    if( !calcErr( bestLabels, testLabels, sizes, err, true ) )
    {
        std::cout<<"Bad output labels.\n"<<std::endl;
    }
    else if( err > 0.01f )
    {
        std::cout<<"Bad accuracy (%f) on test data.\n"<<std::endl;
    }
*/

    Ptr<KNearest> knn = KNearest::create();
//    knnKdt->setAlgorithmType(KNearest::KDTREE);
    knn->setIsClassifier(true);

    {

        float d_train[10][2] = {{1, 1}, {1.1, 1.1}, {1.2, 1.2},
                                {2, 2}, {2.1, 2.1}, {2.2, 2.2},
                                {3, 3}, {3.1, 3.1}, {3.2, 3.2}, {3.2, 3.2}
        };

        float d_label[10][1] = {{1}, {1}, {1}, {2}, {2}, {2}, {3}, {3}, {3}, {3}};

        Mat trainData = Mat(10, 2, CV_32FC1, &d_train), trainLabels = Mat(10, 1, CV_32FC1, &d_label);
//        Mat trainData = Mat(10, 2, CV_64F, &d_train), trainLabels = Mat(10, 1, CV_64F, &d_label);

        knn->train(trainData, ml::ROW_SAMPLE, trainLabels);

        cout << "===============trainData===================="<< endl;
        cout << "trainData (python)  = " << endl << format(trainData, Formatter::FMT_PYTHON) << endl << endl;

        cout << "===============trainLabels===================="<< endl;
        cout << "trainLabels (python)  = " << endl << format(trainLabels, Formatter::FMT_PYTHON)<< endl << endl;
    }

//    Mat testData( 2, 2, CV_32FC1 ), yTestLabels(2, 1, CV_32FC1), retBestLabels(2, 1, CV_32FC1);
//    float d_test[3][2] = {{1.3, 1.3}, {2.2, 2.4}, {8, 8}};
    float d_test[1][2] = {{1.3, 1.3}};

//    Mat testData(3, 2, CV_32FC1, &d_test);
    Mat testData(1, 2, CV_32FC1, &d_test);

    Mat retBestLabels;
    Mat retLabels, retDist;

    cout << "===============testData===================="<< endl;
    cout << "testData (python)  = " << endl << format(testData, Formatter::FMT_PYTHON)<< endl << endl;

//    knn->findNearest(testData, 3, retBestLabels,retLabels, retDist);
    knn->findNearest(testData, 1, retBestLabels,retLabels, retDist);

    cout << "===============retBestLabels===================="<< endl;
    cout << "retBestLabels (python)  = " << endl << format(retBestLabels, Formatter::FMT_PYTHON)<< endl << endl;

    cout<<"retBestLabels size: "<<retBestLabels.rows<<" "<<retBestLabels.cols<<endl;
    cout<<"retBestLabels (0, 0): "<<retBestLabels.at<float>(0, 0)<<" "<<endl;

//    retBestLabels (python)  =
//    [1,
//     2,
//     3]

//    cout << "===============retLabels===================="<< endl;
//    cout << "retLabels (python)  = " << endl << format(retLabels, Formatter::FMT_PYTHON)<< endl << endl;

//    retLabels (python)  =
//    [[1, 1, 1],
//    [2, 2, 2],
//    [3, 3, 3]]


//    cout << "===============retDist===================="<< endl;
//    cout << "retDist (python)  = " << endl << format(retDist, Formatter::FMT_PYTHON)<< endl << endl;
//    retDist (python)  =
//    [[0.019999962, 0.079999946, 0.17999995],
//    [0.040000018, 0.10000014, 0.20000009],
//    [46.080002, 46.080002, 48.02]]

//    cout<<"retDist size: "<<retDist.rows<<" "<<retDist.cols<<endl;
//    cout<<"v (0, 0): "<<retDist.at<float>(0, 0)<<" "<<endl;

    return 0;
}

