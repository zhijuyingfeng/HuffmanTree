//
// Created by nigao on 11/13/18.
//

#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#include <cstdlib>
#include "huffman.h"
using namespace std;

extern int weight[maxn];
extern char charset[maxn];
extern int TotalCharNum;
extern int CipherCharNum;

TreeNode::TreeNode()
{
    this->weight = -1;//-1表示暂时没有用到该节点
    memset(this->path, 0, sizeof(path));
    tag = L;
    parent = -1;//-1表示该节点暂时没有父节点
    LeftChild = -1;//-1表示暂时没有左儿子
    RightChild = -1;//-1表示暂时没有右儿子
}

void Init(TreeNode arr[])
{
    for (int i = 0; i < maxn; i++)
    {
        cin >> arr[i].weight;
    }
    for (int i = 0; i < maxn; i++)
    {
        arr[i].weight = weight[i];
        arr[i].pos = i;
    }
}

void GenerateHuffmanTree(TreeNode arr[])
{
    priority_queue<TreeNode, vector<TreeNode>, cmp> Heap;

    //将所有节点入堆
    for (int i = 0; i < maxn; i++)
    {
        Heap.push(arr[i]);
    }

    TreeNode temp1, temp2, sum;
    int cnt = maxn;//记录新的和节点被放到数组中的位置

    //取堆中最小的两个节点合并成一个大的节点
    for (int i = 0; i < maxn - 1; i++)//maxn-1次合并
    {
        //从堆中取出两个最小值
        temp1 = Heap.top();
        Heap.pop();
        temp2 = Heap.top();
        Heap.pop();

        sum.weight = temp1.weight + temp2.weight;
        sum.pos = cnt;

        //记录将temp1.pos所在节点和temp2.pos所在节点两个节点的父节点
        arr[temp1.pos].parent = cnt;
        arr[temp2.pos].parent = cnt;

        arr[cnt] = sum;
        Heap.push(sum);//将合并后的节点入堆

        if (temp1.weight < temp2.weight)//权重较小的节点成为左儿子，较大的节点成为右儿子
        {
            arr[temp1.pos].tag = L;
            arr[temp2.pos].tag = R;

            //父节点记录左右儿子节点的位置
            arr[cnt].LeftChild = temp1.pos;
            arr[cnt].RightChild = temp2.pos;
        }
        else
        {
            arr[temp1.pos].tag = R;
            arr[temp2.pos].tag = L;

            //父节点记录左右儿子节点的位置
            arr[cnt].LeftChild = temp2.pos;
            arr[cnt].RightChild = temp1.pos;
        }
        cnt++;
    }

    for (int i = ((maxn - 1) << 1) - 1; i >= 0; i--)
    {
        int ParentPos = arr[i].parent;//父节点在数组中的位置
        strcpy(arr[i].path, arr[ParentPos].path);//复制父节点已有的路径
        int len = strlen(arr[i].path);//父节点路径的长度

        //再加上自己的路径
        if (arr[i].tag == L)
        {
            arr[i].path[len++] = '0';
        }
        else if (arr[i].tag == R)
        {
            arr[i].path[len++] = '1';
        }
    }
}

void GenerateClearText()//生成明文
{
    const int str_len=1000;
    FILE* file = fopen("/home/nigao/Documents/HuffmanTree/cleartext.txt", "w");
    char str[str_len+1] = { 0 };
    int r;
    for (int i = 0; i < 100; i++)
    {
        memset(str, 0, sizeof(str));
        for (int j = 0; j < str_len; j++)//随机生成明文
        {
            r = rand() % maxn;
            str[j] = charset[r];
        }
        fprintf(file, "%s", str);//将明文写入文件
    }
    fclose(file);
}

void encode()
{
    FILE* file = fopen("/home/nigao/Documents/HuffmanTree/cleartext.txt", "r");//打开明文文件
    char str[Max];//读明文文件的缓冲区
    int len;
    while (!feof(file))//没有到达文件尾
    {
        //fgets(str, Max, file);//从文件中读一行到缓冲区
        fscanf(file,"%[a-zA-Z\n,. ]",str);
        len = strlen(str);
        TotalCharNum += len;
        for (int i = 0; i < len; i++)//计算字符集中各字符个数
        {
            switch (str[i])
            {
                case '\n':weight[0]++; break;
                case ' ':weight[1]++; break;
                case ',':weight[2]++; break;
                case '.':weight[3]++; break;
                default:
                    if (str[i] >= 'A'&&str[i] <= 'Z')
                    {
                        weight[str[i] - 'A' + 4]++;
                    }
                    else if (str[i] >= 'a'&&str[i] <= 'z')
                    {
                        weight[str[i] - 'a' + 30]++;
                    }
            }
        }
    }
    fclose(file);//关闭明文文件
}

