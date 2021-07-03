from .ds.run import DsProblems

subjects = [
    {
        "subject" : "数据结构",
        "problems" : DsProblems()
    }
]

class Problems:
    def __init__(self):
        self.problems = []
        for subject in subjects:
            for problem in subject["problems"].problems:
                self.problems.append(dict({
                    "subject" : subject["subject"],
                }, **problem))