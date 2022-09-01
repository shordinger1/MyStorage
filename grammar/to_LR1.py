from collections import defaultdict

epsilon = 'ε'
point = '·'


class LR1:
    def __init__(self, symbol):
        self.symbol = symbol  # input symbol 终结符表
        self.project_family_num = 0  # 项目集族数
        self.project_num = 0  # 项目数
        self.next_states = defaultdict(defaultdict)  # [id][进位符号]=下一个项目集族
        self.projects = defaultdict(defaultdict)  # S' -> ·S: projects[S'][·S] = 1 记录所有项目及编号
        self.links = defaultdict(defaultdict)  # 项目集族的转换连接用 links[当前项目集族编号][下一个项目集族编号] = 转换需要的进位符
        self.first = defaultdict(defaultdict)  # first集合
        self.search = defaultdict(defaultdict)  # 前向搜索符
        self.project_index = dict()  # e.g. 1. S' -> ·S: project_index[1] = (S', ·S) 根据编号查找项目
        self.project_family = dict()  # 项目集族
        self.production = []  # 产生式，从1开始方便后续action等操作
        self.production_index = defaultdict(defaultdict)  # 通过编号来索引产生式    S->·aA  =  S->aA    [左部][右部]
        self.states = set()  # 项目集族编号
        self.start_state = 0  # 从0开始

    def add_first(self, big_sign, sign):  # 添加到first集中
        if type(sign) == type("字符串"):
            sign = set([sign])
        if big_sign in self.first:
            self.first[big_sign] = self.first[big_sign].union(sign)
        else:
            self.first[big_sign] = sign

    def add_search(self, state_num, project, sign):  # 添加到搜索符中
        if type(sign) == type('a'):
            sign = set([sign])
        if state_num in self.search and project in self.search[state_num]:
            self.search[state_num][project] = self.search[state_num][project].union(sign)  # 并集
        else:
            self.search[state_num][project] = sign

    def add_project(self, left, right):  # 添加项目
        if right == epsilon:
            right_temp = point
            self.project_num += 1
            self.production_index[left][right_temp] = len(self.production)  # 0开始随着产生式进入逐步增加
            self.projects[left][right_temp] = self.project_num  # 用于index查找
            self.project_index[self.project_num] = (left, right_temp)
        for i in range(len(right) + 1):
            right_temp = right[:i] + point + right[i:]
            self.project_num += 1
            self.production_index[left][right_temp] = len(self.production)  # 0开始随着产生式进入逐步增加
            self.projects[left][right_temp] = self.project_num  # 用于index查找
            self.project_index[self.project_num] = (left, right_temp)
        self.production.append((left, right))

    def get_first_ahead(self, big_sign):  # 添加first集
        for i in self.production:
            if i[0] == big_sign and big_sign not in self.first:
                if 'a' <= i[1][0] <= 'z':
                    self.first[big_sign] = set([i[1][0]])
                    return i[1][0]
                else:
                    tmp = self.get_first_ahead(i[1][0])
                    self.first[big_sign] = set([tmp])
                    return tmp
                    # S = searchA   A = c B = c
            elif i[0] == big_sign and big_sign in self.first:
                if 'a' <= i[1][0] <= 'z':
                    self.first[big_sign].union(set([i[1][0]]))
                    return i[1][0]
                else:
                    tmp = self.get_first_ahead(i[1][0])
                    self.first[big_sign].union(set([tmp]))
                    return tmp

    def get_first(self, big_sign):  # 寻找first集
        if big_sign in self.first:  # 直接得到
            return self.first[big_sign]
        else:  # 间接得到 比如S->AB A->BC B->ac
            for i in self.production:
                if i[0] == big_sign:
                    if 'a' <= i[1][0] <= 'z':
                        self.add_first(big_sign, i[1][0])  # 直接添加到first集
                    else:
                        if len(i[1]) == 1:
                            self.add_first(big_sign, self.get_first(i[1]))  # 递归寻找first集
                        else:  # Aa/AB
                            for j in range(len(i[1])):
                                if i[1][j] == i[0]:
                                    continue  # 查看下一个同左部的产生式
                                if j == len(i[1]) - 1:  # 搜索到末尾
                                    self.add_first(big_sign, self.get_first(i[1]))
                                elif 'A' <= i[1][j] <= 'Z':
                                    tmp = self.get_first(i[1][j])
                                    if epsilon in tmp:
                                        tmp.remove(epsilon)  # 去epsilon
                                        self.add_first(big_sign, tmp)
                                    else:
                                        self.add_first(big_sign, tmp)
                                        break
                                else:
                                    self.add_first(big_sign, i[1][j])
                                    break
        return self.first[big_sign]

    def get_closure_search(self, left, state_num, sign):  # 补全项目
        tmp = set()
        for i in self.projects[left]:
            if i.find(point) == 0:
                tmp.add((left, i, tuple(sign)))
                self.add_search(state_num, (left, i), sign)
        return tmp

    def get_project_family(self, project_family, state_num):  # 项目集，状态
        visit = set()
        while len(visit) != len(project_family):
            for i in project_family:
                if i in visit:
                    continue
                visit.add(i)
                next_place = i[1].find(point) + 1  # ·S  -> ·_S_
                if next_place == len(i[1]) or ('a' <= i[1][next_place] <= 'z'):  # 不用添加新的项目
                    continue
                next_next_place = next_place + 1  # ·S_ ，寻找前向搜索符
                if next_next_place == len(i[1]):  # 到头了S·_
                    if state_num not in self.search or (i[0], i[1]) not in self.search[state_num]:
                        visit.remove(i)
                        continue
                    sign = self.search[state_num][(i[0], i[1])]
                elif 'a' <= i[1][next_next_place] <= 'z':  # 后面直接跟终结符
                    sign = i[1][next_next_place]
                else:
                    sign = self.get_first(i[1][next_next_place])
                tmp = self.get_closure_search(i[1][next_place], state_num, sign)  # 补全项目
                project_family = project_family.union(tmp)
        return project_family

    def be_the_only(self, project_family):  # 去重复，合并搜索符不同导致的两个项目
        for i in project_family:
            for j in project_family:
                if i == j:
                    continue
                if i[0] == j[0] and i[1] == j[1]:
                    # if i[2] in self.first[i[0]] and j[2] in self.first[j[0]]:
                    tmp = (i[0], i[1], tuple(set(i[2]).union(set(j[2]))))
                    project_family = project_family.difference(set([i]))
                    project_family = project_family.difference(set([j]))
                    project_family = project_family.union(set([tmp]))
        return project_family

    def get_closure(self, big_sign):  # 补全项目
        tmp = set()
        for i in self.projects[big_sign]:  # 寻找以该非终结符开头的项目
            if i.find(point) == 0:
                tmp.add((big_sign, i))
        return tmp

    def get_tmp_family(self, tmp_family):
        visit = set()
        while len(visit) != len(tmp_family):
            for i in tmp_family:
                if i in visit:
                    continue
                visit.add(i)
                next_place = i[1].find(point) + 1
                if next_place == len(i[1]) or (next_place < len(i[1]) and 'a' <= i[1][next_place] <= 'z'):
                    continue  # 不会产生新的项目
                next_next_place = next_place + 1
                if next_next_place == len(i[1]):
                    sign = tuple('#')
                elif 'a' <= i[1][next_next_place] <= 'z':
                    sign = i[1][next_next_place]
                else:
                    sign = self.get_first(i[1][next_next_place])
                tmp = self.get_closure(i[1][next_place])  # 非终结符的开头的项目
                for j in tmp:
                    tmp_family = tmp_family.union(set([(j[0], j[1], tuple(sign))]))
        return tmp_family

    def link(self, sta, next_sta, carry):  # 进行项目集族之间的连接
        if type(carry) == type("字符串"):
            carry = set([carry])
        self.states.add(sta)
        self.states.add(next_sta)
        if sta in self.links and next_sta in self.links[sta]:
            self.links[sta][next_sta] = self.links[sta][next_sta].union(carry)
        else:
            self.links[sta][next_sta] = carry

    def get_next_sta(self, sta, carry):  # 如果存在下一个项目集族的编号就返回编号，否则新建一个项目集族
        if sta not in self.next_states or carry not in self.next_states[sta]:  # 新增项目集族
            self.project_family_num += 1
            self.next_states[sta][carry] = self.project_family_num
            next_sta = self.project_family_num
            return next_sta, True
        else:
            return self.next_states[sta][carry], False

    def add_project_family(self, next_sta, tmp_family):  # 添加到项目集族中
        if next_sta in self.project_family:
            self.project_family[next_sta] = self.project_family[next_sta].union(tmp_family)
        else:
            self.project_family[next_sta] = tmp_family


