import to_LR1
import read
epsilon = 'ε'


def main():
    file = open("./expression/ex_2.txt", encoding="utf-8")
    list_row = file.readlines()
    list_source = []
    for i in range(len(list_row)):
        list_col = list_row[i].strip().split('->')
        list_source.append(list_col)
    file.close()
    alphabet = []  # 终结符表，后期action-goto索引用
    for i in range(len(list_source)):
        for j in list_source[i][1]:
            if (j > 'Z' or j < 'A') and j not in alphabet:
                alphabet.append(j)
    lr1 = to_LR1.go_LR1(alphabet, list_source)      # 创建LR(1)项目集族
    # a = 0
    # for i in range(len(lr1.project_family)):
    #     print("I", a, ": ", lr1.project_family[i])
    #     a += 1
    action, goto, shall_we = to_LR1.go_LR1_table(lr1)   # shall_we 为True则说明action-goto表创建完成
    if shall_we:
        read.read(action, goto, lr1)            # 开始分析


if __name__ == "__main__":
    main()