void SetCharset()
{
    //字符集顺序为｛'\n', '  ', ',', '.', A-Z, a-z｝
    charset[0] = '\n';
    charset[1] = ' ';
    charset[2] = ',';
    charset[3] = '.';
    for (int i = 0; i <26; i++)
    {
        charset[4 + i] = 'A' + i;
        charset[30 + i] = 'a' + i;
    }
}

//void GenerateCipherText(const TreeNode arr[])
//{
//	FILE* cleartext = fopen("D:/cleartext.txt", "r");
//	FILE* ciphertext = fopen("D:/ciphertext.txt", "w");
//	char buffer[buffer_size+4];//写入密文文件的缓冲区,为4KB
//	char str[Max];//读入明文文件的缓冲区
//	int clear_len;//明文中某一行的长度
//	int pos;//某个字符在字符集中的位置
//	int buffer_len;//缓冲区当前长度
//	int path_len;//某个字符的哈夫曼编码的长度
//
//	while (!feof(cleartext))//当没有到达明文文件尾时循环
//	{
//		fgets(str, Max, cleartext);//每次处理一行
//		memset(buffer, 0, sizeof(buffer));
//		buffer_len = 0;
//		clear_len = strlen(str);
//		for (int i = 0; i < clear_len; i++)
//		{
//			pos = GetCharPos(str[i]);
//			path_len = strlen(arr[pos].path);
//
//			if (buffer_len + path_len < buffer_size)//缓冲区未满
//			{
//				//将哈夫曼编码写入缓冲区
//				strcat(buffer, arr[pos].path);
//				buffer_len += path_len;
//			}
//			else//缓冲区已满
//			{
//				fprintf(ciphertext, "%s", buffer);//将缓冲区写入文件
//				memset(buffer, 0, sizeof(buffer));//刷新缓冲区
//				buffer_len = 0;
//
//				//将哈夫曼编码写入缓冲区
//				strcat(buffer, arr[pos].path);
//				buffer_len += path_len;
//			}
//		}
//
//		//一行处理完毕后，将缓冲区中内容写入文件
//		fprintf(ciphertext, "%s", buffer);//将缓冲区写入文件
//	}
//
//	fclose(cleartext);
//	fclose(ciphertext);
//}

