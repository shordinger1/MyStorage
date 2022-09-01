num=['0','1','2','3','4','5','6','7','8','9']
key=["longlong","int","char","float","main","double","case", "for","if","auto","else","do","while","void","static", "return","break","struct","const","union","switch","typedef","enum","def","range"]
limit=['(',')','{','}','[',']',';',',',':','#']
operator=['+','-','*','/','=','%']

def find(str,i):
    for x in range(i,len(str)):
        if str[x].isalpha():
            continue
        elif str[x] in num:
            continue
        else:
            return x


def findnum(str,i):
    number=True
    for x in range(i,len(str)):
        temp=False
        if str[x] in num:
            continue
        elif str[x]=='.' and temp==False:
            temp=True
            continue
        elif str[x]=='.' and temp==True:
            number=False
        elif number:
            return x
        else:
            return 0-x
    return 0-x


def is_Chinese(word):
    for ch in word:
        if '\u4e00' <= ch <= '\u9fff':
            return True
    return False



def analyse(str):
    i=0
    str=str+" "
    while(i<len(str)):
        if str[i]==" " or str[i]=="\n":
            i=i+1
        elif str[i].isalpha():
            notkey=True
            for x in key:
                if str.find(x,i)==i:
                    notkey=False
                    if str[i+len(x)].isalpha():
                        temp=find(str,i+1)
                        symbol=str[i:temp]
                        i=i+len(symbol)
                        if len(symbol)>8:
                            word=str[0:8]
                        for a in word:
                            if is_Chinese(a):
                                print("(错误,",word,")")
                                break
                        print("(标识符,",word,")")
                        break
                    else:
                        print("(关键字,",x,")")
                        i=i+len(x)
                        break
            if notkey:
                temp=find(str,i)
                symbol=str[i:temp]
                i=i+len(symbol)
                if len(symbol)>8:
                    symbol=str[0:8]
                mistake=False
                for a in symbol:
                    if is_Chinese(a):
                        mistake=True
                        break
                if mistake:
                    print("(错误,",symbol,")")
                else:
                    print("(标识符,",symbol,")")
                
        elif str[i] in num:
            numpos=int(findnum(str,i))
            if numpos>0:
                number=str[i:numpos]
                i=numpos
                if str[i].isalpha():
                    temp=find(str,i)
                    symbol=str[i:temp]
                    i=i+len(symbol)
                    symbol=number+symbol
                    print("(错误,",symbol,")")
                    
                else:
                    print("(常量,",number,")")
                
            else:
                number=str[i:-numpos]
                print("(错误,",number,")")
                i=0-numpos
            
        else:
            if str[i]=='+':
                if str[i+1]=="+":
                    print("(自加运算符,++)")
                    i=i+2
                elif str[i+1]=="=":
                    print("(运算符,+=)")
                    i=i+2
                else:
                    print("(运算符,+)")
                    i=i+1
            elif str[i]=="-":
                if str[i+1]=="-":
                    print("(自减运算符,--)")
                    i=i+2
                elif str[i+1]=="=":
                    print("(运算符,-=)")
                    i=i+2
                else:
                    print("(运算符,-)")
                    i=i+1
            elif str[i]=="*":
                print("(运算符,*)")
                i=i+1
            elif str[i]=="/":
                print("(运算符,/)")
                i=i+1
            elif str[i] in limit:
                print("(分界符,",str[i],")")
                i=i+1
            elif str[i]=="<":
                if str[i+1]=="=":
                    print("(运算符,<=)")
                    i=i+2
                else:
                    print("(运算符,<")
                    i=i+1
            elif str[i]==">":
                if str[i+1]=="=":
                    print("(运算符,>=)")
                    i=i+2
                else:
                    print("(运算符,>")
                    i=i+1
            elif str[i]=="=":
                print("(运算符,=)")
                i=i+1

            else:
                print("(错误,",str[i],")")
                i=i+1
                
            
if __name__ == '__main__':
    #file= open("sting.txt", "r")
    #txt=file.read()
    txt=input("enter:")
    analyse(txt)

