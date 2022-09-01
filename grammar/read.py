vn = {'关键词': 'a', '标识符': 'b', '限定符': 'c', '运算符': 'd', '常量': 'e'}


def get_code():
    line = []
    code = []
    with open("./code/out_from_task1.txt",encoding="utf-8") as f:
        list_row = f.readlines()
        a = len(list_row)
        for i in range(len(list_row)):
            list_col = list_row[i].split()
            list_col.append(a)
            code.append(list_col)
            a -= 1
        # for i in range(len(code)):
        #     code[i][1],code[i][2]=code[i][2],code[i][1]
        for i in range(len(list_row) - 1, -1, -1):
            list_col = list_row[i].split()
            line.append(list_col[0])
    print(code,end=":")
    print(line)
    return code, line


def analyse(action, goto, lr1):
    code, line = get_code()
    state_stack = [0]
    sign_stack = ['#']
    input_stack = []
    for i in code:
        input_stack.append(vn[i[1]])
    input_stack.append('#')
    input_stack = input_stack[::-1]
    wrong = ['start', '?']  # 用于错误检查
    while True:
        state_now = state_stack[-1]  # 此刻状态
        # print(input_stack)
        if len(line) == 1 and len(input_stack) > 2:  # 输入的不全(1) : 正确序列的缺失导致报错
            return False, False, line_now
        else:
            sign_now = input_stack[-1]  # 最外围的词
            # line_now = line[-1]
            wrong[1] = sign_now
        if sign_now not in action[state_now]:  # 取上一个词，表示不能这么接
            if wrong[1] == '#':
                return False, False, line_now  # 输入的不全(2) : 开头就缺少输入段
            else:
                return wrong, False, line[-1]  # 输入错误
        op = list(action[state_now][sign_now])[0]
        if op[0] == 'S':
            sign_stack.append(sign_now)  # 符号入栈
            input_stack = input_stack[:-1]  # 输入串缩短，表示进栈
            line_now = line[-1]  # 用于记录上一个词的行号，防止出现正确序列的缺失导致的错误
            line = line[:-1]  # 行数跟进
            state_stack.append(int(op[1:]))  # 状态入栈
            wrong[0] = sign_now
        elif op[0] == 'r':
            production = lr1.production[int(op[1:])]  # 产生式
            change = len(production[1])  # 变换数目
            state_stack = state_stack[:-change]
            sign_stack = sign_stack[:-change]
            state_stack.append(list(goto[state_stack[-1]][production[0]])[0])
            sign_stack.append(production[0])
        elif op == 'acc':
            return 'acc', True, True


def read(action, goto, lr1):
    info, sta, info_line = analyse(action, goto, lr1)
    with open("./result.txt", "w") as f:
        if sta:
            a = 'YES'
            f.write(a)
            print(a)
        else:
            if type(info) == type(False):
                print('NO')
                a = '错误行： ' + info_line + '，\t是否未输入全？'
                f.write(a)
                print(a)
            else:
                print('NO')
                a = '错误行： ' + info_line + '\t' + list(vn.keys())[list(vn.values()).index(info[0])] + "后面不可以接: " + list(vn.keys())[list(vn.values()).index(info[1])]
                f.write(a)
                print(a)