void GenerateCipherText(const TreeNode arr[])
{
    FILE* cleartext = fopen("/home/nigao/Documents/HuffmanTree/cleartext.txt", "r");//打开明文文件
    FILE* ciphertext = fopen("/home/nigao/Documents/HuffmanTree/ciphertext2.txt", "w");//打开密文文件
    bool bin_buffer[buffer_size + maxn] = { 0 };//用来存放哈夫曼编码
    char char_buffer[buffer_size + maxn] = { 0 };//用来存放8个哈夫曼编码组成的字符
    char str[Max];//读入明文文件的缓冲区
    int clear_len;//明文中某一行的长度
    int pos;//某个字符在字符集中的位置
    int bin_buffer_len=0;//bool缓冲区当前长度
    int char_buffer_len = 0;//char缓冲区当前长度
    int path_len;//某个字符的哈夫曼编码的长度

    while (!feof(cleartext))//当没有到达明文文件尾时循环
    {
        fgets(str, Max, cleartext);//每次处理一行
        clear_len = strlen(str);
        for (int i = 0; i < clear_len; i++)
        {
            pos = GetCharPos(str[i]);//获取当前字符在字符集中的位置
            path_len = strlen(arr[pos].path);
            CipherCharNum += path_len;

            for (int j = 0; j < path_len; j++)//将该字符的哈夫曼编码写入bool缓冲区
            {
                bin_buffer[bin_buffer_len++] = arr[pos].path[j] - '0';
            }

            if (bin_buffer_len >= buffer_size)//bool缓冲区已满
            {
                char temp;
                for (int j = 0; j < (buffer_size >> 3); j++)//将bool缓冲区中内容写入char缓冲区
                {
                    temp = Bin2Char(bin_buffer+(j << 3));//要考虑到temp可能为0的问题
                    if (!temp)//如果temp为0
                    {
                        fputs(char_buffer, ciphertext);//则将当前char缓冲区写入文件
                        fprintf(ciphertext, "%c", temp);//将0写入文件
                        memset(char_buffer, 0, char_buffer_len * sizeof(char));//刷新缓冲区
                        char_buffer_len = 0;
                        continue;
                    }

                    char_buffer[char_buffer_len++] = temp;
                }

                //刷新bool缓冲区
                bin_buffer_len -= buffer_size;
                memset(bin_buffer, 0, sizeof(bool)*buffer_size);
                //将bin_buffer尾部的内容移到数组首部
                for (int j = 0; j < bin_buffer_len ; j++)
                {
                    bin_buffer[j] = bin_buffer[j + buffer_size];
                }
                memset(bin_buffer + buffer_size, 0, sizeof(bool)*maxn);

                fputs(char_buffer, ciphertext);//将char缓冲区中内容写入文件
                memset(char_buffer, 0, sizeof(char)*char_buffer_len);//刷新char缓冲区
                char_buffer_len = 0;//char缓冲区长度置0
            }
        }
    }

    //文件读取完后，bool缓冲区内数据还没有处理
    char temp;
    for (int i = 0; i < (bin_buffer_len >> 3) + 1; i++)
    {
        temp = Bin2Char(bin_buffer + (i << 3));
        if (!temp)
        {
            fputs(char_buffer, ciphertext);//则将当前char缓冲区写入文件
            fprintf(ciphertext, "%c", temp);//将0写入文件
            memset(char_buffer, 0, char_buffer_len * sizeof(char));//刷新缓冲区
            char_buffer_len = 0;
            continue;
        }
        char_buffer[char_buffer_len++] = temp;
    }
    fputs(char_buffer, ciphertext);

    CipherCharNum = (CipherCharNum >> 3) + 1;
    fclose(cleartext);
    fclose(ciphertext);
}

int GetCharPos(const char& ch)//获取某个字符在字符集中的位置
{
    int pos;
    switch (ch)
    {
        case '\n':pos = 0; break;
        case ' ':pos = 1; break;
        case ',':pos = 2; break;
        case '.':pos = 3; break;
        default:
            if (ch >= 'A'&&ch <= 'Z')
            {
                pos = ch - 'A' + 4;
            }
            else if (ch >= 'a'&&ch <= 'z')
            {
                pos = ch - 'a' + 30;
            }
            break;
    }
    return pos;
}

void decode(const TreeNode *arr)
{
    FILE* cipertext2 = fopen("/home/nigao/Documents/HuffmanTree/ciphertext2.txt", "r");//密文文件
    FILE* cleartext2 = fopen("/home/nigao/Documents/HuffmanTree/cleartext2.txt", "w");//由密文解密得到的文件
    int ch;//存储从密文文件中读取的字符
    int DecodedCharNum = 0;//存储已经解密的字符数
    bool bin_buffer[buffer_size + (maxn<<1)];//bool缓冲区
    int bin_buffer_len = 0;//bool缓冲区长度
    char char_buffer[Max] = { 0 };//存放已经解密的字符


    while (!feof(cipertext2))
    {
        //fscanf(cipertext2, "%c", &ch);//逐字符读取
        ch = getc(cipertext2);
        Char2Bin(ch, bin_buffer + bin_buffer_len);//将字符转换为0和1
        bin_buffer_len += 8;

        if (bin_buffer_len >= buffer_size+maxn)//bool缓冲区满
        {
            int i = 0;
            while(i<buffer_size)
            {
                int pos = (maxn << 1) - 2;//该节点在数组中的位置
                while (arr[pos].LeftChild >= 0 && arr[pos].RightChild >= 0)//没有到达叶节点
                {
                    if (bin_buffer[i])//为1，往右子树
                    {
                        pos = arr[pos].RightChild;
                    }
                    else//为0，往左子树
                    {
                        pos = arr[pos].LeftChild;
                    }
                    i++;
                }
                char_buffer[DecodedCharNum++] = GetPosChar(pos);
            }

            memset(bin_buffer, 0, sizeof(bool)*buffer_size);//
            for (int j = i; j < bin_buffer_len; j++)//将尾部数据移到首部
            {
                bin_buffer[j - i] = bin_buffer[j];
            }
            bin_buffer_len -= i;
            memset(bin_buffer + buffer_size, 0, sizeof(bool)*(maxn<<1));
        }
    }

    int i = 0;
    while (i < bin_buffer_len)
    {
        int pos = (maxn << 1) - 2;
        while (arr[pos].LeftChild >= 0 || arr[pos].RightChild >= 0)//没有到达叶节点
        {
            if (bin_buffer[i])//为1，往右子树
            {
                pos = arr[pos].RightChild;
            }
            else//为0，往左子树
            {
                pos = arr[pos].LeftChild;
            }
            i++;
        }
        char_buffer[DecodedCharNum++] = GetPosChar(pos);
        if (DecodedCharNum >= TotalCharNum)
            break;
    }
//    for (int i = 0; i < DecodedCharNum; i++)
//    {
//    	fprintf(cleartext2, "%c", char_buffer[i]);
//    }

    fputs(char_buffer, cleartext2);
    fclose(cipertext2);
    fclose(cleartext2);
}

