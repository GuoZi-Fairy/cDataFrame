#include "include\dataframe.h"
#include "include\csvReader.h"
#include "include\statu_p.h"
#include <windows.h>
int main(int argc, char const *argv[])
{
    SetConsoleOutputCP(65001); //设置输出编码为utf-8
    dataframe df = read_csv("test.csv"); //从test读取数据
    dfprint(&df,1); //以预览模式输出 df
    operate_trans(&df,dfTRUE); //将df转化为可统计对象
    cell sum = dfsum(&df,0); // 获取df第一列的和
    cellprint(&sum); //输出和
    dffree(&df);
    return 0;
}
