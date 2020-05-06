#ifndef MYSQL_H
#define MYSQL_H
#include <iostream>
#include <string>
#include <string.h>
#include <mysql/mysql.h>
#include "packages/inc/arcsoft_face_sdk.h"
#include "packages/inc/amcomdef.h"
#include "packages/inc/asvloffscreen.h"
#include <iostream>
#include <map>
#include <vector>
//#include "person.h"
#include "person02.h"

using namespace std;
class MysqlDB {
private:
    MYSQL mysql;
    MYSQL_ROW row;
    MYSQL_RES *result;
    MYSQL_FIELD *field;
public:
    MysqlDB();
    ~MysqlDB();
    void connect( string host, string user, string passwd,  string database );
    void add(ASF_FaceFeature feature,string name);
    void addBigData(ASF_FaceFeature feature,string name);
    void print();
    vector<ASF_FaceFeature> getAllFeatures();
    vector<ASF_FaceFeature> getAllFeaturesByBigData();
    map<string,ASF_FaceFeature> getAllFeaturesByBigDataByMap();
    map<person,ASF_FaceFeature> getAllFeatureByBigDataMap02();
    string findNameByFeature(ASF_FaceFeature feature);
    string findNameByFeature02(char *feature);

};

#endif // MYSQL_H
