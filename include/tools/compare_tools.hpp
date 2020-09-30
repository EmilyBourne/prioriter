#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H
#include <string>

inline int compare(int const& i1, int const& i2)
{
    if (i1>i2) return 1;
    else if (i1<i2) return -1;
    else return 0;
}

inline int compare(double const& i1, double const& i2)
{
    if (i1>i2) return 1;
    else if (i1<i2) return -1;
    else return 0;
}

inline int compare(std::string const& str1,std::string const& str2)
{
    size_t i1(0);
    size_t i2(0);
    while (i1!=str1.size() && i2!=str2.size()) {
        char c1(str1[i1]);
        char c2(str2[i2]);
        if (c1<='9' && c1>='0' && c2<='9' && c2>='0') {
            char num1[20] = "";
            char num2[20] = "";
            int n1(0);
            int n2(0);
            while (c1<='9' && c1>='0' && i1!=str1.size()) {
                num1[n1++]=c1;
                c1=str1[++i1];
            }
            num1[n1]='\0';
            while (c2<='9' && c2>='0' && i2!=str2.size()) {
                num2[n2++]=c2;
                c2=str2[++i2];
            }
            num2[n2]='\0';
            float f1(atof(num1));
            float f2(atof(num2));
            if (f1!=f2) return f1<f2 ? -1 : 1;
        }
        else if (c1!=c2) return c1<c2 ? -1 : 1;
        else {
            i1++;
            i2++;
        }
    }
    if (i1!=str1.size()) return 1;
    else if (i2!=str2.size()) return -1;
    else return 0;
}

#endif // STRING_TOOLS_H