char Bin2Char(const bool *arr)
{
    char sum = 0;
    for (int i = 7; i >=0; i--)
    {
        sum += (arr[i] << (7 - i));
    }
    return sum;
}

void Char2Bin(char ch, bool* arr)
{
    unsigned char temp = (unsigned char)ch;
    memset(arr, 0, sizeof(bool) * 8);
    int i = 7;
    while (temp > 0)
    {
        arr[i--] = (temp & 1);
        temp = (temp >> 1);
    }
}

char GetPosChar(const int&pos)
{
    switch (pos)
    {
        case 0:return '\n';
        case 1:return ' ';
        case 2:return ',';
        case 3:return '.';
        default:
            if (pos < 30)
            {
                return pos - 4 + 'A';
            }
            else
            {
                return pos - 30 + 'a';
            }
    }
}

void CreateVisual(const TreeNode arr[])
{
    //create tree.dot file
    FILE* stream=fopen("/home/nigao/Documents/HuffmanTree/tree.dot","w");
    fprintf(stream,"digraph {\n");
    fprintf(stream,"  node[width=0.5,height=0.5];\n");
    fprintf(stream,"  edge[color=blue, arrowhead=normal];\n");
    fprintf(stream,"  nodesep=0.5;\n\n");

    for(int i=0;i<((maxn<<1)-1);i++)
    {
        fprintf(stream,"%d[label=%d]",i,arr[i].weight);//create nodes
    }

    tree_create_dot(arr, ((maxn-1)<<1), stream);

    fprintf(stream,"}\n");
    fclose(stream);


    //invoke external command line to transfer tree.dot to tree.svg
    system("dot -Tsvg /home/nigao/Documents/HuffmanTree/tree.dot "
           "-o /home/nigao/Documents/HuffmanTree/tree.svg");
    system("eog /home/nigao/Documents/HuffmanTree/tree.svg");
}

void tree_create_dot(const TreeNode arr[],int pos,FILE* stream)
{
    int child;
    if(arr[pos].LeftChild>=0)
    {
        child=arr[pos].LeftChild;
        fprintf(stream,"%d -> %d;\n",pos,child);//draw edge
        tree_create_dot(arr,child,stream);//recur to draw left subtree
    }
    if(arr[pos].RightChild>=0)
    {
        child=arr[pos].RightChild;
        fprintf(stream,"%d -> %d;\n",pos,child);//draw edge
        tree_create_dot(arr,child,stream);//recur to draw left subtree
    }
}

void ComputeRate()
{
    cout<<"Compressed rate:\t"<<100.0*CipherCharNum/TotalCharNum<<"%"<<endl;
}

void ShowHuffmanCoding(TreeNode arr[])
{
    for(int i=0;i<maxn;i++)
    {
        cout<<GetPosChar(i)<<":\t"<<arr[i].path<<endl;
    }
}

void Compare()
{
    FILE* cleartext = fopen("/home/nigao/Documents/HuffmanTree/cleartext.txt", "r");//密文文件
    FILE* cleartext2 = fopen("/home/nigao/Documents/HuffmanTree/cleartext2.txt", "r");//由密文解密得到的文件

    char str1[Max]={0};
    char str2[Max]={0};
    fscanf(cleartext,"%[a-zA-Z,. \n]",str1);
    fscanf(cleartext2,"%[a-zA-Z,. \n]",str2);
    int res=strcmp(str1,str2);
    if(!res)
        cout<<"There is no difference between original text and decoded text."<<endl;
    else
        cout<<"There are some difference between original text and decoded text."<<endl;

    fclose(cleartext);
    fclose(cleartext2);
}