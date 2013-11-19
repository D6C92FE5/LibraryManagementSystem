
#include "config.h"

namespace Config {

    const bool ADMIN_USER = true;
    const bool NORMAL_USER = false;

    const USER_ACCESS ADMIN_ACCESS = {
        true,   // SEARCH_BOOK;     ///����ͼ��
        true,   // CHANGE_PASSWORD; ///�޸�����
        true,   // ADD_USER;        ///����û�
        true,   // DELETE_USER;     ///ɾ���û�
        true,   // RESET_PASSWORD;  ///�����û�����
        true,   // BOLLOW_BOOK;     ///����ͼ��
        true,   // RETURN_BOOK;     ///�黹ͼ��
        true,   // RENEW_BOOK;      ///����ͼ��
        true,   // BOLLOW_HISTORY;  ///�鿴������ʷ
    };

    const USER_ACCESS NORMAL_ACCESS = {
        true,   // SEARCH_BOOK;     ///����ͼ��
        true,   // CHANGE_PASSWORD; ///�޸�����
        false,  // ADD_USER;        ///����û�
        false,  // DELETE_USER;     ///ɾ���û�
        false,  // RESET_PASSWORD;  ///�����û�����
        false,  // BOLLOW_BOOK;     ///����ͼ��
        false,  // RETURN_BOOK;     ///�黹ͼ��
        false,  // RENEW_BOOK;      ///����ͼ��
        true,   // BOLLOW_HISTORY;  ///�鿴������ʷ
    };

    const int MAX_BOLLOW_TIME = 30;
    const int MAX_BOLLOW_NUM = 5;

    const char* DATASTORE_PATH = ".\\Datastore\\";

    const int FILE_READ_CACHE_COUNT = 100;
}
