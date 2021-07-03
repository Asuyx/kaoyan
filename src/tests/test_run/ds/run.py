from .chap01 import *

chapters = [
    {
        "title" : "第1章 算法及其评价",
        "basic_problems" : problems_chap01,
        "extended" : []
    }
]

class DsProblems:
    def __init__(self):
        self.problems = []
        for chap in chapters:
            for problem in chap["basic_problems"]:
                self.problems.append(dict({
                    "chapter" : chap["title"],
                }, **problem))
