#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <string>
#include <list>
#include <direct.h>

#include "config.h"

namespace Datastore {

    // ͼ����Ϣʵ��
    struct Book
    {
        int Index;
        bool IsDeleted;
        char Isbn[LEN_BOOK_ISBN];
        char Name[LEN_BOOK_NAME];
        char Author[LEN_BOOK_AUTHOR];
        char Publisher[LEN_BOOK_PUBLISHER];
        int Total;
        int Remain;
    };

    // �û�ʵ��
    struct User
    {
        int Index;
        bool IsDeleted;//trueΪ��ɾ
        char Name[LEN_USER_NAME];
        char Password[LEN_USER_PASSWORD];
        char Type[LEN_USER_TYPE];
        char Info[LEN_USER_INFO];
    };

    // ���ļ�¼ʵ��
    struct Record
    {
        int Index;
        bool IsDeleted;
        int BookIndex;
        int UserIndex;
        time_t Datetime;
        bool IsRenew;
        bool IsReturned;
    };

    // ����ʵ�����������ļ���
    char* _GenerateFilePathByTypeName(const char* name);
    template <typename T>
    char* _GenerateFilePathByType() {
        auto name = typeid(T).name();
        return _GenerateFilePathByTypeName(name);
    }

    // �򿪴洢ĳ�����͵�ʵ����ļ�
    template <typename T>
    FILE* _OpenFile() {
        FILE *file = fopen(_GenerateFilePathByType<T>(), "rb+");
        if (file == NULL) {
            throw new std::exception("�ļ���ʧ��");
        }
        setvbuf(file, NULL, _IOFBF, (sizeof T) * Config::FILE_READ_CACHE_COUNT);
        return file;
    }

    // ���洢ĳ�����͵�ʵ����ļ��Ƿ����
    template <typename T>
    bool _IsFileExist() {
        auto file = fopen(_GenerateFilePathByType<T>(), "rb");
        auto exist = true;
        if (file == NULL) {
            exist = false;
        } else {
            fclose(file);
        }
        return exist;
    }

    // ����ĳ��ʵ��
    template <typename T>
    T* Create() {
        auto item = new T();
        memset(item, 0, sizeof T);
        item->Index = -1;
        return item;
    }

    // ���� Index ѡ��ĳ��ʵ��
    template <typename T>
    T* Select(int index) {
        auto file = _OpenFile<T>();
        auto size = sizeof T;
        auto item = new T();
        fseek(file, size * index, SEEK_SET);
        fread(item, size, 1, file);
        return item;
    }

    // ��������ѡ��һЩʵ��
    // ���� NULL ��β������
    // where: ��������
    template <typename T>
    T** Selects(const std::function<bool(const T*)> where, int beginIndex = 0, int maxCount = -1) {
        auto temp = std::list<T*>();
        auto count = 0;
        Traverse<T>([&](const T* item) {
            if (where(item)) {
                T* t = new T();
                memcpy(t, item, sizeof T);
                temp.push_back(t);
                count++;
            }
            return count != maxCount;
        }, beginIndex);

        auto result = new T*[temp.size() + 1];
        int i = 0;
        for (auto it=temp.begin(); it != temp.end(); it++) {
            result[i] = *it;
            i++;
        }
        result[temp.size()] = NULL;
        return result;
    }

    // ��������ѡ��һ��ʵ��
    // ���ط��������ĵ�һ��ʵ�壬û���ҵ�ʱ���� NULL
    // where: ��������
    template <typename T>
    T* Select(const std::function<bool(const T*)> where, int beginIndex = 0) {
        auto results = Selects(where, beginIndex, 1);
        auto result = *results;
        delete [] results;
        return result;
    }

    // ��������һ��ʵ��
    template <typename T>
    void InsertOrUpdate(T* item) {
        auto file = _OpenFile<T>();
        auto size = sizeof T;
        if (item->Index == -1) {
            fseek(file, 0, SEEK_END);            
            auto offset = ftell(file);
            item->Index = offset / size;
        } else {
            fseek(file, size * item->Index, SEEK_SET);
        }
        fwrite(item, size, 1, file);
        fclose(file);
    }

    // ɾ��һ��ʵ��
    template <typename T>
    void Delete(int index) {
        auto file = _OpenFile<T>();
        auto size = sizeof T;
        T* item = new T();
        fseek(file, size * index, SEEK_SET);
        fread(item, size, 1, file);
        item->IsDeleted = true;
        fseek(file, size * index, SEEK_SET);
        fwrite(item, size, 1, file);
        delete item;
        fclose(file);
    }

    // ����һ��ʵ��
    // func: ����һ��ʵ��ĺ���
    template <typename T>
    void Traverse(const std::function<bool(const T*)> func, int beginIndex = 0, int endIndex = 0) {
        auto file = _OpenFile<T>();
        auto size = sizeof T;
        T* item = new T();

        fseek(file, 0, SEEK_END);
        auto count = ftell(file) / size;

        if (endIndex <= 0) {
            endIndex += count;
        }

        fseek(file, beginIndex * size, SEEK_SET);        
        for (auto i = beginIndex; i < endIndex; i++) {
            fread(item, size, 1, file);
            if (!item->IsDeleted && !func(item)) {
                break;
            }
        }

        delete item;
    }

    // ���ݴ洢��ʼ�����������Ŀ¼���ļ�����ӳ�ʼ��Ϣ
    // force: Ϊ true ʱ���������ļ�ǿ�Ƴ�ʼ��
    void Init(bool force = false);

}
