#include <iostream>
#include <iomanip>
#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <fstream>
#include <vector>
#include <sstream>


std::string filePath;

std::string nowDay()
{
    time_t t = time(nullptr);
    //change format
    const tm* lt = localtime(&t);

    //sに独自フォーマットになるように連結していく
    std::stringstream s;
    s<<"20";
    s<<lt->tm_year-100; //100を引くことで20xxのxxの部分になる
    s<<"-";
    s<<lt->tm_mon+1; //月を0からカウントしているため
    s<<"-";
    s<<lt->tm_mday;
    s<<"-";
    s<<lt->tm_hour;
    s<<"-";
    s<<lt->tm_min;
    s<<"-";
    s<<lt->tm_sec;
    std::string result = s.str();

    return result;
}

void callback(const std_msgs::Float32MultiArray& message)
{
    static std::ofstream ofs(filePath.c_str());
    for(const auto& data : message.data){
        ofs << data << ",";
    }
    ofs << std::endl;

}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "CSVout");
    ros::NodeHandle nh;
    ros::NodeHandle pn("~");
    pn.getParam("filePath", filePath);
    ros::Subscriber sub = nh.subscribe("sensorValue", 10, callback);
    filePath = filePath + nowDay() + ".csv";

    while(ros::ok())
    {
        ros::spin();
    }
    return 0;
}