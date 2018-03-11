    //test.c  
    #define _CRT_SECURE_NO_WARNINGS 1  
    #include"commentconvert.h"  
    int main()  
    {  
        FILE *pfRead = NULL;  
        FILE *pfWrite = NULL;  
        printf("注释开始\n");  
        pfRead = fopen(INPUTNAME, "r");  
        if (NULL == pfRead)  
        {  
            perror("open file for read");  
            exit(EXIT_FAILURE);  
        }  
        pfWrite = fopen(OUTPUTNAME, "w");  
        if (NULL == pfWrite)  
        {  
            fclose(pfRead);  
            perror("open file for write");  
            exit(EXIT_FAILURE);  
        }  
        commentcovert(pfRead, pfWrite);  
        printf("注释结束\n");  
        system("pause");  
        return 0;  
    }  
    //commentconvert.h  
    #ifndef  __COMMENTCOVERT_H__  
    #define  __COMMENTCOVERT_H__  
    #define INPUTNAME "input.c"  
    #define OUTPUTNAME "output.c"  
    #include<stdio.h>  
    #include<stdlib.h>  
    enum STATE  
    {  
        NUL_STATE,  
        C_STATE,  
        CPP_STATE,  
        END_STATE,  
        STR_STATE  
    };  
      
    void commentcovert(FILE *pfRead, FILE *pfWrite);  
    void Do_NUL_STATE(FILE *pfRead, FILE *pfWrite);  
    void Do_C_STATE(FILE *pfRead, FILE *pfWrite);  
    void Do_CPP_STATE(FILE *pfRead, FILE *pfWrite);  
    void Do_STR_STATE(FILE *pfRead, FILE *pfWrite);  
    #endif//  
    //commentconvert.c  
    #include"commentconvert.h"  
    enum STATE state = NUL_STATE;  
    void commentcovert(FILE *pfRead, FILE *pfWrite)  
    {  
        while (state != END_STATE)  
        {  
            switch(state)  
            {   
            case NUL_STATE:  
                Do_NUL_STATE(pfRead, pfWrite);  
                break;  
            case C_STATE:  
                Do_C_STATE(pfRead, pfWrite);  
                break;  
            case CPP_STATE:  
                Do_CPP_STATE(pfRead, pfWrite);  
                break;  
            case STR_STATE:  
                Do_STR_STATE(pfRead, pfWrite);  
                break;  
            case END_STATE:  
                break;  
            }  
        }  
    }  
    void Do_NUL_STATE(FILE *pfRead, FILE *pfWrite)  
    {  
        int first = 0;  
        int second = 0;  
        first = fgetc(pfRead);  
        switch (first)  
        {  
        case '/':  
            second = fgetc(pfRead);  
            if (second == '/')  
            {  
                fputc('/', pfWrite);  
                fputc('/', pfWrite);  
                state = CPP_STATE;   //在无状态下遇到//直接将//写进去，并进入cpp状态  
            }  
                else if (second == '*')  
                {  
                    fputc(first, pfWrite);  
                    fputc('/', pfWrite);  
                    state = C_STATE;//在无状态下遇到/*要将/*变成//写进去，并进入c状态  
                }  
                else  
                {  
                    fputc(first, pfWrite);  
                    fputc(second, pfWrite);//如果遇到其他字符，直接写进去，状态不改变。  
                }  
                break;  
            case EOF:  
                state = END_STATE;//如果遇到EOF，进入结束状态  
                break;  
            case '"':  
                fputc(first, pfWrite);  
                state = STR_STATE;//如果遇到"，进入字符串状态  
                break;  
            default:  
                fputc(first, pfWrite);//其他情况的字符，直接写进去就行  
                break;  
            }  
    }  
    void Do_STR_STATE(FILE *pfRead,FILE *pfWrite)  
    {  
        int first = 0;  
        first = fgetc(pfRead);  
        switch (first)  
        {  
        case '"':  
            fputc(first, pfWrite);  
            state = NUL_STATE;//在字符串状态如果遇到后引号，将后引号写入，并进入无状态  
            break;  
        default:  
            fputc(first, pfWrite);//其他情况的字符，直接写进去就行  
            break;  
        }  
    }  
    void Do_C_STATE(FILE *pfRead, FILE *pfWrite)  
    {  
        int first = 0;  
        int second = 0;  
        int third = 0;  
        first = fgetc(pfRead);  
        switch (first)  
        {  
        case '*':  
            second = fgetc(pfRead);  
            if (second == '/')  // 遇到*/ 说明c的注释结束，切换到无状态  
            {  
                state = NUL_STATE;  
                third = fgetc(pfRead);  
                if (third != '\n')  
                {  
                    fputc('\n', pfWrite);    //如果/后边的字符不是回车，需要写进去回车，然后再把独到的字符放回  
                    ungetc(third, pfRead);  
                }  
                else  
                {  
                    ungetc(third, pfRead);//如果是回车，把回车放回  
                }  
            }                             
            else  
            {  
                fputc(first, pfWrite);            
                //如果遇到*，但是后边不是/，把*放进去，*后边的字符放回，比如，/***/,进入c状态以后，  
                //遇到*，后边的字符也是*，如果把两个*都写进去，后边就只剩下/，就不能找到c状态结束的标志，  
                //所以，需要把第二个字符原样写回  
                /*fputc(second, pfWrite);*/  
                ungetc(second,pfRead);    //放回  
            }  
            break;  
        case '\n':  
            fputc(first, pfWrite);//如果需要回车，吧回车写入，在写入//，状态不变。  
            fputc('/', pfWrite);  
            fputc('/', pfWrite);  
            break;  
        default:  
            fputc(first, pfWrite);  
            break;  
        }  
    }  
    void Do_CPP_STATE(FILE *pfRead, FILE *pfWrite)  
    {  
        int first = 0;  
        int second = 0;  
        first = fgetc(pfRead);  
        switch (first)  
        {  
        case '\n':  
            fputc(first,pfWrite);//cpp状态，如果遇到回车，cpp状态结束，进入无状态  
            state = NUL_STATE;  
            break;  
        default:  
            if (first == EOF)  
            {  
                state = END_STATE;  
            }  
            else  
            {  
                fputc(first, pfWrite);  
            }  
                break;  
            }  
    }  
