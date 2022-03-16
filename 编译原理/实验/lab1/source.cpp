#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctype.h>
#include <string.h>

#define END "waiting for your expanding"
 
using namespace std;
 
typedef struct //词的结构
{
    int type; //单词种类
    char* content; //单词的内容
} Token;
char input[255];
char output[255] = "";
int input_index; //index，指向下一个读取的位置，读完++
int output_index;
char temp;
int pre_type;

char* key_table[] = {"int", "long", "short", "float", "double", "char", "unsigned", "signed", "const", "void", "volatile", "enum", "struct", "union",
                 "if", "else", "goto", "switch", "case", "do", "while", "for", "continue", "break", "return", "default", "typedef",
                 "auto", "register", "extern", "static", "sizeof", "begin", "then", "end", "cout", "main", "endl", END};
Token *do_scan(); //扫描
int main()
{
    
    Token *oneword = new Token;
 
    //实现从文件读取代码段
    cout << "begin reading" << endl;
    FILE *fp;
    if ((fp = freopen("data.txt", "r", stdin)) == NULL){
        printf("Not found file!\n");
        return 0;
    }

    ofstream res;
    res.open("res.txt");

    while ((scanf("%[^#]", input)) != EOF)
    {
        input_index = 0;
        while (pre_type < 1000 && pre_type != -1)
        {
            oneword = do_scan();
            if (oneword->type < 1000)
            {
                if (oneword->type != 999)
                    res << "[  " << oneword->type << "\t" << oneword->content << "  ]" << endl;
            }
            pre_type = oneword->type;
        }
    }

    res.close();
    
    return 0;
}

//在输入缓冲区中向后获取一个字符并返回
char get_one_char(){
    temp = input[input_index];
    input_index++;
    return (temp);
}


void connection(){
    output[output_index] = temp;
    output_index++;
    output[output_index] = '\0';
}

//搜索关键字
int search_keys(){
    int num = 0;
    while(strcmp(key_table[num], END)){
        if(!strcmp(key_table[num], output)){
            return num + 1;
        }
        num++;
    }

    //如果前一个是双引号，判断它为字符串
    if(pre_type == 72){
        int move = input_index;
        char next_ch = input[move];
        while(input[move] == ' ' || temp == '\n' || temp == '\t'){
            next_ch = input[move];
            move++;
        }
        if(next_ch == '\"'){
            return 41;
        }
    }

    return 40;  //判定它为标识符
}
 
//词法扫描程序
Token *do_scan()
{
    Token* myword = new Token;
    myword->type = -2;
    myword->content = "";

    output_index = 0;
    get_one_char();

    while(temp == ' ' || temp == '\n' || temp == '\t'){
        temp = input[input_index];
        input_index++;
    }


    if (isalpha(temp) || temp == '_') {
        //如int
        while (isalpha(temp) || isdigit(temp) || temp == '_'){
            connection(); //连接
            get_one_char();
        }
        input_index--;                  //回退一个字符
        myword->type = search_keys();   //关键字、字符串、标识符
        myword->content = output;
        return (myword);

    }else if (isdigit(temp)){ //判断读取到的单词首字符是数字
    
        while (isdigit(temp)){ 
            connection();
            get_one_char();
        }
        input_index--;
        //数字单词种别码统一为50，单词自身的值为数字本身
        myword->type = 50;
        myword->content = output;
        return (myword);
    }else
        switch (temp){

        case '=':
            get_one_char(); //首字符为=,再读取下一个字符判断
            if (temp == '='){
                myword->type = 70;
                myword->content = "==";
                return (myword);
            }
            input_index--; //读取到的下个字符不是=，则要回退，直接输出=
            myword->type = 52;
            myword->content = "=";
            return (myword);
            break;
        case '+':
            myword->type = 53;
            myword->content = "+";
            return (myword);
            break;
        case '-':
            myword->type = 54;
            myword->content = "-";
            return (myword);
            break;
        case '/':          //读取到该符号之后，要判断下一个字符是什么符号，判断是否为注释
            get_one_char();     //首字符为/,再读取下一个字符判断
            if (temp == '*'){ // 说明读取到的是注释
            
                get_one_char();
                while (temp != '*'){
                    get_one_char(); //注释没结束之前一直读取注释，但不输出
                    if (temp == '*'){
                        get_one_char();
                        if (temp == '/'){ //注释结束
                            myword->type = 999;
                            myword->content = "annotation";
                            return (myword);
                            break;
                        }
                    }
                }
            }
            else{
                input_index--; //读取到的下个字符不是*，即不是注释，则要回退，直接输出/
 
                myword->type = 56;
                myword->content = "/";
                return (myword);
                break;
            }
        case '*':
            myword->type = 55;
            myword->content = "*";
            return (myword);
            break;
        case '(':
            myword->type = 57;
            myword->content = "(";
            return (myword);
            break;
        case ')':
            myword->type = 58;
            myword->content = ")";
            return (myword);
            break;
        case '[':
            myword->type = 59;
            myword->content = "[";
            return (myword);
            break;
        case ']':
            myword->type = 60;
            myword->content = "]";
            return (myword);
            break;
        case '{':
            myword->type = 61;
            myword->content = "{";
            return (myword);
            break;
        case '}':
            myword->type = 62;
            myword->content = "}";
            return (myword);
            break;
        case ',':
            myword->type = 63;
            myword->content = ",";
            return (myword);
            break;
        case ':':
            get_one_char();
            if (temp == '='){
                myword->type = 51;
                myword->content = ":=";
                return (myword);
                break;
            }
            else{
                input_index--;
                myword->type = 64;
                myword->content = ":";
                return (myword);
                break;
            }
        case ';':
            myword->type = 65;
            myword->content = ";";
            return (myword);
            break;
        case '>':
            get_one_char();
            if (temp == '='){
                myword->type = 68;
                myword->content = ">=";
                return (myword);
                break;
            }
            input_index--;
            myword->type = 66;
            myword->content = ">";
            return (myword);
            break;
        case '<':
            get_one_char();
            if (temp == '='){
                myword->type = 69;
                myword->content = "<=";
                return (myword);
                break;
            }
            else if (temp == '<'){
                myword->type = 73;
                myword->content = "<<";
                return (myword);
                break;
            }
            else{
                input_index--;
                myword->type = 67;
                myword->content = "<";
                return (myword);
            }
        case '!':
            get_one_char();
            if (temp == '='){
                myword->type = 71;
                myword->content = "!=";
                return (myword);
                break;
            }
            input_index--;
            myword->type = 74;
            myword->content = "NOT";
            return (myword);
            break;
        case '\"':
            myword->type = 72;
            myword->content = "\"";
            return (myword);
            break;
        case '\0':
            myword->type = 1000;
            myword->content = "OVER";
            return (myword);
            break;
        case '#':
            myword->type = 0;
            myword->content = "#";
            return (myword);
            break;
        default:
            myword->type = -1;
            myword->content = "ERROR";
            return (myword);
            break;
        }
}