boardset = {';',',', '(', ')', '{', '}','\n'}
keywordset = {'auto','double','int','struct','break','else','long','switch','case','enum',
				'register','typedef','char','extern','return','union','const','float','short','unsigned',
				'continue','for','signed','void','default','goto','sizeof','volatile','do','if',
				'while','static'}
def is_number(s):
    try:  
        float(s)
        return True
    except ValueError: 
        return False

def findOperation(s:str):
    if('+' in s):
        partset=s.split('+')
        for j in range(0,len(partset)):
            if(j!=0):fw.write(str(count)+ " "+"+ 运算符\n")
            if(is_number(partset[j])):fw.write(str(count)+ " "+partset[j]+" 常量\n")
            elif(partset[j]!=''):
                if(len(partset[j])>7):
                    fw.write(str(count)+ " "+"Error,"+partset[j]+" is longer than 7\n")
                    exit()
                fw.write(str(count)+ " "+partset[j]+" 标识符\n")
    elif('-' in s):
        partset=s.split('-')
        for j in range(0,len(partset)):
            if(j!=0):fw.write(str(count)+ " "+"- 运算符\n")
            if(is_number(partset[j])):fw.write(str(count)+ " "+partset[j]+" 常量\n")
            elif(partset[j]!=''):
                if(len(partset[j])>7):
                    fw.write(str(count)+ " "+"Error,"+partset[j]+" is longer than 7\n")
                    exit()
                fw.write(str(count)+ " "+partset[j]+" 标识符\n")
    elif('!=' in s):
        partset=s.split('!=')
        for j in range(0,len(partset)):
            if(j!=0):fw.write(str(count)+ " "+"!= 运算符\n")
            if(is_number(partset[j])):fw.write(str(count)+ " "+partset[j]+" 常量\n")
            elif(partset[j]!=''):
                if(len(partset[j])>7):
                    fw.write(str(count)+ " "+"Error,"+partset[j]+" is longer than 7\n")
                    exit()
                fw.write(str(count)+ " "+partset[j]+" 标识符\n")
    elif('=' in s):
        partset=s.split('=')
        for j in range(0,len(partset)):
            if(j!=0):fw.write(str(count)+ " "+"= 运算符\n")
            if(is_number(partset[j])):fw.write(str(count)+ " "+partset[j]+" 常量\n")
            elif(partset[j]!=''):
                if(len(partset[j])>7):
                    fw.write(str(count)+ " "+"Error,"+partset[j]+" is longer than 7\n")
                    exit()
                fw.write(str(count)+ " "+partset[j]+" 标识符\n")
    elif(len(s)>7):
        fw.write(str(count)+ " "+"Error,"+s+" is longer than 7\n")
        exit
    else:fw.write(str(count)+ " "+s+" 标识符\n")

fp = open('text.txt','r',encoding='UTF-8')
fw=open("./code/out_from_task1.txt","w",encoding='UTF-8')
text=fp.read()
text=text+'\n'
part=''
count=1
for i in range(0,len(text)):
    #fw.write(str(count)+ " "+i,end=" \n")
    if(text[i]=='\n'):
        count+=1
    #fw.write(str(count)+ " "+count,end=" \n")
    if(text[i] in boardset and part==''):
        if(text[i] not in {'\n',' '}):fw.write(str(count)+ " "+text[i]+" 限定符\n")
    elif(text[i] in boardset):
        if(part[0:8]=='#include'):fw.write(str(count)+ " "+part+" 标识符\n")
        elif(part[0]=="'" and part[-1]=="'"):fw.write(str(count)+ " "+part+" 字符\n")
        elif(part[0]=='"' and part[-1]=='"'):fw.write(str(count)+ " "+part+" 字符串\n")
        elif(' ' in part):
            partset=part.split(' ')
            for j in partset:
                if(j in keywordset):fw.write(str(count)+ " "+j+" 关键词\n")
                elif(is_number(j)):fw.write(str(count)+ " "+j+" 常量\n")
                elif(j!=''):findOperation(j)
        else:findOperation(part)
        part=''
        if(text[i] not in {'\n',' '}):fw.write(str(count)+ " "+text[i]+" 限定符\n")
    else:
        part+=text[i]

