#include "mysql.h"


#define SafeFree(p) { if ((p)) free(p); (p) = NULL; }
#define SafeArrayDelete(p) { if ((p)) delete [] (p); (p) = NULL; }
#define SafeDelete(p) { if ((p)) delete (p); (p) = NULL; }

MysqlDB::MysqlDB(){
    if( mysql_init( &mysql ) == NULL ) {
        //cout << "init error, line: " << __LINE__ << endl;
        exit(-1);
    }else{

        cout << "Init Success" << endl;
    }
}

MysqlDB::~MysqlDB(){
    mysql_close( &mysql );
}



void MysqlDB::connect(string host, string user, string passwd, string database){

    if( !mysql_real_connect(&mysql,"localhost","root","root","mysql",3306,
                             NULL,0) ) {  //test 127.0.0.1  "root" opencv_base  3306
                //cout << "connect error, line: " << __LINE__ << endl;
                //exit(-1);
        mysql_error(&mysql);
        mysql_errno(&mysql);

        cout << mysql_errno(&mysql) << __LINE__ << endl;

        cout << mysql_error(&mysql) << __LINE__ << endl;

        cout << "connect error, line" << __LINE__ << endl;

    }else{

        cout << "Connect Success" << endl;
    }
}


void MysqlDB::add(ASF_FaceFeature feature, string name){

    string face_feature= (char *)&feature.feature;
    printf("face_feature=%s\n",face_feature.c_str());
    int feature_size=feature.featureSize;
    string feature_size_string=to_string(feature_size);

    string sql = "insert into opencv_asfort(`feature`,`feature_size`,`name`) values('"+face_feature+"','"+feature_size_string+"', '"+name+"')";
    mysql_query(&mysql,sql.c_str());
}


void MysqlDB::addBigData(ASF_FaceFeature feature, string name){

    int res;
    unsigned int escape_size = (2 * 1032 + 2);
    char* escape_object = (char *)malloc(escape_size);
    escape_size = mysql_real_escape_string(&mysql, (char*)escape_object, (char *)feature.feature, 1032);//1032
    char buf[2000] = { NULL };

    int sql_len = sprintf(buf, "insert into opencv_asfort(`feature`,`feature_size`,`name`) values('%s',%d,'%s')", escape_object,feature.featureSize,name.c_str());
    res = mysql_real_query(&mysql,buf,sql_len);
    cout<< res <<endl;


        //std::cout << "Error: query failed, sql: " << str_sqls.c_str() << std::endl;
        std::cout << mysql_error(&mysql) << std::endl;

    // CR_COMMANDS_OUT_OF_SYNC//命令以一个不适当的次序被执行。
    // CR_SERVER_GONE_ERROR//：MySQL服务器关闭了。
    // CR_SERVER_LOST//：对服务器的连接在查询期间失去。
    // CR_UNKNOWN_ERROR//：发生一个未知的错误。
}



vector<ASF_FaceFeature> MysqlDB::getAllFeatures(){

    vector<ASF_FaceFeature> features;
    string sql = "select feature,feature_size,name from opencv_asfort;";
    mysql_query(&mysql, sql.c_str() );
    result = mysql_store_result( &mysql );
    if( !result ) {
           cout << "result error, line : "<< endl;
    }

    while(row=mysql_fetch_row(result)){

        string face_feature=row[0];
        int feature_size=atoi(row[1]);
        printf("feature=%s\n",face_feature.c_str());
        printf("feature_size=%d\n",feature_size);
        ASF_FaceFeature asf_feature;
        asf_feature.feature=(unsigned char *)face_feature.c_str();
        asf_feature.featureSize=feature_size;
        features.push_back(asf_feature);
    }

    return features;
}


vector<ASF_FaceFeature> MysqlDB::getAllFeaturesByBigData(){

    vector<ASF_FaceFeature> features;
    ASF_FaceFeature asf_feature;
    unsigned long *row_len;
    unsigned int size = 1032;
    char *object = NULL;
    char sql[100] = { NULL };
    unsigned long objsize;
    int ret;
    sprintf(sql, "select feature,feature_size,name from opencv_asfort;");
    ret = mysql_real_query(&mysql, sql, strlen(sql));
    result = mysql_store_result( &mysql );
    while (row= mysql_fetch_row(result)){

        row_len = mysql_fetch_lengths(result);
        //printf("long =%ld\n",row_len[0]);
        string size_string=row[1];
        //int size=atoi(row[1]);
        asf_feature.feature = (MByte *)malloc(row_len[0]);
        if(asf_feature.feature==NULL){

            printf("malloc error\n");
            exit(1);
        }
        asf_feature.featureSize=row_len[0];
        memset(asf_feature.feature, 0,row_len[0]);
        memcpy(asf_feature.feature,row[0],row_len[0]);
        features.push_back(asf_feature);
    }
    //free(asf_feature.feature);
    return features;
}

