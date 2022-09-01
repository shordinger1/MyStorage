import jieba
import time
f=open("C:\\Users\\ACMer\\Desktop\\ACM\\ML\\NLP\\readme.txt",encoding="utf-8")
t=0
while(1):
    t1=time.process_time()
    sstr=f.readline()
    if(len(sstr)<1):
        break;
    seg_list = jieba.cut(sstr, cut_all=True)
    print(list(seg_list))
    t=t+time.process_time()-t1
print(t)