def go_dfa(lr1):
    lr1.states.add(lr1.start_state)  # 添加初始I0项目集族编号
    set_now = [0]
    lr1.add_search(lr1.start_state, lr1.project_index[1], '#')
    while len(set_now):  # 从I0开始按照项目集族来遍历，通过内部循环拓展项目集族并补全，按照深度优先搜索规则
        sta = set_now.pop()  # 当前项目集族编号
        lr1.project_family[sta] = lr1.get_project_family(lr1.project_family[sta], sta)  # 项目集族完成
        lr1.project_family[sta] = lr1.be_the_only(lr1.project_family[sta])  # 去重复
        tmp_family = {}  # 表示·进位产生的新项目，以进位的符号为索引
        tmp_search = defaultdict(defaultdict)  # 新项目的搜索符
        for i in lr1.project_family[sta]:
            next_place = i[1].find(point) + 1
            if next_place == len(i[1]):  # ·S
                continue
            carry = i[1][next_place]  # carry表示·进位的符号，比如说·S ，进位后的carry为S
            after = i[1][:next_place - 1] + carry + point + i[1][next_place + 1:]  # 进位后的项目变化
            if carry in tmp_family:  # 若存在某进位符导致的新项目集族则进行并集
                tmp_family[carry].add((i[0], after, tuple(lr1.search[sta][(i[0], i[1])])))
            else:  # 没有则自己开辟一个集合
                tmp_family[carry] = set([(i[0], after, tuple(lr1.search[sta][(i[0], i[1])]))])
            if carry in tmp_search and (i[0], after) in tmp_search[carry]:  # 合并搜索符
                tmp_search[carry][(i[0], after)] = tmp_search[carry][(i[0], after)].union(lr1.search[sta][(i[0], i[1])])
            else:
                tmp_search[i[0]][(i[0], after)] = lr1.search[sta][(i[0], i[1])]  # 祖传的搜索符
        # {'a': {('S', 'a·Ad', '#'), ('S', 'a·Br', '#')}
        for i in tmp_family:
            tmp_family[i] = lr1.get_tmp_family(tmp_family[i])  # 寻找闭包
            tmp_family[i] = lr1.be_the_only(tmp_family[i])  # 去重
            if tmp_family[i] in lr1.project_family.values():  # 项目集族已经存在
                next_sta = list(lr1.project_family.keys())[list(lr1.project_family.values()).index(tmp_family[i])]
                lr1.link(sta, next_sta, i)
                lr1.next_states[sta][i] = next_sta
                continue
            next_sta, whether = lr1.get_next_sta(sta, i)
            if whether:  # 表示新增了项目集族，循环会继续下去
                set_now.append(next_sta)
            lr1.add_project_family(next_sta, tmp_family[i])  # 从temporary项目转正
            for j in tmp_search:
                for k in tmp_search[j]:
                    lr1.add_search(next_sta, k, tmp_search[j][k])
            lr1.link(sta, next_sta, carry)  # 进行项目集族之间的连接