map<string,ASF_FaceFeature> MysqlDB::getAllFeaturesByBigDataByMap(){

    map<string,ASF_FaceFeature> map;
    ASF_FaceFeature asf_feature;
    unsigned long *row_len;
    char sql[100] = { NULL };
    int ret;
    sprintf(sql, "select feature,feature_size,name from opencv_asfort;");
    cout << "mysql_real_query Success" << endl;
    ret = mysql_real_query(&mysql, sql, strlen(sql));
    cout << "mysql_store_result Success" << endl;
    result = mysql_store_result( &mysql );
    cout << "mysql_fetch_row Success" << endl;
    while (row= mysql_fetch_row(result)){
        cout << "mysql_fetch_lengths Success" << endl;
        row_len = mysql_fetch_lengths(result);
        string size_string=row[1];
        asf_feature.feature = (MByte *)malloc(row_len[0]);
        if(asf_feature.feature==NULL){

            printf("malloc error\n");
            exit(1);
        }

        asf_feature.featureSize=row_len[0];
        memset(asf_feature.feature, 0,row_len[0]);
        memcpy(asf_feature.feature,row[0],row_len[0]);
        //
        string name=row[2];
        map.insert(pair<string,ASF_FaceFeature>(name,asf_feature));
    }

    SafeFree(asf_feature.feature);
    SafeDelete(asf_feature.feature);

    return map;
}

map<person,ASF_FaceFeature> MysqlDB::getAllFeatureByBigDataMap02(){

    //Person person;
    person p;
    map<person,ASF_FaceFeature> map;
    ASF_FaceFeature asf_feature;
    unsigned long *row_len;
    char sql[100] = { NULL };
    int ret;
    sprintf(sql, "select feature,feature_size,name from opencv_asfort;");
    ret = mysql_real_query(&mysql, sql, strlen(sql));
    result = mysql_store_result( &mysql );
    while (row= mysql_fetch_row(result)){

        row_len = mysql_fetch_lengths(result);
        string size_string=row[1];
        asf_feature.feature = (MByte *)malloc(row_len[0]);
        if(asf_feature.feature==NULL){

            printf("malloc error\n");
            exit(1);
        }

        asf_feature.featureSize=row_len[0];
        memset(asf_feature.feature, 0,row_len[0]);
        memcpy(asf_feature.feature,row[0],row_len[0]);
        //
        string name=row[2];
        //person.setName(name);
        person p1(name,20);
        //p.setName(name);
        //p.setAge(20);
        cout << "maps name" << endl; 
        cout << name << endl; 

        map.insert(pair<person,ASF_FaceFeature>(p1,asf_feature));
    }

    SafeFree(asf_feature.feature);
    SafeDelete(asf_feature.feature);
    return map;
}



string MysqlDB::findNameByFeature(ASF_FaceFeature feature){

    //char buf[2000] = { NULL };
    int ret;
    string name;
    unsigned int escape_size = 2 * 1032 + 2;
    char* escape_object = (char *)malloc(escape_size);
    escape_size = mysql_real_escape_string(&mysql, (char*)escape_object, (char *)feature.feature, 1032);
    char buf[2000] = { NULL };
    int sql_len = sprintf(buf, "select name from opencv_asfort where feature='%s'",feature.feature);
    ret = mysql_real_query(&mysql,buf,strlen(buf));
    result = mysql_store_result( &mysql );
    while (row= mysql_fetch_row(result)) {

        name=row[0];
    }
    return name;
}

string MysqlDB::findNameByFeature02(char *feature){

    //printf("feature=%s\n)
    string name;
    int ret;
    unsigned long *row_len;
    unsigned int escape_size = 2 * 1032 + 2;
    char* escape_object = (char *)malloc(escape_size);
    escape_size = mysql_real_escape_string(&mysql, (char *)escape_object, feature, 1100);

    char buf[2000] = { NULL };
    int len=sprintf(buf, "select name from opencv_asfort where feature='%s'",escape_object);
    mysql_real_query(&mysql,buf,len);
    result = mysql_store_result(&mysql);
    while (row= mysql_fetch_row(result)) {

        name=row[0];
    }
    return name;

}





