/*  OTUS C-2023-07 �������� �������� �� � 3 ������� "���� ������ 1�.
����: ���� �����
�����: "���� ���� �������� zip-�����, ������ ������ ��������: " � ������ ������ � ������, ���� ������� ���� �������� zip ����,
�����: "���� ���� �� �������� zip-�����"*/
#include <stdio.h>
#include <stdlib.h>

void is_it_contain_zip(void)
{
    setlocale(0, "rus");  // ����� ������������ �������� ������� � printf
    FILE * file;    // ���� ����� ������ ����
    unsigned char path[500]; // ���� � �����
    int i, j;    // ���������� ��� ������ for
    unsigned char buf[4] = {0, 0, 0, 0};    // ����� ��� ���������� EOCD, Central Directory Size � ������� �����
    unsigned char file_name[200];  // ��� ����� � ������
    char is_it_contain_zip_ = 0;	// ������� �������� ���� zip-����� ��� ���
    int Central_Directory_size = 0; // ������ ����������� ���������� � ������
    unsigned char * Central_Directory;    // ������, ���������� Central Directory
    int file_name_size; // ������ ����� ����� � ������
    unsigned char CD[4] = {0x50, 0x4b, 0x01, 0x02};  // ��������� ��� Central Directory Header
    int a = 0;  // ������� ������

    printf("������� ���� � �����: ");
    scanf("%s", path);  // ������ ���� � �����
    file = fopen(path, "r");    // ��������� ���� �� ������, ������� ��������� �� ������ path
    if(file == NULL) {perror("Error") ; exit(0);}   // ���� �� ����� �������� ����� ������, ������� ��������� �� ������ � ����� �� ���������

    for (i = 0; i < 1000; i++)  // ���������� 1000 ��� �� 1 ����� � ����� ����� � ������� EOCD
    {
        fseek(file, -i, SEEK_END);  // ��������� 1, 2, 3 ... ����� � ����� �����, ����� ����� EOCD
        fread(buf, 1, 4, file);     // ���������� ������ 4 ����� � ����� ����� � ������ buf

        if( (buf[0] == 0x50) && (buf[1] == 0x4b) && (buf[2] == 0x05) && (buf[3] == 0x06) )  // ��������� EOCD zip-����� �������� 4 ������ ������ �����
        {
            is_it_contain_zip_ = 1;
            printf("���� ���� �������� zip-�����, ������ ������ ��������:\n");
        }
        if(is_it_contain_zip_) {break;}
    }

    if(!is_it_contain_zip_) {printf("���� ���� �� �������� zip-�����\n"); return 0;}

    fseek(file, 8, SEEK_CUR);   // ������������ ����� ��������� EOCD ��������� � Central Directory size
    fread(buf, 1, 4, file);     // ������ 4 �����, ������� ��������� � Central Directory size
    Central_Directory_size = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];  // Central_Directory_size ������ ����������� ����������

    fseek(file, -16 - Central_Directory_size, SEEK_CUR);    // ���������� � ������ Central Directory, � ������ header

    Central_Directory = (unsigned char*)malloc(Central_Directory_size*sizeof(unsigned char)); // �������� ������ ��� ������ Central_Directory

    if(!Central_Directory) {    // ����� �� ������� ������
        printf("Out of memory. Press any key: ");
        _getch();
        exit(1);
    }

    fread(Central_Directory, 1, Central_Directory_size, file);  // ��������� ��� Central Directory � ������ Central_Directory

    for (j = 0; j < Central_Directory_size; j++)
    {
        if ( (memcmp(CD, Central_Directory+j, 4)) == 0 )
        {
            ++a;
            memcpy(buf, Central_Directory+j+28, 2);
            file_name_size = (buf[1] << 8) | buf[0]; // ���������� ������ ����� ����� � buf

            memset(file_name, 0, 100);  // �������� ������ file_name, ����� �� ���� ������
            memcpy(file_name, Central_Directory+j+46, file_name_size);  // ���������� � ������ file_name ��� �����
            //file_1 = fopen("text", "w");
            //fwrite(file_name, 1, file_name_size, file_1 );
            //fclose(file_1);
            printf("%s \n", file_name);
        }
    }
    if(!a) {printf("� ������ %d ������ \n", a);}  // ���� ����� ������

    free(Central_Directory);    // ����������� ������, ���������� ��� ������ Central_Directory
    fclose(file);   // ��������� ����
    return 0;
}

int main()
{
    is_it_contain_zip();
    return 0;
}
