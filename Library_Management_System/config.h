#pragma once

namespace Config {

    //����ʵ�����Գ��ȣ�ע�� extern const ������Ϊ���鳤��
    #define LEN_BOOK_ISBN 14
    #define LEN_BOOK_NAME 100
    #define LEN_BOOK_AUTHOR 100
    #define LEN_BOOK_PUBLISHER 100
    #define LEN_USER_NAME 100
    #define LEN_USER_PASSWORD 40
    #define LEN_USER_TYPE 10
    #define LEN_USER_INFO 100
    
    //�û�����
    extern const bool ADMIN_USER;
    extern const bool NORMAL_USER;

    //�û�Ȩ��
    struct USER_ACCESS {
        bool SEARCH_BOOK;       ///����ͼ��
        bool CHANGE_PASSWORD;   ///�޸�����
        bool ADD_USER;          ///����û�
        bool DELETE_USER;       ///ɾ���û�
        bool RESET_PASSWORD;    ///�����û�����
        bool BOLLOW_BOOK;       ///����ͼ��
        bool RETURN_BOOK;       ///�黹ͼ��
        bool RENEW_BOOK;        ///����ͼ��
        bool BOLLOW_HISTORY;    ///�鿴������ʷ
    };
    extern const USER_ACCESS ADMIN_ACCESS;
    extern const USER_ACCESS NORMAL_ACCESS;

    //��������
    extern const int MAX_BOLLOW_TIME;

    //��������
    extern const int MAX_BOLLOW_NUM;

    //���ݴ洢�ļ����ڵ�Ŀ¼
    extern const char* DATASTORE_PATH;

    //���ݴ洢�ļ���ȡ���棬ÿ�ζ�ȡ�ļ�¼����
    extern const int FILE_READ_CACHE_COUNT;

    // �Ƿ�Ϊ���԰汾
    extern const bool DEBUG;
}
