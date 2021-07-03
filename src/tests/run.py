import os, sys
from random import shuffle
from test_run.run import Problems

problems = Problems()
config_file = "config.txt"
config = []
config_dict = dict()

# 将当前目录加入到系统路径
sys.path.append(os.getcwd())

# 生成配置信息
def generate_config():
    global config
    global config_dict
    config = [
        ("显示学科", 1),
        ("显示章节", 1)
    ]
    for value in range(1,6):
        config.append(("重要度" + str(value), 1))
    for value in range(1,6):
        config.append(("难度" + str(value), 1))
    chapters = dict()
    for problem in problems.problems:
        if problem["subject"] not in chapters.keys():
            chapters[problem["subject"]] = {problem["chapter"]}
        elif problem["chapter"] not in chapters[problem["subject"]]:
            chapters[problem["subject"]].add(problem["chapter"])
    for subject, chapter_list in chapters.items():
        for chapter in chapter_list:
            config.append((subject + " - " + chapter, 1))
    config_dict = dict(config)

# 写入配置信息
def write_config():
    global config
    f = open(config_file, "w", encoding="utf-8")
    for item in config:
        f.write(item[0] + "\t" + str(item[1]) + "\n")
    f.close()

# 读取配置信息
def read_config():
    global config
    global config_dict
    config = []
    f = open(config_file, "r", encoding="utf-8")
    for s in f.readlines():
        temp = s.strip().split("\t")
        if len(temp) == 2:
            config.append((temp[0], int(temp[1])))
    f.close()
    config_dict = dict(config)

# 随机过滤知识点
def apply_config(problem):
    global config_dict
    value = problem["value"]
    diff  = problem["diff"]
    chapter = problem["chapter"]
    subject = problem["subject"]
    if config_dict["重要度"+str(value)] < 1:
        return False
    if config_dict["难度"+str(diff)] < 1:
        return False
    if config_dict[subject + " - " + chapter] < 1:
        return False
    return True

random_problems = []

# 初始化配置信息
def init_config():
    global random_problems
    if os.path.exists(config_file):
        read_config()
    else:
        generate_config()
        write_config()
    random_problems = list(range(len(problems.problems)))

def display_problem(problem):
    first_line = ""
    display_subject = bool(config_dict["显示学科"])
    display_chapter = bool(config_dict["显示章节"])
    if display_subject:
        first_line += problem["subject"]
        if display_chapter:
            first_line += "/"
    if display_chapter:
        first_line += problem["chapter"]
    if len(first_line) > 0:
        print(first_line)
    prefix = "生成题" if "extended" in problem.keys() else "普通题"
    print("【%s】%s" % (prefix, problem["title"]))
    print("重要度\t" + "☆ " * problem["value"])
    print("难度\t" + "☆ " * problem["diff"])
    print(problem["tips"])
    if len(problem["ans"]) > 0:
        input("--------------按回车查看答案")
        print(problem["ans"])
    print("----------------------------------------------------")

# 训练界面
def train():
    global random_problems
    shuffle(random_problems)
    index = 0
    while index < len(random_problems):
        problem = problems.problems[random_problems[index]]
        if not apply_config(problem):
            continue
        display_problem(problem)
        esc = input("-----输入escape结束训练，回车继续--------------------")
        print("----------------------------------------------------")
        if esc == "escape":
            break
        index += 1

# 主界面交互
def run_test():
    print("测试环境已启动")
    print("配置文件位于" + config_file)
    while True:
        reload = input("输入reload重新加载配置文件，回车开始测试")
        if reload == "reload":
            read_config()
        else:
            train()

if __name__ == "__main__":
    init_config()
    run_test()