def extend(list_source):  # 拓广文法
    extended_list_source = []
    a = ['S\'', list_source[0][0]]
    extended_list_source.append(a)
    for i in list_source:
        extended_list_source.append(i)
    return extended_list_source


def go_LR1(alphabet, list_source):
    lr1 = LR1(alphabet)
    list_source = extend(list_source)  # 添加拓广文法
    for i in range(len(list_source)):  # 全部添加到项目中
        lr1.add_project(list_source[i][0], list_source[i][1])
    # for i in lr1.projects:
    #     for j in lr1.projects[i]:
    #         print(i, '->', j)
    lr1.project_family[0] = {(lr1.project_index[1][0], lr1.project_index[1][1], tuple('#'))}  # 设置项目集族的出发点
    for i in range(1, len(lr1.production)):  # 补全first集
        lr1.get_first_ahead(lr1.production[i][0])
    # print(lr1.first)
    go_dfa(lr1)  # 创建DFA
    # for i in range(len(lr1.project_family)):
    #     print('I', i, ':   ', lr1.project_family[i])
    return lr1


def add_action(sta, sign, state, action):  # 添加到action表中
    if type(state) == type("字符串"):
        state = set([state])
    if sta in action and sign in action[sta]:
        action[sta][sign] = action[sta][sign].union(state)
    else:
        action[sta][sign] = state


def go_jump(lr1, sta, sign):  # 项目集族的编号跳转，返回指定的下一个项目集族编号
    return lr1.next_states[sta][sign]


def add_goto(sta, carry, go, goto):  # 添加到goto表中
    if type(go) == type(1):
        go = set([go])
    if sta in goto and carry in goto[sta]:
        goto[sta][carry] = goto[sta][carry].union(go)
    else:
        goto[sta][carry] = go


def go_LR1_table(lr1):
    action = defaultdict(defaultdict)
    goto = defaultdict(defaultdict)
    for i in lr1.project_family:
        for j in lr1.project_family[i]:  # 每个项目进行查找
            next_place = j[1].find(point) + 1  # ·进位的位置
            if next_place == len(j[1]):  # 到头了
                if lr1.projects[j[0]][j[1]] == 2:
                    add_action(i, '#', 'acc', action)  # 接受状态
                else:  # 需要进行规约
                    op = 'r' + str(lr1.production_index[j[0]][j[1]])
                if lr1.production_index[j[0]][j[1]] == 0:
                    continue
                if set('#') in lr1.search[i][(j[0], j[1])] or set('#') == lr1.search[i][(j[0], j[1])]:
                    add_action(i, '#', op, action)
                for sign in lr1.symbol:  # 按索引表寻找添加action
                    if sign in lr1.search[i][(j[0], j[1])] or sign == lr1.search[i][(j[0], j[1])]:
                        add_action(i, sign, op, action)
                continue
            carry = j[1][next_place]  # 进位符，下面是进位不是规约
            op = 'S' + str(go_jump(lr1, i, carry))
            if 'a' <= carry <= 'z':
                add_action(i, carry, op, action)
            else:  # 若为非终结符进位则添加到goto表
                add_goto(i, carry, int(op[1:]), goto)
    for i in action:  # 检查是否符合LR(1)
        for j in action[i]:
            if len(action[i][j]) > 1:
                print('创建LR(1)分析表失败')
                return False, False, False
    print('创建LR(1)分析表成功')  # 分析表创建成功
    return action, goto, True