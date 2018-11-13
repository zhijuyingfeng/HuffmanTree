#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#include "huffman.h"
using namespace std;

int weight[maxn];//各个字符出现的次数
int TotalCharNum = 0;//存储明文中字符的个数
int CipherCharNum = 0;//存储密文中用到的字符数
char charset[maxn];//存储字符集

int main()
{
    SetCharset();

    GenerateClearText();

    encode();

    TreeNode arr[(maxn<<1)-1];
    for (int i = 0; i < maxn; i++)
    {
        arr[i].weight = weight[i];
        arr[i].pos = i;
    }
    GenerateHuffmanTree(arr);
    GenerateCipherText(arr);
    decode(arr);
    return 0;
}