import numpy as np
import statistics as stat
from scipy.stats import norm
from tabulate import tabulate

def get_est1(dataset):
    theta = 0
    for i in dataset:
        theta += i
    return theta / len(dataset)

def get_est2(dataset):
    return stat.median(dataset)

def get_random_weights(n):
    weights = []
    sum = 0
    for i in range(n):
        new_weight = np.random.uniform()
        sum += new_weight
        weights.append(new_weight)
    weights = list(map(lambda x: x / sum, weights))
    return weights

def get_est3(dataset, weights):
    assert len(dataset) == len(weights)
    theta = 0
    for i in range(len(dataset)):
        theta += dataset[i] * weights[i]
    return theta

def get_gauss_weight(i, n):
    phi1 = 0
    phi2 = 0
    if(i != 1):
        phi1 = norm.pdf(norm.ppf((i - 1) / n))
    if i != n:
        phi2 = norm.pdf(norm.ppf(i / n))
    return phi1 - phi2

def get_gauss_weights(n):
    return [get_gauss_weight(i, n) for i in range(1, n + 1)]

def get_est4(dataset):
    dataset.sort()
    theta = 0
    weights = get_gauss_weights(len(dataset))
    for i in range(len(dataset)):
        theta += dataset[i] * weights[i]
    return theta

def probe(n, loc, scale, my_weights):
    dataset = np.random.normal(loc=loc, scale=scale, size=n)
    estimators = [get_est1(dataset), get_est2(dataset), get_est3(dataset, my_weights), get_est4(dataset)]
    return estimators

def experiment(n, loc, scale):
    number_of_probes = 500
    my_weights = get_random_weights(n)
    estimators = []
    for i in range(number_of_probes):
        estimators.append(probe(n, loc, scale, my_weights))
    
    return [list(estimator) for estimator in list(zip(*estimators))]

def main():
    cases = [['A',50,1,1],['B',50,4,1],['C',50,1,2]]
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
