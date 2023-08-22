/*  OTUS C-2023-07 Барашева Ангелина ДЗ к 3 занятию "Типы данных 1Ч.
вход: путь файла
выход: "Этот файл содержит zip-архив, список файлов которого: " и список файлов в архиве, если входной файл содержит zip файл,
иначе: "Этот файл не содержит zip-архив"*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
int is_it_contain_zip(void)
{
    setlocale(LC_ALL, "");  // чтобы пользоваться русскими буквами в printf
    FILE * file;    // сюда будет считан файл
    char path[500]; // путь к файлу
    int i, j;    // переменные для циклов for
    unsigned char buf[4] = {0, 0, 0, 0};    // буфер для считывания EOCD, Central Directory Size и размера имени
    unsigned char file_name[200];  // имя файла в архиве
    char is_it_contain_zip_ = 0;	// признак содержит файл zip-архив или нет
    int Central_Directory_size = 0; // размер центральной директории в байтах
    unsigned char * Central_Directory;    // массив, содержащий Central Directory
    int file_name_size; // размер имени файла в байтах
    unsigned char CD[4] = {0x50, 0x4b, 0x01, 0x02};  // сигнатура для Central Directory Header
    int a = 0;  // счётчик файлов

    printf("Введите путь к файлу: ");
    scanf("%s", path);  // вводим путь к файлу
    file = fopen(path, "r");    // открываем файл на чтение, который находится по адресу path
    if(file == NULL) {perror("Error") ; exit(0);}   // если на этапе открытия файла ошибка, вывести сообщение об ошибке и выйти из программы

    for (i = 0; i < 1000; i++)  // проходимся 1000 раз по 1 байту с конца файла в поисках EOCD
    {
        fseek(file, -i, SEEK_END);  // считываем 1, 2, 3 ... байта с конца файла, чтобы найти EOCD
        fread(buf, 1, 4, file);     // записываем каждые 4 байта с конца файла в массив buf

        if( (buf[0] == 0x50) && (buf[1] == 0x4b) && (buf[2] == 0x05) && (buf[3] == 0x06) )  // сигнатура EOCD zip-файла содержит 4 подряд идущих байта
        {
            is_it_contain_zip_ = 1;
            printf("Этот файл содержит zip-архив, список файлов которого:\n");
        }
        if(is_it_contain_zip_) {break;}
    }

    if(!is_it_contain_zip_) {printf("Этот файл не содержит zip-архив\n"); return 0;}

    fseek(file, 8, SEEK_CUR);   // относительно конца сигнатуры EOCD переходим к Central Directory size
    fread(buf, 1, 4, file);     // читаем 4 байта, которые относятся к Central Directory size
    Central_Directory_size = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];  // Central_Directory_size размер центральной директории

    fseek(file, -16 - Central_Directory_size, SEEK_CUR);    // сместились в начало Central Directory, в первый header

    Central_Directory = (unsigned char*)malloc(Central_Directory_size*sizeof(unsigned char)); // выделяем память под массив Central_Directory

    if(!Central_Directory) {    // выход за пределы памяти
        printf("Out of memory. Press any key: ");
      //  _getch();
        exit(1);
    }

    fread(Central_Directory, 1, Central_Directory_size, file);  // считываем всю Central Directory в массив Central_Directory

    for (j = 0; j < Central_Directory_size; j++)
    {
        if ( (memcmp(CD, Central_Directory+j, 4)) == 0 )
        {
            ++a;
            memcpy(buf, Central_Directory+j+28, 2);
            file_name_size = (buf[1] << 8) | buf[0]; // записываем размер имени файла в buf

            memset(file_name, 0, 100);  // обнуляем массив file_name, чтобы не было мусора
            memcpy(file_name, Central_Directory+j+46, file_name_size);  // записываем в массив file_name имя файла
            //file_1 = fopen("text", "w");
            //fwrite(file_name, 1, file_name_size, file_1 );
            //fclose(file_1);
            printf("%s \n", file_name);
        }
    }
    if(!a) {printf("В архиве %d файлов \n", a);}  // если архив пустой

    free(Central_Directory);    // освобождаем память, выделенную под массив Central_Directory
    fclose(file);   // закрываем файл
    return 0;
}

int main()
{
    is_it_contain_zip();
    system("pause");
    return 0;
}
