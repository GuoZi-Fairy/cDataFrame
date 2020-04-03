## (void)match(dataframePtr,index,expr,destn)
### 该接口允许通过包含至多1个x的表达式expr来对dataframe的一列进行匹配,并将匹配结果存放至足够大的size_t类型的数组destn中
##### 参数列表
* **dataframePtr** 匹配的dataframe的指针
* **index** 匹配的列在dataframe中的索引
* **expr** 允许包含x的表达式,x为列中每一个cell的数据
* destn 足够大的size_t类型的数组destn（*为了使用size_t类型,你不得不包含'stdio.h'文件*）