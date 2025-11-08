import scipy.special
import numpy as np
import statistics as stat
from scipy.stats import norm
from tabulate import tabulate

def f(n, p, k):
    return scipy.special.binom(n, k) * (p ** k) * ((1 - p) ** (n - k))

def litf(p):
    return f(5, p, 3) + f(5, p, 4) + f(5, p, 5)

def get_estimator(dataset):
    n = len(dataset)
    return sum(dataset) / (n ** 2)

def probe(n, p):
    dataset = np.random.binomial(n, p)
    return get_estimator(dataset)
    # estimators = [get_est1(dataset), get_est2(dataset), get_est3(dataset, my_weights), get_est4(dataset)]
    # return estimators

def experiment(n, p):
    number_of_probes = 500
    estimators = []
    for i in range(number_of_probes):
        estimators.append(litf(probe(n, p)))
    
    return estimators

def main():
    cases = [['A',50, 0.1],['B',50, 0.3],['C',50, 0.5],['D',50, 0.7],['E',50, 0.9]]
    for case in cases:
        print(f"Case {case[0]}:")
        results = experiment(case[1], case[2], case[3])
        table = [['Type','Bias', 'Var', 'MSE']]
        estimators = ['Mean', 'Med', 'random weights', 'gauss weights']
        for i in range(len(results)):
            tmptable = [estimators[i]]
            tmptable.append(np.mean(results[i]) - case[2])                     # BIAS
            tmptable.append(np.var(results[i]))                                # VAR
            tmptable.append(np.mean((np.array(results[i]) - case[2]) ** 2))    # MSE
            table.append(tmptable)
        print(tabulate(table, headers="firstrow", tablefmt="grid"))
        # print(table)

main()

# print(experiment(50, 1, 1))

